#define TIMSWITCH_C_

#include "timswitch.h"


#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif
/******* timestampʱ������� ��ʼ**********/
#define  SECOND_OF_DAY  86400   //һ�������
unsigned char DayOfMon[]={31,28,31,30,31,30,31,31,30,31,30,31};

//StructTM StTime;//����ʱ��ṹ����
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
* Description    : �������м�������ڼ�
* EntryParameter : ʱ��ṹ��ַ.
* ReturnValue    : ����һΪ0��...��������Ϊ6
********************************************************************************************************/
int GetWeekFromDate(struct StructTM *tm) 
{
  //�ڹ�ʽ��day��ʾ�����е�������mon��ʾ�·�����year��ʾ������
  //ע�⣺�ڹ�ʽ���и���������ʽ��ͬ�ĵط���
  //��1�º�2�¿�������һ���13�º�14�£����������2021-1-11����ɣ�2020-13-11�����빫ʽ���㡣
  int iWeek=  (tm->tm_mon==1||tm->tm_mon==2)? 
  (tm->tm_day+2*(tm->tm_mon+12)+3*(tm->tm_mon+12+1)/5+(tm->tm_year-1)+(tm->tm_year-1)/4-(tm->tm_year-1)/100+(tm->tm_year-1)/400) % 7:
  (tm->tm_day+2*tm->tm_mon+3*(tm->tm_mon+1)/5+tm->tm_year+tm->tm_year/4-tm->tm_year/100+tm->tm_year/400) % 7; 
  return iWeek;
}

/********************************************************************************************************
* FunctionName   : PrintTime()
* Description    : ���ڸ�ʽ��ӡ.
* EntryParameter : ʱ��ṹ��ַ.
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
* Description    : ���ڸ�ʽת��Ϊunix time
* EntryParameter : startt����UNIX�Ŀ�ʼʱ�䣻time_zoneҪת����ʱ��������ʱ��Ϊ8*3600
* ReturnValue    : UINXʱ�������
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
  CountDay = CountDay - time_zone;//����ʱ��ת��Ϊ����ʱ��8*3600
  return CountDay;

}

/********************************************************************************************************
* FunctionName   : GetDateTimeFromUnixSecond()
* Description    : unix timeת��Ϊ���ø�ʽ
* EntryParameter : unixSecʱ��������startYearʱ�俪ʼ�������
* ReturnValue    : ת����ɵ�ʱ��ṹ.
********************************************************************************************************/
struct StructTM GetDateTimeFromUnixSecond(unsigned long unixSec,int startYear )
{
  StructTM stt;
  unsigned int yearCounter,monCounter,dayCounter;
  unsigned long longDay;
  
  unixSec     = unixSec + 8*3600;         //ת��Ϊ����ʱ��
  longDay     = unixSec / SECOND_OF_DAY;  //UNIXһ���ж�����       
  unixSec     = unixSec % SECOND_OF_DAY;  //UNIX��������
  yearCounter = startYear;
  while(longDay > 365)
  {
    if(((yearCounter%4==0)&& (yearCounter%100!=0)) || (yearCounter%400==0))//����&& (yearCounter%100!=0)) || (yearCounter%400==0)
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

