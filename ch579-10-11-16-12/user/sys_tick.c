#define sys_tick_c_

#include "sys_tick.h"


#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

extern void xPortSysTickHandler(void);
void SysTick_Handler( void )      
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) //系统已经运行
	{
		xPortSysTickHandler();
	}
}

uint64_t SysTickTimer=0;

/*获取系统运行时间(ms)*/
uint64_t sys_tick_get_timer(void)
{
	return SysTickTimer;
}

