/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

/*
	Ϊ�˽��ʹ���������Cout  PreLoad �� uint32_t �޸�Ϊ uint16_t
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H


#define TMR_COUNT	50			/* �����ʱ���ĸ���  */

#define TMR_ID_ZIGBEE_ON_OFF_LIGHT 1 
//#define TMR_ID_ZIGBEE_TIMEROUT 2// �汾�ŵ���ʱ

#define TMR_ID_ZIGBEE_HA_TIMEROUT 3//
#define TMR_ID_ZIGBEE_HA_UART_DATAIN 4//

#define TMR_ID_ZIGBEE_HA_ENDPOINT_TIMEROUT 5//

#define TMR_ID_dispaly_refresh 6
#define TMR_ID_report_refresh 		7
#define TMR_ID_485_UART 8//
#define TMR_ID_caiji_gps_refresh 9
#define TMR_ID_caiji_gps_cycle 10 //  8Сʱ�ɼ�һ�Ρ�
#define TMR_ID_caiji_T_H_cycle 11 // 1min�ɼ�һ�Ρ�
#define TMR_ID_report_poweron 12 // �����ϱ�
#define TMR_ID_report_loop 	13

#define TMR_ID_ZIGBEE_HA_CHAXUN_TIMER 14//1���Ӳ�ѯһ�Σ�������״̬
#define TMR_ID_ZIGBEE_HA_report_socke_state 15//5�����ϱ�һ�β�����״̬
#define TMR_ID_ZIGBEE_HA_OFF_TIMER 16//1����ʱ5���ӹر��ϱ�

#define TMR_ID_read_NB_buffer 17 //  10s ��ȡһ�Ρ�
#define TMR_ID_read_NB_signal 18 //  2min ��ȡһ�Ρ�
#define TMR_ID_UPDATE_TX_DATA 19 // �������Ͷ�ʱ��
#define TMR_ID_GPS_UART_DATAIN 20//
#define TMR_ID_caiji_soil_TH_refresh 21

#define TMR_ID_ZIGBEE_HA_cmd_socke_state_ask 22 
#define TMR_ID_server_ok 23 // ������ͨѶok 45���ӵ���ʱ��
#define TMR_ID_caiji_camera 24//����ͷ
#define TMR_ID_caiji_camera_poweroff 25//����ͷ
#define TMR_ID_camera_TIMEROUT 26//
#define TMR_ID_Charge_Voltage_Indicator 27//�Ƿ���ļ��

#define TMR_ID_LED_ADDRESS	28 //  
#define TMR_ID_LED_ADDRESS_100MS	29 //  
#define TMR_ID_caiji_PLC_refresh 30
#define TMR_ID_caiji_PLC_TimerOutError 31
#define TMR_ID_caiji_PLC_ask_server 32//Ӧ�������
#define TMR_ID_caiji_PLC_cmd_chaxun 33//��ѯcmd�����ƽ���
#define TMR_ID_app_fifo_NB_CoapST 34//app_fifo_NB_CoapST

#define TMR_ID_sheshisuo_PLC_refresh 35 
#define TMR_ID_sheshisuo_PLC_cmd_chaxun 36//��ѯcmd�����ƽ���
#define TMR_ID_sheshisuo_PLC_ask_server 37//Ӧ�������
#define TMR_ID_sheshisuo_PLC_TimerOutError 38
#define TMR_ID_sheshisuo_PLC_ReportDelay30S 39//�ӳ��ϱ�30s
#define TMR_ID_4G_HeartBeat 40

#define SYSTICK_PERIOD	1				/* ϵͳ�δ���������Ϊ2ms�� ��λms ������ϵͳʱ��16M�����2ms��*/

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
	TMR_ONCE_MODE = 0,	/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
};

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint32_t Count;	/* ������ */
	volatile uint32_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;


extern uint32_t g_iRunTime ;

/* ���ⲿ���õĺ������� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

