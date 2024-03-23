
#ifndef  _app_wenshi_H_
#define  _app_wenshi_H_

#include "stm32l4xx_hal.h"
#include "bsp_gps_l70_r.h"


#define VERSION_1  1
#define VERSION_2  0
#define VERSION_3  0


#define PIC_BUFF_LEN (80*1024)//13312  60
#define FRAME_PICTURE_DATA_SIZE 1024//1015   (1024-9-9)// ???????????????С



#define  SocketStatusReport 0x01 // 插排状�?
#define  DataReport 0x04	 // 设备数据上报

#define  CheckState 0x02	 //插座命令下发
#define  DeviceUpdate 0x07	 //插座命令下发
#define  UpdateDispatch 0x04


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef enum 
{
	WENSHI_INIT=0,// 初始化�?
	WENSHI_WAIT_3S_VALVE,
	WENSHI_WAIT_3S_PUMP,
		
	WENSHI_WAIT_FOR_WENSHI_OFF,
	WENSHI_WAIT_FOR_WENSHI_OFF_5S_CLOSE_VALE,
	WENSHI_LOOP,
	WENSHI_TIMER_OUT,
		
	WENSHI_MAX,
}ENUM_WENSHI;

typedef enum
{
	POWER_OFF = 0,
	POWER_ON,
	POWER_LOW,

}ENUM_POWER;
#define NPKsensor_NUM 3	
struct WENSHI
{
	u8 version[3];
	u8 device_ID[6];
	
	/*  ���ػ�״̬ ע��״̬*/
	u8 power_onoff;//true ����false 	
	u8 register_state;//true �Ѿ�ע�����false û��ע�ᡣ	
	u8 work_state;
	
	u8 flag_start_camera;// ��ʼ����ͷ
	
	uint8_t m_Symbol;
	uint16_t m_T;//�����¶� �Ŵ�10��
	uint16_t m_H;//����ʪ�� �Ŵ�10��

	uint32_t m_Lux ;//����ǿ��

	uint8_t m_soil_Symbol;
	uint16_t m_soil_T ;//�����¶� �Ŵ�10��
	uint16_t m_soil_H ;//����ʪ�� �Ŵ�10��
	uint16_t m_soil_EC ;//����electrical conductivity 
	uint16_t m_soil_LUX ;//��������

	uint16_t m_soil_PH ;//����PH   70���Ŵ�10��

	uint8_t m_co2;
	uint16_t m_co2_value;
    
	uint16_t m_soil_N[NPKsensor_NUM] ;//����N  mg/kg	
	//uint16_t m_soil_P ;//����P   mg/kg	
	//uint16_t m_soil_K ;//����K  mg/kg	

	uint16_t m_soil_Water ;//�������
	uint16_t m_camera ;//
	uint16_t m_led_display ;//�Ƿ�����led��Ļ��1��ʾ���������
	uint16_t mLedFlag ;//�Ƿ�����led��Ļ��1��ʾ���������
	
	uint16_t mPlcdisplay ;// ����
	uint8_t mPlcFlag ;//�Ƿ����� plc ��Ļ��1��ʾ���������
	uint8_t mPlcCoil ;//��ȡ������Ȧֵ
	uint8_t mPlcStateInsulation ;//  Insulation quilt ���±�
	uint8_t mPlcStateRoofVentilation ;// Roof ventilation ��ͨ��  
	
	uint16_t weatherStationOnline ;// ����
	uint8_t  weatherStationExisted ;//�Ƿ���������վ��1��ʾ���������
    uint16_t weatherStation[16];
    
	uint16_t m_pt100 ;//
	int16_t m_pt100_value ;//��ֵ10��

	uint16_t m_TH_LUX ;//
	int16_t m_TH_LUX_t_value ;//��ֵ10��
	int16_t m_TH_LUX_h_value ;//��ֵ10��
	int32_t m_TH_LUX_lux_value ;//��ֵ1��
	
	uint8_t m_UV;
	uint16_t m_UV_value;//0-1500 mW/cm2

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

	uint8_t m_flag_gps;
	double m_longtitude;
	double m_latitude;
	
