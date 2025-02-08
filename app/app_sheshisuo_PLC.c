#include "app_sheshisuo_PLC.h"

#include "bsp.h"

#include "app.h"
// 1、传感器定义	M寄存器	3448	读写
// 2、设备定义	M寄存器	3496	读写
// 3、限位开关数量定义	D寄存器	4686	读写
// 4、PLC时间校准	D寄存器	4196	读写
// 5、设备开关量反馈	M寄存器	2505	只读
// 6、设备控制	M寄存器	2560	读写
// 7、限位开关反馈	M寄存器	2048	只读
// 8、传感器数据	D寄存器	4096	只读
// 9、设备控制参数	D寄存器	4296	读写
// 10、报警值设置	D寄存器	4382	读写
// 11、设备报警	M寄存器	2148	只读
// 12、传感器值报警	M寄存器	2195	只读

typedef enum VALUETYPE_EM
{
	VALUETYPE_01 =1,
	VALUETYPE_02 ,	
	VALUETYPE_03 ,
	VALUETYPE_04 ,
	VALUETYPE_05,
	VALUETYPE_06, // 定制给plc的 保温被和顶通风使用
	VALUETYPE_07,
	VALUETYPE_08,
	VALUETYPE_09,
	VALUETYPE_10,
	VALUETYPE_11,	
	VALUETYPE_12
}VALUETYPE_em;

