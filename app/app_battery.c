#include "bsp.h"
#include "app.h"

#include "app_battery.h"

#define NUMBER_ADC 5*4
uint16_t   ADC_Convertedvalue[NUMBER_ADC];
uint32_t count = 0;

//ȫ�ֱ���
u8 Electricity_10rate_g = 25; //������10���ڷ��ͽ���
u8 voltage_level_g = 4;
BATTERY_STRCUT g_tBattery;

#if 0
#define BATTERY_VOTAGE_LEVEL_0 3.7f
#else
#define BATTERY_VOTAGE_LEVEL_0 3.4f//��ȿ����ʱ�򣬿�����ʾ����
#endif
#define BATTERY_VOTAGE_LEVEL_1 3.9f
#define BATTERY_VOTAGE_LEVEL_2 4.0f
#define BATTERY_VOTAGE_LEVEL_3 4.1f

/*
*********************************************************************************************************
*	�� �� ��: HAL_ADC_ConvCpltCallback
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//__NOP;
	count++;
}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_Init_Var
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_Init_Var(void)
{
	memset(&g_tBattery,0,sizeof(g_tBattery));
	
	g_tBattery.work_state = BATTERY_INIT;

	g_tBattery.low_flag= 1;//��ʼ����ȱ��ģ��͵�����־
	g_tBattery.low_flag_count = 0;
	g_tBattery.voltage_level_g = 0;
	
	g_tBattery.charge_flag = CHARGE_NO;
	
}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_process
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_process_Entrace(void)
{
	//bsp_InitAdc();
}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_process
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_process_Exit(void)
{
	//bsp_AdcPro();
	//bsp_InitAdc_Close();
}
/*
*********************************************************************************************************
*	�� �� ��: App_Battery_Voltage_Display_Data
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
//���ݵ�ص�ѹ�����ѹ��ʾ�ȼ����ݲ�������ʾ����
*********************************************************************************************************
*/
void App_Battery_Voltage_Display_Data(void)
{
	float vlotage_f = 0;
	u16 i = 0;
	u32 sum = 0;
	#if 0
	if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Convertedvalue,NUMBER_ADC)!= HAL_OK)
	{
		Error_Handler();
	}
	
	bsp_DelayMS(10);
	
	//if(count >= 1)
	{
		for(i=0;i<4;i++)
		{
			sum = sum + ADC_Convertedvalue[i+3];
		}
		vlotage_f = sum/4;
		
	}
	#else
	sum = 0;
	for(i=0;i<10;i++)
	{
		sum = sum + HAL_ADC1_IN1_Read();
	}
	vlotage_f = sum/10.0f;	
	vlotage_f = (vlotage_f*3.3f);
	vlotage_f = vlotage_f/4095.0f;

	#if 1	
	/******����0.06V����Ϊʵ��3.24��ADC�ɼ�Ϊ3.18******/
	vlotage_f += 0.06f;
	#endif	
	vlotage_f = vlotage_f*2;// �Ŵ�2��
	//vlotage_f = vlotage_f+0.1;//����ƫ�õ�ѹ0.1V
	p_info("���Adc_value = %.2f\r\n",vlotage_f);
	#endif
	
	if ((vlotage_f >= 0) && (vlotage_f < BATTERY_VOTAGE_LEVEL_0)) voltage_level_g = 0;
	else if ((vlotage_f >= BATTERY_VOTAGE_LEVEL_0) && (vlotage_f < BATTERY_VOTAGE_LEVEL_1)) voltage_level_g = 1; 
	else if ((vlotage_f >= BATTERY_VOTAGE_LEVEL_1) && (vlotage_f < BATTERY_VOTAGE_LEVEL_2)) voltage_level_g = 2; 
	else if ((vlotage_f >= BATTERY_VOTAGE_LEVEL_2) && (vlotage_f < BATTERY_VOTAGE_LEVEL_3)) voltage_level_g = 3; 
	else if ((vlotage_f >= BATTERY_VOTAGE_LEVEL_3)) voltage_level_g = 4;
	else voltage_level_g = 4;
	

	/* */
	if(voltage_level_g < 1)
	{
		g_tBattery.low_flag = 1;
	}
	else
	{
		g_tBattery.low_flag = 0;
	}
	g_tBattery.voltage_level_g = voltage_level_g;
	//p_info("LEVEL:%d;low_flag:%d. \r\n",voltage_level_g,g_tBattery.low_flag);

	#if 0
	sum = 0;
	for(i=0;i<10;i++)
	{
		sum = sum + HAL_ADC2_Read();
	}
	vlotage_f = sum/10;	
	vlotage_f = (vlotage_f*3.3f);
	vlotage_f = vlotage_f/4095;
	#if 1	
	/******����0.06V����Ϊʵ��3.24��ADC�ɼ�Ϊ3.18******/
	vlotage_f += 0.06f;
	#endif		
	vlotage_f = vlotage_f*2;// �Ŵ�2��	
	p_info("����ʪ�ȵ��Adc_value = %.2f\r\n",vlotage_f);
	#endif
			
}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_Loop
*	����˵��:     
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_Loop(void)
{	
	
	
	switch(g_tBattery.work_state)
	{
		case BATTERY_INIT :
		{				
			g_tBattery.work_state = BATTERY_LOOP;							

			p_info("g_tBattery BATTERY_INIT");
		}
			break;
			
		case BATTERY_LOOP :
		{	
			App_Battery_Voltage_Display_Data();//��ʾ ��ص�����
		
		}		
			break;	
			
		default:
			break;
	}

}





