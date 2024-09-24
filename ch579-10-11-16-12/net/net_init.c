#define net_init_c

#include "net_init.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

#define KEEPLIVE_ENABLE                      1                                  /* ����KEEPLIVE���� */

/* ����Ļ�������ȫ�ֱ�������Ҫ���壬���е��� */
__align(16)UINT8    CH57xMACRxDesBuf[(RX_QUEUE_ENTRIES )*16];                   /* MAC������������������16�ֽڶ��� */
__align(4) UINT8    CH57xMACRxBuf[RX_QUEUE_ENTRIES*RX_BUF_SIZE];                /* MAC���ջ�������4�ֽڶ��� */
__align(4) SOCK_INF SocketInf[CH57xNET_MAX_SOCKET_NUM];                         /* Socket��Ϣ��4�ֽڶ��� */

UINT16 MemNum[8] = {CH57xNET_NUM_IPRAW,
                    CH57xNET_NUM_UDP,
                    CH57xNET_NUM_TCP,
                    CH57xNET_NUM_TCP_LISTEN,
                    CH57xNET_NUM_TCP_SEG,
                    CH57xNET_NUM_IP_REASSDATA,
                    CH57xNET_NUM_PBUF,
                    CH57xNET_NUM_POOL_BUF
                    };
 UINT16 MemSize[8] = {CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_IPRAW_PCB),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_UDP_PCB),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_TCP_PCB),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_TCP_PCB_LISTEN),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_TCP_SEG),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_IP_REASSDATA),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_PBUF) + CH57xNET_MEM_ALIGN_SIZE(0),
                    CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_PBUF) + CH57xNET_MEM_ALIGN_SIZE(CH57xNET_SIZE_POOL_BUF)
                    };
__align(4)UINT8 Memp_Memory[CH57xNET_MEMP_SIZE];
__align(4)UINT8 Mem_Heap_Memory[CH57xNET_RAM_HEAP_SIZE];
__align(4)UINT8 Mem_ArpTable[CH57xNET_RAM_ARP_TABLE_SIZE];
/******************************************************************************/
/* ����ʾ�������غ� */
#define RECE_BUF_LEN                          536  /* ���ջ������Ĵ�С */
/* CH57xNET��TCP��MSS����Ϊ536�ֽڣ���һ��TCP��������ݲ����Ϊ536�ֽ� */
/* TCPЭ��ջ���û������ڽ������أ��������ֵΪsocket�Ľ��ջ��������ȡ����趨 */
/* RX_QUEUE_ENTRIESʱҪ����MSS�ʹ���֮��Ĺ�ϵ�����細��ֵΪ4*MSS����Զ��һ�λᷢ�� */
/* 4��TCP�������RX_QUEUE_ENTRIESС��4�����Ȼ�ᵼ�����ݰ���ʧ���Ӷ�����ͨѶЧ�ʽ��� */
/* ����RX_QUEUE_ENTRIESҪ����( ����/MSS ),������socketͬʱ���д������������ݣ��� */ 
/* ����RX_QUEUE_ENTRIESҪ����(( ����/MSS )*socket����) �ڶ��socketͬʱ���д��������շ�ʱ */
/* Ϊ�˽�ԼRAM���뽫���ջ������ĳ�������ΪMSS */
																		
/* CH579��ض��� */
#define net_use_custom_mac 0 //1:ʹ���û��Զ����MAC 0:ʹ��ģ���Դ���MAC
UINT8 MACAddr[6] = {0x84,0xc2,0xe4,0x02,0x03,0x04};                             /* CH579MAC��ַ */
UINT8 IPAddr[4] = {192,168,128,15};                                              /* CH579IP��ַ */
UINT8 GWIPAddr[4] = {192,168,128,11};                                              /* CH579���� */
UINT8 IPMask[4] = {255,255,255,0};                                              /* CH579�������� */

/* ���ڵƶ��� ֻ���PB�� */
UINT16 CH57xNET_LEDCONN=GPIO_Pin_2;                                                  /* ����ָʾ�� PB4 */
UINT16 CH57xNET_LEDDATA=GPIO_Pin_3;                                                 /* ͨѶָʾ�� PB7 */ 


/*******************************************************************************
* Function Name  : net_initkeeplive
* Description    : keeplive��ʼ��
* Input          : None      
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef  KEEPLIVE_ENABLE
void net_initkeeplive(void)
{
    struct _KEEP_CFG  klcfg;

    klcfg.KLIdle = 20000;   /* ���и�msʱ������̽��*/
    klcfg.KLIntvl = 10000;  /* �����ms̽��һ��*/
    klcfg.KLCount = 5;      /* ̽�������� */
    CH57xNET_ConfigKeepLive(&klcfg);
}
#endif



/*******************************************************************************
* Function Name  : CH57xNET_LibInit
* Description    : ���ʼ������
* Input          : ip      ip��ַָ��
*                ��gwip    ����ip��ַָ��
*                : mask    ����ָ��
*                : macaddr MAC��ַָ�� 
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 CH57xNET_LibInit(/*const*/ UINT8 *ip,/*const*/ UINT8 *gwip,/*const*/ UINT8 *mask,/*const*/ UINT8 *macaddr)
{
    UINT8 i;
    struct _CH57x_CFG cfg;

    if(CH57xNET_GetVer() != CH57xNET_LIB_VER)return 0xfc;                       /* ��ȡ��İ汾�ţ�����Ƿ��ͷ�ļ�һ�� */
    CH57xNETConfig = LIB_CFG_VALUE;                                             /* ��������Ϣ���ݸ�������ñ��� */
    cfg.RxBufSize = RX_BUF_SIZE; 
    cfg.TCPMss   = CH57xNET_TCP_MSS;
    cfg.HeapSize = CH57x_MEM_HEAP_SIZE;
    cfg.ARPTableNum = CH57xNET_NUM_ARP_TABLE;
    cfg.MiscConfig0 = CH57xNET_MISC_CONFIG0;
    CH57xNET_ConfigLIB(&cfg);
    i = CH57xNET_Init(ip,gwip,mask,macaddr);
#ifdef  KEEPLIVE_ENABLE
    net_initkeeplive( );
#endif
    return (i);                      
}


/*******************************************************************************
* Function Name  : GetMacAddr
* Description    : ϵͳ��ȡMAC��ַ
* Input          : pMAC:ָ�������洢Mac��ַ�Ļ���
* Output         : None
* Return         : None
*******************************************************************************/
void GetMacAddr(UINT8 *pMAC)
{
	UINT8 transbuf[6],i;
	
	GetMACAddress(transbuf);
	for(i=0;i<6;i++)
	{
		pMAC[5-i]=transbuf[i];
	}
}

/*******************************************************************************
* Function Name  : IRQ_Handler
* Description    : IRQ�жϷ�����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_IRQHandler( void )						                             	/* ��̫���ж� */
{
	CH57xNET_ETHIsr();								                            /* ��̫���ж��жϷ����� */
}


void net_init(void)
{
	UINT16 state=0;
	#if (net_use_custom_mac==0)
		GetMacAddr(MACAddr);/* ��ȡMAC��ַ */
	#endif
	/*��ʼ��NET*/
	state = CH57xNET_LibInit(IPAddr,GWIPAddr,IPMask,MACAddr);                       /* ���ʼ�� */
	if(state!=CH57xNET_ERR_SUCCESS)
	{
		debug_printf("mStopIfError: %02X\r\n", (UINT16)state);
		DelayMs(10);
		SYS_ResetExecute();//����
	}
	NVIC_EnableIRQ(ETH_IRQn);
}









