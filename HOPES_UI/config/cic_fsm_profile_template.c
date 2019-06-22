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
	// �ش� TASK, PORT���� READ/WRITE�� �߻�
}

void profile_event(int eid, profile_event_value_t* value)
{
	// ������ �׷����� ���� VALUE
}

void profile_task_go_begin(int tid)
{
	// ���� ������ �ʱ�ȭ�Ѵ�.
}


void profile_task_go_end(int tid)
{
	// profile�� ����Ѵ�.
}
