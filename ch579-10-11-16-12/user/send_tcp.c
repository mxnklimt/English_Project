#define SEND_TCP_C_

#include "send_tcp.h"
//#include <inet.h>
#include <db.h>

#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

static  struTCP_MSG               SendMsg;
static  struTCP_REGISTER          REGISTER_DATA;
static  struTCP_BACK              BACK_RESULT_DATA ;
static  struTCP_TEST_BACK         BACK_TEST_RESULT_DATA ;
static  struTCP_LORA_DATA_UPLOAD  LORA_DATA_UPLOAD ;
static  struTCP_LORA_Node_Number  LORA_Node_Number;
static  uint8_t SendBuf[64];
static  uint8_t msg_len;


extern struct fdb_kvdb kvdb;

uint16_t mntohs(uint16_t data)
{
	uint16_t  data1 = 0;
	data1 = data;
	data1 = (data1<<8)|(data>>8) ;
  return data1 ;
}

uint32_t Yntohs(uint32_t data)
{
	uint16_t i ;
	uint16_t  data2[2] = {0 };	
	uint32_t  data1 = 0;	
	
  for(i=0 ; i<2; i++)
	{
	  data2[i] = data>>(i*16) ;
	  data2[i] = mntohs(data2[i]) ; 
		data1 += data2[i] ;
		data1 = data1 << ((1-i)*16) ;
	}
  return data1 ;

}
/**
 * @brief     发送数据包时计算校验和
 * @param     pBuf:需要校验的数据指针
 *            len:数据长度
 * @return    返回校验数据
 * @details   
*/

uint8_t sum_verify(uint8_t* pBuf, uint16_t len)
{
	    uint8_t data_sum = 0 ;
	    uint8_t i ;
      for (i = 0; i < len; i++) data_sum = pBuf[i] + data_sum;
      data_sum = ((~data_sum) + 1);
	    return data_sum ;
       // 校验和
}
/**
 * @brief     接收数据包时验证数据是否正确
 * @param     pBuf:需要校验的数据指针
*             len:数据长度
 * @return    0:校验错误 
 *            1:校验正确
 * @details     
*/
uint8_t msg_send_packed(uint8_t *buf )
{
	uint8_t check;

	msg_len = sizeof(SendMsg) + mntohs(SendMsg.order_len);
	memcpy(SendBuf,&SendMsg,sizeof(SendMsg));		   
	if(mntohs(SendMsg.order_len)> 0) memcpy(SendBuf+sizeof(SendMsg),buf,mntohs(SendMsg.order_len)); //??dat??
	check = sum_verify(SendBuf,msg_len);
	SendBuf[msg_len] = check;
	msg_len += 1;
	
  if(0 == net_tcp_client_send(&net_tcp_client, SendBuf, msg_len) )  
		return 1 ;
	else 
	 return 0;	
	
}

