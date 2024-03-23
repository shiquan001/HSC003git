
#ifndef  _app_sheshisuo_PLC_H_
#define  _app_sheshisuo_PLC_H_

#include "stm32l4xx_hal.h"
#define PLC_RX_DATA_LENTH 256
typedef enum
{
	VERIFICATION_SHORT_LENTH =0,
	VERIFICATION_ADDRESS ,	
	VERIFICATION_CRC ,
	VERIFICATION_NO_DATA,
	VERIFICATION_OK
}
ENMU_VERIFICATION;

typedef struct STATUS_OF_EQUIPMENT_ST
{
	//工作模式 
	uint8_t workMode;
	
	//保温被	 
	uint8_t insulationCover;
	//保温被正转 
	uint16_t insulationCover_forwardTimer;
	//保温被反转 
	uint16_t insulationCover_reverseTimer;
	//保温被位置 
	uint16_t insulationCover_position;
	
	//顶通风
	uint8_t topVentilation;
	//顶通风正转
	uint16_t topVentilation_forwardTimer;
	//顶通风反转
	uint16_t topVentilation_reverseTimer;
	//顶通风位置
	uint16_t topVentilation_position;
	
	//底通风
	uint8_t bottomVentilation;
	//底通风正转
	uint16_t bottomVentilation_forwardTimer;
	//底通风反转
	uint16_t bottomVentilation_reverseTimer;
	//底通风位置
	uint16_t bottomVentilation_position;
	
	//施肥机
	uint8_t manureSpreader;
	//加湿喷雾
	uint8_t wetSpray;
	//风机
	uint8_t fan;
	////湿帘
	uint8_t wetCurtain;
	//补光灯
	uint8_t fillLight;
	//循环风机
	uint8_t circulatingFan; 
	//预留一
	uint8_t ReserveOne;
	//预留二
	uint8_t ReserveTwo;
}
STATUS_OF_EQUIPMENT_st;

typedef struct FacilitiesAccumulateData_ST
{
	//总灌溉流量
	uint32_t TotalIrrigationFlow;
	//单次灌溉流量
	uint32_t flow;
	//水肥机肥水灌溉流量
	uint32_t flowFertileIrrigation ;
	//水肥机单次肥水灌溉流量
	uint32_t flowSingleFertilizerAndWaterIrrigation ;
	//加湿喷雾总运行时长
	uint32_t totalRunningTimeOfTheHumidificationSpray;
	//加湿喷雾单次运行时长
	uint32_t singleRunningTimeOfHumidificationSpray;
	//风机总运行时长
	uint32_t totalRunningTimeOfTheFan;
	//风机单次运行时长
	uint32_t singleRunningTimeOfTheFan;
	//湿帘总运行时长
	uint32_t totalRunningTimeOfTheWetCurtain;
	//湿帘单次运行时长
	uint32_t singleRunningTimeOfTheWetCurtain;
	//补光灯总开启时长
	uint32_t totalRunningTimeOfTheFillLightIsTurnedOn;
	//补光灯单次开启时长
	uint32_t SingleRunningTimeOfTheFillLightIsTurnedOn;
	//循环风机总运行时长
	uint32_t totalRunningTimeOfTheCirculatingFan;
	//循环风机单次运行时长
	uint32_t SingleRunningTimeOfTheCirculatingFan	;
}
FacilitiesAccumulateData_st;


