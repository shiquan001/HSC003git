#ifndef _APP_BATTERY_H__
#define _APP_BATTERY_H__
#include "bsp.h"

#define CHARGE_YES 1
#define CHARGE_NO  0

typedef enum 
{
	BATTERY_INIT=0,// 初始化态

	BATTERY_LOOP,
	BATTERY_TIMER_OUT,
		
	BATTERY_MAX,
}ENUM_BATTERY;

typedef struct
{

	u8 work_state;

	u8 low_flag;//低电量标志
	u8 low_flag_count;
	u8 voltage_level_g;
	
	u8 charge_flag;//充电状态 1充电
}BATTERY_STRCUT;

extern BATTERY_STRCUT g_tBattery;

void App_Battery_Init_Var(void);
void App_Battery_process_Exit(void);

void App_Battery_Voltage_Display_Data(void);
void App_Charge_Voltage_Indicator(void);


void App_Battery_process_Entrace(void);
void App_Battery_power_on_Loop(void);

#endif
