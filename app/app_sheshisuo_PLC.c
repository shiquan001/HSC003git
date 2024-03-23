#include "app_sheshisuo_PLC.h"

#include "bsp.h"

#include "app.h"

typedef enum VALUETYPE_EM
{
	VALUETYPE_01 =1,
	VALUETYPE_02 ,	
	VALUETYPE_03 ,
	VALUETYPE_04 ,
	VALUETYPE_05,
	VALUETYPE_06 // 定制给plc的 保温被和顶通风使用
}VALUETYPE_em;

typedef enum INDEX_EM
{
	INDEX_M_REGITER =0,
	INDEX_D0_32_REGITER ,	
	
	INDEX_D128_155_REGITER ,
	INDEX_D180_211_REGITER ,
	INDEX_D212_243_REGITER ,

	INDEX_D350_378_REGITER,
	INDEX_M400_REGITER
	
}INDEX_em;


#define CMD_RETRY_COUNT 3
#define M_REGISTER_NUM_all  83
#define M_REGISTER_BYTERS  11

SHESHISUO_PLC_t g_sheshisuoPLC;
//SHESHISUO_PLC_t g_sheshisuoPLC_copy;
STATUS_OF_EQUIPMENT_st g_statusEquipment;
FacilitiesAccumulateData_st g_FacilitiesAccumulateData;
SensorRealTimeData_st g_SensorRealTimeData ;
AbnormalAlarm_st g_AbnormalAlarm;
strategyControl_st g_strategyControl;

STATUS_OF_EQUIPMENT_st g_statusEquipmentCopy;
FacilitiesAccumulateData_st g_FacilitiesAccumulateDataCopy;
SensorRealTimeData_st g_SensorRealTimeDataCopy ;
AbnormalAlarm_st g_AbnormalAlarmCopy;
strategyControl_st g_strategyControlCopy;

static uint8_t rx_data_PLC[PLC_RX_DATA_LENTH];

uint8_t g_m400Register;
M_REGISTER_un g_mRegister;
D_REGISTER_un g_dRegister;
#if 0
M_REGISTER_un g_mRegisterCopy;
D_REGISTER_un g_dRegisterCopy;
#endif
static uint8_t buffer[16];

uint8_t m_flagCmdAsk = FALSE;//调整cmd应答的优先级，先应答命令，在回答数据

//读取的次数
#define READ_COUNT 7
uint8_t functionCode[READ_COUNT]=
{FUNCTION_CODE_01,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_01};

uint16_t startAddress[READ_COUNT]=
{0,0,128,180,212,350,400};
//10.页面右上角控制柜的模式应该可以切换（m400，常闭为手动，常开为自动）

#define NUM32 (32u)
#define NUM33 (33u)
#define NUM116 (116u)
#define NUM28 (28u)
#define NUM29 (29u)

uint8_t txByters[READ_COUNT]=
{83,NUM33,NUM28,NUM32,NUM32,NUM28,1};

uint8_t rxByters[READ_COUNT]=
{11,NUM33*2,NUM28*2,NUM32*2,NUM32*2,NUM28*2,1};

