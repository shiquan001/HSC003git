
#ifndef  _app_caiji_H_
#define  _app_caiji_H_

#include "stm32l4xx_hal.h"



/*
	�������Ƿ��͸��������ı�־
*/
#define  SENSOR_TX_SERVER_NULL				(0X0000)  
#define  SENSOR_TX_SERVER_TURANG_TH	 	(0X0001<<0) //
#define  SENSOR_TX_SERVER_TURANG_LUX 		(0X0001<<1) //
#define  SENSOR_TX_SERVER_TURANG_NPK   	(0X0001<<2) 
#define  SENSOR_TX_SERVER_TURANG_WATER 	(0X0001<<3) 
#define  SENSOR_TX_SERVER_CAMERA 			(0X0001<<4) 
#define  SENSOR_TX_SERVER_LED_DISPLAY 		(0X0001<<5) 

#define  SENSOR_TX_SERVER_WEATHER_STATION 			(0X0001<<6) 
#define  SENSOR_TX_SERVER_TH_LUX 			(0X0001<<7) 
#define  SENSOR_TX_SERVER_AIRTH				(0X0001<<8) //

#define  SENSOR_TX_SERVER_PLC_TIANJING		(0X0001<<9) 
#define  SENSOR_TX_SERVER_PLC_SHESHISUO	(0X0001<<10) 
#define  SENSOR_TX_SERVER_UV				(0X0001<<11) 
#define  SENSOR_TX_SERVER_TURANG_PH 		(0X0001<<12) 
#define  SENSOR_TX_SERVER_CO2				(0X0001<<13) 


#define  TURANG_HAS_WATER1 0x01 // ����ˮ
#define  TURANG_NOT_WATER0 0x00 // û��ˮ


#define  TURANG_HAS_WATER 0x0B // ����ˮ
#define  TURANG_NOT_WATER 0x0A // û��ˮ


#define  ADDRESS_TURANG_TH 	26 //�����¶ȴ�����modbus��ַ26  늌���
#define  ADDRESS_TURANG_LUX 	25 //���մ�����modbus��ַ25
#define  ADDRESS_AIR_TH 	15 //��ʪ�ȴ�����modbus��ַ15

#define  ADDRESS_WEATHER_STATION 	1 //Ĭ�ϼĴ�����ַ��1
#define  ADDRESS_TURANG_PH 	9//2  
#define  ADDRESS_TURANG_NPK 	11 // 
#define  ADDRESS_TURANG_WATER 4 // 
#define  ADDRESS_CAMERA	    5 //0��

#define  ADDRESS_LED_DISPLAY  27 //led display ��ʩ������ĻID����Ϊ27
#define  ADDRESS_PLC_DISPLAY  30 // ������Ļ��ID�ݶ�Ϊ30

#define  ADDRESS_PT100		 	7//pt100
#define  ADDRESS_TH_LUX	 	8//pt100
#define  ADDRESS_CO2		 	10
#define  ADDRESS_UV		 	10//������ǿ�� ֻ�� ʵ��ֵ��100�� //������ָ��


#define  FUNCTION_CODE_01 0X01 //01 ��ȡ��Ȧ
#define  FUNCTION_CODE_05 0X05 //05 д����Ȧ

#define  FUNCTION_CODE_03 0X03 //03��ȡ�Ĵ���
#define  FUNCTION_CODE_06 0X06 //06д��Ĵ���

#define  FUNCTION_CODE_10 0X10 //�����Ĵ���д��
#define  FUNCTION_CODE_15 0X0F //������Ȧд��

#define  REPORT_CAIJI_gettime_count 5//2//3//����  Уʱ����Ҫ

#define  ADDRESS_PLC_200 30//��ʩ��
#define  ADDRESS_PLC_201 201//���

typedef enum 
{
	REPORT_CAIJI_INIT=0,// 
	REPORT_CAIJI_gettime,// ������Уʱ

	REPORT_CAIJI_getinfo,// getinfo 
	
	REPORT_CAIJI_getreport,// 1��getreport�������������ȡС�������ò����� 
	REPORT_CAIJI_report_imei_imsi,//�ϱ�imei  imsi  

	REPORT_CAIJI_report_Connectivity,//�ϱ�     ���߲�������
	
	REPORT_CAIJI_POWER_ON,//�����ϱ�10����һ��
	REPORT_CAIJI_LOOP,
	REPORT_CAIJI_TIMER_OUT,
	REPORT_CAIJI_REPORT,// �ϱ�����
			
	REPORT_CAIJI_MAX, 
}ENUM_CAIJI;

