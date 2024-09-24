#define TIMSWITCH_C_

#include "timswitch.h"


#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif
/******* timestamp时间戳函数 开始**********/
#define  SECOND_OF_DAY  86400   //一天多少秒
unsigned char DayOfMon[]={31,28,31,30,31,30,31,31,30,31,30,31};

//StructTM StTime;//声明时间结构变量
struct StructTM StTime;

//StTime.tm_year = 2022;
//StTime.tm_mon  = 6;
//StTime.tm_day  = 16;
//StTime.tm_hour = 20;
//StTime.tm_min  = 0;
//StTime.tm_sec  = 21;

unsigned long timeZone =8*3600;

/********************************************************************************************************
* FunctionName   : printTime()
* Description    : 从日期中计算出星期几
* EntryParameter : 时间结构地址.
* ReturnValue    : 星期一为0，...，星期日为6
********************************************************************************************************/
int GetWeekFromDate(struct StructTM *tm) 
{
  //在公式中day表示日期中的日数，mon表示月份数，year表示年数。
  //注意：在公式中有个与其他公式不同的地方：
  //把1月和2月看成是上一年的13月和14月，例：如果是2021-1-11则换算成：2020-13-11来代入公式计算。
  int iWeek=  (tm->tm_mon==1||tm->tm_mon==2)? 
  (tm->tm_day+2*(tm->tm_mon+12)+3*(tm->tm_mon+12+1)/5+(tm->tm_year-1)+(tm->tm_year-1)/4-(tm->tm_year-1)/100+(tm->tm_year-1)/400) % 7:
  (tm->tm_day+2*tm->tm_mon+3*(tm->tm_mon+1)/5+tm->tm_year+tm->tm_year/4-tm->tm_year/100+tm->tm_year/400) % 7; 
  return iWeek;
}

/********************************************************************************************************
* FunctionName   : PrintTime()
* Description    : 日期格式打印.
* EntryParameter : 时间结构地址.
* ReturnValue    : 
********************************************************************************************************/
void PrintTime(struct StructTM *tm) 
{
  debug_printf("%04d/%02d/%02d %02d:%02d:%02d   %02d\r\n",
                tm->tm_year,
                tm->tm_mon,
                tm->tm_day,
                tm->tm_hour,
                tm->tm_min,
                tm->tm_sec,
                tm->tm_week);
}

/********************************************************************************************************
* FunctionName   : GetUnixSecond()
* Description    : 日期格式转化为unix time
* EntryParameter : startt计算UNIX的开始时间；time_zone要转化的时区，北京时间为8*3600
* ReturnValue    : UINX时间戳秒数
********************************************************************************************************/
unsigned long GetUnixSecond(struct StructTM *startt,int time_zone)
{
  unsigned short  rYear,i,Cyear=0;
  unsigned char   rMon,rDay,rHour,rMin,rSec;
  unsigned long   CountDay = 0;

  rSec  = startt->tm_sec;
  rMin  = startt->tm_min;
  rHour = startt->tm_hour;
  rDay  = startt->tm_day;
  rMon  = startt->tm_mon;
  rYear = startt->tm_year;
 
  for(i = 1970; i < rYear; i++)
  {
    if(((i%4==0) && (i%100!=0)) || (i%400==0))  Cyear++;
  }
  CountDay = Cyear * 366 + (rYear-1970-Cyear) * 365;
 
  for(i=1; i<rMon;i++)  {
    if((i==2) && (((rYear%4==0)&&(rYear%100!=0)) || (rYear%400==0)))
        CountDay += 28;
  else
    CountDay += DayOfMon[i-1];
  }
  CountDay += (rDay-1);
  CountDay = CountDay*SECOND_OF_DAY + (unsigned long)rHour*3600 + (unsigned long)rMin*60 + rSec;
  CountDay = CountDay - time_zone;//北京时间转化为世界时间8*3600
  return CountDay;

}

/********************************************************************************************************
* FunctionName   : GetDateTimeFromUnixSecond()
* Description    : unix time转化为常用格式
* EntryParameter : unixSec时间秒数；startYear时间开始计算年份
* ReturnValue    : 转换完成的时间结构.
********************************************************************************************************/
struct StructTM GetDateTimeFromUnixSecond(unsigned long unixSec,int startYear )
{
  StructTM stt;
  unsigned int yearCounter,monCounter,dayCounter;
  unsigned long longDay;
  
  unixSec     = unixSec + 8*3600;         //转换为北京时间
  longDay     = unixSec / SECOND_OF_DAY;  //UNIX一共有多少天       
  unixSec     = unixSec % SECOND_OF_DAY;  //UNIX当天秒数
  yearCounter = startYear;
  while(longDay > 365)
  {
    if(((yearCounter%4==0)&& (yearCounter%100!=0)) || (yearCounter%400==0))//闰年&& (yearCounter%100!=0)) || (yearCounter%400==0)
    {
      longDay -= 366; 
    }else
    {
      longDay -= 365;
    }
      yearCounter++;
  }
  if((longDay == 365) && !(((yearCounter%4==0)&&(yearCounter%100!=0)) || (yearCounter%400==0)))
  {
    longDay -= 365;
    yearCounter++;
  }
  
  for(monCounter=0;monCounter<12;monCounter++)  
  {
    if((monCounter==1) && (((yearCounter%4==0)&&(yearCounter%100!=0)) || (yearCounter%400==0)))
    {
    dayCounter = 29;
    }else
    {
      dayCounter = DayOfMon[monCounter];
    }
    if(longDay >= dayCounter)
    {
      longDay -= dayCounter;
    }
    else
    {
      break;
    }
  }
   
   stt.tm_year  = yearCounter;
   stt.tm_mon   = monCounter + 1;
   stt.tm_day   = longDay + 1;
   stt.tm_hour  = (unixSec/3600)%24;
   stt.tm_min   = (unixSec%3600)/60;
   stt.tm_sec   = (unixSec%3600)%60;
   stt.tm_week =  GetWeekFromDate(&stt);
    return stt;
}

unsigned long get_rtc_tim_switch_sec(void)
{
	 unsigned long i;
	
	 RTC_GetTime( &GET_TIME.Year, &GET_TIME.Month, &GET_TIME.Date, &GET_TIME.Hours, &GET_TIME.Minutes, &GET_TIME.Seconds ); 	
	
   StTime.tm_year = GET_TIME.Year;
   StTime.tm_mon  = GET_TIME.Month;
   StTime.tm_day  = GET_TIME.Date;
   StTime.tm_hour = GET_TIME.Hours;
   StTime.tm_min  = GET_TIME.Minutes;
   StTime.tm_sec  = GET_TIME.Seconds;
	
	 i=GetUnixSecond(&StTime,timeZone);
	
	 return i;
	
}