typedef struct SensorRealTimeData_ST
{

	//实时室外温度
	int16_t RealTimeOutdoorTemperature;
	//实时室外湿度
	uint16_t RealTimeOutdoorHumidity;
	//实时室外光照
	uint32_t RealTimeOutdoorLighting;
	//实时室外风速
	uint16_t RealTimeOutdoorWindSpeed;
	//实时室外风向
	uint16_t RealTimeOutdoorWindDirection;
	//实时室外二氧化碳
	uint16_t RealTimeOutdoorCO2;
	//室外累计雨量
	uint16_t AccumulatedOutdoorRainfall;
	//室外当前雨量
	uint16_t CurrentOutdoorRainfall;
	//室外瞬时雨量
	uint16_t OutdoorInstantaneousRainfall;
	//室外大气压
	uint16_t OutdoorAtmosphericPressure;
	//室外实时雨雪状态
	uint16_t OutdoorRealTimeRainAndSnowState;
	//实时室内温度1
	int16_t RealTimeIndoorTemperature1;
	//实时室内湿度1
	uint16_t RealTimeIndoorHumidity1;
	//实时室内光照1
	uint32_t RealTimeIndoorIllumination1;
	//实时室内二氧化碳1
	uint16_t RealTimeIndoorCO21;
	//实时室内土壤温度1
	int16_t RealTimeIndoorSoilTemperature1;
	//实时室内土壤湿度1
	uint16_t RealTimeIndoorSoilHumidity1;
	//实时室内温度2
	int16_t RealTimeIndoorTemperature2;
	//实时室内湿度2
	uint16_t RealTimeIndoorHumidity2;
	//实时室内光照2
	uint32_t RealTimeIndoorIllumination2;
	//实时室内二氧化碳2
	uint16_t RealTimeIndoorCO22;
	//实时室内土壤温度2
	int16_t RealTimeIndoorSoilTemperature2;
	//实时室内土壤湿度2
	uint16_t RealTimeIndoorSoilHumidity2;
	//实时室内温度3
	int16_t RealTimeIndoorTemperature3;
	//实时室内湿度3
	uint16_t RealTimeIndoorHumidity3;
	//实时室内光照3
	uint32_t RealTimeIndoorIllumination3;
	//实时室内二氧化碳3
	uint16_t RealTimeIndoorCO23;
	//实时室内土壤温度3
	int16_t RealTimeIndoorSoilTemperature3;
	//实时室内土壤湿度3
	uint16_t RealTimeIndoorSoilHumidity3;
}
SensorRealTimeData_st;



typedef struct AbnormalAlarm_ST
{
	//高温报警
	uint8_t HighTemperatureAlarm;
	//低温报警
	uint8_t LowTemperatureWarning ;
	//高湿报警
	uint8_t HighHumidityAlarm;
	//低湿报警
	uint8_t LowWetAlarm;
	//下雨报警
	uint8_t RainAlarm;
	//强风报警
	uint8_t StrongWindAlarm;
	//光强过低报警
	uint8_t LightIsTooStrongToAlarm;
	//二氧化碳浓度过低报警
	uint8_t TooLowCO2ConcentrationAlarm;
	//土壤温度过低报警
	uint8_t SoilTemperatureAlarmIsTooLow;
	//土壤温度过高报警
	uint8_t SoilTemperatureExcessiveAlarm;
	//土壤湿度过低报警
	uint8_t SoilMoistureAlarmIsTooLow;
	//土壤湿度过高报警
	uint8_t SoilMoistureAlarmIsTooHigh;
	//烟感报警
	uint8_t SmokeSenseAlarm;
	//高温报警值
	int16_t HighTemperatureAlarmValue;
	//低温报警值
	int16_t LowTemperatureAlarmValue;
	//高湿报警值
	uint16_t HighHumidityAlarmValue;
	//低湿报警值//
	uint16_t LowWetAlarmValue;
	//下雨报警值
	uint16_t RainAlarmValue;
	//强风报警值
	uint16_t StrongWindAlarmValue;
	//二氧化碳浓度过低报警值
	uint16_t TheCO2ConcentrationIsTooLowAlarmValue;
	//光强过低报警值
	int32_t LightIntensityIsTooLowForTheAlarmValue;
	//土壤温度过低报警值
	int16_t SoilTemperatureAlarmValueIsTooLow;
	//土壤温度过高报警值
	int16_t SoilTemperatureIsTooHighAlarmValue;
	//土壤湿度过低报警值
	uint16_t SoilMoistureAlarmValueIsTooLow;
	//土壤湿度过高报警值
	uint16_t TooHighSoilHumidityAlarmValue;
	//烟感报警值//
	uint16_t SmokeAlarmValue;
}
AbnormalAlarm_st;
typedef struct strategyControl_time_ST
{
	uint16_t HH;
	uint16_t MM;
}strategyControl_time_st;
#define POINT_NUM 8
#define RELAY_NUM 15