/*
*********************************************************************************************************
*	�� �� ��: App_Battery_power_on_Loop
*	����˵��:     ��ؿ�����������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_power_on_Loop(void)
{	
	
	
	switch(g_tBattery.work_state)
	{
		case BATTERY_INIT :
		{				
			g_tBattery.work_state = BATTERY_LOOP;							

			p_info("g_tBattery BATTERY_INIT");
		}
			break;
			
		case BATTERY_LOOP :
		{	
			App_Battery_Voltage_Display_Data();//��ʾ ��ص�����
		
		}		
			break;	
			
		default:
			break;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_test
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Battery_test(void)
{
	#if 0
	//bsp_InitAdc();
	
	//bsp_InitAdc_Close();
	while(1)
	{

		//halt();
		//bsp_AdcPro();
		App_Battery_Voltage_Display_Data();//��ʾ ��ص�����
		
	}
	#endif
	
}


/*
*********************************************************************************************************
*	�� �� ��: App_Charge_Voltage_Indicator
*	����˵��: ������ѹ�Ƿ����
*	��    �Σ���
*	�� �� ֵ: ��

�е�ϵ硢�����⹦��

*********************************************************************************************************
*/
void App_Charge_Voltage_Indicator(void)
{
	float vlotage_f = 0;
	u16 i = 0;
	u32 sum = 0;

	sum = 0;
	for(i=0;i<10;i++)
	{
		sum = sum + HAL_ADC1_IN4_Read();
	}
	vlotage_f = sum/10.0f;	
	vlotage_f = (vlotage_f*3.3f);
	vlotage_f = vlotage_f/4095.0f;

	/******����0.06V����Ϊʵ��3.24��ADC�ɼ�Ϊ3.18******/
	vlotage_f += 0.06f;
	vlotage_f = vlotage_f*2;// �Ŵ�2��
	//vlotage_f = vlotage_f+0.1;//����ƫ�õ�ѹ0.1V
	//p_info("����ѹcharge_Adc_value = %.2f\r\n",vlotage_f);

	/* */
	if(vlotage_f >= BATTERY_VOTAGE_LEVEL_3)
	{
		g_tBattery.charge_flag = CHARGE_YES;
	}
	else
	{
		g_tBattery.charge_flag = CHARGE_NO;
	}
	p_info("����ѹ vlotage_f:%f;charge_flag:%d.\r\n",vlotage_f,g_tBattery.charge_flag);
			
}

