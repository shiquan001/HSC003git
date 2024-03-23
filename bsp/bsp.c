/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For STM32F103)
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序模块的主文件。主要提供 bsp_Init()函数供主程序调用。主程序的每个c文件可以在开
*			  头	添加 #include "bsp.h" 来包含所有的外设驱动模块。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2015-09-01    正式发布
*
*	Copyright (C), 2015-2020,   
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "app.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_test
*	功能说明:  测试专用
*			  
*	形    参：无
*	返 回 值: 无
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

	
	/* 初始化zigbee uart1 	 调试串口lpuart1*/
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1);// 

	Bsp_zigbee_power_on();// ZIGBEE  供电
	p_info("bsp_DelayMS 100");
	bsp_DelayMS(100);
	p_info("while 2");

	bsp_GPS_Init(App_gps_Msg_cb); //初始化串口
	App_caiji_report_Init_Var();
	/*开始 gps 采集初始化*/ 
	g_caiji.start_gps = TRUE;
	g_caiji.work_state_gps = CAIJI_GPS_INIT;

	//HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1);// 

//    __HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE); //?a???óê??D??
	bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//开始采集温湿度的信息		

	bsp_SHT30_Init();
	p_info("bsp_SHT30_Init 。。。");

	//如果大于1则进行sht30的测量	
	if(SHT20_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H))
	{
		bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);//空气温度 湿度		
	}

	
	Bsp_nb_power_on();
	bsp_DelayMS(3000);
	NBModule_open(&nb_config);	
	
	App_nb_Init_Var();// app 初始化

	while(1)
	{
		App_key_process();
		bsp_Idle();
		
		/*关机了*/
		if(g_wenshi.power_onoff == POWER_OFF)
		{
			App_caiji_turang_Loop();
					
		
		}
		else
		{
#if 1
			/* NB模块的函数处理*/
			MX_TimerPoll();// NB的专用定时器
			HAL_UART_Poll();// 串口数据通过回调函数进行解析。NB和 GPS
			NBModule_Main(&nb_config);
#endif
		
			App_nb_Loop();	  //nb 通信		
			App_caiji_gps_Loop();//gps 定位

			App_display_Loop();
			App_caiji_Air_T_H_Loop();//采集温度湿度

			App_ZIGBEE_HA_loop();
		}

	}

}

/*
*********************************************************************************************************
*	函 数 名: bsp_Init_bootloard
*	功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
*			 全局变量。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init_bootloard(void)
{
 	bsp_InitUart();

	/* 初始化电源控制**/
	Bsp_gps_power_off();	//关闭gps供电。
	Bsp_sensor_power_off();	//sensor供电。	
	Bsp_nb_power_off();		//关闭NB供电
	Bsp_12V_power_off();	//关闭12v	供电
	Bsp_zigbee_power_off();	//关闭zigbee供电

	/* 初始化key 	 */
	bsp_InitKey_set();

	/* 初始化软件定时器 */
	bsp_InitTimer();
	
	bsp_InitSPIBus();	/* 初始化SPI总线 */	
	bsp_InitSFlash();	/* 配置SPI总线 */
	
	p_info("1s 。。。");
	bsp_DelayMS(1000);
	p_info("1s 。。。");
	bsp_DelayMS(1000);

	p_info("bsp_Init 。。。");

}

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
*			 全局变量。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
 	bsp_InitUart();

	/* 初始化 电源管理 	 */
	Bsp_gps_power_off();	//关闭gps供电。
	Bsp_sensor_power_off();	//sensor供电。	
	Bsp_sensor_power_off_power_off();
	
	Bsp_nb_power_off();		//关闭NB供电
	Bsp_12V_power_off();	//关闭12v	供电
	Bsp_zigbee_power_off();	//关闭zigbee供电

	/* 初始化软件定时器 */
	bsp_InitTimer();

	/* 初始化断码屏 */
	bsp_HT1621_Init();// 初始化 ht1621
	p_info("bsp_HT1621_Init 。。。");

	/* 初始化key 	 */
	bsp_InitKey_set();

	/* 初始化光照传感器 */
	bsp_LUX_Init();
	p_info("B_LUX_Init 。。。");

	#if 0
	/* 初始化温湿度传感器 */	
	Bsp_sensor_power_on();	//sensor供电。	
	bsp_SHT30_Init();
	p_info("bsp_SHT30_Init 。。。");
	bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);//空气温度 湿度		
	p_info("g_caiji, m_Symbol:%c;温度:%d;湿度:%d;",(char)g_wenshi.m_Symbol,g_wenshi.m_T,g_wenshi.m_H);
	
	Bsp_sensor_power_off();	//sensor供电。	
	#endif


	bsp_InitSPIBus();	/* 初始化SPI总线 */	
	bsp_InitSFlash();	/* 配置SPI总线 */
	
	//bsp_test();

	/*
		由于ST固件库的启动文件已经执行了CPU系统时钟的初始化，所以不必再次重复配置系统时钟。
		启动文件配置了CPU主时钟频率、内部Flash访问速度和可选的外部SRAM FSMC初始化。

		系统时钟缺省配置为72MHz，如果需要更改，可以修改 system_stm32f103.c 文件
	*/

	/* 优先级分组设置为4 */
	bsp_camera_ov_ItDisable();// 线禁止中断
	

	/* 初始化温湿度传感器sht30 */
	/*	sht20 不成功则进行shet30的读取	*/
	#if 0	
	if(g_wenshi.m_H ==0)
	{
		bsp_SHT30_Init();
		p_info("SHT30_Init 。。。");
		bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);
		p_info("g_caiji, m_Symbol:%c;温度:%d;湿度:%d;",(char)g_wenshi.m_Symbol,g_wenshi.m_T,g_wenshi.m_H);
	}	
	#endif

	/* 打开nb模块的供电*/
	#if 0
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_RESET);	
	bsp_DelayMS(10);
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_SET);
	bsp_DelayMS(5*1000);
	#endif
	
	/* 初始化gps 	 */
	#if 0
	Bsp_gps_disenable();
	bsp_DelayMS(100);
	bsp_GPS_Init(App_gps_Msg_cb); 
	#endif
	
	/* 初始化zigbee uart1 	 调试串口lpuart1*/
	//HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1);// 
	//HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1);// 

	p_info("bsp_Init 。。。");

	  /* Enable PWR clock */
	  __HAL_RCC_PWR_CLK_ENABLE(); 	
 }

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些处理时间要求不严格的
*			任务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	bsp_KeyScan_set();

}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被Systick中断调用1次。详见 bsp_timer.c的定时中断服务程序。一些需要周期性处理的事务
*			 可以放在此函数。比如：触摸坐标扫描。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer1ms(void)
{
//	App_ZIGBEE_HA_light_switch_process_loop();

}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void SaveScreenToBmp(uint16_t _index);
void bsp_Idle(void)
{
	#if ENABLE_WATCHDOG	
	HAL_IWDG_Refresh(&hiwdg) ;/* --- 喂狗 */
	#endif
	
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

    /* Enter Sleep Mode , wake up is done once User push-button is pressed */
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();



}

/*****************************    (END OF FILE) *********************************/