void App_sheshisuo_PLC_writer_ask(void);

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_Init_Var
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuo_PLC_Init_Var(void)
{	
	memset(&g_sheshisuoPLC,0,sizeof(g_sheshisuoPLC));	

	
	g_sheshisuoPLC.PLC_start = TRUE;
	g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_OPEN;


	memset(&g_mRegister,0,sizeof(g_mRegister));	
	memset(&g_dRegister,0,sizeof(g_dRegister)); 
	#if 0
	memcpy(&g_mRegisterCopy,&g_mRegister,sizeof(g_mRegister));	
	memcpy(&g_dRegisterCopy,&g_dRegister,sizeof(g_dRegister));	
	#endif

	g_m400Register = 0;

}

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuoni_dataExchange_statusEquipment
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuoni_dataExchange_statusEquipment(void)
{	
	/*工作模式 workMode	状态：1=手动、2=自动*/
	g_statusEquipment.workMode = g_mRegister.mRegister.M0+1;
	
	g_statusEquipment.insulationCover = (g_mRegister.mRegister.M1<<1)|(g_mRegister.mRegister.M2<<0);
	g_statusEquipment.insulationCover_forwardTimer= g_dRegister.dRegister.D350_378[376-D350];
	g_statusEquipment.insulationCover_reverseTimer= g_dRegister.dRegister.D350_378[377-D350];
	g_statusEquipment.insulationCover_position = g_dRegister.dRegister.D350_378[378-D350];;// D378

	g_statusEquipment.topVentilation= (g_mRegister.mRegister.M3<<1)|(g_mRegister.mRegister.M4<<0);
	g_statusEquipment.topVentilation_forwardTimer= g_dRegister.dRegister.D350_378[370-D350];
	g_statusEquipment.topVentilation_reverseTimer= g_dRegister.dRegister.D350_378[371-D350];
	g_statusEquipment.topVentilation_position= g_dRegister.dRegister.D350_378[372-D350];

	g_statusEquipment.bottomVentilation= (g_mRegister.mRegister.M5<<1)|(g_mRegister.mRegister.M6<<0);
	g_statusEquipment.bottomVentilation_forwardTimer= g_dRegister.dRegister.D350_378[373-D350];
	g_statusEquipment.bottomVentilation_reverseTimer= g_dRegister.dRegister.D350_378[374-D350];
	g_statusEquipment.bottomVentilation_position= g_dRegister.dRegister.D350_378[375-D350];

	g_statusEquipment.manureSpreader= g_mRegister.mRegister.M7;
	g_statusEquipment.wetSpray= g_mRegister.mRegister.M8;
	g_statusEquipment.fan= g_mRegister.mRegister.M9;
	g_statusEquipment.wetCurtain= g_mRegister.mRegister.M10;

	g_statusEquipment.fillLight= g_mRegister.mRegister.M11;
	g_statusEquipment.circulatingFan= g_mRegister.mRegister.M12;
	g_statusEquipment.ReserveOne= g_mRegister.mRegister.M13;
	g_statusEquipment.ReserveTwo= g_mRegister.mRegister.M14;

	/* 对比数据是否有变化 */
	uint8_t res = FALSE;
	#if 0
	if(NULL != memcmp(&g_mRegister,&g_mRegisterCopy,sizeof(g_mRegister)))
	{												
		res = TRUE;
	}
	memcpy(&g_mRegisterCopy,&g_mRegister,sizeof(g_mRegister)); 
	
	/* 对比数据是否有变化 */
	if(NULL != memcmp(&g_dRegister,&g_dRegisterCopy,sizeof(g_dRegister)))
	{												
		res = TRUE;
	}	
	memcpy(&g_dRegisterCopy,&g_dRegister,sizeof(g_dRegister)); 
	#endif
	if(NULL != memcmp(&g_statusEquipment,&g_statusEquipmentCopy,sizeof(g_statusEquipment)))
	{												
		res = TRUE;
	}
	memcpy(&g_statusEquipmentCopy,&g_statusEquipment,sizeof(g_statusEquipment)); 

	if(NULL != memcmp(&g_FacilitiesAccumulateData,&g_FacilitiesAccumulateDataCopy,sizeof(g_FacilitiesAccumulateData)))
	{												
		res = TRUE;
	}
	memcpy(&g_FacilitiesAccumulateDataCopy,&g_FacilitiesAccumulateData,sizeof(g_FacilitiesAccumulateData)); 
	
	if(NULL != memcmp(&g_SensorRealTimeData,&g_SensorRealTimeDataCopy,sizeof(g_SensorRealTimeData)))
	{												
		res = TRUE;
	}
	memcpy(&g_SensorRealTimeDataCopy,&g_SensorRealTimeData,sizeof(g_SensorRealTimeData)); 

	if(NULL != memcmp(&g_AbnormalAlarm,&g_AbnormalAlarmCopy,sizeof(g_AbnormalAlarm)))
	{												
		res = TRUE;
	}
	memcpy(&g_AbnormalAlarmCopy,&g_AbnormalAlarm,sizeof(g_AbnormalAlarm)); 

	if(NULL != memcmp(&g_strategyControl,&g_strategyControlCopy,sizeof(g_strategyControl)))
	{												
		res = TRUE;
	}
	memcpy(&g_strategyControlCopy,&g_strategyControl,sizeof(g_strategyControl)); 
						
	/* 强制进行命令的应答 ，、延时10s执行*/
	if(m_flagCmdAsk == TRUE)
	{
		m_flagCmdAsk = FALSE ;
		App_sheshisuo_PLC_writer_ask();
		p_info("SHESHISUO_PLC_DELAY ask");							
		g_sheshisuoPLC.tx_count++;

		/* 数据变化上报 */
		if(res == TRUE)
		{
			if((g_tConfig.report_changed))
			{
				bsp_StartTimer(TMR_ID_sheshisuo_PLC_ReportDelay30S,  TMR_DELAY_30S);
				
				p_info("g_sheshisuo plc 数据变化上报ReportDelay40S"); 			
			}
			g_sheshisuoPLC.tx_count++;
		}		
	}		
	else
	{
		/* 数据变化上报 立刻执行*/
		if(res == TRUE)
		{
			if((g_tConfig.report_changed))
			{
				App_caiji_report_start();//
				p_info("g_sheshisuo plc 数据变化上报"); 			
			}
			g_sheshisuoPLC.tx_count++;
		}
	}

}
#if 1

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuoni_FacilitiesAccumulateData_dataExchange
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuoni_FacilitiesAccumulateData_dataExchange(void)
{
	g_FacilitiesAccumulateData.TotalIrrigationFlow = ((uint32_t)(g_dRegister.dRegister.D128_243[128-D128]<<16))|((uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[129-D128]));	

	g_FacilitiesAccumulateData.flow = (uint32_t)(g_dRegister.dRegister.D128_243[131-D128]<<0);

	g_FacilitiesAccumulateData.flowFertileIrrigation = (uint32_t)(g_dRegister.dRegister.D128_243[132-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[133-D128] );

	g_FacilitiesAccumulateData.flowSingleFertilizerAndWaterIrrigation = (uint32_t)(g_dRegister.dRegister.D128_243[135-D128]<<0);
	g_FacilitiesAccumulateData.totalRunningTimeOfTheHumidificationSpray = (uint32_t)(g_dRegister.dRegister.D128_243[136-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[137-D128]);
	g_FacilitiesAccumulateData.singleRunningTimeOfHumidificationSpray = (uint32_t)(g_dRegister.dRegister.D128_243[139-D128]<<0);
	g_FacilitiesAccumulateData.totalRunningTimeOfTheFan = (uint32_t)(g_dRegister.dRegister.D128_243[140-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[141-D128]);
	g_FacilitiesAccumulateData.singleRunningTimeOfTheFan = (uint32_t)(g_dRegister.dRegister.D128_243[143-D128]<<0);
	g_FacilitiesAccumulateData.totalRunningTimeOfTheWetCurtain = (uint32_t)(g_dRegister.dRegister.D128_243[144-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[145-D128]);
	g_FacilitiesAccumulateData.singleRunningTimeOfTheWetCurtain = (uint32_t)(g_dRegister.dRegister.D128_243[147-D128]<<0);

	g_FacilitiesAccumulateData.totalRunningTimeOfTheFillLightIsTurnedOn = (uint32_t)(g_dRegister.dRegister.D128_243[148-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[149-D128]);
	g_FacilitiesAccumulateData.SingleRunningTimeOfTheFillLightIsTurnedOn = (uint32_t)(g_dRegister.dRegister.D128_243[151-D128]<<0);
	g_FacilitiesAccumulateData.totalRunningTimeOfTheCirculatingFan = (uint32_t)(g_dRegister.dRegister.D128_243[152-D128]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[153-D128]);
	g_FacilitiesAccumulateData.SingleRunningTimeOfTheCirculatingFan =  (uint32_t)(g_dRegister.dRegister.D128_243[155-D128]<<0);

}
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuoni_strategyControl_dataExchange
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuoni_strategyControl_dataExchange(void)
{
	uint8_t i =0 ;

	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.TimePoint[i].HH= (g_dRegister.dRegister.D128_243[180-D128+i*2]<<0);
		g_strategyControl.TimePoint[i].MM= (g_dRegister.dRegister.D128_243[181-D128+i*2]<<0);
	}
	
	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.TemperaturePoint[i]= (g_dRegister.dRegister.D128_243[196-D128+i]<<0);
	}
	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.TemperatureErrorPoint[i]= (g_dRegister.dRegister.D128_243[204-D128+i]<<0);
	}
	
	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.HumidityPoint[i]= (g_dRegister.dRegister.D128_243[212-D128+i]<<0);
	}
	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.HumidityErrorPoint[i]= (g_dRegister.dRegister.D128_243[220-D128+i]<<0);
	}

	for(i=0;i<POINT_NUM;i++)
	{
		g_strategyControl.LightStrongPoint[i]= (uint32_t)(g_dRegister.dRegister.D128_243[229-D128+i*2]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D128_243[228-D128+i*2]);
	}
	//for(i=0;i<RELAY_NUM;i++)
	{
		g_strategyControl.RelayPointRemoteControl[0]= g_mRegister.mRegister.M30;
		g_strategyControl.RelayPointRemoteControl[1]= g_mRegister.mRegister.M31;
		g_strategyControl.RelayPointRemoteControl[2]= g_mRegister.mRegister.M32;
		g_strategyControl.RelayPointRemoteControl[3]= g_mRegister.mRegister.M33;
		g_strategyControl.RelayPointRemoteControl[4]= g_mRegister.mRegister.M34;
		g_strategyControl.RelayPointRemoteControl[5]= g_mRegister.mRegister.M35;
		g_strategyControl.RelayPointRemoteControl[6]= g_mRegister.mRegister.M36;
		g_strategyControl.RelayPointRemoteControl[7]= g_mRegister.mRegister.M37;
		g_strategyControl.RelayPointRemoteControl[8]= g_mRegister.mRegister.M38;
		g_strategyControl.RelayPointRemoteControl[9]= g_mRegister.mRegister.M39;
		g_strategyControl.RelayPointRemoteControl[10]= g_mRegister.mRegister.M40;		

		g_strategyControl.RelayPointRemoteControl[11]= g_mRegister.mRegister.M41;		
		g_strategyControl.RelayPointRemoteControl[12]= g_mRegister.mRegister.M42;		
		g_strategyControl.RelayPointRemoteControl[13]= g_mRegister.mRegister.M43;		

		g_strategyControl.RelayPointRemoteControl[14]= g_m400Register;
	}

}
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuoni_SensorRealTimeData_dataExchange
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuoni_SensorRealTimeData_dataExchange(void)
{
	g_SensorRealTimeData.RealTimeOutdoorTemperature = g_dRegister.dRegister.D0_32[0-D0];		
	g_SensorRealTimeData.RealTimeOutdoorHumidity = g_dRegister.dRegister.D0_32[1-D0];		
#if 0	
	g_SensorRealTimeData.RealTimeOutdoorLighting = (uint32_t)(g_dRegister.dRegister.D0_32[2-D0]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D0_32[3-D0]);		
#else
	g_SensorRealTimeData.RealTimeOutdoorLighting = (uint32_t)(g_dRegister.dRegister.D0_32[3-D0]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D0_32[2-D0]);		
#endif
	g_SensorRealTimeData.RealTimeOutdoorWindSpeed = g_dRegister.dRegister.D0_32[4-D0];		

	g_SensorRealTimeData.RealTimeOutdoorWindDirection = g_dRegister.dRegister.D0_32[5-D0];		
	g_SensorRealTimeData.RealTimeOutdoorCO2 = g_dRegister.dRegister.D0_32[6-D0]; 	
	g_SensorRealTimeData.AccumulatedOutdoorRainfall = g_dRegister.dRegister.D0_32[7-D0]; 	
	g_SensorRealTimeData.CurrentOutdoorRainfall = g_dRegister.dRegister.D0_32[8-D0];		

	g_SensorRealTimeData.OutdoorInstantaneousRainfall = g_dRegister.dRegister.D0_32[9-D0];		
	g_SensorRealTimeData.OutdoorAtmosphericPressure = g_dRegister.dRegister.D0_32[10-D0];	
	g_SensorRealTimeData.OutdoorRealTimeRainAndSnowState = g_dRegister.dRegister.D0_32[11-D0];	

	g_SensorRealTimeData.RealTimeIndoorTemperature1 = g_dRegister.dRegister.D0_32[12-D0];		
	g_SensorRealTimeData.RealTimeIndoorHumidity1 = g_dRegister.dRegister.D0_32[13-D0];		
	g_SensorRealTimeData.RealTimeIndoorIllumination1 = (uint32_t)(g_dRegister.dRegister.D0_32[14-D0]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D0_32[15-D0]);	
	g_SensorRealTimeData.RealTimeIndoorCO21 = g_dRegister.dRegister.D0_32[16-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilTemperature1 = g_dRegister.dRegister.D0_32[17-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilHumidity1 = g_dRegister.dRegister.D0_32[18-D0];	

	g_SensorRealTimeData.RealTimeIndoorTemperature2 = g_dRegister.dRegister.D0_32[19-D0];		
	g_SensorRealTimeData.RealTimeIndoorHumidity2 = g_dRegister.dRegister.D0_32[20-D0];		
	g_SensorRealTimeData.RealTimeIndoorIllumination2 = (uint32_t)(g_dRegister.dRegister.D0_32[21-D0]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D0_32[22-D0]);	
	g_SensorRealTimeData.RealTimeIndoorCO22 = g_dRegister.dRegister.D0_32[23-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilTemperature2 = g_dRegister.dRegister.D0_32[24-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilHumidity2 = g_dRegister.dRegister.D0_32[25-D0];	

	g_SensorRealTimeData.RealTimeIndoorTemperature3 = g_dRegister.dRegister.D0_32[26-D0];		
	g_SensorRealTimeData.RealTimeIndoorHumidity3 = g_dRegister.dRegister.D0_32[27-D0];		
	g_SensorRealTimeData.RealTimeIndoorIllumination3 = (uint32_t)(g_dRegister.dRegister.D0_32[28-D0]<<16)|(uint32_t)(0x0000FFFF&g_dRegister.dRegister.D0_32[29-D0]);	
	g_SensorRealTimeData.RealTimeIndoorCO23 = g_dRegister.dRegister.D0_32[30-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilTemperature3 = g_dRegister.dRegister.D0_32[31-D0];	
	g_SensorRealTimeData.RealTimeIndoorSoilHumidity3 = g_dRegister.dRegister.D0_32[32-D0];	

}
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuoni_AbnormalAlarm_dataExchange
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuoni_AbnormalAlarm_dataExchange(void)
{
	g_AbnormalAlarm.HighTemperatureAlarm= g_mRegister.mRegister.M70;		
	g_AbnormalAlarm.LowTemperatureWarning= g_mRegister.mRegister.M71;		
	g_AbnormalAlarm.HighHumidityAlarm= g_mRegister.mRegister.M72;		
	g_AbnormalAlarm.LowWetAlarm= g_mRegister.mRegister.M73;		

	g_AbnormalAlarm.RainAlarm= g_mRegister.mRegister.M74;		
	g_AbnormalAlarm.StrongWindAlarm= g_mRegister.mRegister.M75;		
	g_AbnormalAlarm.LightIsTooStrongToAlarm= g_mRegister.mRegister.M76;		
	g_AbnormalAlarm.TooLowCO2ConcentrationAlarm= g_mRegister.mRegister.M77;		

	g_AbnormalAlarm.SoilTemperatureAlarmIsTooLow= g_mRegister.mRegister.M78;		
	g_AbnormalAlarm.SoilTemperatureExcessiveAlarm= g_mRegister.mRegister.M79;		
	g_AbnormalAlarm.SoilMoistureAlarmIsTooLow= g_mRegister.mRegister.M80;		
	g_AbnormalAlarm.SoilMoistureAlarmIsTooHigh= g_mRegister.mRegister.M81;	
	g_AbnormalAlarm.SmokeSenseAlarm= g_mRegister.mRegister.M82;	

	g_AbnormalAlarm.HighTemperatureAlarmValue = g_dRegister.dRegister.D350_378[350-D350];	
	g_AbnormalAlarm.LowTemperatureAlarmValue = g_dRegister.dRegister.D350_378[351-D350];	
	g_AbnormalAlarm.HighHumidityAlarmValue = g_dRegister.dRegister.D350_378[352-D350];	
	g_AbnormalAlarm.LowWetAlarmValue = g_dRegister.dRegister.D350_378[353-D350];	

	g_AbnormalAlarm.RainAlarmValue = g_dRegister.dRegister.D350_378[354-D350];	
	g_AbnormalAlarm.StrongWindAlarmValue = g_dRegister.dRegister.D350_378[355-D350];	
	g_AbnormalAlarm.TheCO2ConcentrationIsTooLowAlarmValue = g_dRegister.dRegister.D350_378[356-D350];	
	int tempHigh = 0;
	int tempLow = 0;
	tempHigh = (g_dRegister.dRegister.D350_378[358-D350]<<16);
	tempLow = 	0x00000000|g_dRegister.dRegister.D350_378[357-D350]<<0;	
	tempLow = 	0x0000FFFF&tempLow;
	g_AbnormalAlarm.LightIntensityIsTooLowForTheAlarmValue = tempHigh|tempLow;
	
	g_AbnormalAlarm.SoilTemperatureAlarmValueIsTooLow = g_dRegister.dRegister.D350_378[359-D350];	
	g_AbnormalAlarm.SoilTemperatureIsTooHighAlarmValue = g_dRegister.dRegister.D350_378[360-D350];	
	g_AbnormalAlarm.SoilMoistureAlarmValueIsTooLow = g_dRegister.dRegister.D350_378[361-D350];	
	g_AbnormalAlarm.TooHighSoilHumidityAlarmValue = g_dRegister.dRegister.D350_378[362-D350];	

	g_AbnormalAlarm.SmokeAlarmValue = g_dRegister.dRegister.D350_378[363-D350];	
}
#endif
/**************************************************************************************                      
* 名    称: App_sheshisuo_PLC_txReadMRegisterCmd
* 功    能:
* 入口参数：
	address:   地址	
	function:       功能码
* 出口参数：无
* 说    明：采集PLC
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_sheshisuo_PLC_txReadMRegisterCmd(uint8_t address ,uint8_t function,uint8_t index)
{
	u8 buffer_lenth=0;
	u16 crc1 = 0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = address;//地址码
	buffer_lenth++;	
	
	buffer[buffer_lenth] = function;//功能码
	buffer_lenth++;	
	
	buffer[buffer_lenth] = startAddress[index]>>8;//寄存器起始地址 2字节
	buffer_lenth++; 		
	buffer[buffer_lenth] = (uint8_t)startAddress[index];// 
	buffer_lenth++; 				

	buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
	buffer_lenth++; 		
	buffer[buffer_lenth] = txByters[index]; //M0-M82
	buffer_lenth++; 

	crc1 = CRC16_Modbus(buffer, buffer_lenth);

	buffer[buffer_lenth] = crc1>>8;//	 
	buffer_lenth++;		
	buffer[buffer_lenth] = (uint8_t)crc1;//	 
	buffer_lenth++;	
	
	//RS485_SendBuf(buffer, buffer_lenth);

	if (g_tUart5.SendBefor != 0)
	{
		g_tUart5.SendBefor();		/* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	}
	HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	RS485_SendOver();
}


/**************************************************************************************                      
* 名    称: App_sheshisuo_tx_PLC_write_cmd
* 功    能:
* 入口参数：
	address:   地址	
	function:       功能码06
* 出口参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_sheshisuo_tx_PLC_write_cmd(uint8_t address ,uint8_t function,uint16_t reg_address,uint16_t reg_num,uint8_t *data)
{

	u8 buffer_lenth=0;
	u16 crc1 = 0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = address;//地址码
	buffer_lenth++;	
	
	buffer[buffer_lenth] = function;//功能码
	buffer_lenth++;	
	
	buffer[buffer_lenth] = (reg_address-0)>>8;//寄存器起始地址high address
	buffer_lenth++;			
	buffer[buffer_lenth] = (uint8_t)(reg_address-0)>>0;// 
	buffer_lenth++;			
	if(function == FUNCTION_CODE_15)//多个线圈写入
	{
		buffer[buffer_lenth] = (reg_num-0)>>8;//寄存器起始地址high address
		buffer_lenth++;			
		buffer[buffer_lenth] = (uint8_t)(reg_num-0)>>0;// 
		buffer_lenth++;	

		uint8_t l_number = 0;
		l_number = reg_num/8;
		if(reg_num%8 != 0)
		{
			l_number++;
		}
		
		buffer[buffer_lenth] = l_number;//2个字节
		buffer_lenth++; 
		
		for(uint8_t i = 0;i < l_number; i++)
		{
			buffer[buffer_lenth] = *data++; //高字节在前，低字节在后；
			buffer_lenth++; 		
		}
		//buffer[buffer_lenth] = *data++; //高字节在前，低字节在后；
		//buffer_lenth++; 		
	}
	else if(function == FUNCTION_CODE_10)//多个寄存器写入
	{
		buffer[buffer_lenth] = (reg_num-0)>>8;//寄存器起始地址high address
		buffer_lenth++;			
		buffer[buffer_lenth] = (uint8_t)(reg_num-0)>>0;// 
		buffer_lenth++;	

		buffer[buffer_lenth] = 2*reg_num;//2个字节
		buffer_lenth++; 
		
		for(uint8_t i = 0;i < (reg_num*2); i++)
		{
			buffer[buffer_lenth] = *data++; //高字节在前，低字节在后；
			buffer_lenth++; 		
		}		
	}
	else if(function == FUNCTION_CODE_05)//1个线圈写入
	{
		//十六进制值 FF 00 请求输出为 ON。十六进制值 00 00 请求输出为 OFF。
		//其它所有值均是非法的，并且对输出不起作用。
		if(*data == 1)
		{
			buffer[buffer_lenth] = 0xff; //高字节在前，低字节在后；
			buffer_lenth++; 		
		}
		else
		{
			buffer[buffer_lenth] = 0x00; //高字节在前，低字节在后；
			buffer_lenth++; 		
		}
			buffer[buffer_lenth] = 0x00; //高字节在前，低字节在后；
			buffer_lenth++; 		
	}	
	else
	{
		for(uint8_t i = 0;i < (reg_num*2); i++)
		{
			buffer[buffer_lenth] = *data++; //高字节在前，低字节在后；
			buffer_lenth++; 		
		}
	}


	crc1 = CRC16_Modbus(buffer, buffer_lenth);

	buffer[buffer_lenth] = crc1>>8;//	 
	buffer_lenth++;		
	buffer[buffer_lenth] = (uint8_t)crc1;//	 
	buffer_lenth++;	
	

	if (g_tUart5.SendBefor != 0)
	{
		g_tUart5.SendBefor();		/* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	}
	HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	RS485_SendOver();
}

typedef struct PLC_TXDATA_st
{
	uint8_t  function;
	uint16_t reg_address;
	uint16_t reg_num;
	uint8_t  data[4];
}
PLC_TXDATA_ST;
#define NUMBER_TWO 2
//命令类型	寄存器地址	寄存器数量	数据
PLC_TXDATA_ST occupied_ADDRESS_VALUE_01[21+1]=
{
{0,0,0,0},//null
{FUNCTION_CODE_05,0,1,0},//M0

{FUNCTION_CODE_15,1,NUMBER_TWO,0},//M1 M2

{FUNCTION_CODE_06,376,1,0},//D376
{FUNCTION_CODE_06,377,1,0},
{FUNCTION_CODE_06,0,0,0},


{FUNCTION_CODE_15,3,NUMBER_TWO,0},//M3 M4

{FUNCTION_CODE_06,370,1,0},//D370
{FUNCTION_CODE_06,371,1,0},
{FUNCTION_CODE_06,372,1,0},


{FUNCTION_CODE_15,5,NUMBER_TWO,0},//M5 M6

{FUNCTION_CODE_06,373,1,0},//D373
{FUNCTION_CODE_06,374,1,0},
{FUNCTION_CODE_06,375,1,0},

{FUNCTION_CODE_05,7,1,0},//M7
{FUNCTION_CODE_05,8,1,0},
{FUNCTION_CODE_05,9,1,0},
{FUNCTION_CODE_05,10,1,0},
{FUNCTION_CODE_05,11,1,0},
{FUNCTION_CODE_05,12,1,0},

{FUNCTION_CODE_05,13,1,0},
{FUNCTION_CODE_05,14,1,0}//M14
};
//命令类型	寄存器地址	寄存器数量	数据

PLC_TXDATA_ST occupied_ADDRESS_VALUE_04[26+1]=
{
	{0,0,0,0},//0
	{0,0,0,0},//01
	{0,0,0,0},//02
	{0,0,0,0},//03
	{0,0,0,0},//04
	{0,0,0,0},//05
	{0,0,0,0},//06
	{0,0,0,0},//07
	{0,0,0,0},//08
	{0,0,0,0},//09
	{0,0,0,0},//10
	{0,0,0,0},//11
	{0,0,0,0},//12
	{0,0,0,0},//13

	{FUNCTION_CODE_06,350,1,0},//D350
	{FUNCTION_CODE_06,351,1,0},
	{FUNCTION_CODE_06,352,1,0},
	{FUNCTION_CODE_06,353,1,0},
	{FUNCTION_CODE_06,354,1,0},
	{FUNCTION_CODE_06,355,1,0},
	{FUNCTION_CODE_06,356,1,0},

	{FUNCTION_CODE_10,357,NUMBER_TWO,0},//D357  D358
		
	{FUNCTION_CODE_06,359,1,0},//D359
	{FUNCTION_CODE_06,360,1,0},
	{FUNCTION_CODE_06,361,1,0},
	{FUNCTION_CODE_06,362,1,0},
	{FUNCTION_CODE_06,363,1,0}
};
//命令类型	寄存器地址	寄存器数量	数据

PLC_TXDATA_ST occupied_ADDRESS_VALUE_05[63+1]=
{
	{0,0,0,0},//0

	{FUNCTION_CODE_10,180,NUMBER_TWO,0},//D180
	{FUNCTION_CODE_10,182,NUMBER_TWO,0},
	{FUNCTION_CODE_10,184,NUMBER_TWO,0},
	{FUNCTION_CODE_10,186,NUMBER_TWO,0},
	{FUNCTION_CODE_10,188,NUMBER_TWO,0},
	{FUNCTION_CODE_10,190,NUMBER_TWO,0},
	{FUNCTION_CODE_10,192,NUMBER_TWO,0},
	{FUNCTION_CODE_10,194,NUMBER_TWO,0},
	
	{FUNCTION_CODE_06,196,1,0},//D196
	{FUNCTION_CODE_06,197,1,0},
	{FUNCTION_CODE_06,198,1,0},
	{FUNCTION_CODE_06,199,1,0},
	{FUNCTION_CODE_06,200,1,0},
	{FUNCTION_CODE_06,201,1,0},
	{FUNCTION_CODE_06,202,1,0},
	{FUNCTION_CODE_06,203,1,0},

	{FUNCTION_CODE_06,204,1,0},
	{FUNCTION_CODE_06,205,1,0},
	{FUNCTION_CODE_06,206,1,0},
	{FUNCTION_CODE_06,207,1,0},
	{FUNCTION_CODE_06,208,1,0},
	{FUNCTION_CODE_06,209,1,0},
	{FUNCTION_CODE_06,210,1,0},
	{FUNCTION_CODE_06,211,1,0},

	{FUNCTION_CODE_06,212,1,0},
	{FUNCTION_CODE_06,213,1,0},
	{FUNCTION_CODE_06,214,1,0},
	{FUNCTION_CODE_06,215,1,0},
	{FUNCTION_CODE_06,216,1,0},
	{FUNCTION_CODE_06,217,1,0},
	{FUNCTION_CODE_06,218,1,0},
	{FUNCTION_CODE_06,219,1,0},	

	{FUNCTION_CODE_06,220,1,0},
	{FUNCTION_CODE_06,221,1,0},
	{FUNCTION_CODE_06,222,1,0},
	{FUNCTION_CODE_06,223,1,0},
	{FUNCTION_CODE_06,224,1,0},
	{FUNCTION_CODE_06,225,1,0},
	{FUNCTION_CODE_06,226,1,0},
	{FUNCTION_CODE_06,227,1,0},	

	{FUNCTION_CODE_10,228,NUMBER_TWO,0},
	{FUNCTION_CODE_10,230,NUMBER_TWO,0},
	{FUNCTION_CODE_10,232,NUMBER_TWO,0},
	{FUNCTION_CODE_10,234,NUMBER_TWO,0},
	{FUNCTION_CODE_10,236,NUMBER_TWO,0},
	{FUNCTION_CODE_10,238,NUMBER_TWO,0},
	{FUNCTION_CODE_10,240,NUMBER_TWO,0},
	{FUNCTION_CODE_10,242,NUMBER_TWO,0},	

/*
(1)保温被开 ：手动远程控制的继电器点1（m30）
(2)保温被关：手动远程控制的继电器点2（m31）
(3)顶通风打开：手动远程控制的继电器点3（m32）
(4)顶通风闭合：手动远程控制的继电器点4（m33）
(5)底通风打开：手动远程控制的继电器点5（m34）
(6)底通风闭合：手动远程控制的继电器点6（m35）
(7)施肥机：手动远程控制的继电器点7（m36）
(8)加湿喷雾：手动远程控制的继电器点8（m37）
(9)风机：手动远程控制的继电器点9（m38）
(10)湿帘：手动远程控制的继电器点10（m39）
(11)补光灯：手动远程控制的继电器点11（m40）
(12)循环风机：手动远程控制的继电器点12（m41）
(13)预留一：手动远程控制的继电器点13（m42）
(14)预留二：手动远程控制的继电器点14（m43）
页面右上角控制柜的模式应该可以切换（m400，常闭为手动，常开为自动）

*/
	{FUNCTION_CODE_05,30,1,0},//M30
	{FUNCTION_CODE_05,31,1,0},
	{FUNCTION_CODE_05,32,1,0},
	{FUNCTION_CODE_05,33,1,0},
	{FUNCTION_CODE_05,34,1,0},
	{FUNCTION_CODE_05,35,1,0},	
	{FUNCTION_CODE_05,36,1,0},
	{FUNCTION_CODE_05,37,1,0},
	{FUNCTION_CODE_05,38,1,0},
	{FUNCTION_CODE_05,39,1,0},
	{FUNCTION_CODE_05,40,1,0}, //M40
	
	{FUNCTION_CODE_05,41,1,0},//M41
	{FUNCTION_CODE_05,42,1,0},
	{FUNCTION_CODE_05,43,1,0}, //M43
	{FUNCTION_CODE_05,400,1,0} //M400

	
};


PLC_TXDATA_ST occupied_ADDRESS_VALUE_06[6+1]=
{
	{0,0,0,0},//0

	{FUNCTION_CODE_05,0,1,0},// 线圈 00
	{FUNCTION_CODE_05,1,1,0},// 线圈 01
};

/*
*********************************************************************************************************
*	函 数 名:  PLC_TypeOccupiedValue_convertToAddress
*	功能说明:   根据占位、值确定plc寄存器的地址
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void PLC_TypeOccupiedValue_convertToAddress(uint8_t valuetype,uint8_t occupied,int32_t value)
{
	PLC_TXDATA_ST plc_txdata;
	memset(&plc_txdata,0,sizeof(PLC_TXDATA_ST));
	
	switch(valuetype)
	{
		case VALUETYPE_01:
		{
			memcpy((uint8_t*)&plc_txdata,(uint8_t*)&occupied_ADDRESS_VALUE_01[occupied],sizeof(PLC_TXDATA_ST));
			if((occupied == 3)||(occupied == 4)||(occupied == 5)||
				(occupied == 7)||(occupied == 8)||(occupied == 9)||
				(occupied == 11)||(occupied == 12)||(occupied == 13))
			{
				plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>0;
			}	
			else if((occupied == 2)||(occupied == 6)||(occupied == 10))
			{
				// 特殊处理，保温被子、低通风、顶通风
				if(value == MOTOR_STATE_positive) value = MOTOR_STATE_reversal;
				else if(value == MOTOR_STATE_reversal) value = MOTOR_STATE_positive;
				plc_txdata.data[0] = value>>0;// D 数据高8bit在前；
			}		
			else if((occupied == 1))
			{
				// 工作模式：1=手动、2=自动
				if(value == 2) value = 1;
				else if(value == 1) value = 0;
				plc_txdata.data[0] = value>>0;// D 数据高8bit在前；
			}				
			else
			{
				plc_txdata.data[0] = value>>0;// M 数据低8bit在前；				
			}
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;
		case VALUETYPE_04:
		{
			memcpy((uint8_t*)&plc_txdata,(uint8_t*)&occupied_ADDRESS_VALUE_04[occupied],sizeof(PLC_TXDATA_ST));
			if((occupied == 21))//21	光强过低报警值
			{
#if 0
				plc_txdata.data[0] = value>>24;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>16;
				plc_txdata.data[2] = value>>8; 
				plc_txdata.data[3] = value>>0;
#else
				plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>0;
				plc_txdata.data[2] = value>>24; 
				plc_txdata.data[3] = value>>16;
#endif

			}	
			else
			{
				plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>0;
			}			
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);			
		}
			break;
		case VALUETYPE_05:
		{
			memcpy((uint8_t*)&plc_txdata,(uint8_t*)&occupied_ADDRESS_VALUE_05[occupied],sizeof(PLC_TXDATA_ST));
			if(((occupied >=1)&&(occupied <=8)))//时间点1（HH:MM）
			{
				plc_txdata.data[0] = value>>24;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>16;
				plc_txdata.data[2] = value>>8; 
				plc_txdata.data[3] = value>>0;
			}	
			else if ((occupied >=41)&&(occupied <=48))//光强点1
			{
				#if 0
				plc_txdata.data[0] = value>>24;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>16;
				plc_txdata.data[2] = value>>8; 
				plc_txdata.data[3] = value>>0;
				#else
				plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>0;
				plc_txdata.data[2] = value>>24; 
				plc_txdata.data[3] = value>>16;
				#endif
			}			
			else if ((occupied >= 49)&&(occupied <= 62))//62
			{
				plc_txdata.data[0] = value>>0;// M 数据低8bit在前；				
			}
			else if ((occupied == 63))//63
			{
				if(value == 2)
					plc_txdata.data[0] = 0; //常开0为    2自动				
				else 
					plc_txdata.data[0] = 1;//常闭1为1手动 								
			}			
			else if ((occupied >= 9)&&(occupied <= 40))
			{
				plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
				plc_txdata.data[1] = value>>0;
			}				
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);			
		}
			break;
		case VALUETYPE_06://特殊定制的 
		{
			memcpy((uint8_t*)&plc_txdata,(uint8_t*)&occupied_ADDRESS_VALUE_06[occupied],sizeof(PLC_TXDATA_ST));
            /* 保温被开启  00     线圈    保温被停止 01       线圈  保温被关闭      02    线圈 */
			if ((occupied  == Equipment_Insulation_TYPE))//保温被
			{
			    if(value == Equipment_Stopped_CMD)
                {         
    			    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x01;
    				plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
                else if(value == Equipment_Opened_CMD)
                {         
                    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x00;
                    plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
                else if(value == Equipment_Closed_CMD)
                {         
                    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x02;
                    plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
			}
            /* 顶通风开启  03       顶通风停止 04         顶通风关闭 05 */
			if ((occupied  == Equipment_RoofVentilation_TYPE))//  顶通风
			{
			    if(value == Equipment_Stopped_CMD)
                {         
    			    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x04;
    				plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
                else if(value == Equipment_Opened_CMD)
                {         
                    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x03;
                    plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
                else if(value == Equipment_Closed_CMD)
                {         
                    plc_txdata.function = FUNCTION_CODE_05;
                    plc_txdata.reg_address = 0x05;
                    plc_txdata.reg_num = 0x01;
                    plc_txdata.data[0] = 1;plc_txdata.data[1] = 0;
                    plc_txdata.data[2] = 0;plc_txdata.data[3] = 0;
                }
			}
            
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);			
            				
            gModbus.address = ADDRESS_PLC_DISPLAY;
            gModbus.function = plc_txdata.function;
            gModbus.dataAddress= plc_txdata.reg_address;
            gModbus.dataLen = plc_txdata.reg_num;
		}
			break;
        
		default:
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名:  PLC_485FIFO_Protocolverification
*	功能说明:   PLC
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t PLC_485FIFO_Protocolverification(uint8_t * start_index,uint8_t address)
{
	uint8_t ucData = 0;
	uint16_t pos = 0;
	uint8_t ret = 0;
	
	uint16_t crc1 = 0;
	uint16_t rx_crc = 0;

	uint16_t i = 0;

	/*等待获得有效的PLC_TH数据	 */ 		
	if (bsp_CheckTimer(TMR_ID_485_UART))
	{
		memset(rx_data_PLC,0,sizeof(rx_data_PLC));
		
		while(1)
		{				
			if(comGetChar(Reserve_485_uart, &ucData))
			{			
				rx_data_PLC[pos++] = ucData;
				//printf("%02X",ucData);
			}
			else
				break;
	
			if(pos >= (PLC_RX_DATA_LENTH))
				break;
		}	
		
		p_info("plc 485 rx:%d!",pos);
		//printf("\r\n");
		/* log */
		if(pos <= 4)
		{
			ret = VERIFICATION_SHORT_LENTH;
			return ret;
		}		
		for(i = 0;i<pos;i++)
		{
			if(rx_data_PLC[i] == address)
			{
				break;
			}
		}
		/* 数据长度太短了 丢弃*/
		if((pos-i) <= 4)
		{
			ret = VERIFICATION_ADDRESS;
			return ret;
		}	
		
		/* 计算CRC校验和 */
		crc1 = CRC16_Modbus(&rx_data_PLC[i], pos-i-2);
		rx_crc = ((uint16_t)rx_data_PLC[pos-i-2] << 8 | rx_data_PLC[pos-i-1]);
		
		if (crc1 != rx_crc)
		{
			ret = VERIFICATION_CRC;
			return ret;
		}

		* start_index = i;
		ret = VERIFICATION_OK;
		return ret;		
	}
	
	ret = VERIFICATION_NO_DATA;
	return ret; 	

}


/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_rxIndexLoop
*	功能说明:    
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuo_PLC_rxIndexLoop(void)
{
	g_sheshisuoPLC.PLC_indexState++;
	if(g_sheshisuoPLC.PLC_indexState<= INDEX_M400_REGITER)
	{
		g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_TX_READ_MREGISTER;
	}
	else
	{
		#if 1
		App_sheshisuoni_FacilitiesAccumulateData_dataExchange();
		App_sheshisuoni_strategyControl_dataExchange();
		#if ENABLE_WATCHDOG	
		HAL_IWDG_Refresh(&hiwdg) ;/* --- 喂狗 */
		#endif		
		App_sheshisuoni_AbnormalAlarm_dataExchange();
		App_sheshisuoni_SensorRealTimeData_dataExchange();
		#if ENABLE_WATCHDOG	
		HAL_IWDG_Refresh(&hiwdg) ;/* --- 喂狗 */
		#endif		
		#endif
		App_sheshisuoni_dataExchange_statusEquipment();//
		g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_DELAY;
		bsp_StartTimer(TMR_ID_sheshisuo_PLC_refresh , TMR_DELAY_20S);//2秒钟采集，不能太快会导致重启 												
	}
}

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_Loop
*	功能说明: 采集 PLC
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
RXVALUE_TYPE_ST  _CmdLastCode;		//对比上一个命令的mid是否一 		


void App_sheshisuo_PLC_Loop(void)
{
	uint16_t i= 0;

	if(g_sheshisuoPLC.PLC_start == TRUE)
	{
	}
	else
	{
		return;
	}

	switch(g_sheshisuoPLC.PLC_work_state)
	{
		case SHESHISUO_PLC_OPEN :
		{	
			Bsp_12V_power_on();	//12 供电。				
			Bsp_sensor_power_on(); //sensor供电。	
			MX_UART5_Init();//初始化串口5防止死机； 20211007
			g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_DELAY;

			bsp_SetTimer(TMR_ID_sheshisuo_PLC_refresh);			
			bsp_StartAutoTimer(TMR_ID_sheshisuo_PLC_cmd_chaxun,10);//10ms

			g_wenshi.m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//发送给服务器数据传感器的标志

			p_info("g_sheshisuoPLC 打开电源PLC");				
		}		
			break;	
		case SHESHISUO_PLC_DELAY:
		{		
			/* 5S	发送一次 */
			if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_ReportDelay30S))
			{
				App_caiji_report_start();
				p_info("SHESHISUO_PLC_DELAY 30后上报");				
			}				
			/* 20S   轮询一次*/
			if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_refresh))
			{
				g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_TX_READ_MREGISTER;
				g_sheshisuoPLC.PLC_indexState = INDEX_M_REGITER;
				p_info("SHESHISUO_PLC_DELAY");				
			}
			else
			{								
				/* 5S	发送一次 */
				//if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_ask_server))
				//{
				//	App_sheshisuo_PLC_writer_ask();
				//	p_info("SHESHISUO_PLC_DELAY ask");				
				//}	
				/* 10mS	定时查询，控制节奏 */
				if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_cmd_chaxun))
				{
					uint32_t CmdCode = 0;

					#if 0
					RXVALUE_TYPE_ST  _CmdLastCode;		//对比上一个命令的mid是否一			
					memset((uint8_t *)&_CmdLastCode,0,sizeof(_CmdLastCode));
					bsp_GetLastrCmd(_CmdLastCode);
					#endif	
					RXVALUE_TYPE_ST  _CmdCode;					
					memset((uint8_t *)&_CmdCode,0,sizeof(_CmdCode));
					#if 1
					CmdCode = bsp_GetCmd(& _CmdCode); 		
					if(CmdCode != Cmd_NONE)
					{
						g_sheshisuoPLC.PLC_work_state = SHESHISUO_CMD_PLC_TX;					
						g_sheshisuoPLC.reg_retry_tx_count = 0;//clear 0
						
						g_sheshisuoPLC.rx_valuetype = _CmdCode.valuetype;	
						g_sheshisuoPLC.rx_occupied  = _CmdCode.occupied;					
						g_sheshisuoPLC.rx_value     = _CmdCode.value;
						g_sheshisuoPLC.tx_mid 		= _CmdCode.mid;


						/*fifo没有cmd 了再去查询和应答*/
						//if((bsp_GetFIFOState_IsNull())||(_CmdLastCode.mid!=_CmdCode.mid))
						if(_CmdLastCode.mid!=_CmdCode.mid)
						{
							bsp_SetTimer(TMR_ID_sheshisuo_PLC_refresh);//cmd之后，快速查询plc状态
							//bsp_SetTimer(TMR_ID_sheshisuo_PLC_ask_server);//再发送应答
							m_flagCmdAsk = TRUE;//
						}
						memcpy((uint8_t * )&_CmdLastCode ,(uint8_t * )&_CmdCode,sizeof(_CmdLastCode));
					}
					#endif
				  }
				}
					

			/* 大于3次，掉线*/
			if(	g_sheshisuoPLC.plc_not_ask_count > CMD_RETRY_COUNT)
			{
				g_sheshisuoPLC.plc_not_ask_count = 0;
				p_err("PLC_TX %d次无应答，掉线",CMD_RETRY_COUNT);	
				g_wenshi.m_sensor_TXflag &= ~SENSOR_TX_SERVER_PLC_SHESHISUO; 	
				if(g_sheshisuoPLC.plc_not_ask_state_changed == 0)
				{
					g_sheshisuoPLC.plc_not_ask_state_changed = 1;
					if((g_tConfig.report_changed))
					{
						App_caiji_report_start();//
						p_info("g_sheshisuo plc 数据变化上报"); 			
					}
					g_sheshisuoPLC.tx_count++;
				}
			}			
		}		
			break;				
		case SHESHISUO_PLC_TX_READ_MREGISTER :
		{	
			comClearRxFifo(COM5);//COM5
		
			/*开始  采集 */ 	
			App_sheshisuo_PLC_txReadMRegisterCmd(ADDRESS_PLC_200,functionCode[g_sheshisuoPLC.PLC_indexState],g_sheshisuoPLC.PLC_indexState);//
			
			bsp_StartTimer(TMR_ID_sheshisuo_PLC_TimerOutError , TMR_DELAY_2S);//5秒钟采集，不能太快会导致重启
			g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_RX_MREGISTER;

			//p_info("g_sheshisuoPLC 发送查询命令PLC:%d",g_sheshisuoPLC.PLC_indexState);											
		}		
			break;	
			
		case SHESHISUO_PLC_RX_MREGISTER :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_PLC_200);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_PLC_200)&&(rx_data_PLC[start_index+1]==functionCode[g_sheshisuoPLC.PLC_indexState])
					&&(rx_data_PLC[start_index+2]==(rxByters[g_sheshisuoPLC.PLC_indexState])))
				{				
					//p_info("g_sheshisuoPLC PLC ok:%d",g_sheshisuoPLC.PLC_indexState);
					g_wenshi.m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_SHESHISUO;//发送给服务器数据传感器的标志
					#if 1
					if(g_sheshisuoPLC.PLC_indexState == INDEX_M_REGITER)
					{
						memcpy(&g_mRegister,&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D0_32_REGITER)
					{
						memcpy(&g_dRegister.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);

						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<NUM33;i++)
						{
							temp = g_dRegister.data[0+i*2];	
							g_dRegister.data[0+i*2] = g_dRegister.data[1+i*2];	
							g_dRegister.data[1+i*2] = temp;	
						}						
					}	
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D128_155_REGITER)
					{
						memcpy(&g_dRegister.data[0+NUM33*2],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<NUM28;i++)
						{
							temp = g_dRegister.data[NUM33*2+0+i*2];	
							g_dRegister.data[NUM33*2+0+i*2] = g_dRegister.data[NUM33*2+1+i*2];	
							g_dRegister.data[NUM33*2+1+i*2] = temp;	
						}						
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D180_211_REGITER)
					{
						memcpy(&g_dRegister.data[0+(NUM33+52)*2],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<NUM32;i++)
						{
							temp = g_dRegister.data[(NUM33+52)*2+0+i*2];	
							g_dRegister.data[(NUM33+52)*2+0+i*2] = g_dRegister.data[(NUM33+52)*2+1+i*2];	
							g_dRegister.data[(NUM33+52)*2+1+i*2] = temp;	
						}						
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D212_243_REGITER)
					{
						memcpy(&g_dRegister.data[0+(NUM33+84)*2],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<NUM32;i++)
						{
							temp = g_dRegister.data[(NUM33+84)*2+0+i*2];	
							g_dRegister.data[(NUM33+84)*2+0+i*2] = g_dRegister.data[(NUM33+84)*2+1+i*2];	
							g_dRegister.data[(NUM33+84)*2+1+i*2] = temp;	
						}						
					}					
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D350_378_REGITER)
					{
						memcpy(&g_dRegister.data[(0+NUM33+NUM116)*2],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<NUM29;i++)
						{
							temp = g_dRegister.data[(NUM33+NUM116)*2+0+i*2];	
							g_dRegister.data[(NUM33+NUM116)*2+0+i*2] = g_dRegister.data[(NUM33+NUM116)*2+1+i*2];	
							g_dRegister.data[(NUM33+NUM116)*2+1+i*2] = temp;	
						}	
					}	
					if(g_sheshisuoPLC.PLC_indexState == INDEX_M400_REGITER)
					{
						memcpy(&g_m400Register,&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
					}						
					#endif
					#if 0
					/* 对比数据是否有变化 */
					if(NULL != memcmp(&g_sheshisuoPLC,&g_sheshisuoPLC_copy,M_REGISTER_NUM_all))
					{												
						if((g_tConfig.report_changed))
						{
							App_caiji_report_start();//
							p_info("g_sheshisuo plc 数据变化上报"); 			
						}
						g_sheshisuoPLC.tx_count++;
					}
					memcpy(&g_sheshisuoPLC_copy,&g_sheshisuoPLC,sizeof(g_sheshisuoPLC)); //备份		
					#endif
				}
				else
				{
					p_err("g_sheshisuoPLC g_sheshisuo RX error ");	
				}

				App_sheshisuo_PLC_rxIndexLoop();//
				g_sheshisuoPLC.plc_not_ask_count = 0;				
				
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("PLC_485FIFO_Protocolverification:%d",return_data);	
			}	
				
			/*PLC_TH 3s 超时退出的判断	 */
			if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_TimerOutError))
			{				
				p_err("TMR_ID_sheshisuo_PLC_TimerOutError:%d",g_sheshisuoPLC.PLC_indexState);	
				g_sheshisuoPLC.plc_not_ask_count++;				
				App_sheshisuo_PLC_rxIndexLoop();//				
			}
		}		
			break;	

			
		case SHESHISUO_CMD_PLC_TX :
		{	
			/* 大于3次，退出*/
			if(	g_sheshisuoPLC.reg_retry_tx_count> CMD_RETRY_COUNT)
			{
				g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_DELAY;
				p_err("CMD_PLC_TX大于3次，退出");	
				
				App_caiji_report_start();//
				p_info("  CMD_PLC_TX大于3次，退出， 数据变化上报"); 				
			}
			else
			{
				comClearRxFifo(COM5);//COM5		
				
				//App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,FUNCTION_CODE_06,g_sheshisuoPLC.reg_address,1,g_sheshisuoPLC.reg_data);//				
				PLC_TypeOccupiedValue_convertToAddress(g_sheshisuoPLC.rx_valuetype,g_sheshisuoPLC.rx_occupied,g_sheshisuoPLC.rx_value);
				
				bsp_StartTimer(TMR_ID_sheshisuo_PLC_TimerOutError , TMR_DELAY_2S);//5秒钟采集，不能太快会导致重启
				g_sheshisuoPLC.PLC_work_state = SHESHISUO_CMD_PLC_RX;				
				p_info("g_sheshisuoPLC 发送 CMD_PLC_TX  ");	
			}														
		}		
			break;	
			
		case SHESHISUO_CMD_PLC_RX :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_PLC_200);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_PLC_200))
				{
					p_info("g_sheshisuoPLC CMD_PLC_RX  ok  ");	
					g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_DELAY;					
					g_sheshisuoPLC.plc_not_ask_count = 0;				
				}
				else
				{
					p_err("g_sheshisuoPLC CMD_PLC_RX error ");	
					//g_sheshisuoPLC.PLC_work_state = CMD_PLC_TX;//重发
					//g_sheshisuoPLC.reg_retry_tx_count++;// 
				}
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("PLC_485FIFO_Protocolverification:%d",return_data);												
				//g_sheshisuoPLC.PLC_work_state = CMD_PLC_TX;//重发
				//g_sheshisuoPLC.reg_retry_tx_count++;// 
			}	

			/*PLC_TH 3s 超时退出的判断	 */
			if(bsp_CheckTimer(TMR_ID_sheshisuo_PLC_TimerOutError))
			{
				g_sheshisuoPLC.PLC_work_state = SHESHISUO_CMD_PLC_TX;
				g_sheshisuoPLC.reg_retry_tx_count++;// 
				p_err("CMD_PLC_RX PLC_TimerOutError:%d",g_sheshisuoPLC.reg_retry_tx_count);		
				g_sheshisuoPLC.plc_not_ask_count++;				
			}			
		}		
			break;

			
		case SHESHISUO_PLC_CLOSE :
		{	
			bsp_StopTimer(TMR_ID_sheshisuo_PLC_refresh);//
			
			HAL_UART_DeInit(&huart5);
			
			Bsp_12V_power_off();
			Bsp_sensor_power_off(); //sensor供电。	
			
			g_sheshisuoPLC.PLC_start = FALSE;
			g_sheshisuoPLC.PLC_work_state = SHESHISUO_PLC_MAX;
				
			/* :  数据变化上报 */			
			p_info("g_sheshisuoPLC 关闭PLC 电源");		
 		}		
			break;				
		default:
			break;
	}
}
#if 0
void * pValue_01[21+1]=
{
null,
&g_statusEquipment.workMode,
&g_statusEquipment.insulationCover,
&g_statusEquipment.insulationCover_forwardTimer,
&g_statusEquipment.insulationCover_reverseTimer,
&g_statusEquipment.insulationCover_position,

&g_statusEquipment.topVentilation,
&g_statusEquipment.topVentilation_forwardTimer,
&g_statusEquipment.topVentilation_reverseTimer,
&g_statusEquipment.topVentilation_position,

&g_statusEquipment.bottomVentilation,
&g_statusEquipment.bottomVentilation_forwardTimer,
&g_statusEquipment.bottomVentilation_reverseTimer,
&g_statusEquipment.bottomVentilation_position,

&g_statusEquipment.manureSpreader,
&g_statusEquipment.wetSpray,
&g_statusEquipment.fan,
&g_statusEquipment.wetCurtain,
&g_statusEquipment.fillLight,
&g_statusEquipment.circulatingFan,
&g_statusEquipment.ReserveOne,
&g_statusEquipment.ReserveTwo,
};
void * pValue_04[26+1]=
{
null,null,null,null,null,
null,null,null,null,null,
null,null,null,
&g_AbnormalAlarm.HighTemperatureAlarmValue,
&g_AbnormalAlarm.LowTemperatureAlarmValue,
&g_AbnormalAlarm.HighHumidityAlarmValue,
&g_AbnormalAlarm.LowWetAlarmValue,

&g_AbnormalAlarm.RainAlarmValue,
&g_AbnormalAlarm.StrongWindAlarmValue,
&g_AbnormalAlarm.TheCO2ConcentrationIsTooLowAlarmValue,
&g_AbnormalAlarm.LightIntensityIsTooLowForTheAlarmValue,

&g_AbnormalAlarm.SoilTemperatureAlarmValueIsTooLow,
&g_AbnormalAlarm.SoilTemperatureIsTooHighAlarmValue,
&g_AbnormalAlarm.SoilMoistureAlarmValueIsTooLow,
&g_AbnormalAlarm.TooHighSoilHumidityAlarmValue,

&g_AbnormalAlarm.SmokeAlarmValue,

};
void * pValue_05[59+1]=
{
null,
&g_strategyControl.TimePoint[0].HH,
&g_strategyControl.TimePoint[1].HH,
&g_strategyControl.TimePoint[2].HH,
&g_strategyControl.TimePoint[3].HH,
&g_strategyControl.TimePoint[4].HH,
&g_strategyControl.TimePoint[5].HH,
&g_strategyControl.TimePoint[6].HH,
&g_strategyControl.TimePoint[7].HH,

&g_strategyControl.TemperaturePoint[0],
&g_strategyControl.TemperaturePoint[1],
&g_strategyControl.TemperaturePoint[2],
&g_strategyControl.TemperaturePoint[3],
&g_strategyControl.TemperaturePoint[4],
&g_strategyControl.TemperaturePoint[5],
&g_strategyControl.TemperaturePoint[6],
&g_strategyControl.TemperaturePoint[7],

&g_strategyControl.TemperatureErrorPoint[0],
&g_strategyControl.TemperatureErrorPoint[1],
&g_strategyControl.TemperatureErrorPoint[2],
&g_strategyControl.TemperatureErrorPoint[3],
&g_strategyControl.TemperatureErrorPoint[4],
&g_strategyControl.TemperatureErrorPoint[5],
&g_strategyControl.TemperatureErrorPoint[6],
&g_strategyControl.TemperatureErrorPoint[7],

&g_strategyControl.HumidityPoint[0],
&g_strategyControl.HumidityPoint[1],
&g_strategyControl.HumidityPoint[2],
&g_strategyControl.HumidityPoint[3],
&g_strategyControl.HumidityPoint[4],
&g_strategyControl.HumidityPoint[5],
&g_strategyControl.HumidityPoint[6],
&g_strategyControl.HumidityPoint[7],

&g_strategyControl.HumidityErrorPoint[0],
&g_strategyControl.HumidityErrorPoint[1],
&g_strategyControl.HumidityErrorPoint[2],
&g_strategyControl.HumidityErrorPoint[3],
&g_strategyControl.HumidityErrorPoint[4],
&g_strategyControl.HumidityErrorPoint[5],
&g_strategyControl.HumidityErrorPoint[6],
&g_strategyControl.HumidityErrorPoint[7],

&g_strategyControl.LightStrongPoint[0],
&g_strategyControl.LightStrongPoint[1],
&g_strategyControl.LightStrongPoint[2],
&g_strategyControl.LightStrongPoint[3],
&g_strategyControl.LightStrongPoint[4],
&g_strategyControl.LightStrongPoint[5],
&g_strategyControl.LightStrongPoint[6],
&g_strategyControl.LightStrongPoint[7],

&g_strategyControl.RelayPointRemoteControl[0],
&g_strategyControl.RelayPointRemoteControl[1],
&g_strategyControl.RelayPointRemoteControl[2],
&g_strategyControl.RelayPointRemoteControl[3],
&g_strategyControl.RelayPointRemoteControl[4],
&g_strategyControl.RelayPointRemoteControl[5],
&g_strategyControl.RelayPointRemoteControl[6],
&g_strategyControl.RelayPointRemoteControl[7],
&g_strategyControl.RelayPointRemoteControl[8],
&g_strategyControl.RelayPointRemoteControl[9],
&g_strategyControl.RelayPointRemoteControl[10],

};
#endif
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_getValueFromType
*	功能说明: 根据类型，占位返回数值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int  App_sheshisuo_getValueFromType(uint8_t valuetype ,uint8_t occupied)
{	
	int value = 0;
	
	switch(valuetype)
	{
		case VALUETYPE_01:
		{
			if(occupied == 1){value =  g_statusEquipment.workMode;}
			
			if(occupied == 2){value =  g_statusEquipment.insulationCover;}
			if(occupied == 3){value =  g_statusEquipment.insulationCover_forwardTimer;}
			if(occupied == 4){value =  g_statusEquipment.insulationCover_reverseTimer;}
			if(occupied == 5){value =  g_statusEquipment.insulationCover_position;}
			
			if(occupied == 6){value =  g_statusEquipment.topVentilation;}
			if(occupied == 7){value =  g_statusEquipment.topVentilation_forwardTimer;}
			if(occupied == 8){value =  g_statusEquipment.topVentilation_reverseTimer;}
			if(occupied == 9){value =  g_statusEquipment.topVentilation_position;}
			
			if(occupied == 10){value =	g_statusEquipment.bottomVentilation;}
			if(occupied == 11){value =	g_statusEquipment.bottomVentilation_forwardTimer;}
			if(occupied == 12){value =	g_statusEquipment.bottomVentilation_reverseTimer;}
			if(occupied == 13){value =	g_statusEquipment.bottomVentilation_position;}
			
			if(occupied == 14){value =	g_statusEquipment.manureSpreader;}
			if(occupied == 15){value =	g_statusEquipment.wetSpray;}
			if(occupied == 16){value =	g_statusEquipment.fan;}
			if(occupied == 17){value =	g_statusEquipment.wetCurtain;}
			if(occupied == 18){value =	g_statusEquipment.fillLight;}
			if(occupied == 19){value =	g_statusEquipment.circulatingFan;}
			if(occupied == 20){value =	g_statusEquipment.ReserveOne;}
			if(occupied == 21){value =	g_statusEquipment.ReserveTwo;}			
		}
			break;
		case VALUETYPE_04:
		{			
			if(occupied == 14){value =	g_AbnormalAlarm.HighTemperatureAlarmValue;}
			if(occupied == 15){value =	g_AbnormalAlarm.LowTemperatureAlarmValue;}
			if(occupied == 16){value =	g_AbnormalAlarm.HighHumidityAlarmValue;}
			if(occupied == 17){value =	g_AbnormalAlarm.LowWetAlarmValue;}
			
			if(occupied == 18){value =	g_AbnormalAlarm.RainAlarmValue;}
			if(occupied == 19){value =	g_AbnormalAlarm.StrongWindAlarmValue;}
			if(occupied == 20){value =	g_AbnormalAlarm.TheCO2ConcentrationIsTooLowAlarmValue;}
			if(occupied == 21){value =	g_AbnormalAlarm.LightIntensityIsTooLowForTheAlarmValue;}
			
			if(occupied == 22){value =	g_AbnormalAlarm.SoilTemperatureAlarmValueIsTooLow;}
			if(occupied == 23){value =	g_AbnormalAlarm.SoilTemperatureIsTooHighAlarmValue;}
			if(occupied == 24){value =	g_AbnormalAlarm.SoilMoistureAlarmValueIsTooLow;}
			if(occupied == 25){value =	g_AbnormalAlarm.TooHighSoilHumidityAlarmValue;}
			
			if(occupied == 26){value =	g_AbnormalAlarm.SmokeAlarmValue;}
		}
			break;
		case VALUETYPE_05:
		{
			if((occupied>=(8*0+1))&&(occupied<=(8*1)))// 时间特殊处理
			{
				value =  (g_strategyControl.TimePoint[occupied-(8*0+1)].HH<<16)|(g_strategyControl.TimePoint[occupied-(8*0+1)].MM);
			}
			if((occupied>=(8*1+1))&&(occupied<=(8*2)))
			{
				value =  g_strategyControl.TemperaturePoint[occupied-(8*1+1)];
			}
			if((occupied>=(8*2+1))&&(occupied<=(8*3)))
			{
				value =  g_strategyControl.TemperatureErrorPoint[occupied-(8*2+1)];
			}	
			if((occupied>=(8*3+1))&&(occupied<=(8*4)))
			{
				value =  g_strategyControl.HumidityPoint[occupied-(8*3+1)];
			}
			if((occupied>=(8*4+1))&&(occupied<=(8*5)))
			{
				value =  g_strategyControl.HumidityErrorPoint[occupied-(8*4+1)];
			}	
			if((occupied>=(8*5+1))&&(occupied<=(8*6)))
			{
				value =  g_strategyControl.LightStrongPoint[occupied-(8*5+1)];
			}	
			if((occupied>=(8*6+1))&&(occupied<=(8*7+7)))// 49-59;  63
			{
				value =  g_strategyControl.RelayPointRemoteControl[occupied-(8*6+1)];
			}							
		}
			break;
		default:
			break;
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_writer_ask
*	功能说明: 
*	形	  参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuo_PLC_writer_ask(void)
{
	if((g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_SHESHISUO) != SENSOR_TX_SERVER_PLC_SHESHISUO)
	{
		p_err("App_sheshisuo_PLC_writer_ask 掉线");	
		return;///不在线，不应答
	}
	u16 lenth = 0;
	u16 sensor_data_lenth = 0;
	
	unsigned long l_long = 0;
	unsigned long l_long_s = 0;
	
	char json_buf[JSON_BUF_LEN];  //用于存放json格式数据
	uint16_t json_len  =0;


	/* 数据组帧*/
	memset(&json_buf,'0',sizeof(json_buf));
	json_len = 0;
	
	json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//基础传感器	SensorBasic 0x0002
	lenth= lenth + 1;
	json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
	lenth= lenth + 1;
	
	//设备执行完命令，命令执行结果上报中的mid要与收到命令中的mid保持一致，这样平台才能刷新对应命令的状态
	json_buf[lenth] = g_sheshisuoPLC.tx_mid>>8;//mid	2
	lenth= lenth + 1;
	json_buf[lenth] = (u8)g_sheshisuoPLC.tx_mid;
	lenth= lenth + 1;	
 
	json_buf[lenth] = 0x00;//errcode	1
	lenth= lenth + 1;
	
	/*时间*/
	l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
	l_long_s = mktime_second(18,1,1,0,0,0);
	if(l_long>=l_long_s)
	{
		json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
	}
	else
	{
		json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
		lenth= lenth + 1;
	}

	/*sensor_data_lenth*/
	//lenth= lenth + 1;
	//lenth= lenth + 1;
#if 1
	int pValue = 0;
	pValue = App_sheshisuo_getValueFromType(g_sheshisuoPLC.rx_valuetype,g_sheshisuoPLC.rx_occupied);
	
	if((g_sheshisuoPLC.rx_valuetype == VALUETYPE_05)&&(g_sheshisuoPLC.rx_occupied>=1)&&(g_sheshisuoPLC.rx_occupied<=8))
	{
		sprintf(&json_buf[lenth+2],"30201,%d,%d,%d:%d",
		g_sheshisuoPLC.rx_valuetype,g_sheshisuoPLC.rx_occupied,pValue>>16,(pValue&0x0000ffff));
		sensor_data_lenth = strlen(&json_buf[lenth+2]);
	}
	else
	{
		sprintf(&json_buf[lenth+2],"30201,%d,%d,%d",
		g_sheshisuoPLC.rx_valuetype,g_sheshisuoPLC.rx_occupied,pValue);
		sensor_data_lenth = strlen(&json_buf[lenth+2]);
	}
#endif	

	json_buf[lenth] =sensor_data_lenth>>8;// 高字节在低位地址；大端模式；
	lenth= lenth + 1;
	json_buf[lenth] = sensor_data_lenth;// 高字节在低位地址；大端模式；
	lenth= lenth + 1;

	lenth= lenth + sensor_data_lenth;
	
	json_len = lenth;

	app_fifo_NB_CoapST_Put(json_buf,&json_len);
}


#define LENTH_TEMP (64u)
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_DataReport_SensorBasic
*	功能说明: 
*	形	  参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuo_PLC_DataReport_SensorBasic(char *SensorBasic_data_tx, char *SensorBasic_data_temp)
{
#ifdef ENABLE_SHESHISUO_PLC 
	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_SHESHISUO)
	{
		/* 1.1.设备运行状态、97个字节*/
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,1);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,",g_statusEquipment.workMode,g_statusEquipment.insulationCover,
				g_statusEquipment.insulationCover_forwardTimer,g_statusEquipment.insulationCover_reverseTimer,g_statusEquipment.insulationCover_position); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_statusEquipment.topVentilation,g_statusEquipment.topVentilation_forwardTimer,
				g_statusEquipment.topVentilation_reverseTimer,g_statusEquipment.topVentilation_position);	
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_statusEquipment.bottomVentilation,g_statusEquipment.bottomVentilation_forwardTimer,
				g_statusEquipment.bottomVentilation_reverseTimer,g_statusEquipment.bottomVentilation_position); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_statusEquipment.manureSpreader,g_statusEquipment.wetSpray,
				g_statusEquipment.fan,g_statusEquipment.wetCurtain);	
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",g_statusEquipment.fillLight,g_statusEquipment.circulatingFan,
				g_statusEquipment.ReserveOne,g_statusEquipment.ReserveTwo); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		/* 1.2.设施积累数据 */
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,2);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_FacilitiesAccumulateData.TotalIrrigationFlow,g_FacilitiesAccumulateData.flow,
				g_FacilitiesAccumulateData.flowFertileIrrigation ,g_FacilitiesAccumulateData.flowSingleFertilizerAndWaterIrrigation ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_FacilitiesAccumulateData.totalRunningTimeOfTheHumidificationSpray,g_FacilitiesAccumulateData.singleRunningTimeOfHumidificationSpray,
				g_FacilitiesAccumulateData.totalRunningTimeOfTheFan ,g_FacilitiesAccumulateData.singleRunningTimeOfTheFan); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}		
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_FacilitiesAccumulateData.totalRunningTimeOfTheWetCurtain,g_FacilitiesAccumulateData.singleRunningTimeOfTheWetCurtain,
				g_FacilitiesAccumulateData.totalRunningTimeOfTheFillLightIsTurnedOn ,g_FacilitiesAccumulateData.SingleRunningTimeOfTheFillLightIsTurnedOn); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}			
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d;",g_FacilitiesAccumulateData.totalRunningTimeOfTheCirculatingFan,g_FacilitiesAccumulateData.SingleRunningTimeOfTheCirculatingFan);
//				g_FacilitiesAccumulateData.totalRunningTimeOfTheFillLightIsTurnedOn ,g_FacilitiesAccumulateData.SingleRunningTimeOfTheFillLightIsTurnedOn); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		/*  1.3.传感器实时数据   */
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,3);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.RealTimeOutdoorTemperature,g_SensorRealTimeData.RealTimeOutdoorHumidity,
				g_SensorRealTimeData.RealTimeOutdoorLighting ,g_SensorRealTimeData.RealTimeOutdoorWindSpeed ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.RealTimeOutdoorWindDirection,g_SensorRealTimeData.RealTimeOutdoorCO2,
				g_SensorRealTimeData.AccumulatedOutdoorRainfall ,g_SensorRealTimeData.CurrentOutdoorRainfall ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.OutdoorInstantaneousRainfall,g_SensorRealTimeData.OutdoorAtmosphericPressure,
				g_SensorRealTimeData.OutdoorRealTimeRainAndSnowState ,g_SensorRealTimeData.RealTimeIndoorTemperature1 ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.RealTimeIndoorHumidity1,g_SensorRealTimeData.RealTimeIndoorIllumination1,
				g_SensorRealTimeData.RealTimeIndoorCO21 ,g_SensorRealTimeData.RealTimeIndoorSoilTemperature1 ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.RealTimeIndoorSoilHumidity1,g_SensorRealTimeData.RealTimeIndoorTemperature2,
				g_SensorRealTimeData.RealTimeIndoorHumidity2 ,g_SensorRealTimeData.RealTimeIndoorIllumination2 ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,",g_SensorRealTimeData.RealTimeIndoorCO22,g_SensorRealTimeData.RealTimeIndoorSoilTemperature2,
				g_SensorRealTimeData.RealTimeIndoorSoilHumidity2 ,g_SensorRealTimeData.RealTimeIndoorTemperature3 ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
		{
			memset(SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d",g_SensorRealTimeData.RealTimeIndoorHumidity3,g_SensorRealTimeData.RealTimeIndoorIllumination3,
				g_SensorRealTimeData.RealTimeIndoorCO23 ,g_SensorRealTimeData.RealTimeIndoorSoilTemperature3,g_SensorRealTimeData.RealTimeIndoorSoilHumidity3 ); 
			strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		}	
	
	}
	else
	{
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,0);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);						
	}		
