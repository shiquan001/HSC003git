
#ifndef  _app_wenshi_H_
#define  _app_wenshi_H_

#include "stm32l4xx_hal.h"
#include "bsp_gps_l70_r.h"


#define VERSION_1  1
#define VERSION_2  0
#define VERSION_3  0


#define PIC_BUFF_LEN (80*1024)//13312  60
#define FRAME_PICTURE_DATA_SIZE 1024//1015   (1024-9-9)// ???????????????小



#define  SocketStatusReport 0x01 // 鎻掓帓鐘舵�?
#define  DataReport 0x04	 // 璁惧鏁版嵁涓婃姤

#define  CheckState 0x02	 //鎻掑骇鍛戒护涓嬪彂
#define  DeviceUpdate 0x07	 //鎻掑骇鍛戒护涓嬪彂
#define  UpdateDispatch 0x04


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef enum 
{
	WENSHI_INIT=0,// 鍒濆鍖栨�?
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
	
	/*  开关机状态 注册状态*/
	u8 power_onoff;//true 开机false 	
	u8 register_state;//true 已经注册过。false 没有注册。	
	u8 work_state;
	
	u8 flag_start_camera;// 开始摄像头
	
	uint8_t m_Symbol;
	uint16_t m_T;//空气温度 放大10倍
	uint16_t m_H;//空气湿度 放大10倍

	uint32_t m_Lux ;//光照强度

	uint8_t m_soil_Symbol;
	uint16_t m_soil_T ;//土壤温度 放大10倍
	uint16_t m_soil_H ;//土壤湿度 放大10倍
	uint16_t m_soil_EC ;//土壤electrical conductivity 
	uint16_t m_soil_LUX ;//土壤光照

	uint16_t m_soil_PH ;//土壤PH   70；放大10倍

	uint8_t m_co2;
	uint16_t m_co2_value;
    
	uint16_t m_soil_N[NPKsensor_NUM] ;//土壤N  mg/kg	
	//uint16_t m_soil_P ;//土壤P   mg/kg	
	//uint16_t m_soil_K ;//土壤K  mg/kg	

	uint16_t m_soil_Water ;//土壤灌溉
	uint16_t m_camera ;//
	uint16_t m_led_display ;//是否接入过led屏幕；1表示曾经接入过
	uint16_t mLedFlag ;//是否接入过led屏幕；1表示曾经接入过
	
	uint16_t mPlcdisplay ;// 在线
	uint8_t mPlcFlag ;//是否接入过 plc 屏幕；1表示曾经接入过
	uint8_t mPlcCoil ;//读取到的线圈值
	uint8_t mPlcStateInsulation ;//  Insulation quilt 保温被
	uint8_t mPlcStateRoofVentilation ;// Roof ventilation 顶通风  
	
	uint16_t weatherStationOnline ;// 在线
	uint8_t  weatherStationExisted ;//是否接入过气象站；1表示曾经接入过
    uint16_t weatherStation[16];
    
	uint16_t m_pt100 ;//
	int16_t m_pt100_value ;//数值10倍

	uint16_t m_TH_LUX ;//
	int16_t m_TH_LUX_t_value ;//数值10倍
	int16_t m_TH_LUX_h_value ;//数值10倍
	int32_t m_TH_LUX_lux_value ;//数值1倍
	
	uint8_t m_UV;
	uint16_t m_UV_value;//0-1500 mW/cm2

	uint8_t airTH;//是否存在
	uint16_t airTHvalueH;//湿度 0.01
	uint16_t airTHvalueT;// 干球温度 0.01
	uint16_t airTHvalueTwet;//    湿球温度 0.01
	uint16_t airTHvalueLevel;// 液位
	uint16_t airTHvalueHcal;//    湿度05；直接计算湿度值  

	uint8_t airTH_last;	
	uint16_t airTHvalueH_last;
	uint16_t airTHvalueT_last;// 干球温度
	uint16_t airTHvalueTwet_last;//    湿球温度 
	uint16_t airTHvalueLevel_last;// 液

	uint8_t m_flag_gps;
	double m_longtitude;
	double m_latitude;
	
	uint8_t m_voltage_level;//电池电量

	uint8_t fault_number;
	uint16_t version_number;

	char repot_time[14];
	u8 camera_state;
	
	/*上次上报的数值*/
	uint8_t m_Symbol_last;
	uint16_t m_T_last;//空气温度
	uint16_t m_H_last;//空气湿度

	uint32_t m_Lux_last ;//光照强度

	uint8_t m_soil_Symbol_last;
	uint16_t m_soil_T_last ;//土壤温度
	uint16_t m_soil_H_last ;//土壤湿度
	uint8_t m_voltage_level_last;//电池电量
	uint16_t m_soil_LUX_last ;//土壤PH   70；放大10倍
	
	uint16_t m_soil_N_last ;//土壤N  mg/kg	
	uint16_t m_soil_P_last ;//土壤P   mg/kg	
	uint16_t m_soil_K_last ;//土壤K  mg/kg	
	uint16_t m_soil_Water_last ;//土壤灌溉
	uint16_t m_camera_last ;//
	uint16_t m_led_display_last ;//
	
	uint16_t m_pt100_last ;//
	int16_t m_pt100_value_last ;//数值10倍

	uint16_t m_TH_LUX_last ;//
	int16_t m_TH_LUX_t_value_last ;//数值10倍
	int16_t m_TH_LUX_h_value_last ;//数值10倍
	int32_t m_TH_LUX_lux_value_last ;//数值1倍

	uint8_t m_UV_last;
	uint16_t m_UV_value_last;//0-1500 mW/cm2



	int m_nb_signal;//信号数值
	uint8_t m_nb_signal_level;//信号等级

	uint16_t m_sensor_TXflag ;//是否组包发送给服务器

	uint8_t server_ok_count;//服务器应答
	uint16_t address_sensor;//

};

