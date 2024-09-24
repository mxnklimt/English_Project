#ifndef LORA_LINK_H_
#define LORA_LINK_H_

#ifndef LORA_LINK_C_//如果没有定义
#define LORA_LINK_Cx_ extern
#else
#define LORA_LINK_Cx_
#endif

#include "CH57x_common.h"

#define Start_Link_Node      1
#define Send_Bind_Data       2
#define Bind_Node_Fail       3
#define Delete_Node          4
#define Wait_Node_Data       5
#define OUT_TIME             6

LORA_LINK_Cx_ unsigned char Record_import_flag  ;                // 全局通用变量
LORA_LINK_Cx_ unsigned char Record_delete_flag  ;                // 全局通用变量
LORA_LINK_Cx_ unsigned char Handle_import_delete_flag  ;
LORA_LINK_Cx_ unsigned char Handle_import_flag  ;
LORA_LINK_Cx_ unsigned char Handle_delete_flag  ;
LORA_LINK_Cx_ uint8_t key_count;                         // 全局通用变量    
LORA_LINK_Cx_ uint16_t Bind_Node_Mac ;
LORA_LINK_Cx_ uint16_t Bind_Node_Number ;


void Lora_Link_Net_Start(void);
void Lora_Link_Net_Success(void);
void Lora_Link_Net_Faild(void);
void Lora_Delete_Link_Net(void);
void node_auto_link(void) ;
void Lora_Link_Net(void);

#endif