#endif			

}
char l_SensorBasic_data_tx[482];
char l_SensorBasic_data_temp[LENTH_TEMP];

/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_DataReport_SensorExtend
*	功能说明: 
*	形	  参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void  App_sheshisuo_PLC_DataReport_SensorExtend(void)
{
#if 1
	u16 lenth = 0;
	u16 sensor_data_lenth = 0;
	
	unsigned long l_long = 0;
	unsigned long l_long_s = 0;
	uint8_t i = 0;

	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_SHESHISUO)
	{
		/* 数据组帧*/
		memset(&json_info,'0',sizeof(json_info));
		json_info.json_len = lenth;
		
		json_info.json_buf[lenth] = messageId_SensorExtend>>8;//基础传感器	SensorBasic 0x0002
		lenth= lenth + 1;
		json_info.json_buf[lenth] = (u8)messageId_SensorExtend;
		lenth= lenth + 1;

		/*时间*/
		l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
		l_long_s = mktime_second(18,1,1,0,0,0);
		if(l_long>=l_long_s)
		{
			json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
		}
		else
		{
			json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
		}
		/*sensor_data_lenth*/
		//lenth= lenth + 1;
		//lenth= lenth + 1;
		memset(l_SensorBasic_data_tx,0,sizeof(l_SensorBasic_data_tx));//先清零
		
		/*	1.4.异常报警   */
		memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(l_SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,4);	
		strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.HighTemperatureAlarm,g_AbnormalAlarm.LowTemperatureWarning ,
				g_AbnormalAlarm.HighHumidityAlarm ,g_AbnormalAlarm.LowWetAlarm ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.RainAlarm,g_AbnormalAlarm.StrongWindAlarm ,
				g_AbnormalAlarm.LightIsTooStrongToAlarm ,g_AbnormalAlarm.TooLowCO2ConcentrationAlarm ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.SoilTemperatureAlarmIsTooLow,g_AbnormalAlarm.SoilTemperatureExcessiveAlarm ,
				g_AbnormalAlarm.SoilMoistureAlarmIsTooLow ,g_AbnormalAlarm.SoilMoistureAlarmIsTooHigh ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.SmokeSenseAlarm,g_AbnormalAlarm.HighTemperatureAlarmValue ,
				g_AbnormalAlarm.LowTemperatureAlarmValue ,g_AbnormalAlarm.HighHumidityAlarmValue ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.LowWetAlarmValue,g_AbnormalAlarm.RainAlarmValue ,
				g_AbnormalAlarm.StrongWindAlarmValue ,g_AbnormalAlarm.TheCO2ConcentrationIsTooLowAlarmValue ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d,%d,%d,",g_AbnormalAlarm.LightIntensityIsTooLowForTheAlarmValue,g_AbnormalAlarm.SoilTemperatureAlarmValueIsTooLow ,
				g_AbnormalAlarm.SoilTemperatureIsTooHighAlarmValue ,g_AbnormalAlarm.SoilMoistureAlarmValueIsTooLow ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}
		{
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d,%d;",g_AbnormalAlarm.TooHighSoilHumidityAlarmValue,g_AbnormalAlarm.SmokeAlarmValue);// ,
			//	g_AbnormalAlarm.SoilTemperatureIsTooHighAlarmValue ,g_AbnormalAlarm.SoilMoistureAlarmValueIsTooLow ); 
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
		}	
		/*	1.5.策略控制   */
		memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(l_SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,5);	
		strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);	
		{
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d:%d,",g_strategyControl.TimePoint[i].HH,g_strategyControl.TimePoint[i].MM );
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.TemperaturePoint[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.TemperatureErrorPoint[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.HumidityPoint[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}	
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.HumidityErrorPoint[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}
			for( i=0;i<POINT_NUM;i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.LightStrongPoint[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}	
			for( i=0;i<(RELAY_NUM-1);i++)
			{
				memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
				sprintf(l_SensorBasic_data_temp,"%d,",g_strategyControl.RelayPointRemoteControl[i]);
				strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);
			}		
			memset(l_SensorBasic_data_temp,0,LENTH_TEMP);
			sprintf(l_SensorBasic_data_temp,"%d",g_strategyControl.RelayPointRemoteControl[(RELAY_NUM-1)]);
			strcat(l_SensorBasic_data_tx,l_SensorBasic_data_temp);		
		}	

		/*扩展传感器 有数据就上传 */
		sensor_data_lenth = strlen(l_SensorBasic_data_tx);
		if(sensor_data_lenth >0)
		{
			//if(sensor_data_lenth >=1)	
			//	sensor_data_lenth=sensor_data_lenth-1;//去掉字符串的结尾0
			
			json_info.json_buf[lenth] =sensor_data_lenth>>8;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			json_info.json_buf[lenth] = sensor_data_lenth;// 高字节在低位地址；大端模式；
			lenth= lenth + 1;
			
			strcpy(&json_info.json_buf[lenth],l_SensorBasic_data_tx);// 高字节在低位地址；大端模式；
			lenth= lenth + sensor_data_lenth;	
		}

		json_info.json_len = lenth;
		app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);	
	}		
