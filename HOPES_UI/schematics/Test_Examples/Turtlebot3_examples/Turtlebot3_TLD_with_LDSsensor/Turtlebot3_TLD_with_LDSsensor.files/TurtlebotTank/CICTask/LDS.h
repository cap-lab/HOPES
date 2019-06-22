/************************************
 *
 * File : LDS.cic
 * Date : Feb 25, 2019 11:53 AM
 *
*************************************/

//#include "lds_driver.h" 
#include "lds_driver.h" //originally commented out
#include <stdint.h>
#include <stdlib.h>
//#include <algorithm>
#include <inttypes.h>
//#include <time.h>
#include <iostream>
#include <limits.h>
//#include <UCTime.h>

using namespace std;
/////////////////////////////////////
// include header section
/////////////////////////////////////

#define MAX_LINEAR_VELOCITY 0.22  //defined in turtlebot3_tank.h 
#define MAX_ANGULAR_VELOCITY 2.84 //defined in turtlebot3_tank.h

#define INITIAL_LINEAR_X (MAX_LINEAR_VELOCITY/1.5) 
#define INITIAL_ANGULAR_Z (MAX_ANGULAR_VELOCITY)
#define SAFETY_MARGIN 350
#define THETA_MAX 30

#define NO_STATE 0
#define TURN_STATE 1
#define STRAIGHT_STATE 2

#ifndef STATIC
#define STATIC static
#endif

namespace lds
{
	LFCDLaser::LFCDLaser(const std::string& port, uint32_t baud_rate, boost::asio::io_service& io)
		: port_(port), baud_rate_(baud_rate), shutting_down_(false), serial_(io, port_)
	{
		try{
			serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));
			// Below command is not required after firmware upgrade (2017.10)
			boost::asio::write(serial_, boost::asio::buffer("b", 1));  // start motor
			printf("LDS laser initialize!\n");  
		}
		catch (boost::system::system_error ex)
		{
			printf("An exception was thrown when lds initializing: %s", ex.what());
		}

	}

	LFCDLaser::~LFCDLaser()
	{
		boost::asio::write(serial_, boost::asio::buffer("e", 1));  // stop motor
	}

	void LFCDLaser::start_motor()
	{
		boost::asio::write(serial_, boost::asio::buffer("b", 1));  // stop motor
	}

	void LFCDLaser::poll(AngularDistance* stAngularDistance)
	{
		uint8_t start_count = 0;
		bool got_scan = false;
		boost::array<uint8_t, 2520> raw_bytes;
		//uint8_t good_sets = 0;
		uint32_t motor_speed = 0;
		rpms=0;
		int index;
		
		while (!shutting_down_ && !got_scan)
		{
			// Wait until first data sync of frame: 0xFA, 0xA0
			boost::asio::read(serial_, boost::asio::buffer(&raw_bytes[start_count],1));

			if(start_count == 0)
			{
				if(raw_bytes[start_count] == 0xFA)
				{
					start_count = 1;
				}
			}
			else if(start_count == 1)
			{
				if(raw_bytes[start_count] == 0xA0)
				{
					start_count = 0;

					// Now that entire start sequence has been found, read in the rest of the message
					got_scan = true;
					//uem_time utime3, utime4;
					//UCTime_GetCurTickInMilliSeconds(&utime3);
					boost::asio::read(serial_,boost::asio::buffer(&raw_bytes[2], 2518));
					//UCTime_GetCurTickInMilliSeconds(&utime4);
					//printf("utime4~utime3: %lld\n", (utime4-utime3));

					// scan->angle_min = 0.0;
					// scan->angle_max = 2.0*M_PI;
					// scan->angle_increment = (2.0*M_PI/360.0);
					// scan->range_min = 0.12; //120mm //120 / 1000 = 0.12m
					// scan->range_max = 3.5; //3500mm
					// scan->ranges.resize(360);
					// scan->intensities.resize(360);

					//read data in sets of 6

					for(uint16_t i = 0; i < raw_bytes.size(); i=i+42)
					{
						if(raw_bytes[i] == 0xFA && raw_bytes[i+1] == (0xA0 + i / 42)) //&& CRC check
						{
							//good_sets++;
							motor_speed += (raw_bytes[i+3] << 8) + raw_bytes[i+2]; //accumulate count for avg. time increment
							rpms=(raw_bytes[i+3]<<8|raw_bytes[i+2])/10;

							for(uint16_t j = i+4; j < i+40; j=j+6)
							{
								index = 6*(i/42) + (j-4-i)/6;

								// Four bytes per reading
								uint8_t byte0 = raw_bytes[j];
								uint8_t byte1 = raw_bytes[j+1];
								uint8_t byte2 = raw_bytes[j+2];
								uint8_t byte3 = raw_bytes[j+3];

								// Remaining bits are the range in mm
								//uint16_t intensity = (byte1 << 8) + byte0;

								// Last two bytes represent the uncertanty or intensity, might also be pixel area of target...
								// uint16_t intensity = (byte3 << 8) + byte2;
								uint16_t range = (byte3 << 8) + byte2;

								// scan->ranges[359-index] = range / 1000.0;
								// scan->intensities[359-index] = intensity;
								//printf ("r[%d]=%f\n",359-index, range / 1.0);
								stAngularDistance->r[359-index] = range / 1.0;
								if(range == 0){ //zero = infinity
									stAngularDistance->r[359-index] = numeric_limits<uint16_t>::max();
								}
							}
						}
					}
					// scan->time_increment = motor_speed/good_sets/1e8;
				}
				else
				{
					start_count = 0;
				}
			}
		}
	}
}

