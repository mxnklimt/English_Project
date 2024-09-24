#define LORA_LINK_C_

#include "lora_link.h"
#include "sys_tick.h"
#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

unsigned char Record_import_flag = 0 ;                // ȫ��ͨ�ñ���
unsigned char Record_delete_flag = 0 ;
unsigned char Handle_import_delete_flag = 0 ;
unsigned char Handle_import_flag = 0 ;
unsigned char Handle_delete_flag = 0 ;
uint8_t key_count = 0;    
uint16_t Bind_Node_Mac = 0;                             // ȫ��ͨ�ñ���
uint16_t Bind_Node_Number = 0;                             // ȫ��ͨ�ñ���
// ȫ��ͨ�ñ���   
void Lora_Link_Net_Start(void)
{

  if((0 == Record_import_flag)&&(0 == Record_delete_flag)) 
	{	
    Handle_import_delete_flag	 = 1 ;			
	}	    
	if(Record_delete_flag ==1 )   GPIOB_ResetBits(GPIO_Pin_1);
  else GPIOB_SetBits(GPIO_Pin_1);
  NVIC_EnableIRQ(TMR3_IRQn);
}

void Lora_Link_Net_Success(void)
{
   node_auto_link();	 		   
}

void Lora_Link_Net_Faild(void)
{

   node_auto_link(); 
}
void Lora_Delete_Link_Net(void)
{
     node_auto_link();		
}

