#ifndef _APP_CONFIG_H__
#define _APP_CONFIG_H__


#include "bsp.h"

#define TIMER_SMG_T 200//200ms

#define NUMBERS_UPDATE_CONFIG 4 //升级读取的参数2的倍数

/*
	  小按键的功能：
	 1、装置数量（1-4）
	 2、装置高度（6-15）
	 3、红外灵敏度（1-4）（5000、1万、2万、3万）
	 4、紫外灵敏度（1-4）（5、10、15、20）
	 5、水平补偿值（0 - 999）
	 6、垂直补偿值（0 - 999）
	 7、水平回转角度、（180-360）
*/
typedef struct
{	/*  保存的参数 */	
	u8 power_onoff; //true 开机false 没有注册。
	u8 setAcqTimer;   //4.3.6.9.设置传感器的采集频率，秒钟，1-255

	//Name；T_up；T_down；H_up；H_down；soil_T_up;soil_T_down;soil_H_up; soil_H_down; Lux_up;Lux_down
	char Name[12+1];// 6个字
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
	u16 report_second;//  	定时更新频率（秒），
	u8 report_changed;//数据变化是否上报	

	u8 sum;	
}CONFIG_STRCUT;

typedef enum 
{
	CONFIG_INIT=0,// 初始化态
	CONFIG_LOOP,
	CONFIG_OVER,
	CONFIG_TIMER_OUT,
		
	CONFIG_MAX,
}ENUM_CONFIG;
typedef enum 
{
	CONFIG_MODE_F1=1,// 初始化态
	CONFIG_MODE_F2,
	CONFIG_MODE_F3,
	CONFIG_MODE_F4,
	CONFIG_MODE_F5,
	#if 1
	CONFIG_MODE_F6,
	CONFIG_MODE_F7,
	CONFIG_MODE_F8,
	
	CONFIG_MODE_F9,//工作模式
	CONFIG_MODE_F10,// 电磁阀数量
		
	CONFIG_MODE_F11,// 升级，1现场，2炮头
	#endif		
	CONFIG_MODE_MAX,
}ENUM_CONFIG_MODE;


extern CONFIG_STRCUT g_tConfig;

extern void App_Config_Init_Var(void);
extern u8 App_Config_Loop(void);
extern u8  App_Config_RS485_Com(void);
extern void App_Config_Save_Into_Flash(void);

#endif