//注册函数，主动发送需要回复
void Hub_register_server(void)
{
  static unsigned short msgld = 0;	
	msgld = msgld + 1;
	
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	//
	SendMsg.header    = TCP_HEADWE_REGISTER ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;  //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(REGISTER_len) ;	
	SendMsg.curf      = TCP_CRUF_REPLY ;
	SendMsg.order     = mntohs(TCP_ORDER_REGISTER) ;
	SendMsg.order_len = mntohs(REGISTER_order_len) ;

	
	REGISTER_DATA.PRODUCT              = mntohs(EML_LIGHT) ;
	REGISTER_DATA.PRODUCT_TYPE         = GATEWAY_DEVICE ;
	REGISTER_DATA.CHILD_NUMBER         = mntohs(Node_counter) ;
	REGISTER_DATA.COMMUNICATE_TYPE     = COMMUNICATE_TCP_CABLE ;	
	REGISTER_DATA.SIGNAL               = 0 ;
  REGISTER_DATA.HARDWARE_VERSION     = mntohs(0) ;
  REGISTER_DATA.MCU_HARDWARE_VERSION = mntohs(0);	
  REGISTER_DATA.BEAT_TIME            = mntohs(0);	
	memset(&REGISTER_DATA.MAC_H, 0, sizeof(REGISTER_DATA.MAC_H));
  GetMACAddress(REGISTER_DATA.MAC_L);	
  memset(&REGISTER_DATA.SIM, 0, sizeof(REGISTER_DATA.SIM));  
  	
	msg_send_packed((uint8_t *)&REGISTER_DATA) ;		
	
  debug_printf("[<--tcp_register<--hub] len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);	
}

//时间函数，主动发送需要回复
void Hub_GetTime_server(void)
{
  static unsigned short msgld = 0;
	
  msgld = msgld + 1;
	
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	//
	SendMsg.header    = GATEWAY_DEVICE ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;  //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(TIME_len) ;	
	SendMsg.curf      = TCP_CRUF_REPLY ;
	SendMsg.order     = mntohs(TCP_ORDER_TIME) ;
	SendMsg.order_len = mntohs(TIME_order_len) ;	
	
  msg_send_packed(NULL);	
  debug_printf("[<--get_time<--hub] len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);	
}
//心跳函数，主动发送需要回复
void Hub_Beat_server(void)
{
  static unsigned short msgld = 0;
  msgld = msgld + 1;
	
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	//
	SendMsg.header    = GATEWAY_DEVICE ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;  //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(BEAT_len) ;	
	SendMsg.curf      = TCP_CRUF_REPLY ;
	SendMsg.order     = mntohs(TCP_ORDER_BEAT) ;
	SendMsg.order_len = mntohs(BEAT_order_len) ;	

  msg_send_packed(NULL);		
	debug_printf("[<--tcp_beat<--hub] len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);
}
//接收消息，仅回复
void server_import_delete_date(unsigned short msgld, unsigned short order ,unsigned short result , unsigned short macnumber,unsigned char *buf) 
{
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	           //
	SendMsg.header    = GATEWAY_DEVICE ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(IMPORT_DELETE_len) ;	
	SendMsg.curf      = TCP_CRUF_BACK ;
	SendMsg.order     = mntohs(order) ;
	SendMsg.order_len = mntohs(IMPORT_DELETE_order_len) ;	
	
	
  BACK_RESULT_DATA.NUMBER = macnumber ;    
	BACK_RESULT_DATA.RESULT = mntohs(result) ;	
  memcpy(	BACK_RESULT_DATA.MAC ,buf,6);		
	
	msg_send_packed((uint8_t *)&BACK_RESULT_DATA) ;			
  debug_printf("tcp_import_delete_node <-- hub len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);

}

//接收消息，仅回复
void server_test_back_date(unsigned short msgld, unsigned short order ,unsigned short result ) 
{
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	           //
	SendMsg.header    = GATEWAY_DEVICE ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(TEST_len) ;	
	SendMsg.curf      = TCP_CRUF_BACK ;
	SendMsg.order     = mntohs(order) ;
	SendMsg.order_len = mntohs(TEST_order_len) ;	
	
	BACK_TEST_RESULT_DATA.RESULT = mntohs(result) ;	

	msg_send_packed((uint8_t *)&BACK_TEST_RESULT_DATA) ;			
  debug_printf("tcp_import_delete_node <-- hub len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);

}
//节点数据上传，主动发送要求回复
void Lora_Data_Upload_Server(unsigned char *buf) 
{
  static unsigned short msgld = 0;
  static unsigned long Data_ip = 0;	
  msgld = msgld + 1;
  Data_ip = Data_ip + 1;	
 
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	           //
	SendMsg.header    = GATEWAY_DEVICE ;           //
	SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(LORA_DATA_len) ;	
	SendMsg.curf      = TCP_CRUF_REPLY ;
	SendMsg.order     = mntohs(TCP_ORDER_updata_back) ;
	SendMsg.order_len = mntohs(LORA_DATA_order_len) ;		

	
  LORA_DATA_UPLOAD.LORA_DATA_NUMBER = 1 ;
  LORA_DATA_UPLOAD.LORA_DATA_ID 	  = Yntohs(Data_ip) ;
	LORA_DATA_UPLOAD.LORA_DATA_TIME   = Yntohs(get_rtc_tim_switch_sec());
	LORA_DATA_UPLOAD.LORA_TEST_TYPE   = buf[10] ;
  LORA_DATA_UPLOAD.LORA_BAT_VOL     = buf[11] ;	
	LORA_DATA_UPLOAD.LORA_BAT_VOL     = mntohs((LORA_DATA_UPLOAD.LORA_BAT_VOL<<8) + buf[12] );
	LORA_DATA_UPLOAD.LORA_LIGHT_VOL   = buf[13] ;
	LORA_DATA_UPLOAD.LORA_LIGHT_VOL   = mntohs((LORA_DATA_UPLOAD.LORA_LIGHT_VOL<<8) + buf[14] );	
	LORA_DATA_UPLOAD.LORA_TEST_RESULT = buf[15] ;
  memcpy(LORA_DATA_UPLOAD.LORA_MAC ,&buf[4],6);	
	

	if(APPEND_SUCCESS == db_append_log(&LORA_DATA_UPLOAD, sizeof(LORA_DATA_UPLOAD)))
	 {
		 debug_printf("Data append success !\r\n");
	 }
	else
	 {
	   debug_printf("Data append fail !\r\n");
	 }
	if (msg_send_packed((uint8_t *)&LORA_DATA_UPLOAD)) // 发送数据给server,上传成功，改变该条数据的STATUS， WRITE->USER_STATUS1 
   {
     db_set_current_tsl_status(Yntohs(LORA_DATA_UPLOAD.LORA_DATA_TIME));//WRITE -> USER_STATUS1
		 
		 
		 debug_printf("Data upload success !\r\n");      //USER_STATUS1表示已经上传成功
   } 
	else	
	 {
		 //未上传成功STATUS 默认为 WRITE
		 debug_printf("Data upload fail !\r\n");		 
   }
	
  debug_printf("[--hub_node_data--tcp<--hub] len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);

}	
//查询发送hub存储的所有数据信息
bool send_all_data_cb(fdb_tsl_t tsl, void *arg) 
{
  static unsigned short msgld = 0;
  static unsigned long Data_ip = 0;	
  struct fdb_blob blob;	
  struTCP_LORA_DATA_UPLOAD  LORA_DATA_UPLOADBUF;	
  fdb_tsdb_t db = arg;
  msgld = msgld + 1;
  Data_ip = Data_ip + 1;
	
  fdb_blob_read(
      (fdb_db_t)db,
      fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &LORA_DATA_UPLOADBUF, sizeof(LORA_DATA_UPLOADBUF))));	

	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	           //
	SendMsg.header    = GATEWAY_DEVICE ;           //
	SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(LORA_DATA_len) ;	
	SendMsg.curf      = TCP_CRUF_REPLY ;
	SendMsg.order     = mntohs(TCP_ORDER_updata_back) ;
	SendMsg.order_len = mntohs(LORA_DATA_order_len) ;		

  memcpy(&LORA_DATA_UPLOAD ,&LORA_DATA_UPLOADBUF,sizeof(LORA_DATA_UPLOADBUF));		

	msg_send_packed((uint8_t *)&LORA_DATA_UPLOAD) ;	
	
  debug_printf("[<--tcp_all——data<--hub] len=%02d: ", msg_len);
  log_hex((char *)SendBuf, msg_len);		
	return true;		
}
//查询发送所有未上传的数据信息
bool send_not_upload_data_cb(fdb_tsl_t tsl, void *arg)
{
  static unsigned short msgld = 0;
  static unsigned long Data_ip = 0;	
  struct fdb_blob blob;	
  struTCP_LORA_DATA_UPLOAD  LORA_DATA_UPLOADBUF;	
  fdb_tsdb_t db = arg;
  msgld = msgld + 1;
  Data_ip = Data_ip + 1;
	
  fdb_blob_read(
      (fdb_db_t)db,
      fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &LORA_DATA_UPLOADBUF, sizeof(LORA_DATA_UPLOADBUF))));	

  if(tsl->status == FDB_TSL_USER_STATUS1)
    return false;
  else if(tsl->status == FDB_TSL_WRITE)
	 {
	   SendMsg.pre_headH = PRE_HEADER_H ;	
	   SendMsg.pre_headL = PRE_HEADER_L ;	           //
	   SendMsg.header    = GATEWAY_DEVICE ;           //
	   SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	   SendMsg.version   = mntohs(TCP_version);
	   SendMsg.msgld     = mntohs(msgld);
	   SendMsg.len       = mntohs(LORA_DATA_len) ;	
	   SendMsg.curf      = TCP_CRUF_REPLY ;
	   SendMsg.order     = mntohs(TCP_ORDER_updata_back) ;
	   SendMsg.order_len = mntohs(LORA_DATA_order_len) ;	
		 
     memcpy(&LORA_DATA_UPLOAD ,&LORA_DATA_UPLOADBUF,sizeof(LORA_DATA_UPLOADBUF));		

	   msg_send_packed((uint8_t *)&LORA_DATA_UPLOAD) ;	
		 
		 fdb_tsl_set_status(db, tsl, FDB_TSL_USER_STATUS1);//
		 
     debug_printf("[<--tcp_no_upload—data<--hub] len=%02d: ", msg_len);
     log_hex((char *)SendBuf, msg_len);		
  }		
	   return true;	
}	


void Server_Query_Lora_Node_Num( unsigned short msgld,unsigned short order,unsigned short node)
{
  uint8_t * SBuff =NULL;	
  uint16_t  node_mac=0;
	unsigned char i =0;	
	
	node_mac = node * 6 ;
	
	SendMsg.pre_headH = PRE_HEADER_H ;	
	SendMsg.pre_headL = PRE_HEADER_L ;	           //
	SendMsg.header    = GATEWAY_DEVICE ; //
	SendMsg.encrytype = TCP_Encry_Type_NO ;        //
	SendMsg.version   = mntohs(TCP_version);
	SendMsg.msgld     = mntohs(msgld);
	SendMsg.len       = mntohs(Lora_Node_Num_len+node_mac) ;	
	SendMsg.curf      = TCP_CRUF_BACK ;
	SendMsg.order     = mntohs(order) ;
	SendMsg.order_len = mntohs(Lora_Node_Num_order_len+node_mac) ;	

	LORA_Node_Number.LORA_Node_NUMBER = mntohs(node);
	LORA_Node_Number.LORA_Node_BUF    = (uint8_t*)malloc(node_mac);
  LORA_Node_Number.LORA_Node_RESULE = mntohs(0);	
	
	struct fdb_kv_iterator iterator;
  fdb_kv_t cur_kv;
  struct fdb_blob blob;
  size_t data_size;
  uint8_t *data_buf;

  fdb_kv_iterator_init(&kvdb, &iterator);
		
  while (fdb_kv_iterate(&kvdb, &iterator)) 
	 {
     cur_kv = &(iterator.curr_kv);
     data_size = (size_t) cur_kv->value_len;
     data_buf = (uint8_t *) malloc(data_size);
     if (data_buf == NULL) {
        FDB_INFO("Error: malloc failed.\r\n");
            break;
     }
     fdb_blob_read((fdb_db_t)&kvdb, fdb_kv_to_blob(cur_kv, fdb_blob_make(&blob, data_buf, data_size)));
		 FDB_INFO("KV iterator read ");
		 log_hex((char *) data_buf, data_size);
		 memcpy(&LORA_Node_Number.LORA_Node_BUF[17+i*6], data_buf, data_size);
		 i++;
     free(data_buf);
	}				
	SBuff = (uint8_t*)malloc(19+node_mac) ;
	
	msg_len = sizeof(SendMsg) + mntohs(SendMsg.order_len);
	memcpy(SBuff,&SendMsg,sizeof(SendMsg));		
	memcpy(SBuff,&LORA_Node_Number,2);
	if(mntohs(SendMsg.order_len)> 0) 
	memcpy(SBuff+sizeof(SendMsg)+2,LORA_Node_Number.LORA_Node_BUF,node_mac); //??dat??
	free(LORA_Node_Number.LORA_Node_BUF);
  memcpy(SBuff,&LORA_Node_Number.LORA_Node_RESULE,2);
	SBuff[msg_len] = 	sum_verify(SendBuf,msg_len);;
	msg_len += 1;
  net_tcp_client_send(&net_tcp_client, SBuff, msg_len);	
	
  debug_printf("[--HUB_node--tcp<--hub] len=%02d: ", msg_len);
  log_hex((char *)SBuff, msg_len);		
	free(SBuff);	
	
}



