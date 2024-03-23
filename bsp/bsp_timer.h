/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/*
	为了降低代码容量，Cout  PreLoad 由 uint32_t 修改为 uint16_t
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H


#define TMR_COUNT	50			/* 软件定时器的个数  */

#define TMR_ID_ZIGBEE_ON_OFF_LIGHT 1 
//#define TMR_ID_ZIGBEE_TIMEROUT 2// 版本号倒计时

#define TMR_ID_ZIGBEE_HA_TIMEROUT 3//
#define TMR_ID_ZIGBEE_HA_UART_DATAIN 4//

#define TMR_ID_ZIGBEE_HA_ENDPOINT_TIMEROUT 5//

#define TMR_ID_dispaly_refresh 6
#define TMR_ID_report_refresh 		7
#define TMR_ID_485_UART 8//
#define TMR_ID_caiji_gps_refresh 9
#define TMR_ID_caiji_gps_cycle 10 //  8小时采集一次。
#define TMR_ID_caiji_T_H_cycle 11 // 1min采集一次。
#define TMR_ID_report_poweron 12 // 开机上报
#define TMR_ID_report_loop 	13

#define TMR_ID_ZIGBEE_HA_CHAXUN_TIMER 14//1秒钟查询一次，插座的状态
#define TMR_ID_ZIGBEE_HA_report_socke_state 15//5秒钟上报一次插座的状态
#define TMR_ID_ZIGBEE_HA_OFF_TIMER 16//1倒计时5分钟关闭上报

#define TMR_ID_read_NB_buffer 17 //  10s 读取一次。
#define TMR_ID_read_NB_signal 18 //  2min 读取一次。
#define TMR_ID_UPDATE_TX_DATA 19 // 升级发送定时器
#define TMR_ID_GPS_UART_DATAIN 20//
#define TMR_ID_caiji_soil_TH_refresh 21

#define TMR_ID_ZIGBEE_HA_cmd_socke_state_ask 22 
#define TMR_ID_server_ok 23 // 服务器通讯ok 45分钟倒计时；
#define TMR_ID_caiji_camera 24//摄像头
#define TMR_ID_caiji_camera_poweroff 25//摄像头
#define TMR_ID_camera_TIMEROUT 26//
#define TMR_ID_Charge_Voltage_Indicator 27//是否充电的监测

#define TMR_ID_LED_ADDRESS	28 //  
#define TMR_ID_LED_ADDRESS_100MS	29 //  
#define TMR_ID_caiji_PLC_refresh 30
#define TMR_ID_caiji_PLC_TimerOutError 31
#define TMR_ID_caiji_PLC_ask_server 32//应答服务器
#define TMR_ID_caiji_PLC_cmd_chaxun 33//查询cmd，控制节奏
#define TMR_ID_app_fifo_NB_CoapST 34//app_fifo_NB_CoapST

#define TMR_ID_sheshisuo_PLC_refresh 35 
#define TMR_ID_sheshisuo_PLC_cmd_chaxun 36//查询cmd，控制节奏
#define TMR_ID_sheshisuo_PLC_ask_server 37//应答服务器
#define TMR_ID_sheshisuo_PLC_TimerOutError 38
#define TMR_ID_sheshisuo_PLC_ReportDelay30S 39//延迟上报30s
#define TMR_ID_4G_HeartBeat 40

#define SYSTICK_PERIOD	1				/* 系统滴答周期设置为2ms， 单位ms （对于系统时钟16M，最大2ms）*/

#define TMR_DELAY_0_25S 250
#define TMR_DELAY_0_3S  300
#define TMR_DELAY_0_5S  500
#define TMR_DELAY_1S 1*1000
#define TMR_DELAY_1_5S 1*1500

#define TMR_DELAY_2S 2*1000
#define TMR_DELAY_3S 3*1000
#define TMR_DELAY_4S 4*1000
#define TMR_DELAY_5S 5*1000
#define TMR_DELAY_6S 6*1000

#define TMR_DELAY_10S 10*1000
#define TMR_DELAY_20S 20*1000
#define TMR_DELAY_22S 22*1000
#define TMR_DELAY_30S 30*1000
#define TMR_DELAY_40S 40*1000
#define TMR_DELAY_60S 60*1000
#define TMR_DELAY_2min (uint32_t)2*TMR_DELAY_60S
#define TMR_DELAY_3min (uint32_t)3*TMR_DELAY_60S
#define TMR_DELAY_4min (uint32_t)4*TMR_DELAY_60S

#define TMR_DELAY_5min (uint32_t)5*TMR_DELAY_60S
#define TMR_DELAY_8min (uint32_t)8*TMR_DELAY_60S

#define TMR_DELAY_10min 10*TMR_DELAY_60S
#define TMR_DELAY_30min 30*TMR_DELAY_60S
#define TMR_DELAY_45min 45*TMR_DELAY_60S

#define TMR_DELAY_1hour  60*TMR_DELAY_60S
#define TMR_DELAY_2hour  2*TMR_DELAY_1hour
#define TMR_DELAY_8hour  8*TMR_DELAY_1hour
#define TMR_DELAY_24hour  24*TMR_DELAY_1hour
#define TMR_DELAY_48hour  48*TMR_DELAY_1hour

enum
{
	TMR_ONCE_MODE = 0,	/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
};

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint32_t Count;	/* 计数器 */
	volatile uint32_t PreLoad;	/* 计数器预装值 */
}SOFT_TMR;


extern uint32_t g_iRunTime ;

/* 供外部调用的函数声明 */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);

void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
uint32_t bsp_GetRunTime(void);

void bsp_InitHardTimer(void);
void bsp_StartHardTimer(uint8_t _CC, uint16_t _usTimeOut, void * _pCallBack);
uint8_t bsp_SetTimer(uint8_t _id);

void SysTick_ISR(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

