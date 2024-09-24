#ifndef __MAIN_H_
#define __MAIN_H_

#ifndef MAIN_Cx_
#define MAIN_Cx_ extern
#else
#define MAIN_Cx_
#endif

#include "CH57x_common.h"


//缓存数据使用                                          // 全局通用变量
MAIN_Cx_ char send_flash_data_flag;                            // 全局通用变量
MAIN_Cx_ uint8_t number_not_upload ;



#endif

