#ifndef  _app_display_H_
#define  _app_display_H_

#include "stm32l4xx_hal.h"

typedef enum 
{
	DISPLAY_INIT=0,// ³õÊ¼»¯Ì¬
	DISPLAY_WAIT_3S_VALVE,
	DISPLAY_WAIT_3S_PUMP,
		
	DISPLAY_LOOP,
	DISPLAY_TIMER_OUT,
		
	DISPLAY_MAX,
}ENUM_DISPLAY;


typedef enum
{
	LCD_ENUM_SEG_0 = 0,
	LCD_ENUM_SEG_1,
	LCD_ENUM_SEG_2,
	LCD_ENUM_SEG_3,
	LCD_ENUM_SEG_4,
	LCD_ENUM_SEG_5,
	LCD_ENUM_SEG_6,
	LCD_ENUM_SEG_7,
	
	LCD_ENUM_SEG_8,
	LCD_ENUM_SEG_9,
	LCD_ENUM_SEG_10,
	LCD_ENUM_SEG_11,
	LCD_ENUM_SEG_12,
	LCD_ENUM_SEG_13,
	LCD_ENUM_SEG_14,
	LCD_ENUM_SEG_15,
		
	LCD_ENUM_SEG_16,		
	LCD_ENUM_SEG_17,
	LCD_ENUM_SEG_18,
	LCD_ENUM_SEG_19,
	LCD_ENUM_SEG_20,
	LCD_ENUM_SEG_21,
	LCD_ENUM_SEG_22,
	LCD_ENUM_SEG_23,
		
	LCD_ENUM_SEG_24,
	LCD_ENUM_SEG_25,
	LCD_ENUM_SEG_26,
	LCD_ENUM_SEG_27,
	LCD_ENUM_SEG_28,
	LCD_ENUM_SEG_29,
	LCD_ENUM_SEG_30,
	LCD_ENUM_SEG_31,
		
		
}LCD_ENUM_SEG_0_39;


typedef enum
{
	LCD_ENUM_COM1 = 0,
	LCD_ENUM_COM2,
	LCD_ENUM_COM3,
	LCD_ENUM_COM4,
}LCD_ENUM_COM_1_4;


typedef struct
{
	uint8_t SEG0_31[32];
}LCD_COM1;
#if 0
typedef struct
{
	uint8_t SEG0_7;
	uint8_t SEG8_15;
	uint8_t SEG16_23;
	uint8_t SEG24_31;	
	uint8_t SEG32__36_37_38_39;		
}LCD_COM2;


typedef struct
{
	uint8_t SEG0_7;
	uint8_t SEG8_15;
	uint8_t SEG16_23;
	uint8_t SEG24_31;	
	uint8_t SEG32__36_37_38_39;			
}LCD_COM3;

typedef struct
{
	uint8_t SEG0_7;
	uint8_t SEG8_15;
	uint8_t SEG16_23;
	uint8_t SEG24_31;	
	uint8_t SEG32__36_37_38_39;			
}LCD_COM4;
#endif

struct DISPLAY
{

	uint8_t work_state;

};

extern RTC_TimeTypeDef g_Time;
extern RTC_DateTypeDef g_Date;

extern struct DISPLAY g_display ;
void App_display_logo_set(uint8_t show);

void App_display_battery_value(uint8_t show_level, uint8_t show);

void App_display_battery_n(uint8_t show_level );
void App_display_battery_and_charge(uint8_t show_level, uint8_t charge);


void App_display_clock(uint8_t hour,uint8_t mini,uint8_t dot_toggle,uint8_t hour_toggle,uint8_t mini_toggle);
void App_display_temp(uint16_t temp_value,uint8_t show_flag);
void App_display_humidity(uint16_t humidity_value,uint8_t show_flag);
void App_display_illumination(uint32_t illumination_value,uint8_t show_flag);
void App_display_wifi_value(uint8_t show_level, uint8_t show);

void App_display_Init_Var(void);
void App_display_Loop(void);
void App_display_TEST(void);

#endif

