/*****************************************************************************
 *
 * File : profile.cic
 * Date : [DATE]
 *
******************************************************************************/

typedef struct task_profile_t
{
	int time;
	int numberOfPort;
	int* rateOfPort;

} task_profile_t;

typedef union profile_event_value_t
{
	int intValue;
	long longValue;
	float floatValue;
	double doubleValue;
} profile_event_value_t;

//-----------------------------------------------------------------------------
// include header section
//-----------------------------------------------------------------------------

// ##DEFINE_SECTION::START
// ##DEFINE_SECTION::END



//-----------------------------------------------------------------------------
// global definition
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// profile code initialize
//-----------------------------------------------------------------------------

void profile_port_done(int tid, int pid)
{
	// 해당 TASK, PORT에서 READ/WRITE가 발생
}

void profile_event(int eid, profile_event_value_t* value)
{
	// 각각의 그래프에 따른 VALUE
}

void profile_task_go_begin(int tid)
{
	// 이전 정보를 초기화한다.
}


void profile_task_go_end(int tid)
{
	// profile을 기록한다.
}
