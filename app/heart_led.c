/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-10-12     ST�̼���汾 V2.1.0
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
*	�� �� ��: Heart_Led_Start
*	����˵��:
*	��    �Σ�_usLedTime : ����ʱ�䣬��λ10ms; 0 ��ʾ������
*			  _usStopTime : ֹͣʱ�䣬��λ10ms; 0 ��ʾ��������
*			 _usCycle : ���д����� 0 ��ʾ��������
*	�� �� ֵ: ��
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
	g_tLed.ucEnalbe = 1;	/* ������ȫ�ֲ�������ʹ�ܷ�����־ */

	App_display_logo_set(TRUE);
}


/*
*********************************************************************************************************
*	�� �� ��: Heart_Led_Pro
*	����˵��: ÿ��10ms����1�θú��������ڿ��Ʒ������������ú����� bsp_timer.c �б����á�
*	��    �Σ���
*	�� �� ֵ: ��
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
		if (g_tLed.usStopTime > 0)	/* ��Ϸ��� */
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
			;	/* �����κδ����������� */
		}
	}
	else if (g_tLed.ucState == 1)
	{
		if (++g_tLed.usCount >= g_tLed.usStopTime)
		{
			/* ��������ʱ��ֱ������stopֹͣΪֹ */
			if (g_tLed.usCycle > 0)
			{
				if (++g_tLed.usCycleCount >= g_tLed.usCycle)
				{
					/* ѭ����������ֹͣ���� */
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
*	�� �� ��: Heart_Led_Init
*	����˵��: ��ʼ��ָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: Heart_Led_Long_ON
*	����˵��:   
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: Heart_Led_Long_ON
*	����˵��:   
*	��    �Σ���
*	�� �� ֵ: ��
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
*	�� �� ��: App_address_sensor_Led_Loop
*	����˵��: ָʾ��״̬��ʾ
*	��    �Σ���
*	�� �� ֵ: ��
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
				/* ����״̬��������*/
				else
				{
					if(bsp_CheckTimer(TMR_ID_LED_ADDRESS))
					{
						bsp_StartTimer(TMR_ID_LED_ADDRESS,TMR_DELAY_5S);
					
						Heart_Led_Start(4,2,g_wenshi.address_sensor);//��300ms��200ms
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
*	�� �� ��: Heart_LED_TEST
*	����˵��: ����
*	��    �Σ���
*	�� �� ֵ: ��
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