	uint8_t m_voltage_level;//��ص���

	uint8_t fault_number;
	uint16_t version_number;

	char repot_time[14];
	u8 camera_state;
	
	/*�ϴ��ϱ�����ֵ*/
	uint8_t m_Symbol_last;
	uint16_t m_T_last;//�����¶�
	uint16_t m_H_last;//����ʪ��

	uint32_t m_Lux_last ;//����ǿ��

	uint8_t m_soil_Symbol_last;
	uint16_t m_soil_T_last ;//�����¶�
	uint16_t m_soil_H_last ;//����ʪ��
	uint8_t m_voltage_level_last;//��ص���
	uint16_t m_soil_LUX_last ;//����PH   70���Ŵ�10��
	
	uint16_t m_soil_N_last ;//����N  mg/kg	
	uint16_t m_soil_P_last ;//����P   mg/kg	
	uint16_t m_soil_K_last ;//����K  mg/kg	
	uint16_t m_soil_Water_last ;//�������
	uint16_t m_camera_last ;//
	uint16_t m_led_display_last ;//
	
	uint16_t m_pt100_last ;//
	int16_t m_pt100_value_last ;//��ֵ10��

	uint16_t m_TH_LUX_last ;//
	int16_t m_TH_LUX_t_value_last ;//��ֵ10��
	int16_t m_TH_LUX_h_value_last ;//��ֵ10��
	int32_t m_TH_LUX_lux_value_last ;//��ֵ1��

	uint8_t m_UV_last;
	uint16_t m_UV_value_last;//0-1500 mW/cm2



	int m_nb_signal;//�ź���ֵ
	uint8_t m_nb_signal_level;//�źŵȼ�

	uint16_t m_sensor_TXflag ;//�Ƿ�������͸�������

	uint8_t server_ok_count;//������Ӧ��
	uint16_t address_sensor;//

};

typedef enum 
{
	MessageId_SocketStatusReport = 0X0001,// 3.2.1 ����״̬�ϱ�SocketStatusReport��01���ϱ���
	MessageId_CMD_ChangeSocketState  ,// 3.2.4ChangeSocketState�ı����״̬02�����·���
	
	MessageId_ChangeSocketState,	//3.2.5changeSocketState�ı����״̬03�����ϱ���
	MessageId_DataReport= 0X0004   ,	//3.3.1��������DataReport 04�����ϱ���

	MessageId_CMD_UpdateNotice   ,	//3.4.2 ֪ͨ�豸��������UpdateNotice��05���·���
	MessageId_UpdateNotice, //3.4.3 ֪ͨ�豸��������UpdateNotice��06����Ӧ�ϱ���

	MessageId_DeviceUpdate	, //3.4.4 ��ȡ������DeviceUpdate��07���ϱ���

	MessageId_CMD_UpdateDispatch   ,	//3.4.6 �����豸������UpdateDispatch��08���·���
	MessageId_CMD_ZigbeeNetwork, //3.2.6ZigbeeNetwork����09�����·���

	MessageId_MAX,
}
ENUM_MessageId;


#define SOCKET_OPEN 0X01
#define SOCKET_CLOSE 0X00


/*messageId�µ�Э��20200724*/
#define messageId_SensorBasic 		0X0002 // ����������	SensorBasic	0x0002	����	no
#define messageId_SensorExtend 		0X0003 // ��չ������	SensorExtend	0x0003	����	no
#define messageId_Socket 	 		0X0004 // ����	Socket	0x0004	����	no
#define messageId_DeviceState  		0X0001 //�豸״̬	DeviceState	0x0001	����	no
#define messageId_Connectivity  		0X0005 //���߲�������


#define messageId_DEVICE_STATE 		0Xa001 //�豸״̬����	DEVICE_STATE	0xa001��0xa002	����	yes
#define messageId_DEVICE_STATE_ask 	0Xa007

#define messageId_ZIGBEE_NETWORK 	0Xa003 //�豸�����������	ZIGBEE_NETWORK	0xa003 , 0xa004	����	yes
#define messageId_ZIGBEE_NETWORK_ask 0Xa004

