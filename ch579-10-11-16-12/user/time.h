#ifndef __TIME_H
#define __TIME_H

#ifndef TIME_C_//如果没有定义
#define TIME_Ex_ extern
#else
#define TIME_Ex_
#endif


#include "CH57x_common.h"

TIME_Ex_ uint8_t timer1_value;
TIME_Ex_ uint8_t timer3_value;

void time0_init(void);
void time1_init(void);
void time3_init(void);
#endif

