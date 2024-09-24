#define sys_tick_c_

#include "sys_tick.h"


#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

extern void xPortSysTickHandler(void);
void SysTick_Handler( void )      
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) //ϵͳ�Ѿ�����
	{
		xPortSysTickHandler();
	}
}

uint64_t SysTickTimer=0;

/*��ȡϵͳ����ʱ��(ms)*/
uint64_t sys_tick_get_timer(void)
{
	return SysTickTimer;
}