typedef struct strategyControl_ST
{
	//时间点1（HH:MM）
	strategyControl_time_st TimePoint[POINT_NUM];
	/*
	//时间点2（HH:MM）
	strategyControl_time_st TimePoint2;
	//时间点3（HH:MM）
	strategyControl_time_st TimePoint3;
	//时间点4（HH:MM）
	strategyControl_time_st TimePoint4;
	//时间点5（HH:MM）
	strategyControl_time_st TimePoint5;
	//时间点6（HH:MM）
	strategyControl_time_st TimePoint6;
	//时间点7（HH:MM）
	strategyControl_time_st TimePoint7;
	//时间点8（HH:MM）
	strategyControl_time_st TimePoint8;*/
	
	//温度点1
	int16_t TemperaturePoint[POINT_NUM];
	/*
	//温度点2
	uint16_t TemperaturePoint2;
	//温度点3
	uint16_t TemperaturePoint3;
	//温度点4
	uint16_t TemperaturePoint4;
	//温度点5
	uint16_t TemperaturePoint5;
	//温度点6
	uint16_t TemperaturePoint6;
	//温度点7
	uint16_t TemperaturePoint7;
	//温度点8
	uint16_t TemperaturePoint8;
	*/
	//温度误差点1
	int16_t TemperatureErrorPoint[POINT_NUM];
	/*
	//度误差点2
	uint16_t TemperatureErrorPoint2;
	//度误差点3
	uint16_t TemperatureErrorPoint3;
	//温度误差点4
	uint16_t TemperatureErrorPoint4;
	//温度误差点5
	uint16_t TemperatureErrorPoint5;
	//温度误差点6
	uint16_t TemperatureErrorPoint6;
	//温度误差点7
	uint16_t TemperatureErrorPoint7;
	//温度误差点8
	uint16_t TemperatureErrorPoint8;
	*/
	//湿度点1
	uint16_t HumidityPoint[POINT_NUM];
	/*
	//湿度点2
	uint16_t HumidityPoint2;
	//湿度点3
	uint16_t HumidityPoint3;
	//湿度点4
	uint16_t HumidityPoint4;
	//湿度点5
	uint16_t HumidityPoint5;
	//湿度点6
	uint16_t HumidityPoint6;
	//湿度点7
	uint16_t HumidityPoint7;
	//湿度点8
	uint16_t HumidityPoint8;
	*/
	//湿度误差点1
	uint16_t HumidityErrorPoint[POINT_NUM];
	/*
	//湿度误差点2
	uint16_t HumidityErrorPoint2;
	//湿度误差点3
	uint16_t HumidityErrorPoint3;
	//湿度误差点4
	uint16_t HumidityErrorPoint4;
	//湿度误差点5
	uint16_t HumidityErrorPoint5;
	//湿度误差点6
	uint16_t HumidityErrorPoint6;
	//湿度误差点7
	uint16_t HumidityErrorPoint7;
	//湿度误差点8
	uint16_t HumidityErrorPoint8;
	*/
	//光强点1
	uint32_t LightStrongPoint[POINT_NUM];
	/*
	//光强点2
	uint16_t LightStrongPoint2;
	//光强点3
	uint16_t LightStrongPoint3;
	//光强点4
	uint16_t LightStrongPoint4;
	//光强点5
	uint16_t LightStrongPoint5;
	//光强点6
	uint16_t LightStrongPoint6;
	//光强点7
	uint16_t LightStrongPoint7;
	//光强点8
	uint16_t LightStrongPoint8;
	*/
	//可强制手动远程控制的继电器点1
	uint8_t RelayPointRemoteControl[RELAY_NUM];
	/*
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl2;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl3;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl4;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl5;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl6;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl7;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl8;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl9;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl10;
	//可强制手动远程控制的继电器点
	uint16_t RelayPointRemoteControl11;
	*/
}
strategyControl_st;

