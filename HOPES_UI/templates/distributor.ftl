/************************************
 *
 * File : ${task.name}_Distributor.cic
 * Date : Mar 18, 2019 3:52 PM
 *
*************************************/

#include <string.h>

/////////////////////////////////////
// include header section
/////////////////////////////////////

// ##DEFINE_SECTION::START
// ##DEFINE_SECTION::END


TASK_CODE_BEGIN

/////////////////////////////////////
// global definition
/////////////////////////////////////

// ##DEFINE_PORT_SECTION::START
<#list task.port as port>
  <#if port.direction == "INPUT">
STATIC int port_${port.name};
STATIC int port_${port.name}_output;
  </#if>
</#list>
// ##DEFINE_PORT_SECTION::END


#define DISTRIBUTION_COUNT (${task.faultTolerance.number})

<#assign total_port_num = 0>

<#list task.port as port>
  <#if port.direction == "INPUT">
#define s_nPortDataSize_${port.name} (${port.sampleSize?c} * ${port.rate[0].rate})
//const int s_nPortDataSize_${port.name} = (${port.sampleSize?c} * ${port.rate[0].rate});
    <#assign total_port_num = total_port_num + 1>
  </#if>
</#list>

#define TOTAL_INPUT_PORT_NUMBER (${total_port_num})


/////////////////////////////////////
// init code
/////////////////////////////////////

TASK_INIT
{
	uem_result result;
// ##INIT_PORT_SECTION::START
<#list task.port as port>
  <#if port.direction == "INPUT">
	result = UFPort_Initialize(TASK_ID, "${port.name}", &port_${port.name});
	ERRIFGOTO(result, _EXIT);
	result = UFPort_Initialize(TASK_ID, "${port.name}_output", &port_${port.name}_output);
	ERRIFGOTO(result, _EXIT);
  </#if>
</#list>
// ##INIT_PORT_SECTION::END

    // TODO: task initialize code
_EXIT:
	return;
}

<#list task.port as port>
  <#if port.direction == "INPUT">
static unsigned char s_pInputData_${port.name}[s_nPortDataSize_${port.name}];
static unsigned char s_pOutputData_${port.name}[s_nPortDataSize_${port.name} * DISTRIBUTION_COUNT];
  </#if>
</#list>


/////////////////////////////////////
// go code
/////////////////////////////////////

TASK_GO
{
    int nLoop = 0;
    uem_result result;
   	int nDataRead = 0;
	int nDataWritten = 0;
	
    // TODO: task main code
<#list task.port as port>
  <#if port.direction == "INPUT">
	result = UFPort_ReadFromQueue(port_${port.name}, s_pInputData_${port.name}, s_nPortDataSize_${port.name}, 0, &nDataRead);
	ERRIFGOTO(result, _EXIT);  
  </#if>
</#list>

    for(nLoop = 0 ; nLoop < DISTRIBUTION_COUNT ; nLoop++)
    {
<#list task.port as port>
  <#if port.direction == "INPUT">  
    	memcpy(&s_pOutputData_${port.name}[s_nPortDataSize_${port.name} * nLoop], s_pInputData_${port.name}, s_nPortDataSize_${port.name});
  </#if>
</#list>
    }


<#list task.port as port>
  <#if port.direction == "INPUT">
	result = UFPort_WriteToQueue(port_${port.name}_output, s_pOutputData_${port.name}, s_nPortDataSize_${port.name} * DISTRIBUTION_COUNT, 0, &nDataWritten);
	ERRIFGOTO(result, _EXIT);
  </#if>
</#list>
_EXIT:
	return;
}



/////////////////////////////////////
// wrapup code
/////////////////////////////////////

TASK_WRAPUP
{
    // TODO: task wrapup code

}

TASK_CODE_END
