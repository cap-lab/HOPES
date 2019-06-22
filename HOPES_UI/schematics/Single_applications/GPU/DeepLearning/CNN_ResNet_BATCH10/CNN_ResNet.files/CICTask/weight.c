#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

#include "weight.h"

#define WEIGHT_SIZE     57559981

#define PRINTopt 0
//float *pWeight[153];
//static float *weight;

static float *s_pWeight[153];
static float *s_weight;
static int s_initWeight = 0;

void init_weight()
{
    int fd = open("resnet152.weights", O_RDWR);
	struct stat statbuf;

    fstat(fd, &statbuf);

	int weight_size = statbuf.st_size;
	if (weight_size != WEIGHT_SIZE * sizeof(float)) {
		fprintf(stderr, "Weight file is wrong. Generate a new one.\n");
		exit(-1);
	}  

    s_weight = (float *)mmap(0, WEIGHT_SIZE * sizeof(float), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); 

    s_pWeight[1] = s_weight + 5;
    s_pWeight[2] = s_weight + 9669;
    s_pWeight[3] = s_weight + 14021;
    s_pWeight[4] = s_weight + 51141;
    s_pWeight[5] = s_weight + 68549;
    s_pWeight[6] = s_weight + 85189;
    s_pWeight[7] = s_weight + 122309;
    s_pWeight[8] = s_weight + 139717;
    s_pWeight[9] = s_weight + 156357;
    s_pWeight[10] = s_weight + 193477;
    s_pWeight[11] = s_weight + 210885;
    s_pWeight[12] = s_weight + 244165;
    s_pWeight[13] = s_weight + 392133;
    s_pWeight[14] = s_weight + 459717;
    s_pWeight[15] = s_weight + 525765;
    s_pWeight[16] = s_weight + 673733;
    s_pWeight[17] = s_weight + 741317;
    s_pWeight[18] = s_weight + 807365;
    s_pWeight[19] = s_weight + 955333;
    s_pWeight[20] = s_weight + 1022917;
    s_pWeight[21] = s_weight + 1088965;
    s_pWeight[22] = s_weight + 1236933;
    s_pWeight[23] = s_weight + 1304517;
    s_pWeight[24] = s_weight + 1370565;
    s_pWeight[25] = s_weight + 1518533;
    s_pWeight[26] = s_weight + 1586117;
    s_pWeight[27] = s_weight + 1652165;
    s_pWeight[28] = s_weight + 1800133;
    s_pWeight[29] = s_weight + 1867717;
    s_pWeight[30] = s_weight + 1933765;
    s_pWeight[31] = s_weight + 2081733;
    s_pWeight[32] = s_weight + 2149317;
    s_pWeight[33] = s_weight + 2215365;
    s_pWeight[34] = s_weight + 2363333;
    s_pWeight[35] = s_weight + 2430917;
    s_pWeight[36] = s_weight + 2563013;
    s_pWeight[37] = s_weight + 3153861;
    s_pWeight[38] = s_weight + 3420101;
    s_pWeight[39] = s_weight + 3683269;
    s_pWeight[40] = s_weight + 4274117;
    s_pWeight[41] = s_weight + 4540357;
    s_pWeight[42] = s_weight + 4803525;
    s_pWeight[43] = s_weight + 5394373;
    s_pWeight[44] = s_weight + 5660613;
    s_pWeight[45] = s_weight + 5923781;
    s_pWeight[46] = s_weight + 6514629;
    s_pWeight[47] = s_weight + 6780869;
    s_pWeight[48] = s_weight + 7044037;
    s_pWeight[49] = s_weight + 7634885;
    s_pWeight[50] = s_weight + 7901125;
    s_pWeight[51] = s_weight + 8164293;
    s_pWeight[52] = s_weight + 8755141;
    s_pWeight[53] = s_weight + 9021381;
    s_pWeight[54] = s_weight + 9284549;
    s_pWeight[55] = s_weight + 9875397;
    s_pWeight[56] = s_weight + 10141637;
    s_pWeight[57] = s_weight + 10404805;
    s_pWeight[58] = s_weight + 10995653;
    s_pWeight[59] = s_weight + 11261893;
    s_pWeight[60] = s_weight + 11525061;
    s_pWeight[61] = s_weight + 12115909;
    s_pWeight[62] = s_weight + 12382149;
    s_pWeight[63] = s_weight + 12645317;
    s_pWeight[64] = s_weight + 13236165;
    s_pWeight[65] = s_weight + 13502405;
    s_pWeight[66] = s_weight + 13765573;
    s_pWeight[67] = s_weight + 14356421;
    s_pWeight[68] = s_weight + 14622661;
    s_pWeight[69] = s_weight + 14885829;
    s_pWeight[70] = s_weight + 15476677;
    s_pWeight[71] = s_weight + 15742917;
    s_pWeight[72] = s_weight + 16006085;
    s_pWeight[73] = s_weight + 16596933;
    s_pWeight[74] = s_weight + 16863173;
    s_pWeight[75] = s_weight + 17126341;
    s_pWeight[76] = s_weight + 17717189;
    s_pWeight[77] = s_weight + 17983429;
    s_pWeight[78] = s_weight + 18246597;
    s_pWeight[79] = s_weight + 18837445;
    s_pWeight[80] = s_weight + 19103685;
	s_pWeight[81] = s_weight + 19366853;
    s_pWeight[82] = s_weight + 19957701;
    s_pWeight[83] = s_weight + 20223941;
    s_pWeight[84] = s_weight + 20487109;
    s_pWeight[85] = s_weight + 21077957;
    s_pWeight[86] = s_weight + 21344197;
    s_pWeight[87] = s_weight + 21607365;
    s_pWeight[88] = s_weight + 22198213;
    s_pWeight[89] = s_weight + 22464453;
    s_pWeight[90] = s_weight + 22727621;
    s_pWeight[91] = s_weight + 23318469;
    s_pWeight[92] = s_weight + 23584709;
    s_pWeight[93] = s_weight + 23847877;
    s_pWeight[94] = s_weight + 24438725;
    s_pWeight[95] = s_weight + 24704965;
    s_pWeight[96] = s_weight + 24968133;
    s_pWeight[97] = s_weight + 25558981;
    s_pWeight[98] = s_weight + 25825221;
    s_pWeight[99] = s_weight + 26088389;
    s_pWeight[100] = s_weight + 26679237;
    s_pWeight[101] = s_weight + 26945477;
    s_pWeight[102] = s_weight + 27208645;
    s_pWeight[103] = s_weight + 27799493;
    s_pWeight[104] = s_weight + 28065733;
    s_pWeight[105] = s_weight + 28328901;
    s_pWeight[106] = s_weight + 28919749;
    s_pWeight[107] = s_weight + 29185989;
    s_pWeight[108] = s_weight + 29449157;
    s_pWeight[109] = s_weight + 30040005;
    s_pWeight[110] = s_weight + 30306245;
    s_pWeight[111] = s_weight + 30569413;
    s_pWeight[112] = s_weight + 31160261;
    s_pWeight[113] = s_weight + 31426501;
    s_pWeight[114] = s_weight + 31689669;
    s_pWeight[115] = s_weight + 32280517;
    s_pWeight[116] = s_weight + 32546757;
    s_pWeight[117] = s_weight + 32809925;
    s_pWeight[118] = s_weight + 33400773;
    s_pWeight[119] = s_weight + 33667013;
    s_pWeight[120] = s_weight + 33930181;
    s_pWeight[121] = s_weight + 34521029;
    s_pWeight[122] = s_weight + 34787269;
    s_pWeight[123] = s_weight + 35050437;
    s_pWeight[124] = s_weight + 35641285;
    s_pWeight[125] = s_weight + 35907525;
    s_pWeight[126] = s_weight + 36170693;
    s_pWeight[127] = s_weight + 36761541;
    s_pWeight[128] = s_weight + 37027781;
    s_pWeight[129] = s_weight + 37290949;
    s_pWeight[130] = s_weight + 37881797;
    s_pWeight[131] = s_weight + 38148037;
    s_pWeight[132] = s_weight + 38411205;
    s_pWeight[133] = s_weight + 39002053;
    s_pWeight[134] = s_weight + 39268293;
    s_pWeight[135] = s_weight + 39531461;
    s_pWeight[136] = s_weight + 40122309;
    s_pWeight[137] = s_weight + 40388549;
    s_pWeight[138] = s_weight + 40651717;
    s_pWeight[139] = s_weight + 41242565;
    s_pWeight[140] = s_weight + 41508805;
    s_pWeight[141] = s_weight + 41771973;
    s_pWeight[142] = s_weight + 42362821;
    s_pWeight[143] = s_weight + 42629061;
    s_pWeight[144] = s_weight + 43155397;
    s_pWeight[145] = s_weight + 45516741;
    s_pWeight[146] = s_weight + 46573509;
    s_pWeight[147] = s_weight + 47624133;
    s_pWeight[148] = s_weight + 49985477;
    s_pWeight[149] = s_weight + 51042245;
    s_pWeight[150] = s_weight + 52092869;
    s_pWeight[151] = s_weight + 54454213;
    s_pWeight[152] = s_weight + 55510981;

	close(fd);
	s_initWeight = 1;
}

