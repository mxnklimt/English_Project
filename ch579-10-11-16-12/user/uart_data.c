#define UART_DATA_C_

#include "uart_data.h"
#include "db.h"
#include "sys_tick.h"
#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

unsigned char MACnet[6] = {0};                          // 缓存数据,全局通用
uint32_t main_len;                                     // 全局通用变量
uint32_t Node_counter = 0;                              // 全局通用变量



void bind_set(volatile unsigned char *mac_address)  //9.5? ????,?????????????????  ???????,????????
{
   unsigned long i;
	 unsigned char Date1[5] = {0x06,0, 0, 0, 0};


	 StTime.tm_year = GET_TIME.Year;
   StTime.tm_mon  = GET_TIME.Month;
   StTime.tm_day  = GET_TIME.Date;
   StTime.tm_hour = GET_TIME.Hours;
   StTime.tm_min  = GET_TIME.Minutes;
   StTime.tm_sec  = GET_TIME.Seconds;
	 i=GetUnixSecond(&StTime,timeZone);
		   	 						 
	 Date1[1]=(i>>24);	 
	 Date1[2]=(i>>16);
	 Date1[3]=(i>>8);	 
	 Date1[4]=i;
	
	 LoRa_SendData(Date1, mac_address, 5);						
}   

void unbind_set(volatile unsigned char *mac_address)  //9.5? ????,?????????????????  ???????,????????
{
	unsigned char Date1[1] = {0x07};
	LoRa_SendData(Date1, mac_address, 1);						
 }  

