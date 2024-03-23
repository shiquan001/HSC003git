#ifndef _APP_CONFIG_H__
#define _APP_CONFIG_H__


#include "bsp.h"

#define TIMER_SMG_T 200//200ms

#define NUMBERS_UPDATE_CONFIG 4 //������ȡ�Ĳ���2�ı���

/*
	  С�����Ĺ��ܣ�
	 1��װ��������1-4��
	 2��װ�ø߶ȣ�6-15��
	 3�����������ȣ�1-4����5000��1��2��3��
	 4�����������ȣ�1-4����5��10��15��20��
	 5��ˮƽ����ֵ��0 - 999��
	 6����ֱ����ֵ��0 - 999��
	 7��ˮƽ��ת�Ƕȡ���180-360��
*/
typedef struct
{	/*  ����Ĳ��� */	
	u8 power_onoff; //true ����false û��ע�ᡣ
	u8 setAcqTimer;   //4.3.6.9.���ô������Ĳɼ�Ƶ�ʣ����ӣ�1-255

	//Name��T_up��T_down��H_up��H_down��soil_T_up;soil_T_down;soil_H_up; soil_H_down; Lux_up;Lux_down
	char Name[12+1];// 6����
	int8_t T_down;//  	
	int8_t T_up;//  
	
	u8 H_down;//  
	u8 H_up;//  
	
	int8_t soil_T_down;//  
	int8_t soil_T_up;//  
	
	u8 soil_H_down;//  
	u8 soil_H_up;//  
	u8 PLC_Type;//20211121
	u16 Lux_down;//  0-65535
	u16 Lux_up;//  	 0-65535
	u16 report_second;//  	��ʱ����Ƶ�ʣ��룩��
	u8 report_changed;//���ݱ仯�Ƿ��ϱ�	

	u8 sum;	
}CONFIG_STRCUT;

typedef enum 
{
	CONFIG_INIT=0,// ��ʼ��̬
	CONFIG_LOOP,
	CONFIG_OVER,
	CONFIG_TIMER_OUT,
		
	CONFIG_MAX,
}ENUM_CONFIG;
typedef enum 
{
	CONFIG_MODE_F1=1,// ��ʼ��̬
	CONFIG_MODE_F2,
	CONFIG_MODE_F3,
	CONFIG_MODE_F4,
	CONFIG_MODE_F5,
	#if 1
	CONFIG_MODE_F6,
	CONFIG_MODE_F7,
	CONFIG_MODE_F8,
	
	CONFIG_MODE_F9,//����ģʽ
	CONFIG_MODE_F10,// ��ŷ�����
		
	CONFIG_MODE_F11,// ������1�ֳ���2��ͷ
	#endif		
	CONFIG_MODE_MAX,
}ENUM_CONFIG_MODE;


extern CONFIG_STRCUT g_tConfig;

extern void App_Config_Init_Var(void);
extern u8 App_Config_Loop(void);
extern u8  App_Config_RS485_Com(void);
extern void App_Config_Save_Into_Flash(void);

#endif

