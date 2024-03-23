
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
	//����ģʽ 
	uint8_t workMode;
	
	//���±�	 
	uint8_t insulationCover;
	//���±���ת 
	uint16_t insulationCover_forwardTimer;
	//���±���ת 
	uint16_t insulationCover_reverseTimer;
	//���±�λ�� 
	uint16_t insulationCover_position;
	
	//��ͨ��
	uint8_t topVentilation;
	//��ͨ����ת
	uint16_t topVentilation_forwardTimer;
	//��ͨ�練ת
	uint16_t topVentilation_reverseTimer;
	//��ͨ��λ��
	uint16_t topVentilation_position;
	
	//��ͨ��
	uint8_t bottomVentilation;
	//��ͨ����ת
	uint16_t bottomVentilation_forwardTimer;
	//��ͨ�練ת
	uint16_t bottomVentilation_reverseTimer;
	//��ͨ��λ��
	uint16_t bottomVentilation_position;
	
	//ʩ�ʻ�
	uint8_t manureSpreader;
	//��ʪ����
	uint8_t wetSpray;
	//���
	uint8_t fan;
	////ʪ��
	uint8_t wetCurtain;
	//�����
	uint8_t fillLight;
	//ѭ�����
	uint8_t circulatingFan; 
	//Ԥ��һ
	uint8_t ReserveOne;
	//Ԥ����
	uint8_t ReserveTwo;
}
STATUS_OF_EQUIPMENT_st;

typedef struct FacilitiesAccumulateData_ST
{
	//�ܹ������
	uint32_t TotalIrrigationFlow;
	//���ι������
	uint32_t flow;
	//ˮ�ʻ���ˮ�������
	uint32_t flowFertileIrrigation ;
	//ˮ�ʻ����η�ˮ�������
	uint32_t flowSingleFertilizerAndWaterIrrigation ;
	//��ʪ����������ʱ��
	uint32_t totalRunningTimeOfTheHumidificationSpray;
	//��ʪ����������ʱ��
	uint32_t singleRunningTimeOfHumidificationSpray;
	//���������ʱ��
	uint32_t totalRunningTimeOfTheFan;
	//�����������ʱ��
	uint32_t singleRunningTimeOfTheFan;
	//ʪ��������ʱ��
	uint32_t totalRunningTimeOfTheWetCurtain;
	//ʪ����������ʱ��
	uint32_t singleRunningTimeOfTheWetCurtain;
	//������ܿ���ʱ��
	uint32_t totalRunningTimeOfTheFillLightIsTurnedOn;
	//����Ƶ��ο���ʱ��
	uint32_t SingleRunningTimeOfTheFillLightIsTurnedOn;
	//ѭ�����������ʱ��
	uint32_t totalRunningTimeOfTheCirculatingFan;
	//ѭ�������������ʱ��
	uint32_t SingleRunningTimeOfTheCirculatingFan	;
}
FacilitiesAccumulateData_st;


typedef struct SensorRealTimeData_ST
{

	//ʵʱ�����¶�
	int16_t RealTimeOutdoorTemperature;
	//ʵʱ����ʪ��
	uint16_t RealTimeOutdoorHumidity;
	//ʵʱ�������
	uint32_t RealTimeOutdoorLighting;
	//ʵʱ�������
	uint16_t RealTimeOutdoorWindSpeed;
	//ʵʱ�������
	uint16_t RealTimeOutdoorWindDirection;
	//ʵʱ���������̼
	uint16_t RealTimeOutdoorCO2;
	//�����ۼ�����
	uint16_t AccumulatedOutdoorRainfall;
	//���⵱ǰ����
	uint16_t CurrentOutdoorRainfall;
	//����˲ʱ����
	uint16_t OutdoorInstantaneousRainfall;
	//�������ѹ
	uint16_t OutdoorAtmosphericPressure;
	//����ʵʱ��ѩ״̬
	uint16_t OutdoorRealTimeRainAndSnowState;
	//ʵʱ�����¶�1
	int16_t RealTimeIndoorTemperature1;
	//ʵʱ����ʪ��1
	uint16_t RealTimeIndoorHumidity1;
	//ʵʱ���ڹ���1
	uint32_t RealTimeIndoorIllumination1;
	//ʵʱ���ڶ�����̼1
	uint16_t RealTimeIndoorCO21;
	//ʵʱ���������¶�1
	int16_t RealTimeIndoorSoilTemperature1;
	//ʵʱ��������ʪ��1
	uint16_t RealTimeIndoorSoilHumidity1;
	//ʵʱ�����¶�2
	int16_t RealTimeIndoorTemperature2;
	//ʵʱ����ʪ��2
	uint16_t RealTimeIndoorHumidity2;
	//ʵʱ���ڹ���2
	uint32_t RealTimeIndoorIllumination2;
	//ʵʱ���ڶ�����̼2
	uint16_t RealTimeIndoorCO22;
	//ʵʱ���������¶�2
	int16_t RealTimeIndoorSoilTemperature2;
	//ʵʱ��������ʪ��2
	uint16_t RealTimeIndoorSoilHumidity2;
	//ʵʱ�����¶�3
	int16_t RealTimeIndoorTemperature3;
	//ʵʱ����ʪ��3
	uint16_t RealTimeIndoorHumidity3;
	//ʵʱ���ڹ���3
	uint32_t RealTimeIndoorIllumination3;
	//ʵʱ���ڶ�����̼3
	uint16_t RealTimeIndoorCO23;
	//ʵʱ���������¶�3
	int16_t RealTimeIndoorSoilTemperature3;
	//ʵʱ��������ʪ��3
	uint16_t RealTimeIndoorSoilHumidity3;
}
SensorRealTimeData_st;