typedef struct M_REGISTER_ST
{
	uint8_t M0:1 ;
	uint8_t M1:1 ;
	uint8_t M2:1 ;
	uint8_t M3:1 ;
	uint8_t M4:1 ;
	uint8_t M5:1 ;
	uint8_t M6:1 ;
	uint8_t M7:1 ;
	
	uint8_t M8:1 ;
	uint8_t M9:1 ;
	uint8_t M10:1 ;
	uint8_t M11:1 ;
	uint8_t M12:1 ;
	uint8_t M13:1 ;
	uint8_t M14:1 ;
	uint8_t M15:1 ;	//不用

	uint8_t M16_23:8 ;//不用

	uint8_t M24_29:6 ;//不用
	uint8_t M30:1 ;
	uint8_t M31:1 ;

	uint8_t M32:1 ;
	uint8_t M33:1 ;
	uint8_t M34:1 ;
	uint8_t M35:1 ;
	uint8_t M36:1 ;
	uint8_t M37:1 ;
	uint8_t M38:1 ;
	uint8_t M39:1 ;


	uint8_t M40:1 ;
	
	uint8_t M41:1 ;//不用
	uint8_t M42:1 ;//不用
	uint8_t M43:1 ;//不用
	uint8_t M44_47:4 ;//不用

	uint8_t M48_55:8 ;//不用

	uint8_t M56_63:8 ;//不用

	uint8_t M64_69:6 ;//不用
	uint8_t M70:1 ;
	uint8_t M71:1 ;

	uint8_t M72:1 ;
	uint8_t M73:1 ;
	uint8_t M74:1 ;
	uint8_t M75:1 ;
	uint8_t M76:1 ;
	uint8_t M77:1 ;
	uint8_t M78:1 ;
	uint8_t M79:1 ;

	uint8_t M80:1 ;
	uint8_t M81:1 ;
	uint8_t M82:1 ;
	uint8_t M83_87:5 ;//不用
}M_REGISTER_st;

typedef union M_REGISTER_UN
{
	uint8_t data[11];

	M_REGISTER_st mRegister;
}M_REGISTER_un;

#define D0 	(0u)
#define D128  (128u)
#define D350  (350u)

typedef struct D_REGISTER_ST
{
	int16_t D0_32[33];
	int16_t D128_243[116];
	int16_t D350_378[28+1];//29
}D_REGISTER_st;

typedef union D_REGISTER_UN
{
	int8_t data[(33+116+28+1)*2];

	D_REGISTER_st dRegister;

}D_REGISTER_un;

typedef enum SHESHISUO_PLC_E
{		
	SHESHISUO_PLC_OPEN=0x00,	
		
	SHESHISUO_PLC_DELAY,//延时5秒钟，发送采集命令
		
	SHESHISUO_PLC_TX_READ_MREGISTER,
	SHESHISUO_PLC_RX_MREGISTER, 

//	SHESHISUO_PLC_TX_READ_DREGISTER,
//	SHESHISUO_PLC_RX_DREGISTER, 

	SHESHISUO_CMD_PLC_TX,
	SHESHISUO_CMD_PLC_RX,

	SHESHISUO_CMD_PLC_SERVER_ASK,//CMD ASK 
	
	SHESHISUO_PLC_CLOSE,
	
	SHESHISUO_PLC_MAX, 
}SHESHISUO_PLC_e;


typedef struct SHESHISUO_PLC_T
{
	uint8_t PLC_start; 	
	uint8_t PLC_work_state;
	uint8_t PLC_indexState;//
	
	uint8_t tx_count;//发给服务器的次数 	

	uint8_t rx_valuetype;
	uint8_t rx_occupied;//[NUM_MOTOR];//占位
	int32_t rx_value;//值

	uint16_t tx_mid;
	
	uint16_t reg_address;//PLC 	地址（只写）
	uint8_t reg_data[2];//PLC 	地址（只写）
	uint8_t reg_retry_tx_count; //重发的计数	
	
	uint16_t plc_not_ask_count;//断线的检测3次
	
	uint16_t plc_not_ask_state_changed; //掉线、上报

//	STATUS_OF_EQUIPMENT_st statusEquipment;
}
SHESHISUO_PLC_t;

extern SHESHISUO_PLC_t g_sheshisuoPLC;
extern STATUS_OF_EQUIPMENT_st g_statusEquipment;
extern FacilitiesAccumulateData_st g_FacilitiesAccumulateData;
extern SensorRealTimeData_st g_SensorRealTimeData ;
extern AbnormalAlarm_st g_AbnormalAlarm;
extern strategyControl_st g_strategyControl;

extern void App_sheshisuo_PLC_Init_Var(void);
extern void App_sheshisuo_PLC_Loop(void);
extern void App_sheshisuo_PLC_DataReport_SensorBasic(char *SensorBasic_data_tx, char *SensorBasic_data_temp);
extern void  App_sheshisuo_PLC_DataReport_SensorExtend(void);
extern void App_sheshisuo_PLC_Dinfo_Data_process(uint8_t pos,char* msg);
extern void PLC_TypeOccupiedValue_convertToAddress(uint8_t valuetype,uint8_t occupied,int32_t value);

#endif
