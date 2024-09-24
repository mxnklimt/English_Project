#ifndef __RTC_H__
#define __RTC_H__


#ifndef RTC_C_
#define RTC_Ex_ extern
#else
#define RTC_Ex_
#endif

#include "CH57x_common.h" 


typedef struct RTC_TIME
{

	unsigned short Year;
	unsigned short Month;
	unsigned short Date;
	unsigned short Hours;
	unsigned short Minutes;
	unsigned short Seconds;	

	unsigned short Old_Year;
	unsigned short Old_Month;
	unsigned short Old_Date;
	unsigned short Old_Hours;
  unsigned short Old_Minutes;
	unsigned short Old_Seconds;		
	
}RTC_TIME;


RTC_Ex_ RTC_TIME GET_TIME;

RTC_Ex_ unsigned char count1;

void RTC_Int( void );
void RTC_Date( void );
//void test_30s(void) ; //��Ϊ����ʱ���

#endif  // __CH57x_ADC_H__	