void uart_data(void) 
{
 if(usart0_idle_flag) 
  {
		db_res result;
    usart0_idle_flag = 0;
	  uint8_t i ;			
    main_len = rbCanRead(&rb_t_usart0_read);    /*获取模组返回的数据*/
    memset(usart0_read_buff_copy, 0, rb_t_usart0_read_buff_len);
    if (main_len > 0) // 如果里面的数据个数大于0
    {
      rbRead(&rb_t_usart0_read, usart0_read_buff_copy, main_len);
      if((0x68 == usart0_read_buff_copy[0]) &&(0xAA == usart0_read_buff_copy[1]) &&(usart0_read_buff_copy[2] <= 13)) 
        {
          if(0xA0 == usart0_read_buff_copy[10]) 
           {
						 debug_printf("[usart0<--loar_mac_data] len=%02d: ", main_len);
             log_hex((char *)usart0_read_buff_copy, main_len);	
						 
						 NVIC_DisableIRQ(TMR3_IRQn); 		
			       timer3_value = 0;		
						 
             memcpy(&MACnet[0], &usart0_read_buff_copy[4],6); // 将节点的MAC地址存入数组里边
						 result = db_manage_mac((char *)MACnet, sizeof(MACnet));	
						 
						 if(result == ADD_SUCCESS)
						 {								 
								key_count = Send_Bind_Data ;
							  if( 1 == Handle_import_delete_flag)  Handle_import_flag = 1 ;  
							  debug_printf("no the mac in the hub  ");
                log_hex((char *)MACnet, sizeof(MACnet));
						 }
						 else if(result == DELETE_SUCCESS)
						 {		
                GPIOB_ResetBits(GPIO_Pin_1); 							 
								key_count = Delete_Node ;                     //配网指示会关闭
							  if( 1 == Handle_import_delete_flag)  Handle_delete_flag = 1 ;  							 
							  debug_printf("had the mac in the hub  ");
                log_hex((char *)MACnet, sizeof(MACnet));							 
						 }
           } 
       } 
      else if((0x68 == usart0_read_buff_copy[0]) && (0xAA == usart0_read_buff_copy[1]) &&(usart0_read_buff_copy[2] == 17)) 
       {
				 NVIC_DisableIRQ(TMR3_IRQn); 		
			   timer3_value = 0;		
         debug_printf("[usart0<--loar_time] len=%02d: ", main_len);
         log_hex((char *)usart0_read_buff_copy, main_len);	   	 
				 if((0x08 == usart0_read_buff_copy[10]) && (1 == Handle_import_delete_flag) && (key_count == Wait_Node_Data) && 1 == Handle_import_flag )
				 {				 
					  db_add_mac((char *)MACnet, sizeof(MACnet)) ;	
					  Handle_import_delete_flag = 0 ;
            Handle_import_flag	= 0 ;				 
		      	key_count = 0 ;							 
            GPIOB_ResetBits(GPIO_Pin_1);						 
            debug_printf("Handle_MAC_Add_Success ->  ");							 
            debug_printf("MACnet len=%02d  :  ", 1);
            log_hex((char *)MACnet, 6);			
            debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
            debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
            debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
            debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
            debug_printf("key_count= %d : \r\n",key_count);						 
				 }	
        else  if(9 == usart0_read_buff_copy[10] && (1 == Handle_import_delete_flag) && (key_count == Wait_Node_Data) && 1== Handle_delete_flag)		
				 {

				    db_delete_mac((char *)MACnet)  ;	
					  Handle_import_delete_flag = 0 ;	
            Handle_delete_flag	 = 0 ;								
            debug_printf("Handle_MAC_Delete_Success ->");					
            debug_printf("MACnet_set len=%02d: ", 1);
            log_hex((char *)MACnet, 6);		
	 
            GPIOB_ResetBits(GPIO_Pin_0);	
            for( i = 0; i < 5; i++)
             {
               GPIOB_ResetBits(GPIO_Pin_0);
               vTaskDelay(250);
               GPIOB_SetBits(GPIO_Pin_0);
               vTaskDelay(250);
             }		
            GPIOB_ResetBits(GPIO_Pin_0); 	
		      	key_count = 0 ;								 
            debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
            debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
            debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
            debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
            debug_printf("key_count= %d : \r\n",key_count);							
				 }
        else if(8 == usart0_read_buff_copy[10] && 1 == Record_import_flag && Wait_Node_Data == key_count)
			   {
			      db_add_mac((char *)MACnet, sizeof(MACnet)) ;		
					 
            server_import_delete_date(Msgld, 0xFF0D ,IMPORT_DELETE_SUCCESS , Bind_Node_Number-Bind_Node_Mac,MACnet);				 
         
					  Bind_Node_Mac = Bind_Node_Mac - 1 ;
            key_count	= 0 ;						 
            GPIOB_ResetBits(GPIO_Pin_1);					 
            debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
            debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
            debug_printf("key_count= %d : \r\n",key_count);						 
            debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);					 
            Lora_Link_Net_Success();			 
			   }
        else if(9 == usart0_read_buff_copy[10] && 1 == Record_delete_flag)
			   {
			   		db_delete_mac((char *)MACnet)  ;				 
            GPIOB_ResetBits(GPIO_Pin_0); 				 
            for( i = 0; i < 5; i++)
             {
               GPIOB_ResetBits(GPIO_Pin_0);
               vTaskDelay(250);
               GPIOB_SetBits(GPIO_Pin_0);
               vTaskDelay(250);
             }		
           GPIOB_ResetBits(GPIO_Pin_0); 
	
           server_import_delete_date(Msgld, 0xFF0E ,IMPORT_DELETE_SUCCESS , Bind_Node_Number-Bind_Node_Mac,MACnet);						 
           Bind_Node_Mac = Bind_Node_Mac - 1 ;		
           key_count	= 0 ;	
						 
           debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
           debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
           debug_printf("key_count= %d : \r\n",key_count);						 
           debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);		
						 
           Lora_Delete_Link_Net()		;		 
			  }			 
			 
       }
			 
      else
       {
				 debug_printf("[usart0<--loar_test_data] len=%02d: ", main_len);
         log_hex((char *)usart0_read_buff_copy, main_len);	
	       Lora_Data_Upload_Server(usart0_read_buff_copy) ;					 
       }
    }
  } 
 else if(usart3_idle_flag) 
  {
    usart3_idle_flag = 0;   
    main_len = rbCanRead(&rb_t_usart3_read); /*获取模组返回的数据*/
    memset(usart3_read_buff_copy, 0, rb_t_usart3_read_buff_len);
    if(main_len > 0) // 如果里面的数据个数大于0
     {
      rbRead(&rb_t_usart3_read, usart3_read_buff_copy, main_len);
      debug_printf("[-->loar] len=%02d: ", main_len);
      log_hex((char *)usart3_read_buff_copy, main_len);
      //	uart_send_byte_it(0, usart3_read_buff_copy,
      // main_len);//通过串口三配置串口1（相当于配置lora）
     } 
  }
}