typedef enum 
{
	CAIJI_GPS_INIT=0,// open
		
	CAIJI_GPS_OPEN,

	CAIJI_GPS_LOOP,
	CAIJI_GPS_CLOSE,//����close
		
	CAIJI_GPS_MAX, 
}ENUM_CAIJI_GPS;
typedef enum 
{
		
	CAIJI_soil_TH_OPEN,

	CAIJI_SOIL_PH_TX,// ����ph ������ ���Ͳɼ�����
	CAIJI_SOIL_PH_RX_LOOP,// �������յ�������

	CAIJI_CO2_TX,// CO2 ���Ͳɼ�����
	CAIJI_CO2_RX_LOOP,// �������յ�������

	CAIJI_SOIL_NPK_TX,// ���� NPK ���Ͳɼ�����
	CAIJI_SOIL_NPK_RX_LOOP,// �������յ�������

	CAIJI_WEATHER_STATION_TX,// ���Ͳɼ�����
	CAIJI_WEATHER_STATION_RX_LOOP,// �������յ�������
    
	CAIJI_soil_TH_DELAY,//��ʱ2���ӣ����Ͳɼ�����
	CAIJI_soil_TH_LOOP,//�ɼ�������ʪ�ȣ��������յ�������

	CAIJI_soil_LUX_DELAY,//��ʱ2���ӣ����Ͳɼ�����
	CAIJI_soil_LUX_LOOP,//�ɼ� ���մ�����

	CAIJI_AIRTH_DELAY2,
	CAIJI_AIRTH_LOOP2,//�ɼ�CO2   2
	CAIJI_LED_DISPLAY,//led��ʾ
	CAIJI_PLC_DISPLAY,//PLC ��ʾ
	CAIJI_PLC_READCOIL,//PLC ��Ȧ��ȡ
	CAIJI_PLC_WRITE_COIL,//PLC ��Ȧд
	CAIJI_PLC_WRITE_COIL_DELAY,//PLC ��Ȧд DELAY

    CAIJI_airTH_TX,// 8��������
    CAIJI_airTH_RX,

	CAIJI_CLOSE,//����close
		
	CAIJI_soil_TH_MAX, 
}ENUM_CAIJI_soil_TH;

struct CAIJI
{
	uint8_t work_state_report;// �ϱ���״̬
	uint8_t gettime_tx_count;// ���͵Ĵ���

	uint8_t work_state_gps;//gps
	uint8_t start_gps;//gps

	uint8_t soil_TH_work_state;//������ʪ��
	uint8_t soil_TH_start;//������ʪ��

	uint8_t rx_data_in_flag;//rx data 
	uint8_t	sensor_basic_server_ask_ok;
	uint8_t	timer_server_ask_ok;
	uint8_t report_server_ask_ok;
	uint8_t	info_server_ask_ok;
	uint8_t settimer_server_ask_ok;	 
};
/*
1���±�������
2���±��ر���
0���±�ֹͣ	
1��ͨ�翪����
2��ͨ��ر���
0��ͨ��ֹͣ
*/
typedef enum 
{
    Equipment_Stopped_STATE = 0,
    Equipment_Opened_STATE,
    Equipment_Closed_STATE,
}Equipment_STATE;
/*
1��������
2�ر�����
0ֹͣ����
*/
typedef enum 
{
    Equipment_Stopped_CMD = 0,
    Equipment_Opened_CMD,
    Equipment_Closed_CMD,
}Equipment_CMD;
/*
Insulation ;//  Insulation quilt ���±�
RoofVentilation ;// Roof ventilation ��ͨ��  

*/
typedef enum 
{
    Equipment_Insulation_TYPE = 1,
    Equipment_RoofVentilation_TYPE,
}Equipment_TYPE;
typedef struct
{
    uint8_t AppRxBuf[128];
    uint8_t AppRxCount;
    uint8_t AppRxAddr;
    
    uint8_t address;
    uint8_t function;
    uint16_t dataAddress;
    uint16_t dataLen;
    
}MYMODBUS_T;
extern MYMODBUS_T gModbus;

extern struct CAIJI g_caiji ;

typedef struct SensorAirTh_t
{
	uint8_t m_Symbol;
	uint16_t m_T;//�����¶� �Ŵ�10��
	uint16_t m_H;//����ʪ�� �Ŵ�10��
	
	uint8_t airTH;//�Ƿ����
	uint16_t airTHvalueH;//ʪ�� 0.01
	uint16_t airTHvalueT;// �����¶� 0.01
	uint16_t airTHvalueTwet;//    ʪ���¶� 0.01
	uint16_t airTHvalueLevel;// Һλ
	uint16_t airTHvalueHcal;//    ʪ��05��ֱ�Ӽ���ʪ��ֵ  

	uint8_t airTH_last;	
	uint16_t airTHvalueH_last;
	uint16_t airTHvalueT_last;// �����¶�
	uint16_t airTHvalueTwet_last;//    ʪ���¶� 
	uint16_t airTHvalueLevel_last;// Һ
}SensorAirTh_T;
extern SensorAirTh_T airThSensor[8];// �ܹ�8��  0-7

extern void App_caiji_report_Init_Var(void);
extern void App_caiji_report_Loop(void);
extern void App_caiji_report_start(void);
extern void App_caiji_gps_Loop(void);
extern void App_caiji_Air_T_H_Loop(void);
extern uint8_t App_data_report_process(void);

extern void App_caiji_turang_Loop(void);
extern void App_caiji_report_gettime_ok(void);
extern void App_caiji_Lux_Loop(void);
extern void App_485_tx_led_display_cmd(uint8_t address ,uint8_t function);
void App_caiji_sensor_address(void);
void App_485_tx_turang_T_H_cmd(uint8_t address ,uint8_t function);
void app_caiji_sensorDataSync(void);


#endif