typedef struct AbnormalAlarm_ST
{
	//���±���
	uint8_t HighTemperatureAlarm;
	//���±���
	uint8_t LowTemperatureWarning ;
	//��ʪ����
	uint8_t HighHumidityAlarm;
	//��ʪ����
	uint8_t LowWetAlarm;
	//���걨��
	uint8_t RainAlarm;
	//ǿ�籨��
	uint8_t StrongWindAlarm;
	//��ǿ���ͱ���
	uint8_t LightIsTooStrongToAlarm;
	//������̼Ũ�ȹ��ͱ���
	uint8_t TooLowCO2ConcentrationAlarm;
	//�����¶ȹ��ͱ���
	uint8_t SoilTemperatureAlarmIsTooLow;
	//�����¶ȹ��߱���
	uint8_t SoilTemperatureExcessiveAlarm;
	//����ʪ�ȹ��ͱ���
	uint8_t SoilMoistureAlarmIsTooLow;
	//����ʪ�ȹ��߱���
	uint8_t SoilMoistureAlarmIsTooHigh;
	//�̸б���
	uint8_t SmokeSenseAlarm;
	//���±���ֵ
	int16_t HighTemperatureAlarmValue;
	//���±���ֵ
	int16_t LowTemperatureAlarmValue;
	//��ʪ����ֵ
	uint16_t HighHumidityAlarmValue;
	//��ʪ����ֵ//
	uint16_t LowWetAlarmValue;
	//���걨��ֵ
	uint16_t RainAlarmValue;
	//ǿ�籨��ֵ
	uint16_t StrongWindAlarmValue;
	//������̼Ũ�ȹ��ͱ���ֵ
	uint16_t TheCO2ConcentrationIsTooLowAlarmValue;
	//��ǿ���ͱ���ֵ
	int32_t LightIntensityIsTooLowForTheAlarmValue;
	//�����¶ȹ��ͱ���ֵ
	int16_t SoilTemperatureAlarmValueIsTooLow;
	//�����¶ȹ��߱���ֵ
	int16_t SoilTemperatureIsTooHighAlarmValue;
	//����ʪ�ȹ��ͱ���ֵ
	uint16_t SoilMoistureAlarmValueIsTooLow;
	//����ʪ�ȹ��߱���ֵ
	uint16_t TooHighSoilHumidityAlarmValue;
	//�̸б���ֵ//
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
	//ʱ���1��HH:MM��
	strategyControl_time_st TimePoint[POINT_NUM];
	/*
	//ʱ���2��HH:MM��
	strategyControl_time_st TimePoint2;
	//ʱ���3��HH:MM��
	strategyControl_time_st TimePoint3;
	//ʱ���4��HH:MM��
	strategyControl_time_st TimePoint4;
	//ʱ���5��HH:MM��
	strategyControl_time_st TimePoint5;
	//ʱ���6��HH:MM��
	strategyControl_time_st TimePoint6;
	//ʱ���7��HH:MM��
	strategyControl_time_st TimePoint7;
	//ʱ���8��HH:MM��
	strategyControl_time_st TimePoint8;*/
	
	//�¶ȵ�1
	int16_t TemperaturePoint[POINT_NUM];
	/*
	//�¶ȵ�2
	uint16_t TemperaturePoint2;
	//�¶ȵ�3
	uint16_t TemperaturePoint3;
	//�¶ȵ�4
	uint16_t TemperaturePoint4;
	//�¶ȵ�5
	uint16_t TemperaturePoint5;
	//�¶ȵ�6
	uint16_t TemperaturePoint6;
	//�¶ȵ�7
	uint16_t TemperaturePoint7;
	//�¶ȵ�8
	uint16_t TemperaturePoint8;
	*/
	//�¶�����1
	int16_t TemperatureErrorPoint[POINT_NUM];
	/*
	//������2
	uint16_t TemperatureErrorPoint2;
	//������3
	uint16_t TemperatureErrorPoint3;
	//�¶�����4
	uint16_t TemperatureErrorPoint4;
	//�¶�����5
	uint16_t TemperatureErrorPoint5;
	//�¶�����6
	uint16_t TemperatureErrorPoint6;
	//�¶�����7
	uint16_t TemperatureErrorPoint7;
	//�¶�����8
	uint16_t TemperatureErrorPoint8;
	*/
	//ʪ�ȵ�1
	uint16_t HumidityPoint[POINT_NUM];
	/*
	//ʪ�ȵ�2
	uint16_t HumidityPoint2;
	//ʪ�ȵ�3
	uint16_t HumidityPoint3;
	//ʪ�ȵ�4
	uint16_t HumidityPoint4;
	//ʪ�ȵ�5
	uint16_t HumidityPoint5;
	//ʪ�ȵ�6
	uint16_t HumidityPoint6;
	//ʪ�ȵ�7
	uint16_t HumidityPoint7;
	//ʪ�ȵ�8
	uint16_t HumidityPoint8;
	*/
	//ʪ������1
	uint16_t HumidityErrorPoint[POINT_NUM];
	/*
	//ʪ������2
	uint16_t HumidityErrorPoint2;
	//ʪ������3
	uint16_t HumidityErrorPoint3;
	//ʪ������4
	uint16_t HumidityErrorPoint4;
	//ʪ������5
	uint16_t HumidityErrorPoint5;
	//ʪ������6
	uint16_t HumidityErrorPoint6;
	//ʪ������7
	uint16_t HumidityErrorPoint7;
	//ʪ������8
	uint16_t HumidityErrorPoint8;
	*/
	//��ǿ��1
	uint32_t LightStrongPoint[POINT_NUM];
	/*
	//��ǿ��2
	uint16_t LightStrongPoint2;
	//��ǿ��3
	uint16_t LightStrongPoint3;
	//��ǿ��4
	uint16_t LightStrongPoint4;
	//��ǿ��5
	uint16_t LightStrongPoint5;
	//��ǿ��6
	uint16_t LightStrongPoint6;
	//��ǿ��7
	uint16_t LightStrongPoint7;
	//��ǿ��8
	uint16_t LightStrongPoint8;
	*/
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����1
	uint8_t RelayPointRemoteControl[RELAY_NUM];
	/*
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl2;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl3;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl4;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl5;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl6;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl7;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl8;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl9;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
	uint16_t RelayPointRemoteControl10;
	//��ǿ���ֶ�Զ�̿��Ƶļ̵�����
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
	uint8_t M15:1 ;	//����

	uint8_t M16_23:8 ;//����

	uint8_t M24_29:6 ;//����
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
	
	uint8_t M41:1 ;//����
	uint8_t M42:1 ;//����
	uint8_t M43:1 ;//����
	uint8_t M44_47:4 ;//����

	uint8_t M48_55:8 ;//����

	uint8_t M56_63:8 ;//����

	uint8_t M64_69:6 ;//����
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
	uint8_t M83_87:5 ;//����
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
		
	SHESHISUO_PLC_DELAY,//��ʱ5���ӣ����Ͳɼ�����
		
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
	
	uint8_t tx_count;//�����������Ĵ��� 	

	uint8_t rx_valuetype;
	uint8_t rx_occupied;//[NUM_MOTOR];//ռλ
	int32_t rx_value;//ֵ

	uint16_t tx_mid;
	
	uint16_t reg_address;//PLC 	��ַ��ֻд��
	uint8_t reg_data[2];//PLC 	��ַ��ֻд��
	uint8_t reg_retry_tx_count; //�ط��ļ���	
	
	uint16_t plc_not_ask_count;//���ߵļ��3��
	
	uint16_t plc_not_ask_state_changed; //���ߡ��ϱ�

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
