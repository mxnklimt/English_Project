#ifndef SEND_TCP_H_
#define SEND_TCP_H_

#ifndef SEND_TCP_C_//如果没有定义
#define SEND_TCP_Cx_ extern
#else
#define SEND_TCP_Cx_
#endif

#include "CH57x_common.h"
#include <flashdb.h>

#define PRE_HEADER_H    0xFF
#define PRE_HEADER_L    0xEE


//header
enum
{	
	TCP_HEADWE_REGISTER       = 0x01,	  //注册初始化设备
	TCP_HEADWE_LOG_ON         = 0x02,	  //
	TCP_HEADWE_RENEW          = 0x03,	  //
	TCP_HEADWE_CONTROL_ORDER  = 0x04,	  //

};

//encry Type
enum
{	
	TCP_Encry_Type_NO         = 0x00,	  //不加密
	TCP_Encry_Type_XXETA      = 0x01,	  //
	TCP_Encry_Type_OTHER      = 0x02,	  //
};
enum
{	
	TCP_version               = 0x0001,	  //不加密
};

//CRUF
enum
{	
	TCP_CRUF_REPLY            = 0x00,	  //
	TCP_CRUF_NO_REPLY         = 0x40,	  //
	TCP_CRUF_BACK             = 0x80,	  //
};
//ORDER
enum
{	
	TCP_ORDER_REGISTER        = 0xFF00,	  //
  TCP_ORDER_TIME            = 0xFF07,	  //	
	TCP_ORDER_BEAT            = 0xFF08,	  //
	TCP_ORDER_Node_Query      = 0xFF0C,	  //
	TCP_ORDER_batch_import    = 0xFF0D,	  //
	TCP_ORDER_batch_delete    = 0xFF0E,	  //
	TCP_ORDER_test_node       = 0x6000,	  //
	TCP_ORDER_Query_send      = 0x6001,	  //	
	TCP_ORDER_updata_back     = 0x6002,	  //	
};

//PRODUCT
enum
{	
	EML_LIGHT                 = 0x0001,	  //
	GAS_ALARM                 = 0x1000,	    //
};

//PRODUCT TYPE
enum
{	
	COMMOM_DEVICE             = 0x00,	  //
	GATEWAY_DEVICE            = 0x01,	    //
	CHILD_DEVICE              = 0x02,
};

//COMMUNICATE TYPE  
enum
{	
	COMMUNICATE_2G                        = 0x01,	  //
	COMMUNICATE_4G                        = 0x02,	    //
	COMMUNICATE_NB_LOT                    = 0x03,
	COMMUNICATE_TCP_CABLE                 = 0x04,	    //
	COMMUNICATE_WIFI                      = 0x05,	
};
//注册
enum
{	
	REGISTER_len                          = 54,	  //
	REGISTER_order_len                    = 48,	    //	
};
//时间
enum
{	
	TIME_len                              = 6,	  //
	TIME_order_len                        = 0,	    //	
};
//心跳
enum
{	
	BEAT_len                              = 6,	  //
	BEAT_order_len                        = 0,	    //	
};

//导入节点，删除节点回复包
enum
{	
	IMPORT_DELETE_len                     = 15,	  //
	IMPORT_DELETE_order_len               = 9,	    //	
};

//导入节点，删除节点回复包
enum
{	
	TEST_len                              = 8,	  //
	TEST_order_len                        = 2,	    //	
};
//lora数据上传
enum
{	
	LORA_DATA_len                         = 27,	  //
	LORA_DATA_order_len                   = 21,	    //	
};

//lora节点上传
enum
{	
	Lora_Node_Num_len                         = 10,	  //
	Lora_Node_Num_order_len                   = 4,	    //	
};

//导入节点，删除节点回复包
enum
{	
	IMPORT_DELETE_SUCCESS                 = 0X0000,	  //
	IMPORT_DELETE_FAILD                   = 0X0001,	    //	
};
//消息头
__packed typedef struct
{
  uint8_t   pre_headH;                  //
  uint8_t   pre_headL;                  //
  uint8_t   header;                     //	
  uint8_t   encrytype;                  //	
	uint16_t  version;		
	uint16_t  msgld;		                  //
  uint16_t  len;                        //
	uint8_t   curf;   	
	uint16_t  order; 
	uint16_t  order_len; 	
}struTCP_MSG; 

//注册数据包结构
__packed typedef struct
{    
	uint16_t  PRODUCT;		             //	     
	uint8_t   PRODUCT_TYPE;            //
	uint16_t  CHILD_NUMBER ;		
  uint8_t   COMMUNICATE_TYPE;	
	uint8_t   MAC_H[9] ;
  uint8_t   MAC_L[6] ;	
  uint8_t   SIM[20] ;	
	uint8_t   SIGNAL ;
	uint16_t  HARDWARE_VERSION ;	
  uint16_t  MCU_HARDWARE_VERSION ;		
	uint16_t  BEAT_TIME ;               //	  

}struTCP_REGISTER;

//回复数据包结构
__packed typedef struct
{  
  uint8_t   NUMBER; 
  uint8_t   MAC[6]; 	
	uint16_t  RESULT;		             //	       
}struTCP_BACK;

//测试回复数据包结构
__packed typedef struct
{  
	uint16_t  RESULT;		             //	       
}struTCP_TEST_BACK;
//lora上传数据包结构
__packed typedef struct
{    
	uint8_t   LORA_DATA_NUMBER;		             //	                                       //	  
  uint32_t  LORA_DATA_ID;
  uint32_t  LORA_DATA_TIME;	
  uint8_t	  LORA_MAC[6];
  uint8_t	  LORA_TEST_TYPE;	
  uint16_t	LORA_BAT_VOL;		
  uint16_t	LORA_LIGHT_VOL;			
	uint8_t   LORA_TEST_RESULT;		 	
}struTCP_LORA_DATA_UPLOAD;

//lora上传数据包结构
typedef struct
{    
	uint16_t   LORA_Node_NUMBER;		             //	
	uint8_t *  LORA_Node_BUF ;
  uint16_t   LORA_Node_RESULE;  
	 	
}struTCP_LORA_Node_Number;


/*config pin*/
void QueryAndSendData(char flag) ;

uint16_t mntohs(uint16_t data) ;
uint32_t Yntohs(uint32_t data) ;
uint8_t sum_verify(uint8_t* pBuf, uint16_t len);
uint8_t msg_send_packed(uint8_t *buf );
void Hub_register_server(void);
void Hub_GetTime_server(void);
void Hub_Beat_server(void);
void server_import_delete_date(unsigned short msgld, unsigned short order ,unsigned short result , unsigned short macnumber,unsigned char *buf);
void server_test_back_date(unsigned short msgld, unsigned short order ,unsigned short result ) ;
void Lora_Data_Upload_Server(unsigned char *buf);
bool send_all_data_cb(fdb_tsl_t tsl, void *arg) ;
bool send_not_upload_data_cb(fdb_tsl_t tsl, void *arg) ;
#endif