typedef enum 
{
	MessageId_SocketStatusReport = 0X0001,// 3.2.1 插座状态上报SocketStatusReport、01（上报）
	MessageId_CMD_ChangeSocketState  ,// 3.2.4ChangeSocketState改变插座状态02；（下发）
	
	MessageId_ChangeSocketState,	//3.2.5changeSocketState改变插座状态03；（上报）
	MessageId_DataReport= 0X0004   ,	//3.3.1环境数据DataReport 04；（上报）

	MessageId_CMD_UpdateNotice   ,	//3.4.2 通知设备更新升级UpdateNotice、05（下发）
	MessageId_UpdateNotice, //3.4.3 通知设备更新升级UpdateNotice、06（响应上报）

	MessageId_DeviceUpdate	, //3.4.4 获取升级包DeviceUpdate、07（上报）

	MessageId_CMD_UpdateDispatch   ,	//3.4.6 发送设备升级包UpdateDispatch、08（下发）
	MessageId_CMD_ZigbeeNetwork, //3.2.6ZigbeeNetwork网络09；（下发）

	MessageId_MAX,
}
ENUM_MessageId;


#define SOCKET_OPEN 0X01
#define SOCKET_CLOSE 0X00


/*messageId新的协议20200724*/
#define messageId_SensorBasic 		0X0002 // 基础传感器	SensorBasic	0x0002	服务	no
#define messageId_SensorExtend 		0X0003 // 扩展传感器	SensorExtend	0x0003	服务	no
#define messageId_Socket 	 		0X0004 // 插座	Socket	0x0004	服务	no
#define messageId_DeviceState  		0X0001 //设备状态	DeviceState	0x0001	服务	no
#define messageId_Connectivity  		0X0005 //无线参数数据


#define messageId_DEVICE_STATE 		0Xa001 //设备状态命令	DEVICE_STATE	0xa001，0xa002	命令	yes
#define messageId_DEVICE_STATE_ask 	0Xa007

#define messageId_ZIGBEE_NETWORK 	0Xa003 //设备网络控制命令	ZIGBEE_NETWORK	0xa003 , 0xa004	命令	yes
#define messageId_ZIGBEE_NETWORK_ask 0Xa004

#define messageId_SOCKET_STATE 		0xa005 //插座命令	SOCKET_STATE	0xa005 , 0xa006	命令	yess
#define messageId_SOCKET_STATE_ask 	0xa006


/*
2代表传感器；01代表土壤温湿度传感器；01版本；
土壤温湿度                20101
土壤PH传感器           20201
土壤氮磷钾传感器    	 20301
20401	灌溉监测传感器

*/
#define SENSOR_ID_SOIL_PH 	20201
#define SENSOR_ID_SOIL_NPK 	20301

#define SENSOR_ID_SOIL_TH 	20102
#define SENSOR_ID_SOIL_LUX 	20501
#define SENSOR_ID_AIRTH_SHESHISUO 	20004//	设施所温湿度传感器  带计算湿度值

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
编码	备注
30201	设备运行状态
30202	设施积累数据
30203	传感器实时数据
30204	异常报警
30205	策略控制
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