#endif
}





/*
*********************************************************************************************************
*	函 数 名: App_plc_Dinfo_Data_process
*	功能说明: 
*	形	  参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//#define NUM_COUNT 2 //3、个数据一组
int a[256];//16 个参数	48*2+8=104
char str[512];

void App_sheshisuo_PLC_Dinfo_Data_process(uint8_t pos,char* msg)
{
#ifdef ENABLE_SHESHISUO_PLC

	char *p=0;
	int lenth = 0;
	
	int i=0,j=0,n=0,len=0;	
	char str1[10];
	memset((uint8_t *)&a,0,sizeof(a));
	memset((uint8_t *)&str,0,sizeof(str));
	memset((uint8_t *)&str1,0,sizeof(str1));

	p = strstr(&msg[pos],"plcw,");	
	lenth = strlen("plcw,");	
	strcpy(str,p+lenth);

	len=strlen(str);
	j=0;
	for(i=0;i<len;i++)
	{
		if((str[i]!=',')&&(str[i]!=':'))
			str1[j++]=str[i];
		else
		{
			str1[j]=0;
			if(j>0)
			{
				a[n++]=atoi(str1);
			}
			j=0;
		}
	}	
	if(j>0)
	{
		str1[j]=0;
		a[n++]=atoi(str1);
	}

	if(a[0] != SENSOR_ID_PLC_SHESHISUO)
	{
		p_err("SENSOR_ID_PLC_SHESHISUO: %d!不对",a[0]);
		return ;
	}
	RXVALUE_TYPE_ST   _CmdCode;
	uint8_t NUM_COUNT = 2;//默认2
	uint8_t  count= (n-2)/NUM_COUNT;//数据组
	
	uint8_t  time_count= 0;//time 计数
	
	memset((uint8_t *)&_CmdCode,0,sizeof(_CmdCode));
	_CmdCode.mid = (g_mid);
	_CmdCode.valuetype = a[1];// 类型
	
	for(i=0;i<(count-0);i++)
	{
		_CmdCode.occupied  = a[i*NUM_COUNT+2+time_count];
//		if((_CmdCode.valuetype  == VALUETYPE_05)&&((_CmdCode.occupied >=1)&&(_CmdCode.occupied <=8)))
//		{
//			
//			//NUM_COUNT = 3;// 3一组
//			_CmdCode.value	   = (((0x0000FFFF&a[i*NUM_COUNT+3+time_count])<<16)|((0x0000FFFF&a[i*NUM_COUNT+4+time_count])<<0));	
//			
//			time_count++;//time_count
//			count= (n-2-time_count)/NUM_COUNT;//重新计算  循环次数
//			
//		}
//		else
		{
			_CmdCode.value	   = a[i*NUM_COUNT+3+time_count];
		}
		
		bsp_PutCmd(&_CmdCode);
	}
#endif
}