float *get_weight(int layer_idx, float *buffer, int buffersize){

	if(PRINTopt == 1)
		printf("weight/get_weight) layer : %d get_weight call\n",layer_idx);

	while(s_initWeight == 0)
	{
		usleep(10000);
	}

   	if(PRINTopt == 1)
		printf("weigit/get_weight) layer_idx : %d return\n",layer_idx);

	memcpy(buffer, s_pWeight[layer_idx], buffersize * sizeof(float));
	if(PRINTopt == 1){
		printf("weigit/get_weight) layer_idx : %d buffer : %f return\n",layer_idx, *buffer);
	}
}

void set_weight_pointer() {
    int fd = open("resnet152.weights", O_RDWR);
    struct stat statbuf;
    
	float *pWeight[153];
	float *weight;

	printf("weight/set_weight) set weight\n");
    fstat(fd, &statbuf);
    int weight_size = statbuf.st_size;
    if (weight_size != WEIGHT_SIZE * sizeof(float)) {
		fprintf(stderr, "Weight file is wrong. Generate a new one.\n");
		exit(-1);
	}   

    weight = (float *)mmap(0, WEIGHT_SIZE * sizeof(float), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); 
    
    pWeight[1] = weight + 5;
    pWeight[2] = weight + 9669;
    pWeight[3] = weight + 14021;
    pWeight[4] = weight + 51141;
    pWeight[5] = weight + 68549;
    pWeight[6] = weight + 85189;
    pWeight[7] = weight + 122309;
    pWeight[8] = weight + 139717;
    pWeight[9] = weight + 156357;
    pWeight[10] = weight + 193477;
    pWeight[11] = weight + 210885;
    pWeight[12] = weight + 244165;
    pWeight[13] = weight + 392133;
    pWeight[14] = weight + 459717;
    pWeight[15] = weight + 525765;
    pWeight[16] = weight + 673733;
    pWeight[17] = weight + 741317;
    pWeight[18] = weight + 807365;
    pWeight[19] = weight + 955333;
    pWeight[20] = weight + 1022917;
    pWeight[21] = weight + 1088965;
    pWeight[22] = weight + 1236933;
    pWeight[23] = weight + 1304517;
    pWeight[24] = weight + 1370565;
    pWeight[25] = weight + 1518533;
    pWeight[26] = weight + 1586117;
    pWeight[27] = weight + 1652165;
    pWeight[28] = weight + 1800133;
    pWeight[29] = weight + 1867717;
    pWeight[30] = weight + 1933765;
    pWeight[31] = weight + 2081733;
    pWeight[32] = weight + 2149317;
    pWeight[33] = weight + 2215365;
    pWeight[34] = weight + 2363333;
    pWeight[35] = weight + 2430917;
    pWeight[36] = weight + 2563013;
    pWeight[37] = weight + 3153861;
    pWeight[38] = weight + 3420101;
    pWeight[39] = weight + 3683269;
    pWeight[40] = weight + 4274117;
    pWeight[41] = weight + 4540357;
    pWeight[42] = weight + 4803525;
    pWeight[43] = weight + 5394373;
    pWeight[44] = weight + 5660613;
    pWeight[45] = weight + 5923781;
    pWeight[46] = weight + 6514629;
    pWeight[47] = weight + 6780869;
    pWeight[48] = weight + 7044037;
    pWeight[49] = weight + 7634885;
    pWeight[50] = weight + 7901125;
    pWeight[51] = weight + 8164293;
    pWeight[52] = weight + 8755141;
    pWeight[53] = weight + 9021381;
    pWeight[54] = weight + 9284549;
    pWeight[55] = weight + 9875397;
    pWeight[56] = weight + 10141637;
    pWeight[57] = weight + 10404805;
    pWeight[58] = weight + 10995653;
    pWeight[59] = weight + 11261893;
    pWeight[60] = weight + 11525061;
    pWeight[61] = weight + 12115909;
    pWeight[62] = weight + 12382149;
    pWeight[63] = weight + 12645317;
    pWeight[64] = weight + 13236165;
    pWeight[65] = weight + 13502405;
    pWeight[66] = weight + 13765573;
    pWeight[67] = weight + 14356421;
    pWeight[68] = weight + 14622661;
    pWeight[69] = weight + 14885829;
    pWeight[70] = weight + 15476677;
    pWeight[71] = weight + 15742917;
    pWeight[72] = weight + 16006085;
    pWeight[73] = weight + 16596933;
    pWeight[74] = weight + 16863173;
    pWeight[75] = weight + 17126341;
    pWeight[76] = weight + 17717189;
    pWeight[77] = weight + 17983429;
    pWeight[78] = weight + 18246597;
    pWeight[79] = weight + 18837445;
    pWeight[80] = weight + 19103685;
	pWeight[81] = weight + 19366853;
    pWeight[82] = weight + 19957701;
    pWeight[83] = weight + 20223941;
    pWeight[84] = weight + 20487109;
    pWeight[85] = weight + 21077957;
    pWeight[86] = weight + 21344197;
    pWeight[87] = weight + 21607365;
    pWeight[88] = weight + 22198213;
    pWeight[89] = weight + 22464453;
    pWeight[90] = weight + 22727621;
    pWeight[91] = weight + 23318469;
    pWeight[92] = weight + 23584709;
    pWeight[93] = weight + 23847877;
    pWeight[94] = weight + 24438725;
    pWeight[95] = weight + 24704965;
    pWeight[96] = weight + 24968133;
    pWeight[97] = weight + 25558981;
    pWeight[98] = weight + 25825221;
    pWeight[99] = weight + 26088389;
    pWeight[100] = weight + 26679237;
    pWeight[101] = weight + 26945477;
    pWeight[102] = weight + 27208645;
    pWeight[103] = weight + 27799493;
    pWeight[104] = weight + 28065733;
    pWeight[105] = weight + 28328901;
    pWeight[106] = weight + 28919749;
    pWeight[107] = weight + 29185989;
    pWeight[108] = weight + 29449157;
    pWeight[109] = weight + 30040005;
    pWeight[110] = weight + 30306245;
    pWeight[111] = weight + 30569413;
    pWeight[112] = weight + 31160261;
    pWeight[113] = weight + 31426501;
    pWeight[114] = weight + 31689669;
    pWeight[115] = weight + 32280517;
    pWeight[116] = weight + 32546757;
    pWeight[117] = weight + 32809925;
    pWeight[118] = weight + 33400773;
    pWeight[119] = weight + 33667013;
    pWeight[120] = weight + 33930181;
    pWeight[121] = weight + 34521029;
    pWeight[122] = weight + 34787269;
    pWeight[123] = weight + 35050437;
    pWeight[124] = weight + 35641285;
    pWeight[125] = weight + 35907525;
    pWeight[126] = weight + 36170693;
    pWeight[127] = weight + 36761541;
    pWeight[128] = weight + 37027781;
    pWeight[129] = weight + 37290949;
    pWeight[130] = weight + 37881797;
    pWeight[131] = weight + 38148037;
    pWeight[132] = weight + 38411205;
    pWeight[133] = weight + 39002053;
    pWeight[134] = weight + 39268293;
    pWeight[135] = weight + 39531461;
    pWeight[136] = weight + 40122309;
    pWeight[137] = weight + 40388549;
    pWeight[138] = weight + 40651717;
    pWeight[139] = weight + 41242565;
    pWeight[140] = weight + 41508805;
    pWeight[141] = weight + 41771973;
    pWeight[142] = weight + 42362821;
    pWeight[143] = weight + 42629061;
    pWeight[144] = weight + 43155397;
    pWeight[145] = weight + 45516741;
    pWeight[146] = weight + 46573509;
    pWeight[147] = weight + 47624133;
    pWeight[148] = weight + 49985477;
    pWeight[149] = weight + 51042245;
    pWeight[150] = weight + 52092869;
    pWeight[151] = weight + 54454213;
    pWeight[152] = weight + 55510981;
}
