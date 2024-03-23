/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For STM32F103)
*	�ļ����� : bsp.c
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ���������ģ������ļ�����Ҫ�ṩ bsp_Init()��������������á��������ÿ��c�ļ������ڿ�
*			  ͷ	��� #include "bsp.h" ���������е���������ģ�顣
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2015-09-01    ��ʽ����
*
*	Copyright (C), 2015-2020,   
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "app.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_test
*	����˵��:  ����ר��
*			  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_test(void)
{

	p_info("bsp_test");
	#if 0
	bsp_DelayMS(1000);
	
	Bsp_gps_power_on();
	Bsp_nb_power_on();
	Bsp_sensor_power_on();
	Bsp_12V_power_on();
	Bsp_zigbee_power_on();

	bsp_DelayMS(1000);
	#endif
	
	Bsp_gps_power_off();
	Bsp_nb_power_off();
	Bsp_sensor_power_off();
	Bsp_12V_power_off();
	Bsp_zigbee_power_off();
	p_info("while(1)");

	App_ZIGBEE_HA_Init_Var();

	
	/* ��ʼ��zigbee uart1 	 ���Դ���lpuart1*/
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1);// 

	Bsp_zigbee_power_on();// ZIGBEE  ����
	p_info("bsp_DelayMS 100");
	bsp_DelayMS(100);
	p_info("while 2");

	bsp_GPS_Init(App_gps_Msg_cb); //��ʼ������
	App_caiji_report_Init_Var();
	/*��ʼ gps �ɼ���ʼ��*/ 
	g_caiji.start_gps = TRUE;
	g_caiji.work_state_gps = CAIJI_GPS_INIT;

	//HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1);// 

