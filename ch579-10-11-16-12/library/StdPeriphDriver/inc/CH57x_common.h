

#ifndef __CH57x_COMM_H__
#define __CH57x_COMM_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define  NULL			0
#define  ALL			0xFFFF

#ifndef	 SUCCESS	 	 
#define  SUCCESS		0		
#define  FAILED			(!SUCCESS)
#endif

#ifndef	 ENABLE	 
#define  DISABLE		0		
#define  ENABLE			(!DISABLE)	 
#endif

#define Debug_UART0        0
#define Debug_UART1        1
#define Debug_UART2        2
#define Debug_UART3        3
 
#ifdef DEBUG
#include <stdio.h>
#endif
   
#ifndef	 FREQ_SYS  
#define  FREQ_SYS		32000000   
#endif   

#if ( CLK_OSC32K == 1 )
#define CAB_LSIFQ     	32000
#else
#define CAB_LSIFQ     	32768
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "CH57x_clk.h"
#include "CH57x_uart.h"
#include "CH57x_gpio.h"
#include "CH57x_lcd.h"
#include "CH57x_flash.h"
#include "CH57x_pwr.h"
#include "CH57x_pwm.h"	 
#include "CH57x_adc.h"
#include "CH57x_sys.h"
#include "CH57x_timer.h"	 
#include "CH57x_spi.h"
#include "CH579SFR.h"
#include "core_cm0.h"
#include "CH57xNET.h"

#include "net_init.h"
#include "net_dhcp.h"
#include "net_dns.h"
#include "net_tcp_client.h"
#include "net_udp.h"

#include "uart.h"
#include "tcp_data.h"
#include "uart_data.h"
#include "send_tcp.h"  
#include "timswitch.h"	
#include "SPIFlash.H"	
#include "rtc.h"
#include "time.h"
#include "led.h"
#include "tcp_data_handle.h"
#include "lora_link.h"
#include "main.h"
#include "w25q32.h"
#include "Code_Flash.h"
#include <db.h>
#include "sfud.h"
#include "fal.h"
#include <flashdb.h>

#define DelayMs(x)      mDelaymS(x)	  
#define DelayUs(x)      mDelayuS(x)	  


#ifdef __cplusplus
}
#endif

#endif  // __CH57x_COMM_H__	 

