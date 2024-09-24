#ifndef LED_H_
#define LED_H_

#ifndef LED_C_//���û�ж���
#define LED_Cx_ extern
#else
#define LED_Cx_
#endif

#include "CH57x_common.h"

/*config pin*/
#define LED_PIN    GPIO_Pin_0
#define LED_MODE   GPIO_ModeOut_PP_20mA
#define LED_GPIO_MODE_CONFIG() (GPIOB_ModeCfg( LED_PIN, LED_MODE )) //��������Ϊ���ģʽ

#define LED_GPIO_SET_BITS() (GPIOB_SetBits(LED_PIN)) //������������ߵ͵�ƽ
#define LED_GPIO_RESET_BITS() (GPIOB_ResetBits(LED_PIN)) //������������ߵ͵�ƽ

#define LED_GPIO_READ_BITS() (GPIOB_ReadPortPin(LED_PIN)) //��ȡ����״̬

#define KEY0 GPIOB_ReadPortPin( GPIO_Pin_4 )


uint8_t KEY_Scan(void);
void key_init(void);
void led_set(int value);
char led_get(void);
#endif