void node_auto_link(void)
{  
	 uint8_t i ;	
   if((0 != Bind_Node_Mac) && ((1 == Record_import_flag) || (1 == Record_delete_flag)))
		{
      if((1 == Record_import_flag) && (Send_Bind_Data == key_count)) //��λ����󶨣�hub�в�����������ݣ��ɹ���
			{  				
				key_count = Wait_Node_Data ;			
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);			
        debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);					
				debug_printf("Send bind data ok\r\n");
			}				
     else if((1 == Record_delete_flag) && (Delete_Node == key_count)) //��λ������hub�д���������ݣ��ɹ����
			{  		
//      unbind_set((unsigned char *)MACnet) ;  	
        GPIOB_ResetBits(GPIO_Pin_1); 				
				key_count = Wait_Node_Data ;	
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);
        debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);					 
        debug_printf("Send unbind data ok \r\n");				
		  }	
     else if((1 == Record_import_flag) && (Bind_Node_Fail == key_count))
			{		
				//��ʧ�ܣ�lora��һ�ζ�û�л���Ϣ ;�ڶ���û����Ϣͬ�������ﴦ��	
         GPIOB_ResetBits(GPIO_Pin_1);					
         for( i = 0; i < 5; i++)
          {
            GPIOB_ResetBits(GPIO_Pin_1);
            vTaskDelay(500);
            GPIOB_SetBits(GPIO_Pin_1);
            vTaskDelay(500);
          }	
         GPIOB_ResetBits(GPIO_Pin_1);		
					
         server_import_delete_date(Msgld, 0xFF0D ,IMPORT_DELETE_FAILD , Bind_Node_Number-Bind_Node_Mac,MACnet_set);							
         if(0 != Bind_Node_Mac)	 		Bind_Node_Mac =	 Bind_Node_Mac- 1; 		
				 key_count = 0 ;			
					
         debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
         debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
         debug_printf("key_count= %d : \r\n",key_count);			
         debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);						
			   debug_printf("Bind_failed����111 \r\n");							
			}			
     else if((1 == Record_delete_flag) && (Bind_Node_Fail == key_count))
			{		
				// ���ʧ�ܣ�loraû�л���Ϣ		
        GPIOB_ResetBits(GPIO_Pin_1);					
        for( i = 0; i < 5; i++)
         {
            GPIOB_ResetBits(GPIO_Pin_1);
            vTaskDelay(500);
            GPIOB_SetBits(GPIO_Pin_1);
            vTaskDelay(500);
          }	
        GPIOB_ResetBits(GPIO_Pin_1);			
					
        server_import_delete_date(Msgld, 0xFF0E ,IMPORT_DELETE_FAILD , Bind_Node_Number-Bind_Node_Mac,MACnet_set);
        if(0 != Bind_Node_Mac)	 		Bind_Node_Mac =	 Bind_Node_Mac- 1;	
				key_count = 0 ;	
					
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);	
        debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);						
			  debug_printf("Unbind_failed -111 \r\n");						
			}				
      else if((1 == Record_delete_flag) && (Send_Bind_Data == key_count))
			{    
				//��λ������hub��û��������ݣ������ж�Ϊ�󶨣��Ѹģ�,��ʱ��ظ������ɹ�			
        GPIOB_ResetBits(GPIO_Pin_0); 				
        for( i = 0; i < 5; i++)
         {
            GPIOB_ResetBits(GPIO_Pin_0);
            vTaskDelay(250);
            GPIOB_SetBits(GPIO_Pin_0);
            vTaskDelay(250);
         }		
        GPIOB_ResetBits(GPIO_Pin_0); 
//        server_import_delete_date(Msgld, 0xFF0E ,0X0000 , Bind_Node_Number-Bind_Node_Mac,MACnet_set);
        server_import_delete_date(Msgld, 0xFF0E ,IMPORT_DELETE_SUCCESS , Bind_Node_Number-Bind_Node_Mac,MACnet_set);				 
        if(0 != Bind_Node_Mac)	 		Bind_Node_Mac =	 Bind_Node_Mac- 1;	
				key_count = 0 ;		
				 
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);						 
        debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);					 
			  debug_printf("Unbind err \r\n");		
				
				 
			}	
      else if((1 == Record_import_flag) && (Delete_Node == key_count))
			{    //��λ����󶨣�hub����������ݣ������ٰ�һ��  
					
				key_count = Wait_Node_Data ;			
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);			
        debug_printf("Bind_Node_Mac= %d : \r\n",Bind_Node_Mac);					
				debug_printf("Send bind data ok\r\n");
			  debug_printf("  once again Bind  \r\n");		
			}	
     else	 if( ((Record_import_flag ==1) || (Record_delete_flag ==1)) && (key_count == 0 ) ) //Record_import_flag ==1 && Record_delete_flag ==0 &&  key_count == 0 	
		  {			 
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);			

	      memcpy(MACnet_set, &MAC_Record[Bind_Node_Mac-1],6);		 
        key_count = Start_Link_Node ;
        debug_printf("MACnet_set len=%02d: ", Bind_Node_Mac);
        log_hex((char *)MACnet_set, 6);					
		  }	
							
    }
	 else if((0 == Bind_Node_Mac) && ((1 == Record_import_flag)||(1 == Record_delete_flag)))
	 { 
		 if(1 == Record_import_flag)
			{
				Record_import_flag = 0 ;
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);					
				debug_printf("Bind loar all end \r\n");
			}
			else if (1 == Record_delete_flag)
			{
				Record_delete_flag = 0 ;	
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("key_count= %d : \r\n",key_count);					
        debug_printf("Unbind loar all end \r\n");				
			}
	 }


