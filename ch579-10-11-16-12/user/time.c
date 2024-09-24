#define TIME_C_


#include "time.h"


#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif
uint8_t timer1_value=0;
uint8_t timer3_value=0;
uint8_t timer1_time=0;
static unsigned char Date[1]={0X03} ;
static unsigned char Date1[1]={0X04} ;
unsigned char MACnet_set1[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
void time0_init(void)
{
	/*���ö�ʱ��0,��ʱ����ʱ����FREQ_SYS*/
	TMR0_TimerInit( 1*(FREQ_SYS/1000) );   //��ʱ1ms
	TMR0_ITCfg(ENABLE, TMR0_3_IT_CYC_END); //ʹ�ܶ�ʱ�������ж�
  //	NVIC_SetPriority(TMR0_IRQn,18);//�����ж����ȼ�(Ҳ���Բ�����,Ĭ��3)
	NVIC_EnableIRQ( TMR0_IRQn );
}


/*TMR0�����ж�*/
void TMR0_IRQHandler(void)
{
	if( TMR0_GetITFlag( TMR0_3_IT_CYC_END ) )
	{
		TMR0_ClearITFlag( TMR0_3_IT_CYC_END );
		CH57xNET_TimeIsr(CH57xNETTIMEPERIOD);/* ��ʱ���жϷ����� */
	//	net_dns_loop();               //9_9 ����      
	}
}

void time1_init(void)
{
	/*���ö�ʱ��1,��ʱ����ʱ����FREQ_SYS*/
	TMR1_TimerInit( 2*FREQ_SYS );   //��ʱ2s
	TMR1_ITCfg(ENABLE,TMR0_3_IT_CYC_END ); //ʹ�ܶ�ʱ�������ж�
  NVIC_SetPriority(TMR1_IRQn,18);//�����ж����ȼ�(Ҳ���Բ�����,Ĭ��3)
	NVIC_EnableIRQ( TMR1_IRQn );
}

/*TMR1�����ж�*/
void TMR1_IRQHandler(void)
{
	if( TMR1_GetITFlag( TMR0_3_IT_CYC_END ) )
	 {
		timer1_value++;
		TMR1_ClearITFlag( TMR0_3_IT_CYC_END );
		if(25 == timer1_value)
		 {
			 timer1_value=0;
       Hub_Beat_server()	;		 
		 }
	 }
}

void time3_init(void)
{
	/*���ö�ʱ��1,��ʱ����ʱ����FREQ_SYS*/
	TMR3_TimerInit( FREQ_SYS );   //��ʱ1s
	TMR3_ITCfg(ENABLE,TMR0_3_IT_CYC_END ); //ʹ�ܶ�ʱ�������ж�
  NVIC_SetPriority(TMR3_IRQn,19);//�����ж����ȼ�(Ҳ���Բ�����,Ĭ��3)
//	NVIC_EnableIRQ( TMR3_IRQn );
}

/*TMR0�����ж�*/
void TMR3_IRQHandler(void)
{
	if( TMR3_GetITFlag( TMR0_3_IT_CYC_END ) )
	{
		TMR3_ClearITFlag( TMR0_3_IT_CYC_END );
		
		if(Start_Link_Node == key_count  )
		{
	    if(1 == Handle_import_delete_flag)	
		  LoRa_SendData(Date,MACnet_set1,1);					
      else		LoRa_SendData(Date1,MACnet_set,1);	
		  timer3_value = timer3_value + 1 ;		
		  if(10 == timer3_value)                                            // 9.5?????,????????,??10??????????,????
		   {
			   NVIC_DisableIRQ(TMR3_IRQn);  
			   key_count = Bind_Node_Fail ;			 
			   timer3_value = 0;			 
		   }
	   }
	  else if(Wait_Node_Data == key_count)
	   {
		   timer3_value = timer3_value + 1 ;		
	     if(1 == Record_import_flag || 1 == Handle_import_flag)	
		    {
		      bind_set((unsigned char *)MACnet) ;     
		    }
		   else if(1 == Record_delete_flag || 1 == Handle_delete_flag)
		    {
          unbind_set((unsigned char *)MACnet) ; 		 
		    } 
		   if(10 == timer3_value)                                            // 9.5?????,????????,??10??????????,????
		    {
			    NVIC_DisableIRQ(TMR3_IRQn);  
			    key_count = Bind_Node_Fail ;			 
			    timer3_value = 0;			  	 	 
		    }
	    }			 
	}
		
}

