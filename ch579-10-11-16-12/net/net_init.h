#ifndef net_init_h
#define net_init_h

#ifndef net_init_c//���û�ж���
#define net_init_cx extern
#else
#define net_init_cx
#endif

#include "CH57x_common.h"
#include "CH57xNET.H"
#include <stdio.h>

net_init_cx UINT8 MACAddr[6];                             /* CH579MAC��ַ */

void net_init(void);

#endif