//    __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE); //?a???����??D??
	bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//��ʼ�ɼ���ʪ�ȵ���Ϣ		

	bsp_SHT30_Init();
	p_info("bsp_SHT30_Init ������");

	//�������1�����sht30�Ĳ���	
	if(SHT20_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H))
	{
		bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);//�����¶� ʪ��		
	}

	
	Bsp_nb_power_on();
	bsp_DelayMS(3000);
	NBModule_open(&nb_config);	
	
	App_nb_Init_Var();// app ��ʼ��

	while(1)
	{
		App_key_process();
		bsp_Idle();
		
		/*�ػ���*/
		if(g_wenshi.power_onoff == POWER_OFF)
		{
			App_caiji_turang_Loop();
					
		
		}
		else
		{
#if 1
			/* NBģ��ĺ�������*/
			MX_TimerPoll();// NB��ר�ö�ʱ��
			HAL_UART_Poll();// ��������ͨ���ص��������н�����NB�� GPS
			NBModule_Main(&nb_config);
#endif
		
			App_nb_Loop();	  //nb ͨ��		
			App_caiji_gps_Loop();//gps ��λ

			App_display_Loop();
			App_caiji_Air_T_H_Loop();//�ɼ��¶�ʪ��

			App_ZIGBEE_HA_loop();
		}

	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Init_bootloard
*	����˵��: ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
*			 ȫ�ֱ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init_bootloard(void)
{
 	bsp_InitUart();

	/* ��ʼ����Դ����**/
	Bsp_gps_power_off();	//�ر�gps���硣
	Bsp_sensor_power_off();	//sensor���硣	
	Bsp_nb_power_off();		//�ر�NB����
	Bsp_12V_power_off();	//�ر�12v	����
	Bsp_zigbee_power_off();	//�ر�zigbee����

	/* ��ʼ��key 	 */
	bsp_InitKey_set();

	/* ��ʼ�������ʱ�� */
	bsp_InitTimer();
	
	bsp_InitSPIBus();	/* ��ʼ��SPI���� */	
	bsp_InitSFlash();	/* ����SPI���� */
	
	p_info("1s ������");
	bsp_DelayMS(1000);
	p_info("1s ������");
	bsp_DelayMS(1000);

	p_info("bsp_Init ������");

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
*			 ȫ�ֱ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{
 	bsp_InitUart();

	/* ��ʼ�� ��Դ���� 	 */
	Bsp_gps_power_off();	//�ر�gps���硣
	Bsp_sensor_power_off();	//sensor���硣	
	Bsp_sensor_power_off_power_off();
	
	Bsp_nb_power_off();		//�ر�NB����
	Bsp_12V_power_off();	//�ر�12v	����
	Bsp_zigbee_power_off();	//�ر�zigbee����

	/* ��ʼ�������ʱ�� */
	bsp_InitTimer();

	/* ��ʼ�������� */
	bsp_HT1621_Init();// ��ʼ�� ht1621
	p_info("bsp_HT1621_Init ������");

	/* ��ʼ��key 	 */
	bsp_InitKey_set();

	/* ��ʼ�����մ����� */
	bsp_LUX_Init();
	p_info("B_LUX_Init ������");

	#if 0
	/* ��ʼ����ʪ�ȴ����� */	
	Bsp_sensor_power_on();	//sensor���硣	
	bsp_SHT30_Init();
	p_info("bsp_SHT30_Init ������");
	bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);//�����¶� ʪ��		
	p_info("g_caiji, m_Symbol:%c;�¶�:%d;ʪ��:%d;",(char)g_wenshi.m_Symbol,g_wenshi.m_T,g_wenshi.m_H);
	
	Bsp_sensor_power_off();	//sensor���硣	
	#endif


	bsp_InitSPIBus();	/* ��ʼ��SPI���� */	
	bsp_InitSFlash();	/* ����SPI���� */
	
	//bsp_test();

	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
		�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

		ϵͳʱ��ȱʡ����Ϊ72MHz�������Ҫ���ģ������޸� system_stm32f103.c �ļ�
	*/

	/* ���ȼ���������Ϊ4 */
	bsp_camera_ov_ItDisable();// �߽�ֹ�ж�
	

	/* ��ʼ����ʪ�ȴ�����sht30 */
	/*	sht20 ���ɹ������shet30�Ķ�ȡ	*/
	#if 0	
	if(g_wenshi.m_H ==0)
	{
		bsp_SHT30_Init();
		p_info("SHT30_Init ������");
		bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);
		p_info("g_caiji, m_Symbol:%c;�¶�:%d;ʪ��:%d;",(char)g_wenshi.m_Symbol,g_wenshi.m_T,g_wenshi.m_H);
	}	
	#endif

	/* ��nbģ��Ĺ���*/
	#if 0
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_RESET);	
	bsp_DelayMS(10);
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_SET);
	bsp_DelayMS(5*1000);
	#endif
	
	/* ��ʼ��gps 	 */
	#if 0
	Bsp_gps_disenable();
	bsp_DelayMS(100);
	bsp_GPS_Init(App_gps_Msg_cb); 
	#endif
	
	/* ��ʼ��zigbee uart1 	 ���Դ���lpuart1*/
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1);// 
	//HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1);// 

	p_info("bsp_Init ������");

	  /* Enable PWR clock */
	  __HAL_RCC_PWR_CLK_ENABLE(); 	
 }

/*
*********************************************************************************************************
*	�� �� ��: bsp_RunPer10ms
*	����˵��: �ú���ÿ��10ms��Systick�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������һЩ����ʱ��Ҫ���ϸ��
*			������Է��ڴ˺��������磺����ɨ�衢���������п��Ƶȡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	bsp_KeyScan_set();

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_RunPer1ms
*	����˵��: �ú���ÿ��1ms��Systick�жϵ���1�Ρ���� bsp_timer.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ��������
*			 ���Է��ڴ˺��������磺��������ɨ�衣
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
//	App_ZIGBEE_HA_light_switch_process_loop();

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Idle
*	����˵��: ����ʱִ�еĺ�����һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
*			 ������ȱʡΪ�ղ������û��������ι��������CPU��������ģʽ�Ĺ��ܡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern void SaveScreenToBmp(uint16_t _index);
void bsp_Idle(void)
{
	#if ENABLE_WATCHDOG	
	HAL_IWDG_Refresh(&hiwdg) ;/* --- ι�� */
	#endif
	
	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

    /* Enter Sleep Mode , wake up is done once User push-button is pressed */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */
	//GUI_Exec();



}

/*****************************    (END OF FILE) *********************************/
