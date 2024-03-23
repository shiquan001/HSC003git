
#ifndef  _app_caiji_H_
#define  _app_caiji_H_

#include "stm32l4xx_hal.h"



/*
	传感器是否发送给服务器的标志
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


#define  TURANG_HAS_WATER1 0x01 // 有雨水
#define  TURANG_NOT_WATER0 0x00 // 没有水


#define  TURANG_HAS_WATER 0x0B // 有雨水
#define  TURANG_NOT_WATER 0x0A // 没有水


#define  ADDRESS_TURANG_TH 	26 //土壤温度传感器modbus地址26  電導率
#define  ADDRESS_TURANG_LUX 	25 //光照传感器modbus地址25
#define  ADDRESS_AIR_TH 	15 //温湿度传感器modbus地址15

#define  ADDRESS_WEATHER_STATION 	1 //默认寄存器地址是1
#define  ADDRESS_TURANG_PH 	9//2  
#define  ADDRESS_TURANG_NPK 	11 // 
#define  ADDRESS_TURANG_WATER 4 // 
#define  ADDRESS_CAMERA	    5 //0先

#define  ADDRESS_LED_DISPLAY  27 //led display 设施所的屏幕ID设置为27
#define  ADDRESS_PLC_DISPLAY  30 // 海威屏幕的ID暂定为30

#define  ADDRESS_PT100		 	7//pt100
#define  ADDRESS_TH_LUX	 	8//pt100
#define  ADDRESS_CO2		 	10
#define  ADDRESS_UV		 	10//紫外线强度 只读 实际值的100倍 //紫外线指数


#define  FUNCTION_CODE_01 0X01 //01 读取线圈
#define  FUNCTION_CODE_05 0X05 //05 写入线圈

#define  FUNCTION_CODE_03 0X03 //03读取寄存器
#define  FUNCTION_CODE_06 0X06 //06写入寄存器

#define  FUNCTION_CODE_10 0X10 //批量寄存器写入
#define  FUNCTION_CODE_15 0X0F //批量线圈写入

#define  REPORT_CAIJI_gettime_count 5//2//3//次数  校时很重要

#define  ADDRESS_PLC_200 30//设施所
#define  ADDRESS_PLC_201 201//天津

typedef enum 
{
	REPORT_CAIJI_INIT=0,// 
	REPORT_CAIJI_gettime,// 服务器校时

	REPORT_CAIJI_getinfo,// getinfo 
	
	REPORT_CAIJI_getreport,// 1、getreport代表向服务器获取小程序设置参数。 
	REPORT_CAIJI_report_imei_imsi,//上报imei  imsi  

	REPORT_CAIJI_report_Connectivity,//上报     无线参数数据
	
	REPORT_CAIJI_POWER_ON,//开机上报10秒钟一次
	REPORT_CAIJI_LOOP,
	REPORT_CAIJI_TIMER_OUT,
	REPORT_CAIJI_REPORT,// 上报数据
			
	REPORT_CAIJI_MAX, 
}ENUM_CAIJI;

typedef enum 
{
	CAIJI_GPS_INIT=0,// open
		
	CAIJI_GPS_OPEN,

	CAIJI_GPS_LOOP,
	CAIJI_GPS_CLOSE,//、、close
		
	CAIJI_GPS_MAX, 
}ENUM_CAIJI_GPS;
typedef enum 
{
		
	CAIJI_soil_TH_OPEN,

	CAIJI_SOIL_PH_TX,// 土壤ph 传感器 发送采集命令
	CAIJI_SOIL_PH_RX_LOOP,// 解析接收到的数据

	CAIJI_CO2_TX,// CO2 发送采集命令
	CAIJI_CO2_RX_LOOP,// 解析接收到的数据

	CAIJI_SOIL_NPK_TX,// 土壤 NPK 发送采集命令
	CAIJI_SOIL_NPK_RX_LOOP,// 解析接收到的数据

	CAIJI_WEATHER_STATION_TX,// 发送采集命令
	CAIJI_WEATHER_STATION_RX_LOOP,// 解析接收到的数据
    
	CAIJI_soil_TH_DELAY,//延时2秒钟，发送采集命令
	CAIJI_soil_TH_LOOP,//采集土壤温湿度，解析接收到的数据

	CAIJI_soil_LUX_DELAY,//延时2秒钟，发送采集命令
	CAIJI_soil_LUX_LOOP,//采集 光照传感器

	CAIJI_AIRTH_DELAY2,
	CAIJI_AIRTH_LOOP2,//采集CO2   2
	CAIJI_LED_DISPLAY,//led显示
	CAIJI_PLC_DISPLAY,//PLC 显示
	CAIJI_PLC_READCOIL,//PLC 线圈读取
	CAIJI_PLC_WRITE_COIL,//PLC 线圈写
	CAIJI_PLC_WRITE_COIL_DELAY,//PLC 线圈写 DELAY

    CAIJI_airTH_TX,// 8个传感器
    CAIJI_airTH_RX,

	CAIJI_CLOSE,//、、close
		
	CAIJI_soil_TH_MAX, 
}ENUM_CAIJI_soil_TH;

struct CAIJI
{
	uint8_t work_state_report;// 上报的状态
	uint8_t gettime_tx_count;// 发送的次数

	uint8_t work_state_gps;//gps
	uint8_t start_gps;//gps

	uint8_t soil_TH_work_state;//土壤温湿度
	uint8_t soil_TH_start;//土壤温湿度

	uint8_t rx_data_in_flag;//rx data 
	uint8_t	sensor_basic_server_ask_ok;
	uint8_t	timer_server_ask_ok;
	uint8_t report_server_ask_ok;
	uint8_t	info_server_ask_ok;
	uint8_t settimer_server_ask_ok;	 
};
/*
1保温被开启中
2保温被关闭中
0保温被停止	
1顶通风开启中
2顶通风关闭中
0顶通风停止
*/
typedef enum 
{
    Equipment_Stopped_STATE = 0,
    Equipment_Opened_STATE,
    Equipment_Closed_STATE,
}Equipment_STATE;
/*
1开启命令
2关闭命令
0停止命令
*/
typedef enum 
{
    Equipment_Stopped_CMD = 0,
    Equipment_Opened_CMD,
    Equipment_Closed_CMD,
}Equipment_CMD;
/*
Insulation ;//  Insulation quilt 保温被
RoofVentilation ;// Roof ventilation 顶通风  

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
	uint16_t m_T;//空气温度 放大10倍
	uint16_t m_H;//空气湿度 放大10倍
	
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
}SensorAirTh_T;
extern SensorAirTh_T airThSensor[8];// 总共8个  0-7

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