#define messageId_SOCKET_STATE 		0xa005 //��������	SOCKET_STATE	0xa005 , 0xa006	����	yess
#define messageId_SOCKET_STATE_ask 	0xa006


/*
2����������01����������ʪ�ȴ�������01�汾��
������ʪ��                20101
����PH������           20201
�������׼ش�����    	 20301
20401	��ȼ�⴫����

*/
#define SENSOR_ID_SOIL_PH 	20201
#define SENSOR_ID_SOIL_NPK 	20301

#define SENSOR_ID_SOIL_TH 	20102
#define SENSOR_ID_SOIL_LUX 	20501
#define SENSOR_ID_AIRTH_SHESHISUO 	20004//	��ʩ����ʪ�ȴ�����  ������ʪ��ֵ

#define SENSOR_ID_SOIL_NPK 	20301
#define SENSOR_ID_SOIL_WATER 	20401
#define SENSOR_ID_CAMERA 		40201
#define SENSOR_ID_LED_DISPLAY 	20601
#define SENSOR_ID_WEATHER_STATION 	20701//weatherStation
#define SENSOR_ID_TH_LUX 	20801
#define SENSOR_ID_CO2_LUX 	20901
#define SENSOR_ID_UV 	21001

#define SENSOR_ID_PLC_TIANJING 	    30202//20901
#define SENSOR_ID_PLC_SHESHISUO 	30202//
#define SENSOR_ID_PLC_SHESHISUO2 	30202//

/*
����	��ע
30201	�豸����״̬
30202	��ʩ��������
30203	������ʵʱ����
30204	�쳣����
30205	���Կ���
*/
typedef enum
{
    EVENT_REPORT_UPDATERESULT = 0,
    EVENT_REPORT_VERSION,
    EVENT_REPORT_GETTIME,
    EVENT_REPORT_GETINFO,
    EVENT_REPORT_GETREPORT,
    EVENT_REPORT_DownloadStatus,
    EVENT_REPORT_ReportNBID,
}
EVENT_REPORT_E;

typedef enum
{
    CMD_ASK_updateSetParam = 0,
    CMD_ASK_updateGetParam,
    CMD_ASK_updateStart,
    CMD_ASK_Ok,
    CMD_ASK_error,
    CMD_ASK_setReport,
    CMD_ASK_setDtime,
    CMD_ASK_setAcqTimer,
    CMD_ASK_plcwOk,
    CMD_ASK_plcwError,
}
CMD_ASK_TYPE_E;

extern struct WENSHI g_wenshi ;
extern uint16_t g_mid ;

extern void App_wenshi_Init_Var(void);
extern void App_wenshi_Loop(void);
extern void App_key_process(void);

extern void App_DataReport_04(void);

extern void App_wenshi_poweroff(void);
extern uint8_t App_DataRx_process(int len,char* msg);
extern void App_DataReport_04(void);
extern uint8_t App_SocketStatusReport_01(void);
extern void App_SocketStatusReport_clear_bak(void);
extern void App_DataReport_SensorBasic(void);
extern void App_server_gettime(unsigned long gmt_time_server);
extern void App_DeviceState_gettime(void);
extern void App_DeviceState_getreport(void);
extern void App_DeviceState_setreport_ask(void);

extern void App_DeviceState_gettime_ask(void);
extern void App_DeviceState_getpic_ask(void);
extern	void App_ZIGBEE_NETWORK_ask(uint8_t network);
extern void App_DeviceState_Report_NBID(void);
extern void App_DeviceState_poweroffpic_ask(void);
extern void App_DeviceState_getinfo(void);
extern void App_DeviceState_Report_Connectivity(void);
extern void App_Dinfo_Data_process(u8 pos,char* msg);
extern void App_DeviceState_setAcqTimer_ask(void);
extern void app_deviceState_updateResult(void);
extern void app_deviceState_eventReport(EVENT_REPORT_E eventType);
extern void App_DeviceState_cmdAsk( CMD_ASK_TYPE_E asktype);


#endif