if( 1 == Handle_import_delete_flag )  //�ֶ���
	 {  
		 if(Send_Bind_Data == key_count && 1 == Handle_import_flag )
		  {			 
				 //��λ����󶨣�hub�в�����������ݣ��ɹ���
//        bind_set((unsigned char *)MACnet) ;   
        key_count = Wait_Node_Data ;					
        debug_printf("handle_send_bind_data ��\r\n");			
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
        debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
        debug_printf("key_count= %d : \r\n",key_count);	
				
		  }
		 else if(Delete_Node == key_count && 1 == Handle_delete_flag)
		  {
        key_count = Wait_Node_Data ;				
//        unbind_set((unsigned char *)MACnet) ;  
        debug_printf("handle_send_unbind_data ��\r\n");		
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
        debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
        debug_printf("key_count= %d : \r\n",key_count);					
		  }
     else if(Bind_Node_Fail == key_count && 1 == Handle_import_flag)
	    {	
        Handle_import_delete_flag	= 0		;	
        Handle_import_flag	 = 0 ;							
	      debug_printf("import lora_no_ack  \r\n");			
        GPIOB_ResetBits(GPIO_Pin_1);					
        for( i = 0; i < 5; i++)
         {
           GPIOB_ResetBits(GPIO_Pin_1);
           vTaskDelay(500);
           GPIOB_SetBits(GPIO_Pin_1);
           vTaskDelay(500);
         }	
        GPIOB_ResetBits(GPIO_Pin_1);		
	  	  key_count = 0 ;					 
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
        debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
        debug_printf("key_count= %d : \r\n",key_count);					 
	     }
     else if(Bind_Node_Fail == key_count && 1 == Handle_delete_flag)
	    {	
        Handle_import_delete_flag	= 0		;	
        Handle_delete_flag	 = 0 ;						
	      debug_printf("delete lora_no_ack  \r\n");		
        GPIOB_ResetBits(GPIO_Pin_1);					
        for( i = 0; i < 5; i++)
         {
           GPIOB_ResetBits(GPIO_Pin_1);
           vTaskDelay(500);
           GPIOB_SetBits(GPIO_Pin_1);
           vTaskDelay(500);
         }	
        GPIOB_ResetBits(GPIO_Pin_1);		
	  	  key_count = 0 ;						 
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
        debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
        debug_printf("key_count= %d : \r\n",key_count);					 
	     }	
     else if(Bind_Node_Fail == key_count )
	    {			
        GPIOB_ResetBits(GPIO_Pin_1);					
        for( i = 0; i < 5; i++)
         {
           GPIOB_ResetBits(GPIO_Pin_1);
           vTaskDelay(500);
           GPIOB_SetBits(GPIO_Pin_1);
           vTaskDelay(500);
         }	
         GPIOB_ResetBits(GPIO_Pin_1);					 
        Handle_import_delete_flag	= 0		;			
	  	  key_count = 0 ;	
				 
	      debug_printf("diyici lora_no_ack  \r\n");	
				 		
        debug_printf("Record_delete_flag= %d : \r\n",Record_delete_flag);		
        debug_printf("Record_import_flag= %d : \r\n",Record_import_flag);		
        debug_printf("Handle_import_flag= %d : \r\n",Handle_import_flag);			
        debug_printf("Handle_delete_flag= %d : \r\n",Handle_delete_flag);					
        debug_printf("key_count= %d : \r\n",key_count);					 
	     }				
  }

if( (0 ==Record_import_flag) && (0 == Record_delete_flag) && (0 == Handle_import_delete_flag) && (0 == Handle_import_flag)  && (0 == Handle_delete_flag) && (key_count != 0))	
{

	  	  key_count = 0 ;					
	      debug_printf("err err err err err !  \r\n");	 //9_22
	  	  key_count = 0 ;		
}
}

void Lora_Link_Net(void)
{
    switch(key_count)         //����״̬����
		 {
		   case Start_Link_Node :      
		    {
          Lora_Link_Net_Start();
			    break;
        }
		   case Send_Bind_Data :	
        {		 
          Lora_Link_Net_Success();
			    break;
        }	
		   case Bind_Node_Fail :	
        {		 
          Lora_Link_Net_Faild() ;				 
			    break;
        }
		   case Delete_Node :	
        {		 
          Lora_Delete_Link_Net();
			    break;
        }	
		   case Wait_Node_Data :		
			  {
				  NVIC_EnableIRQ(TMR3_IRQn);
			    break;			 
			  }	
		   case 0 :		
			  {
				  node_auto_link();
			    break;			 
			  }		    
		   default :       
		    {
					key_count = 0;	  //9_22
			    break;
		    }
	   }	
}	
