/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2012-10-12     ST固件库版本 V2.1.0
*
*	Copyright (C), 2012-2013,   
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "app.h"

typedef struct _LED_T
{
	uint8_t ucEnalbe;
	uint8_t ucState;
	uint16_t usLedTime;
	uint16_t usStopTime;
	uint16_t usCycle;
	uint16_t usCount;
	uint16_t usCycleCount;
}LED_T;
static LED_T g_tLed;		/*  */

//#define  TEST_MODE_HEART_LED 

HEART_LED_STRUCT g_HeartLED;

/*
*********************************************************************************************************
*	函 数 名: Heart_Led_Start
*	功能说明:
*	形    参：_usLedTime : 蜂鸣时间，单位10ms; 0 表示不鸣叫
*			  _usStopTime : 停止时间，单位10ms; 0 表示持续鸣叫
*			 _usCycle : 鸣叫次数， 0 表示持续鸣叫
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Led_Start(uint16_t _usLedTime, uint16_t _usStopTime, uint16_t _usCycle)
{
	if (_usLedTime == 0)
	{
		return;
	}

	g_tLed.usLedTime = _usLedTime;
	g_tLed.usStopTime = _usStopTime;
	g_tLed.usCycle = _usCycle;
	g_tLed.usCount = 0;
	g_tLed.usCycleCount = 0;
	g_tLed.ucState = 0;
	g_tLed.ucEnalbe = 1;	/* 设置完全局参数后再使能发声标志 */

	App_display_logo_set(TRUE);
}


/*
*********************************************************************************************************
*	函 数 名: Heart_Led_Pro
*	功能说明: 每隔10ms调用1次该函数，用于控制蜂鸣器发声。该函数在 bsp_timer.c 中被调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Led_Pro(void)
{
	if ((g_tLed.ucEnalbe == 0) || (g_tLed.usStopTime == 0))
	{
		return;
	}

	if (g_tLed.ucState == 0)
	{
		if (g_tLed.usStopTime > 0)	/* 间断发声 */
		{
			if (++g_tLed.usCount >= g_tLed.usLedTime)
			{
				App_display_logo_set(FALSE);
				g_tLed.usCount = 0;
				g_tLed.ucState = 1;
			}
		}
		else
		{
			;	/* 不做任何处理，连续发声 */
		}
	}
	else if (g_tLed.ucState == 1)
	{
		if (++g_tLed.usCount >= g_tLed.usStopTime)
		{
			/* 连续发声时，直到调用stop停止为止 */
			if (g_tLed.usCycle > 0)
			{
				if (++g_tLed.usCycleCount >= g_tLed.usCycle)
				{
					/* 循环次数到，停止发声 */
					g_tLed.ucEnalbe = 0;
				}

				if (g_tLed.ucEnalbe == 0)
				{
					g_tLed.usStopTime = 0;
					return;
				}
			}

			g_tLed.usCount = 0;
			g_tLed.ucState = 0;
			
			App_display_logo_set(TRUE);
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Heart_Led_Init
*	功能说明: 初始化指示灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Led_Init(void)
{
	g_HeartLED.on_time = TMR_DELAY_1S;
	g_HeartLED.off_time= TMR_DELAY_3S;
	g_HeartLED.work_state = HEART_LED_INIT;
	g_HeartLED.count_time = 0;
}

/*
*********************************************************************************************************
*	函 数 名: Heart_Led_Long_ON
*	功能说明:   
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Led_ON_2S(void)
{
	g_HeartLED.on_time = TMR_DELAY_1S;
	g_HeartLED.off_time= TMR_DELAY_2S;
	g_HeartLED.work_state = HEART_LED_INIT;
}

/*
*********************************************************************************************************
*	函 数 名: Heart_Led_Long_ON
*	功能说明:   
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Led_ON_fast_04S(void)
{
	g_HeartLED.on_time = 200;
	g_HeartLED.off_time= 200;
	g_HeartLED.work_state = HEART_LED_INIT;
}



/*
*********************************************************************************************************
*	函 数 名: App_address_sensor_Led_Loop
*	功能说明: 指示灯状态显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_address_sensor_Led_Loop(void)
{
	switch(g_HeartLED.work_state)
	{
		case HEART_LED_INIT :
		{				
			App_display_logo_set(FALSE);
			g_HeartLED.work_state = HEART_LED_LOOP;

			bsp_StartTimer(TMR_ID_LED_ADDRESS,TMR_DELAY_5S);
			bsp_StartAutoTimer(TMR_ID_LED_ADDRESS_100MS,100);
			break;
		}

		case HEART_LED_LOOP :
		{
			if(bsp_CheckTimer(TMR_ID_LED_ADDRESS_100MS))
			{
				if(g_wenshi.address_sensor == 0) 
				{
					App_display_logo_set(FALSE);
				}
				/* 正常状态，慢闪，*/
				else
				{
					if(bsp_CheckTimer(TMR_ID_LED_ADDRESS))
					{
						bsp_StartTimer(TMR_ID_LED_ADDRESS,TMR_DELAY_5S);
					
						Heart_Led_Start(4,2,g_wenshi.address_sensor);//亮300ms灭200ms
					}
					Heart_Led_Pro();
				}
			}			
		}
			break;		
		default:
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Heart_LED_TEST
*	功能说明: 测试
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_LED_TEST(void)
{
	#ifdef TEST_MODE_HEART_LED
	//enableInterrupts();
	//Heart_Led_ON_fast_04S();
	Heart_Led_ON_2S();
	while(1)
	{

	}
	
	#endif
}


/*****************************    (END OF FILE) *********************************/
