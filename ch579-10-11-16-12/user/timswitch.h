#ifndef __TIMSWITCH_H__
#define __TIMSWITCH_H__

#ifndef TIMSWITCH_C_
#define TIMSWITCH_Ex_ extern
#else
#define TIMSWITCH_Ex_
#endif

#include "CH57x_common.h"

typedef	struct StructTM
{
  unsigned short tm_sec;
  unsigned short tm_min;
  unsigned short tm_hour;
  unsigned short tm_day;
  unsigned short tm_mon;
  unsigned short tm_year;
  unsigned short tm_week;
	
} StructTM;

TIMSWITCH_Ex_ struct StructTM StTime;

TIMSWITCH_Ex_ unsigned long timeZone;


int GetWeekFromDate(struct StructTM *tm) ;
void PrintTime(struct StructTM *tm);
unsigned long GetUnixSecond(struct StructTM *startt,int time_zone);
struct StructTM GetDateTimeFromUnixSecond(unsigned long unixSec,int startYear );
unsigned long get_rtc_tim_switch_sec(void);

#endif 	
