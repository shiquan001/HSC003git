/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2012-10-12     ST固件库版本 V2.1.0
*
*	Copyright (C), 2012-2013,   
*
*********************************************************************************************************
*/


#ifndef _HEART_LED_H_
#define _HEART_LED_H_

typedef enum 
{
	HEART_LED_INIT=0,// 初始化态

	HEART_LED_ON,
	HEART_LED_OFF,
	HEART_LED_LOOP,
		
	HEART_LED_MAX,
}ENUM_HEART_LED;

typedef struct
{
	u16 count_time;// 计数1ms运行一次
	u16 on_time;
	u16 off_time_t;//小周期闪烁off timer
	u16 off_time;//大周期
	ENUM_HEART_LED work_state;
}
HEART_LED_STRUCT;



extern 	HEART_LED_STRUCT g_HeartLED;

/* 供外部调用的函数声明 */
void Heart_Led_Init(void);
void Heart_LED_TEST(void);
void Heart_Led_ON_fast_04S(void);
void Heart_Led_ON_2S(void);
void App_address_sensor_Led_Loop(void);
void Heart_Led_Pro(void);

#endif

/*****************************    (END OF FILE) *********************************/