typedef enum INDEX_EM
{
	INDEX_M1400_1467_REGITER =0,// M1400-M1467 传感器定义 \设备定义;	8个字节

	INDEX_D200_285_REGITER ,	//D200-D285	 设备控制参数 
	INDEX_D286_297_REGITER ,//D286-D297x  报警值设置
	INDEX_D590_595_REGITER,// D590-D595  限位开关数量定义
	INDEX_D100_106_REGITER,// D100-D106 PLC时间校准
	INDEX_D0_47_REGITER ,//D0-D47  传感器数据	

	INDEX_M0_557_REGITER//M0-M557 限位开关反馈\设备报警\传感器值报警\开关量反馈\设备控制;	70个字节
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
M_REGISTER70_un g_mRegister70;//70个字节
M_REGISTER8_un g_mRegister8;//8个字节

D_REGISTER_un g_dRegister;

D0_47_REGISTER_un 	 g_dRegister0_47;
D100_106_REGISTER_un g_dRegister100_106;
D200_285_REGISTER_un g_dRegister200_285;
D286_297_REGISTER_un g_dRegister286_297;
D590_595_REGISTER_un g_dRegister590_595;

#if 0
M_REGISTER70_un g_mRegisterCopy;
D_REGISTER_un g_dRegisterCopy;
#endif
static uint8_t buffer[16];

uint8_t m_flagCmdAsk = FALSE;//调整cmd应答的优先级，先应答命令，在回答数据

//读取的次数 7组数据
#define READ_COUNT 7
uint8_t functionCode[READ_COUNT]=
{FUNCTION_CODE_01,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_03,FUNCTION_CODE_01};

uint16_t startAddress[READ_COUNT]=
{3448,4296,4382,4686,4196,4096,2048};
//10.页面右上角控制柜的模式应该可以切换（m400，常闭为手动，常开为自动）

#define NUM32 (32u)
#define NUM33 (33u)
#define NUM116 (116u)
#define NUM28 (28u)
#define NUM29 (29u)
// NUM86 NUM12  NUM6  NUM7  NUM48  NUM64
#define NUM86 (86u)
#define NUM12 (12u)
#define NUM6 (6u)
#define NUM7 (7u)
#define NUM48 (48u)
#define NUM64 (64u)

uint16_t txByters[READ_COUNT]=
{64,NUM86,NUM12,NUM6,NUM7,NUM48,558};

uint8_t rxByters[READ_COUNT]=
{8,NUM86*2,NUM12*2,NUM6*2,NUM7*2,NUM48*2,70};

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


	memset(&g_mRegister8,0,sizeof(g_mRegister8));	//8个字节初始化M寄存器
	memset(&g_mRegister70,0,sizeof(g_mRegister70));	//70个字节初始化M寄存器

	memset(&g_dRegister0_47,0,sizeof(g_dRegister0_47));	//D0-D47  传感器数据初始化
	memset(&g_dRegister100_106,0,sizeof(g_dRegister100_106));	//D100-D106 PLC时间校准初始化
	memset(&g_dRegister200_285,0,sizeof(g_dRegister200_285));	//D200-D285	 设备控制参数初始化
	memset(&g_dRegister286_297,0,sizeof(g_dRegister286_297));	//D286-D297x  报警值设置初始化
	memset(&g_dRegister590_595,0,sizeof(g_dRegister590_595));	//D590-D595  限位开关数量定义初始化

	memset(&g_dRegister,0,sizeof(g_dRegister)); //D寄存器初始化
	#if 0
	memcpy(&g_mRegisterCopy,&g_mRegister70,sizeof(g_mRegister70));	
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
	// /*工作模式 workMode	状态：1=手动、2=自动*/
	// g_statusEquipment.workMode = g_mRegister70.mRegister.M0+1;
	
	// g_statusEquipment.insulationCover = (g_mRegister70.mRegister.M1<<1)|(g_mRegister70.mRegister.M2<<0);
	// g_statusEquipment.insulationCover_forwardTimer= g_dRegister.dRegister.D350_378[376-D350];
	// g_statusEquipment.insulationCover_reverseTimer= g_dRegister.dRegister.D350_378[377-D350];
	// g_statusEquipment.insulationCover_position = g_dRegister.dRegister.D350_378[378-D350];;// D378

	// g_statusEquipment.topVentilation= (g_mRegister70.mRegister.M3<<1)|(g_mRegister70.mRegister.M4<<0);
	// g_statusEquipment.topVentilation_forwardTimer= g_dRegister.dRegister.D350_378[370-D350];
	// g_statusEquipment.topVentilation_reverseTimer= g_dRegister.dRegister.D350_378[371-D350];
	// g_statusEquipment.topVentilation_position= g_dRegister.dRegister.D350_378[372-D350];

	// g_statusEquipment.bottomVentilation= (g_mRegister70.mRegister.M5<<1)|(g_mRegister70.mRegister.M6<<0);
	// g_statusEquipment.bottomVentilation_forwardTimer= g_dRegister.dRegister.D350_378[373-D350];
	// g_statusEquipment.bottomVentilation_reverseTimer= g_dRegister.dRegister.D350_378[374-D350];
	// g_statusEquipment.bottomVentilation_position= g_dRegister.dRegister.D350_378[375-D350];

	// g_statusEquipment.manureSpreader= g_mRegister70.mRegister.M7;
	// g_statusEquipment.wetSpray= g_mRegister70.mRegister.M8;
	// g_statusEquipment.fan= g_mRegister70.mRegister.M9;
	// g_statusEquipment.wetCurtain= g_mRegister70.mRegister.M10;

	// g_statusEquipment.fillLight= g_mRegister70.mRegister.M11;
	// g_statusEquipment.circulatingFan= g_mRegister70.mRegister.M12;
	// g_statusEquipment.ReserveOne= g_mRegister70.mRegister.M13;
	// g_statusEquipment.ReserveTwo= g_mRegister70.mRegister.M14;

	/* 对比数据是否有变化 */
	uint8_t res = FALSE;
	#if 0
	if(NULL != memcmp(&g_mRegister70,&g_mRegisterCopy,sizeof(g_mRegister70)))
	{										
		
		res = TRUE;
	}
	memcpy(&g_mRegisterCopy,&g_mRegister70,sizeof(g_mRegister70)); 
	
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
		// App_sheshisuo_PLC_writer_ask();
		App_DeviceState_cmdAsk(CMD_ASK_plcwOk);// ok

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
		// g_strategyControl.RelayPointRemoteControl[0]= g_mRegister70.mRegister.M30;
		// g_strategyControl.RelayPointRemoteControl[1]= g_mRegister70.mRegister.M31;
		// g_strategyControl.RelayPointRemoteControl[2]= g_mRegister70.mRegister.M32;
		// g_strategyControl.RelayPointRemoteControl[3]= g_mRegister70.mRegister.M33;
		// g_strategyControl.RelayPointRemoteControl[4]= g_mRegister70.mRegister.M34;
		// g_strategyControl.RelayPointRemoteControl[5]= g_mRegister70.mRegister.M35;
		// g_strategyControl.RelayPointRemoteControl[6]= g_mRegister70.mRegister.M36;
		// g_strategyControl.RelayPointRemoteControl[7]= g_mRegister70.mRegister.M37;
		// g_strategyControl.RelayPointRemoteControl[8]= g_mRegister70.mRegister.M38;
		// g_strategyControl.RelayPointRemoteControl[9]= g_mRegister70.mRegister.M39;
		// g_strategyControl.RelayPointRemoteControl[10]= g_mRegister70.mRegister.M40;		

		// g_strategyControl.RelayPointRemoteControl[11]= g_mRegister70.mRegister.M41;		
		// g_strategyControl.RelayPointRemoteControl[12]= g_mRegister70.mRegister.M42;		
		// g_strategyControl.RelayPointRemoteControl[13]= g_mRegister70.mRegister.M43;		

		// g_strategyControl.RelayPointRemoteControl[14]= g_m400Register;
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
	// g_AbnormalAlarm.HighTemperatureAlarm= g_mRegister70.mRegister.M70;		
	// g_AbnormalAlarm.LowTemperatureWarning= g_mRegister70.mRegister.M71;		
	// g_AbnormalAlarm.HighHumidityAlarm= g_mRegister70.mRegister.M72;		
	// g_AbnormalAlarm.LowWetAlarm= g_mRegister70.mRegister.M73;		

	// g_AbnormalAlarm.RainAlarm= g_mRegister70.mRegister.M74;		
	// g_AbnormalAlarm.StrongWindAlarm= g_mRegister70.mRegister.M75;		
	// g_AbnormalAlarm.LightIsTooStrongToAlarm= g_mRegister70.mRegister.M76;		
	// g_AbnormalAlarm.TooLowCO2ConcentrationAlarm= g_mRegister70.mRegister.M77;		

	// g_AbnormalAlarm.SoilTemperatureAlarmIsTooLow= g_mRegister70.mRegister.M78;		
	// g_AbnormalAlarm.SoilTemperatureExcessiveAlarm= g_mRegister70.mRegister.M79;		
	// g_AbnormalAlarm.SoilMoistureAlarmIsTooLow= g_mRegister70.mRegister.M80;		
	// g_AbnormalAlarm.SoilMoistureAlarmIsTooHigh= g_mRegister70.mRegister.M81;	
	// g_AbnormalAlarm.SmokeSenseAlarm= g_mRegister70.mRegister.M82;	

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

	if(txByters[index] >255)// 读取的字节数 大于255
	{
		buffer[buffer_lenth] = txByters[index]>>8;//寄存器长度 2字节
		buffer_lenth++; 		
		buffer[buffer_lenth] = (uint8_t)txByters[index];// 
		buffer_lenth++; 			
	}
	else
	{
		buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
		buffer_lenth++; 			
		buffer[buffer_lenth] = txByters[index]; //M0-M82
		buffer_lenth++; 
	}

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
// 1、传感器定义	M寄存器	3448	读写
// 2、设备定义		M寄存器	3496	读写
// 3、限位开关数量定义	D寄存器	4686	读写
// 4、PLC时间校准		D寄存器	4196	读写

// 6、设备控制		M寄存器	2560	读写

// 9、设备控制参数	D寄存器	4296	读写
// 10、报警值设置	D寄存器	4382	读写
	
	switch(valuetype)
	{
		case VALUETYPE_01:
		{
			plc_txdata.function = FUNCTION_CODE_05;//  	
			plc_txdata.reg_address = 3448 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>0;// M 数据低8bit在前；				
			
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;
		case VALUETYPE_02:
		{
			plc_txdata.function = FUNCTION_CODE_05;//  	
			plc_txdata.reg_address = 3496 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>0;// M 数据低8bit在前；				
			
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;
		case VALUETYPE_03:
		{
			plc_txdata.function = FUNCTION_CODE_06;// 写入D寄存器 	
			plc_txdata.reg_address = 4686 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
			plc_txdata.data[1] = value>>0;
			plc_txdata.data[2] = value>>24; 
			plc_txdata.data[3] = value>>16;
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;

		case VALUETYPE_04:
		{
			plc_txdata.function = FUNCTION_CODE_06;// 写入D寄存器 	
			plc_txdata.reg_address = 4686 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
			plc_txdata.data[1] = value>>0;
			plc_txdata.data[2] = value>>24; 
			plc_txdata.data[3] = value>>16;
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);

		}
			break;
		case VALUETYPE_06:
		{
			plc_txdata.function = FUNCTION_CODE_05;//  	
			plc_txdata.reg_address = 2560 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>0;// M 数据低8bit在前；				
			
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;
		case VALUETYPE_09:
		{
			plc_txdata.function = FUNCTION_CODE_06;//  	
			plc_txdata.reg_address = 4296 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
			plc_txdata.data[1] = value>>0;
			plc_txdata.data[2] = value>>24; 
			plc_txdata.data[3] = value>>16;
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
		}
			break;
		case VALUETYPE_10:
		{
			plc_txdata.function = FUNCTION_CODE_06;//  	
			plc_txdata.reg_address = 4382 - 1 + occupied;//  	地址需要根据占位和值确定
			plc_txdata.reg_num = 1;// 			
			plc_txdata.data[0] = value>>8;// D 数据高8bit在前；
			plc_txdata.data[1] = value>>0;
			plc_txdata.data[2] = value>>24; 
			plc_txdata.data[3] = value>>16;
			App_sheshisuo_tx_PLC_write_cmd(ADDRESS_PLC_200,plc_txdata.function,
			plc_txdata.reg_address,plc_txdata.reg_num,plc_txdata.data);
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
	if(g_sheshisuoPLC.PLC_indexState<= INDEX_M0_557_REGITER)
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
				g_sheshisuoPLC.PLC_indexState = INDEX_M1400_1467_REGITER;
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
						p_info("cmd mid:%d",_CmdCode.mid);
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
					if(g_sheshisuoPLC.PLC_indexState == INDEX_M1400_1467_REGITER)// M 8个字节
					{
						memcpy(&g_mRegister8,&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D200_285_REGITER)
					{
						memcpy(&g_dRegister200_285.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);

						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<(sizeof(g_dRegister200_285)/2);i++)
						{
							temp = g_dRegister200_285.data[0+i*2];	
							g_dRegister200_285.data[0+i*2] = g_dRegister200_285.data[1+i*2];	
							g_dRegister200_285.data[1+i*2] = temp;	
						}						
					}	
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D286_297_REGITER)
					{
						memcpy(&g_dRegister286_297.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<(sizeof(g_dRegister286_297)/2);i++)
						{
							temp = g_dRegister286_297.data[0+i*2];	
							g_dRegister286_297.data[0+i*2] = g_dRegister286_297.data[1+i*2];	
							g_dRegister286_297.data[1+i*2] = temp;		
						}						
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D590_595_REGITER)
					{
						memcpy(&g_dRegister590_595.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<(sizeof(g_dRegister590_595)/2);i++)
						{
							temp = g_dRegister590_595.data[0+i*2];	
							g_dRegister590_595.data[0+i*2] = g_dRegister590_595.data[1+i*2];	
							g_dRegister590_595.data[1+i*2] = temp;		
						}						
					}
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D100_106_REGITER)
					{
						memcpy(&g_dRegister100_106.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<(sizeof(g_dRegister100_106)/2);i++)
						{
							temp = g_dRegister100_106.data[0+i*2];	
							g_dRegister100_106.data[0+i*2] = g_dRegister100_106.data[1+i*2];	
							g_dRegister100_106.data[1+i*2] = temp;		
						}						
					}					
					if(g_sheshisuoPLC.PLC_indexState == INDEX_D0_47_REGITER)
					{
						memcpy(&g_dRegister0_47.data[0],&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
						/* 高低字节互换 */
						uint8_t temp=0;
						for(i = 0;i<(sizeof(g_dRegister0_47)/2);i++)
						{
							temp = g_dRegister0_47.data[0+i*2];	
							g_dRegister0_47.data[0+i*2] = g_dRegister0_47.data[1+i*2];	
							g_dRegister0_47.data[1+i*2] = temp;		
						}
					}	
					if(g_sheshisuoPLC.PLC_indexState == INDEX_M0_557_REGITER)
					{
						memcpy(&g_mRegister70,&rx_data_PLC[start_index+3],rxByters[g_sheshisuoPLC.PLC_indexState]);
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
		sprintf(&json_buf[lenth+2],"SENSOR_ID_PLC_SHESHISUO2,%d,%d,%d:%d",
		g_sheshisuoPLC.rx_valuetype,g_sheshisuoPLC.rx_occupied,pValue>>16,(pValue&0x0000ffff));
		sensor_data_lenth = strlen(&json_buf[lenth+2]);
	}
	else
	{
		sprintf(&json_buf[lenth+2],"SENSOR_ID_PLC_SHESHISUO2,%d,%d,%d",
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

	// app_fifo_NB_CoapST_Put(json_buf,&json_len);
    if(gUpdate4G.updateStart == FALSE)//升级过程中，不再发送数据给4g模块
    	app_4G_sendData(&json_buf[0],&json_len);
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
		/* 1.传感器定义、
		M1400M1401\M1402\M1403\M1404\M1405\M1406\M1407\
		M1408\M1409\M1410\M1411\M1412\M1413\M1414\M1415\
		M1416\M1417\M1418\M1419\M1420\M1421\M1422\M1423\
		M1424\M1425\M1426\M1427\M1428\M1429\M1430\M1431\
		M1432\M1433\M1434\M1435\M1436\M1437\M1438\M1439\
		M1440\M1441\M1442\M1433\M1444\M1445\M1446\M1447\
		48个bit*/
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,1);	// 1.传感器定义,分组信息1
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);	
		
		memset(SensorBasic_data_temp,0,LENTH_TEMP);// 1.1.传感器定义 8个bit
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1400,g_mRegister8.mRegister.M1401,
			g_mRegister8.mRegister.M1402,g_mRegister8.mRegister.M1403,g_mRegister8.mRegister.M1404,g_mRegister8.mRegister.M1405,
			g_mRegister8.mRegister.M1406,g_mRegister8.mRegister.M1407);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);// 1.1.传感器定义 8个bit
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1408,g_mRegister8.mRegister.M1409,
			g_mRegister8.mRegister.M1410,g_mRegister8.mRegister.M1411,g_mRegister8.mRegister.M1412,g_mRegister8.mRegister.M1413,
			g_mRegister8.mRegister.M1414,g_mRegister8.mRegister.M1415);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);// 1.1.传感器定义 8个bit
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1416,g_mRegister8.mRegister.M1417,
			g_mRegister8.mRegister.M1418,g_mRegister8.mRegister.M1419,g_mRegister8.mRegister.M1420,g_mRegister8.mRegister.M1421,
			g_mRegister8.mRegister.M1422,g_mRegister8.mRegister.M1423);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);// 1.1.传感器定义 8个bit
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1424,g_mRegister8.mRegister.M1425,
			g_mRegister8.mRegister.M1426,g_mRegister8.mRegister.M1427,g_mRegister8.mRegister.M1428,g_mRegister8.mRegister.M1429,
			g_mRegister8.mRegister.M1430,g_mRegister8.mRegister.M1431);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);// 1.1.传感器定义 8个bit
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1432,g_mRegister8.mRegister.M1433,
			g_mRegister8.mRegister.M1434,g_mRegister8.mRegister.M1435,g_mRegister8.mRegister.M1436,g_mRegister8.mRegister.M1437,
			g_mRegister8.mRegister.M1438,g_mRegister8.mRegister.M1439);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d;",g_mRegister8.mRegister.M1440,g_mRegister8.mRegister.M1441,
			g_mRegister8.mRegister.M1442,g_mRegister8.mRegister.M1443,g_mRegister8.mRegister.M1444,g_mRegister8.mRegister.M1445,
			g_mRegister8.mRegister.M1446,g_mRegister8.mRegister.M1447);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		
		/*2\设备定义
		M1448 M1449 M1450 M1451 M1452 M1453 M1454 M1455
		M1456 M1457 M1458 M1459 M1460 M1461 M1462 M1463

		*/	
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,2);	// 2设备定义,分组信息2
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//  
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister8.mRegister.M1448,g_mRegister8.mRegister.M1449,
			g_mRegister8.mRegister.M1450,g_mRegister8.mRegister.M1451,g_mRegister8.mRegister.M1452,g_mRegister8.mRegister.M1453,
			g_mRegister8.mRegister.M1454,g_mRegister8.mRegister.M1455);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//  
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d;",g_mRegister8.mRegister.M1456,g_mRegister8.mRegister.M1457,
			g_mRegister8.mRegister.M1458,g_mRegister8.mRegister.M1459,g_mRegister8.mRegister.M1460,g_mRegister8.mRegister.M1461,
			g_mRegister8.mRegister.M1462,g_mRegister8.mRegister.M1463);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		
		/*3、限位开关数量定义D590 D591 D592 D593 D594 D595			*/
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,3);	// 3、限位开关数量定义,分组信息3
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//  
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d;",g_dRegister590_595.dRegister.D590_595[0],g_dRegister590_595.dRegister.D590_595[1],
			g_dRegister590_595.dRegister.D590_595[2],g_dRegister590_595.dRegister.D590_595[3],g_dRegister590_595.dRegister.D590_595[4],
			g_dRegister590_595.dRegister.D590_595[5]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		
		/*4、PLC时间校准 D100 D101 D102 D103 D104 D105 D106 */
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,4);	// 4、PLC时间校准,分组信息4
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//  
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",g_dRegister100_106.dRegister.D100_106[0],g_dRegister100_106.dRegister.D100_106[1],
			g_dRegister100_106.dRegister.D100_106[2],g_dRegister100_106.dRegister.D100_106[3],g_dRegister100_106.dRegister.D100_106[4],
			g_dRegister100_106.dRegister.D100_106[5],g_dRegister100_106.dRegister.D100_106[6]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);


		/*9、设备控制参数
		D200 D201 D202 D203 D204 D205 D206 D207
		D208 D209 D210 D211 D212 D213 D214 D215
		D216 D217 D218 D219 D220 D221 D222 D223
		D224 D225 D226 D227 D228 D229 D230 D231
		D232 D233 D234 D235 D236 D237 D238 D239
		D240 D241 D242 D243 D244 D245 D246 D247
		D248 D249 D250 D251 D252 D253 D254 D255
		D256 D257 D258 D259 D260 D261 D262 D263
		D264 D265 D266 D267 D268 D269 D270 D271
		D272 D273 D274 D275 D276 D277 D278 D279
		D280 D281 D282 D283 D284 D285
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,9);	// 9、设备控制参数,分组信息9
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[0],g_dRegister200_285.dRegister.D200_285[1],
			g_dRegister200_285.dRegister.D200_285[2],g_dRegister200_285.dRegister.D200_285[3],g_dRegister200_285.dRegister.D200_285[4],g_dRegister200_285.dRegister.D200_285[5],
			g_dRegister200_285.dRegister.D200_285[6],g_dRegister200_285.dRegister.D200_285[7]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[8],g_dRegister200_285.dRegister.D200_285[9],
			g_dRegister200_285.dRegister.D200_285[10],g_dRegister200_285.dRegister.D200_285[11],g_dRegister200_285.dRegister.D200_285[12],g_dRegister200_285.dRegister.D200_285[13],
			g_dRegister200_285.dRegister.D200_285[14],g_dRegister200_285.dRegister.D200_285[15]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[16],g_dRegister200_285.dRegister.D200_285[17],
			g_dRegister200_285.dRegister.D200_285[18],g_dRegister200_285.dRegister.D200_285[19],g_dRegister200_285.dRegister.D200_285[20],g_dRegister200_285.dRegister.D200_285[21],
			g_dRegister200_285.dRegister.D200_285[22],g_dRegister200_285.dRegister.D200_285[23]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[24],g_dRegister200_285.dRegister.D200_285[25],
			g_dRegister200_285.dRegister.D200_285[26],g_dRegister200_285.dRegister.D200_285[27],g_dRegister200_285.dRegister.D200_285[28],g_dRegister200_285.dRegister.D200_285[29],
			g_dRegister200_285.dRegister.D200_285[30],g_dRegister200_285.dRegister.D200_285[31]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[32],g_dRegister200_285.dRegister.D200_285[33],
			g_dRegister200_285.dRegister.D200_285[34],g_dRegister200_285.dRegister.D200_285[35],g_dRegister200_285.dRegister.D200_285[36],g_dRegister200_285.dRegister.D200_285[37],
			g_dRegister200_285.dRegister.D200_285[38],g_dRegister200_285.dRegister.D200_285[39]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[40],g_dRegister200_285.dRegister.D200_285[41],
			g_dRegister200_285.dRegister.D200_285[42],g_dRegister200_285.dRegister.D200_285[43],g_dRegister200_285.dRegister.D200_285[44],g_dRegister200_285.dRegister.D200_285[45],
			g_dRegister200_285.dRegister.D200_285[46],g_dRegister200_285.dRegister.D200_285[47]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[48],g_dRegister200_285.dRegister.D200_285[49],
			g_dRegister200_285.dRegister.D200_285[50],g_dRegister200_285.dRegister.D200_285[51],g_dRegister200_285.dRegister.D200_285[52],g_dRegister200_285.dRegister.D200_285[53],
			g_dRegister200_285.dRegister.D200_285[54],g_dRegister200_285.dRegister.D200_285[55]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[56],g_dRegister200_285.dRegister.D200_285[57],
			g_dRegister200_285.dRegister.D200_285[58],g_dRegister200_285.dRegister.D200_285[59],g_dRegister200_285.dRegister.D200_285[60],g_dRegister200_285.dRegister.D200_285[61],
			g_dRegister200_285.dRegister.D200_285[62],g_dRegister200_285.dRegister.D200_285[63]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[64],g_dRegister200_285.dRegister.D200_285[65],
			g_dRegister200_285.dRegister.D200_285[66],g_dRegister200_285.dRegister.D200_285[67],g_dRegister200_285.dRegister.D200_285[68],g_dRegister200_285.dRegister.D200_285[69],
			g_dRegister200_285.dRegister.D200_285[70],g_dRegister200_285.dRegister.D200_285[71]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister200_285.dRegister.D200_285[72],g_dRegister200_285.dRegister.D200_285[73],
			g_dRegister200_285.dRegister.D200_285[74],g_dRegister200_285.dRegister.D200_285[75],g_dRegister200_285.dRegister.D200_285[76],g_dRegister200_285.dRegister.D200_285[77],
			g_dRegister200_285.dRegister.D200_285[78],g_dRegister200_285.dRegister.D200_285[79]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d;",g_dRegister200_285.dRegister.D200_285[80],g_dRegister200_285.dRegister.D200_285[81],
			g_dRegister200_285.dRegister.D200_285[82],g_dRegister200_285.dRegister.D200_285[83],g_dRegister200_285.dRegister.D200_285[84],g_dRegister200_285.dRegister.D200_285[85]
		 );
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		/*10、报警值设置
		D286 D287 D288 D289 D290 D291 D292 D293
		D294 D295 D296 D297
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,10);	// 10、报警值设置,分组信息10
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister286_297.dRegister.D286_297[0],g_dRegister286_297.dRegister.D286_297[1],
			g_dRegister286_297.dRegister.D286_297[2],g_dRegister286_297.dRegister.D286_297[3],g_dRegister286_297.dRegister.D286_297[4],g_dRegister286_297.dRegister.D286_297[5],
			g_dRegister286_297.dRegister.D286_297[6],g_dRegister286_297.dRegister.D286_297[7]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",g_dRegister286_297.dRegister.D286_297[8],g_dRegister286_297.dRegister.D286_297[9],
			g_dRegister286_297.dRegister.D286_297[10],g_dRegister286_297.dRegister.D286_297[11]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

	}
	else
	{
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,0);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);						
	}		
#endif			

}

#ifdef ENABLE_SHESHISUO_PLC 
/*
*********************************************************************************************************
*	函 数 名: App_sheshisuo_PLC_DataReport_SensorBasic
*	功能说明: 
*	形	  参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_sheshisuo_PLC_DataReport_SensorExtend(char *SensorBasic_data_tx, char *SensorBasic_data_temp)
{

	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_SHESHISUO)
	{

		/*5、设备开关量反馈 
		M457 M458 M459 M460 M461 M462 M463 M464 
		M465 M466 M467 M468 M469 M470 M471 M472 
		M473 M474 M475 M476 M477 M478 M479 M480 M481
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,5);	// 5、设备开关量反馈,分组信息5
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m456_479.M457,g_mRegister70.mRegister.m456_479.M458,
			g_mRegister70.mRegister.m456_479.M459,g_mRegister70.mRegister.m456_479.M460,g_mRegister70.mRegister.m456_479.M461,g_mRegister70.mRegister.m456_479.M462,
			g_mRegister70.mRegister.m456_479.M463,g_mRegister70.mRegister.m456_479.M464);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m456_479.M465,g_mRegister70.mRegister.m456_479.M466,
			g_mRegister70.mRegister.m456_479.M467,g_mRegister70.mRegister.m456_479.M468,g_mRegister70.mRegister.m456_479.M469,g_mRegister70.mRegister.m456_479.M470,
			g_mRegister70.mRegister.m456_479.M471,g_mRegister70.mRegister.m456_479.M472);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,%d;",g_mRegister70.mRegister.m456_479.M473,g_mRegister70.mRegister.m456_479.M474,
			g_mRegister70.mRegister.m456_479.M475,g_mRegister70.mRegister.m456_479.M476,g_mRegister70.mRegister.m456_479.M477,g_mRegister70.mRegister.m456_479.M478,
			g_mRegister70.mRegister.m456_479.M479,g_mRegister70.mRegister.m480_559.M480,g_mRegister70.mRegister.m480_559.M481);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		/*6、设备控制 
		M512 M513 M514 M515 M516 M517 M518 M519
		M520 M521 M522 M523 M524 M525 M526 M527
		M528 M529 M530 M531 M532 M533 M534 M535
		M536 M537 M538 M539 M540 M541 M542 M543
		M544 M545 M546 M547 M548 M549 M550 M551
		M552 M553 M554 M555 M556 M557
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,6);	// 6、设备控制,分组信息6
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m480_559.M512,g_mRegister70.mRegister.m480_559.M513,
			g_mRegister70.mRegister.m480_559.M514,g_mRegister70.mRegister.m480_559.M515,g_mRegister70.mRegister.m480_559.M516,g_mRegister70.mRegister.m480_559.M517,
			g_mRegister70.mRegister.m480_559.M518,g_mRegister70.mRegister.m480_559.M519);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m480_559.M520,g_mRegister70.mRegister.m480_559.M521,
			g_mRegister70.mRegister.m480_559.M522,g_mRegister70.mRegister.m480_559.M523,g_mRegister70.mRegister.m480_559.M524,g_mRegister70.mRegister.m480_559.M525,
			g_mRegister70.mRegister.m480_559.M526,g_mRegister70.mRegister.m480_559.M527);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m480_559.M528,g_mRegister70.mRegister.m480_559.M529,
			g_mRegister70.mRegister.m480_559.M530,g_mRegister70.mRegister.m480_559.M531,g_mRegister70.mRegister.m480_559.M532,g_mRegister70.mRegister.m480_559.M533,
			g_mRegister70.mRegister.m480_559.M534,g_mRegister70.mRegister.m480_559.M535);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m480_559.M536,g_mRegister70.mRegister.m480_559.M537,
			g_mRegister70.mRegister.m480_559.M538,g_mRegister70.mRegister.m480_559.M539,g_mRegister70.mRegister.m480_559.M540,g_mRegister70.mRegister.m480_559.M541,
			g_mRegister70.mRegister.m480_559.M542,g_mRegister70.mRegister.m480_559.M543);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m480_559.M544,g_mRegister70.mRegister.m480_559.M545,
			g_mRegister70.mRegister.m480_559.M546,g_mRegister70.mRegister.m480_559.M547,g_mRegister70.mRegister.m480_559.M548,g_mRegister70.mRegister.m480_559.M549,
			g_mRegister70.mRegister.m480_559.M550,g_mRegister70.mRegister.m480_559.M551);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d;",g_mRegister70.mRegister.m480_559.M552,g_mRegister70.mRegister.m480_559.M553,
			g_mRegister70.mRegister.m480_559.M554,g_mRegister70.mRegister.m480_559.M555,g_mRegister70.mRegister.m480_559.M556,g_mRegister70.mRegister.m480_559.M557);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		/*7、限位开关反馈 
		M0 M1 M2 M3 M4 M5 M6 M7 
		M8 M9 M10 M11 M12 M13 M14 M15
		M16 M17 M18 M19 M20 M21 M22 M23
		M24 M25 M26 M27 M28 M29 M30 M31
		M32 M33 M34 M35
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,7);	// 7、限位开关反馈,分组信息7
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m0_39.M0,g_mRegister70.mRegister.m0_39.M1,
			g_mRegister70.mRegister.m0_39.M2,g_mRegister70.mRegister.m0_39.M3,g_mRegister70.mRegister.m0_39.M4,g_mRegister70.mRegister.m0_39.M5,
			g_mRegister70.mRegister.m0_39.M6,g_mRegister70.mRegister.m0_39.M7);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m0_39.M8,g_mRegister70.mRegister.m0_39.M9,
			g_mRegister70.mRegister.m0_39.M10,g_mRegister70.mRegister.m0_39.M11,g_mRegister70.mRegister.m0_39.M12,g_mRegister70.mRegister.m0_39.M13,
			g_mRegister70.mRegister.m0_39.M14,g_mRegister70.mRegister.m0_39.M15);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m0_39.M16,g_mRegister70.mRegister.m0_39.M17,
			g_mRegister70.mRegister.m0_39.M18,g_mRegister70.mRegister.m0_39.M19,g_mRegister70.mRegister.m0_39.M20,g_mRegister70.mRegister.m0_39.M21,
			g_mRegister70.mRegister.m0_39.M22,g_mRegister70.mRegister.m0_39.M23);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m0_39.M24,g_mRegister70.mRegister.m0_39.M25,
			g_mRegister70.mRegister.m0_39.M26,g_mRegister70.mRegister.m0_39.M27,g_mRegister70.mRegister.m0_39.M28,g_mRegister70.mRegister.m0_39.M29,
			g_mRegister70.mRegister.m0_39.M30,g_mRegister70.mRegister.m0_39.M31);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",g_mRegister70.mRegister.m0_39.M32,g_mRegister70.mRegister.m0_39.M33,
			g_mRegister70.mRegister.m0_39.M34,g_mRegister70.mRegister.m0_39.M35);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		/*8、传感器数据 
		D0 D1 D2 D3 D4 D5 D6 D7
		D8 D9 D10 D11 D12 D13 D14 D15
		D16 D17 D18 D19 D20 D21 D22 D23
		D24 D25 D26 D27 D28 D29 D30 D31
		D32 D33 D34 D35 D36 D37 D38 D39
		D40 D41 D42 D43 D44 D45 D46 D47
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,8);	// 8、传感器数据,分组信息8
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister0_47.dRegister.D0_47[0],g_dRegister0_47.dRegister.D0_47[1],
			g_dRegister0_47.dRegister.D0_47[2],g_dRegister0_47.dRegister.D0_47[3],g_dRegister0_47.dRegister.D0_47[4],g_dRegister0_47.dRegister.D0_47[5],
			g_dRegister0_47.dRegister.D0_47[6],g_dRegister0_47.dRegister.D0_47[7]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister0_47.dRegister.D0_47[8],g_dRegister0_47.dRegister.D0_47[9],
			g_dRegister0_47.dRegister.D0_47[10],g_dRegister0_47.dRegister.D0_47[11],g_dRegister0_47.dRegister.D0_47[12],g_dRegister0_47.dRegister.D0_47[13],
			g_dRegister0_47.dRegister.D0_47[14],g_dRegister0_47.dRegister.D0_47[15]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister0_47.dRegister.D0_47[16],g_dRegister0_47.dRegister.D0_47[17],
			g_dRegister0_47.dRegister.D0_47[18],g_dRegister0_47.dRegister.D0_47[19],g_dRegister0_47.dRegister.D0_47[20],g_dRegister0_47.dRegister.D0_47[21],
			g_dRegister0_47.dRegister.D0_47[22],g_dRegister0_47.dRegister.D0_47[23]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister0_47.dRegister.D0_47[24],g_dRegister0_47.dRegister.D0_47[25],
			g_dRegister0_47.dRegister.D0_47[26],g_dRegister0_47.dRegister.D0_47[27],g_dRegister0_47.dRegister.D0_47[28],g_dRegister0_47.dRegister.D0_47[29],
			g_dRegister0_47.dRegister.D0_47[30],g_dRegister0_47.dRegister.D0_47[31]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_dRegister0_47.dRegister.D0_47[32],g_dRegister0_47.dRegister.D0_47[33],
			g_dRegister0_47.dRegister.D0_47[34],g_dRegister0_47.dRegister.D0_47[35],g_dRegister0_47.dRegister.D0_47[36],g_dRegister0_47.dRegister.D0_47[37],
			g_dRegister0_47.dRegister.D0_47[38],g_dRegister0_47.dRegister.D0_47[39]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);

		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d;",g_dRegister0_47.dRegister.D0_47[40],g_dRegister0_47.dRegister.D0_47[41],
			g_dRegister0_47.dRegister.D0_47[42],g_dRegister0_47.dRegister.D0_47[43],g_dRegister0_47.dRegister.D0_47[44],g_dRegister0_47.dRegister.D0_47[45],
			g_dRegister0_47.dRegister.D0_47[46],g_dRegister0_47.dRegister.D0_47[47]);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);		

		/*11、设备报警
		M100 M101 M102 M103 M104 M105 M106 M107
		M108 M109 M110 M111 M112 M113 M114 M115
		M116 M117 M118 M119 M120 M121 M122 M123
		M124 M125 M126 M127 M128 M129 M130 M131
		M132 M133 M134 M135 M136 M137 M138 M139
		M140 M141 M142 M143 M144 M145 M146
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,11);	// 11、设备报警,分组信息11
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m96_127.M100,g_mRegister70.mRegister.m96_127.M101,
			g_mRegister70.mRegister.m96_127.M102,g_mRegister70.mRegister.m96_127.M103,g_mRegister70.mRegister.m96_127.M104,g_mRegister70.mRegister.m96_127.M105,
			g_mRegister70.mRegister.m96_127.M106,g_mRegister70.mRegister.m96_127.M107);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m96_127.M108,g_mRegister70.mRegister.m96_127.M109,
			g_mRegister70.mRegister.m96_127.M110,g_mRegister70.mRegister.m96_127.M111,g_mRegister70.mRegister.m96_127.M112,g_mRegister70.mRegister.m96_127.M113,
			g_mRegister70.mRegister.m96_127.M114,g_mRegister70.mRegister.m96_127.M115);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m96_127.M116,g_mRegister70.mRegister.m96_127.M117,
			g_mRegister70.mRegister.m96_127.M118,g_mRegister70.mRegister.m96_127.M119,g_mRegister70.mRegister.m96_127.M120,g_mRegister70.mRegister.m96_127.M121,
			g_mRegister70.mRegister.m96_127.M122,g_mRegister70.mRegister.m96_127.M123);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m96_127.M124,g_mRegister70.mRegister.m96_127.M125,
			g_mRegister70.mRegister.m96_127.M126,g_mRegister70.mRegister.m96_127.M127,g_mRegister70.mRegister.m128_183.M128,g_mRegister70.mRegister.m128_183.M129,
			g_mRegister70.mRegister.m128_183.M130,g_mRegister70.mRegister.m128_183.M131);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d",g_mRegister70.mRegister.m128_183.M132,g_mRegister70.mRegister.m128_183.M133,
			g_mRegister70.mRegister.m128_183.M134,g_mRegister70.mRegister.m128_183.M135,g_mRegister70.mRegister.m128_183.M136,g_mRegister70.mRegister.m128_183.M137,
			g_mRegister70.mRegister.m128_183.M138,g_mRegister70.mRegister.m128_183.M139);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",g_mRegister70.mRegister.m128_183.M140,g_mRegister70.mRegister.m128_183.M141,
			g_mRegister70.mRegister.m128_183.M142,g_mRegister70.mRegister.m128_183.M143,g_mRegister70.mRegister.m128_183.M144,g_mRegister70.mRegister.m128_183.M145,
			g_mRegister70.mRegister.m128_183.M146);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		/* 12、传感器值报警 
		M147 M148 M149 M150 M151 M152 M153 M154
		M155 M156 M157 M158 M159 M160 M161 M162
		M163 M164 M165 M166 M167 M168 M169 M170
		M171 M172 M173 M174 M175 M176 M177 M178
		*/
		mem_set(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,12);	// 12、传感器值报警,分组信息12
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m128_183.M147,g_mRegister70.mRegister.m128_183.M148,
			g_mRegister70.mRegister.m128_183.M149,g_mRegister70.mRegister.m128_183.M150,g_mRegister70.mRegister.m128_183.M151,g_mRegister70.mRegister.m128_183.M152,
			g_mRegister70.mRegister.m128_183.M153,g_mRegister70.mRegister.m128_183.M154);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m128_183.M155,g_mRegister70.mRegister.m128_183.M156,
			g_mRegister70.mRegister.m128_183.M157,g_mRegister70.mRegister.m128_183.M158,g_mRegister70.mRegister.m128_183.M159,g_mRegister70.mRegister.m128_183.M160,
			g_mRegister70.mRegister.m128_183.M161,g_mRegister70.mRegister.m128_183.M162);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,",g_mRegister70.mRegister.m128_183.M163,g_mRegister70.mRegister.m128_183.M164,
			g_mRegister70.mRegister.m128_183.M165,g_mRegister70.mRegister.m128_183.M166,g_mRegister70.mRegister.m128_183.M167,g_mRegister70.mRegister.m128_183.M168,
			g_mRegister70.mRegister.m128_183.M169,g_mRegister70.mRegister.m128_183.M170);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
		memset(SensorBasic_data_temp,0,LENTH_TEMP);//
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d;",g_mRegister70.mRegister.m128_183.M171,g_mRegister70.mRegister.m128_183.M172,
			g_mRegister70.mRegister.m128_183.M173,g_mRegister70.mRegister.m128_183.M174,g_mRegister70.mRegister.m128_183.M175,g_mRegister70.mRegister.m128_183.M176,
			g_mRegister70.mRegister.m128_183.M177,g_mRegister70.mRegister.m128_183.M178);
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
	}
	else
	{
		memset(SensorBasic_data_temp,0,LENTH_TEMP);
		sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_SHESHISUO,ADDRESS_PLC_200,0);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);						
	}

}
#endif


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