/////////////////////////////////////
// global definition
/////////////////////////////////////

static int curr_state = STRAIGHT_STATE, prev_state = NO_STATE;

int convert0_360(int theta){
	theta = theta % 360;
	if(theta < 0){
		theta = theta + 360;
	}
	return theta;
}

void turn_Control(int index){ //TODO: change type as uem_result.
    if(!(((-1 * THETA_MAX) <= index) && (index <= THETA_MAX))){ 
        //UEM_DEBUG_PRINT("error! illegal theta value : %d\n",index);
        return;
    }
    //double linear_x = INITIAL_LINEAR_X / 2;
    double linear_x = 0;
	int sign_rev = 0; 
	if(index >=0){
		sign_rev = -1;
	}else{
		sign_rev = 1;
	}
	//int theta_offset = THETA_MAX - abs(index); //biggest(THETA_MAX) at center(=0), weakest at side(+=30)
	//printf("theta_offset : %d\n", theta_offset);
    double angular_z = (INITIAL_ANGULAR_Z)/2;
	angular_z *= sign_rev;
	printf("buffer sending in turn_Control!\n");
	//printf("linear_x : %lf, angular_z = %lf, min_angular_z = %lf\n",linear_x, angular_z, (INITIAL_ANGULAR_Z)/2);
}

void straight_Control(){
    double linear_x = INITIAL_LINEAR_X;
    double angular_z = 0;
	printf("buffer sending in straight_Control!\n");
	//printf("linear_x : %lf, angular_z = %lf\n",linear_x, angular_z);
}

void selectMinr_and_Theta(AngularDistance* stAngularDistance){
    uint16_t rmin = numeric_limits<uint16_t>::max();
    int index = numeric_limits<int>::min();

    for(int i = (-1 * THETA_MAX); i <= THETA_MAX; i += THETA_MAX){
        if(stAngularDistance->r[convert0_360(i)] <= rmin){
            rmin = stAngularDistance->r[convert0_360(i)];
            index = i;
        }
    }
    if (rmin <= SAFETY_MARGIN){
		curr_state = TURN_STATE;
		if(curr_state == prev_state){
			return;
		}
        printf("turn_Control!\n");        
        turn_Control(index);
		prev_state = TURN_STATE;
		printf("rmin : %hu , index : %d\n", (int) rmin, index);
    }else{
		curr_state =STRAIGHT_STATE;
		if(curr_state == prev_state){
			return;
		}
        printf("straight_Control!\n");                    
        straight_Control();
		prev_state = STRAIGHT_STATE;
    }
}

std::string port("/dev/ttyUSB0");
int baud_rate(230400);
uint16_t rpms;
boost::asio::io_service io; 
STATIC lds::LFCDLaser laser(port, baud_rate, io);

static AngularDistance stAngularDistance;


/////////////////////////////////////
// go code
/////////////////////////////////////

/*
int main()
{
	//uem_time utime1, utime2;
	try{
		// TODO: task main code               
		//UCTime_GetCurTickInMilliSeconds(&utime1);
		laser.poll(&stAngularDistance);
		//UCTime_GetCurTickInMilliSeconds(&utime2);
		//////////////control part ////////////////////
		selectMinr_and_Theta(&stAngularDistance);		
	}
	catch (boost::system::system_error ex)
	{
		printf("An exception was thrown in TASK_GO: %s", ex.what());
	}
}
*/

/////////////////////////////////////
// wrapup code
/////////////////////////////////////
