#ifndef net_dhcp_h
#define net_dhcp_h

#ifndef net_dhcp_c//如果没有定义
#define net_dhcp_cx extern
#else
#define net_dhcp_cx
#endif

#include "CH57x_common.h"
#include "CH57xNET.H"
#include <stdio.h>
#include "main.h"

net_dhcp_cx UINT8 net_dhcp_ip[4];   /* CH579IP地址 */
net_dhcp_cx UINT8 net_dhcp_gw[4];   /* CH579网关 */
net_dhcp_cx UINT8 net_dhcp_mask[4]; /* CH579子网掩码 */
net_dhcp_cx uint32_t main_len1;                          // 全局通用变量
void net_dhcp_while(void *callback);

#endif

