#ifndef net_dhcp_h
#define net_dhcp_h

#ifndef net_dhcp_c//���û�ж���
#define net_dhcp_cx extern
#else
#define net_dhcp_cx
#endif

#include "CH57x_common.h"
#include "CH57xNET.H"
#include <stdio.h>
#include "main.h"

net_dhcp_cx UINT8 net_dhcp_ip[4];   /* CH579IP��ַ */
net_dhcp_cx UINT8 net_dhcp_gw[4];   /* CH579���� */
net_dhcp_cx UINT8 net_dhcp_mask[4]; /* CH579�������� */
net_dhcp_cx uint32_t main_len1;                          // ȫ��ͨ�ñ���
void net_dhcp_while(void *callback);

#endif

