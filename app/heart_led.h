/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-10-12     ST�̼���汾 V2.1.0
*
*	Copyright (C), 2012-2013,   
*
*********************************************************************************************************
*/


#ifndef _HEART_LED_H_
#define _HEART_LED_H_

typedef enum 
{
	HEART_LED_INIT=0,// ��ʼ��̬

	HEART_LED_ON,
	HEART_LED_OFF,
	HEART_LED_LOOP,
		
	HEART_LED_MAX,
}ENUM_HEART_LED;

typedef struct
{
	u16 count_time;// ����1ms����һ��
	u16 on_time;
	u16 off_time_t;//С������˸off timer
	u16 off_time;//������
	ENUM_HEART_LED work_state;
}
HEART_LED_STRUCT;



extern 	HEART_LED_STRUCT g_HeartLED;

/* ���ⲿ���õĺ������� */
void Heart_Led_Init(void);
void Heart_LED_TEST(void);
void Heart_Led_ON_fast_04S(void);
void Heart_Led_ON_2S(void);
void App_address_sensor_Led_Loop(void);
void Heart_Led_Pro(void);

#endif

/*****************************    (END OF FILE) *********************************/
