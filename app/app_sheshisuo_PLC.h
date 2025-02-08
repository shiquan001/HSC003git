
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



//M0-M39
typedef struct app_sheshisuo_PLC_M0_39_REGISTER_ST
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

	uint8_t M16:1 ;
	uint8_t M17:1 ;
	uint8_t M18:1 ;
	uint8_t M19:1 ;
	uint8_t M20:1 ;
	uint8_t M21:1 ;
	uint8_t M22:1 ;
	uint8_t M23:1 ;

	uint8_t M24:1 ;
	uint8_t M25:1 ;
	uint8_t M26:1 ;
	uint8_t M27:1 ;
	uint8_t M28:1 ;
	uint8_t M29:1 ;
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

}app_sheshisuo_PLC_M0_39_REGISTER_st;
// M40-M79 = 40\5个byte
typedef struct app_sheshisuo_PLC_M_REGISTER_40_79_ST
{
	uint8_t M40:1 ;
	uint8_t M41:1 ;
	uint8_t M42:1 ;
	uint8_t M43:1 ;
	uint8_t M44:1 ;
	uint8_t M45:1 ;
	uint8_t M46:1 ;
	uint8_t M47:1 ;

	uint8_t M48:1 ;
	uint8_t M49:1 ;
	uint8_t M50:1 ;
	uint8_t M51:1 ;
	uint8_t M52:1 ;
	uint8_t M53:1 ;
	uint8_t M54:1 ;
	uint8_t M55:1 ;

	uint8_t M56:1 ;
	uint8_t M57:1 ;
	uint8_t M58:1 ;
	uint8_t M59:1 ;
	uint8_t M60:1 ;
	uint8_t M61:1 ;
	uint8_t M62:1 ;
	uint8_t M63:1 ;

	uint8_t M64:1 ;
	uint8_t M65:1 ;
	uint8_t M66:1 ;
	uint8_t M67:1 ;
	uint8_t M68:1 ;
	uint8_t M69:1 ;
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
}app_sheshisuo_PLC_M40_79_REGISTER_st;	
// M80-M95 = 16\2个byte
typedef struct app_sheshisuo_PLC_M_REGISTER_80_95_ST
{
	uint8_t M80:1 ;
	uint8_t M81:1 ;
	uint8_t M82:1 ;
	uint8_t M83:1 ;
	uint8_t M84:1 ;
	uint8_t M85:1 ;
	uint8_t M86:1 ;
	uint8_t M87:1 ;

	uint8_t M88:1 ;
	uint8_t M89:1 ;
	uint8_t M90:1 ;
	uint8_t M91:1 ;
	uint8_t M92:1 ;
	uint8_t M93:1 ;
	uint8_t M94:1 ;
	uint8_t M95:1 ;
}app_sheshisuo_PLC_M80_95_REGISTER_st;
// M96-M127=32\4个byte
typedef struct app_sheshisuo_PLC_M_REGISTER_96_127_ST
{
	uint8_t M96:1 ;
	uint8_t M97:1 ;
	uint8_t M98:1 ;
	uint8_t M99:1 ;
	uint8_t M100:1 ;
	uint8_t M101:1 ;
	uint8_t M102:1 ;
	uint8_t M103:1 ;

	uint8_t M104:1 ;
	uint8_t M105:1 ;
	uint8_t M106:1 ;
	uint8_t M107:1 ;
	uint8_t M108:1 ;
	uint8_t M109:1 ;
	uint8_t M110:1 ;
	uint8_t M111:1 ;

	uint8_t M112:1 ;
	uint8_t M113:1 ;
	uint8_t M114:1 ;
	uint8_t M115:1 ;
	uint8_t M116:1 ;
	uint8_t M117:1 ;
	uint8_t M118:1 ;
	uint8_t M119:1 ;

	uint8_t M120:1 ;
	uint8_t M121:1 ;
	uint8_t M122:1 ;
	uint8_t M123:1 ;
	uint8_t M124:1 ;
	uint8_t M125:1 ;
	uint8_t M126:1 ;
	uint8_t M127:1 ;
}app_sheshisuo_PLC_M96_127_REGISTER_st;
// M128-M183= 56\7个byte
typedef struct  app_sheshisuo_PLC_M_REGISTER_128_183_ST
{
	uint8_t M128:1 ;
	uint8_t M129:1 ;
	uint8_t M130:1 ;
	uint8_t M131:1 ;
	uint8_t M132:1 ;
	uint8_t M133:1 ;
	uint8_t M134:1 ;
	uint8_t M135:1 ;

	uint8_t M136:1 ;
	uint8_t M137:1 ;
	uint8_t M138:1 ;
	uint8_t M139:1 ;
	uint8_t M140:1 ;
	uint8_t M141:1 ;
	uint8_t M142:1 ;
	uint8_t M143:1 ;

	uint8_t M144:1 ;
	uint8_t M145:1 ;
	uint8_t M146:1 ;
	uint8_t M147:1 ;
	uint8_t M148:1 ;
	uint8_t M149:1 ;
	uint8_t M150:1 ;
	uint8_t M151:1 ;

	uint8_t M152:1 ;
	uint8_t M153:1 ;
	uint8_t M154:1 ;
	uint8_t M155:1 ;
	uint8_t M156:1 ;
	uint8_t M157:1 ;
	uint8_t M158:1 ;
	uint8_t M159:1 ;

	uint8_t M160:1 ;
	uint8_t M161:1 ;
	uint8_t M162:1 ;
	uint8_t M163:1 ;
	uint8_t M164:1 ;
	uint8_t M165:1 ;
	uint8_t M166:1 ;
	uint8_t M167:1 ;

	uint8_t M168:1 ;
	uint8_t M169:1 ;
	uint8_t M170:1 ;
	uint8_t M171:1 ;
	uint8_t M172:1 ;
	uint8_t M173:1 ;
	uint8_t M174:1 ;
	uint8_t M175:1 ;

	uint8_t M176:1 ;
	uint8_t M177:1 ;
	uint8_t M178:1 ;
	uint8_t M179:1 ;
	uint8_t M180:1 ;
	uint8_t M181:1 ;
	uint8_t M182:1 ;
	uint8_t M183:1 ;
}app_sheshisuo_PLC_M128_183_REGISTER_st;
//M457-M480
typedef struct app_sheshisuo_PLC_M_REGISTER_456_479_ST
{
	uint8_t M456:1 ;//456
	uint8_t M457:1 ;
	uint8_t M458:1 ;
	uint8_t M459:1 ;
	uint8_t M460:1 ;
	uint8_t M461:1 ;
	uint8_t M462:1 ;
	uint8_t M463:1 ;
	uint8_t M464:1 ;

	uint8_t M465:1 ;
	uint8_t M466:1 ;
	uint8_t M467:1 ;
	uint8_t M468:1 ;
	uint8_t M469:1 ;
	uint8_t M470:1 ;
	uint8_t M471:1 ;
	uint8_t M472:1 ;

	uint8_t M473:1 ;
	uint8_t M474:1 ;
	uint8_t M475:1 ;
	uint8_t M476:1 ;
	uint8_t M477:1 ;
	uint8_t M478:1 ;
	uint8_t M479:1 ;
	// uint8_t M480:1 ;
}app_sheshisuo_PLC_M456_479_REGISTER_st;
//M481-M560
typedef struct app_sheshisuo_PLC_M_REGISTER_480_559_ST
{
	uint8_t M480:1 ;
	uint8_t M481:1 ;
	uint8_t M482:1 ;
	uint8_t M483:1 ;
	uint8_t M484:1 ;
	uint8_t M485:1 ;
	uint8_t M486:1 ;
	uint8_t M487:1 ;

	uint8_t M488:1 ;
	uint8_t M489:1 ;
	uint8_t M490:1 ;
	uint8_t M491:1 ;
	uint8_t M492:1 ;
	uint8_t M493:1 ;
	uint8_t M494:1 ;
	uint8_t M495:1 ;

	uint8_t M496:1 ;
	uint8_t M497:1 ;
	uint8_t M498:1 ;
	uint8_t M499:1 ;
	uint8_t M500:1 ;
	uint8_t M501:1 ;
	uint8_t M502:1 ;
	uint8_t M503:1 ;

	uint8_t M504:1 ;
	uint8_t M505:1 ;
	uint8_t M506:1 ;
	uint8_t M507:1 ;
	uint8_t M508:1 ;
	uint8_t M509:1 ;
	uint8_t M510:1 ;
	uint8_t M511:1 ;	

	uint8_t M512:1 ;
	uint8_t M513:1 ;
	uint8_t M514:1 ;
	uint8_t M515:1 ;
	uint8_t M516:1 ;
	uint8_t M517:1 ;
	uint8_t M518:1 ;
	uint8_t M519:1 ;

	uint8_t M520:1 ;
	uint8_t M521:1 ;
	uint8_t M522:1 ;
	uint8_t M523:1 ;
	uint8_t M524:1 ;
	uint8_t M525:1 ;
	uint8_t M526:1 ;
	uint8_t M527:1 ;

	uint8_t M528:1 ;
	uint8_t M529:1 ;
	uint8_t M530:1 ;
	uint8_t M531:1 ;
	uint8_t M532:1 ;
	uint8_t M533:1 ;
	uint8_t M534:1 ;
	uint8_t M535:1 ;

	uint8_t M536:1 ;
	uint8_t M537:1 ;
	uint8_t M538:1 ;
	uint8_t M539:1 ;
	uint8_t M540:1 ;
	uint8_t M541:1 ;	
	uint8_t M542:1 ;
	uint8_t M543:1 ;

	uint8_t M544:1 ;
	uint8_t M545:1 ;
	uint8_t M546:1 ;
	uint8_t M547:1 ;
	uint8_t M548:1 ;
	uint8_t M549:1 ;
	uint8_t M550:1 ;
	uint8_t M551:1 ;

	uint8_t M552:1 ;
	uint8_t M553:1 ;
	uint8_t M554:1 ;
	uint8_t M555:1 ;
	uint8_t M556:1 ;
	uint8_t M557:1 ;
	uint8_t M558:1 ;
	uint8_t M559:1 ;

}app_sheshisuo_PLC_M480_559_REGISTER_st;

typedef struct M_REGISTER_ST
{
	app_sheshisuo_PLC_M0_39_REGISTER_st m0_39;//M36 M37M38M39 不用= 5个byte
	app_sheshisuo_PLC_M40_79_REGISTER_st m40_79;//不用 = 5个byte
	app_sheshisuo_PLC_M80_95_REGISTER_st m80_95;//不用 = 2个byte
	app_sheshisuo_PLC_M96_127_REGISTER_st m96_127;//M96-M99 不用 = 4个byte
	app_sheshisuo_PLC_M128_183_REGISTER_st m128_183;// M179M180M181M182M183 不用 = 7个byte
	uint8_t M_data[34];	//M184-455 不用 = 34个byte
	app_sheshisuo_PLC_M456_479_REGISTER_st m456_479;//M457-M480不用 = 3个byte

	app_sheshisuo_PLC_M480_559_REGISTER_st m480_559;//M482-M511不用M558 M559 M560 不用 = 10个byte

}M_REGISTER_st;
typedef union M_REGISTER_UN
{
	uint8_t data[70];

	M_REGISTER_st mRegister;
}M_REGISTER70_un;


//M1400 M1463
typedef struct M_REGISTER_1400_1463_ST
{
	uint8_t M1400:1 ;
	uint8_t M1401:1 ;
	uint8_t M1402:1 ;
	uint8_t M1403:1 ;
	uint8_t M1404:1 ;
	uint8_t M1405:1 ;
	uint8_t M1406:1 ;
	uint8_t M1407:1 ;

	uint8_t M1408:1 ;
	uint8_t M1409:1 ;
	uint8_t M1410:1 ;
	uint8_t M1411:1 ;
	uint8_t M1412:1 ;
	uint8_t M1413:1 ;
	uint8_t M1414:1 ;
	uint8_t M1415:1 ;

	uint8_t M1416:1 ;
	uint8_t M1417:1 ;
	uint8_t M1418:1 ;
	uint8_t M1419:1 ;
	uint8_t M1420:1 ;
	uint8_t M1421:1 ;
	uint8_t M1422:1 ;
	uint8_t M1423:1 ;

	uint8_t M1424:1 ;
	uint8_t M1425:1 ;
	uint8_t M1426:1 ;
	uint8_t M1427:1 ;
	uint8_t M1428:1 ;
	uint8_t M1429:1 ;
	uint8_t M1430:1 ;
	uint8_t M1431:1 ;

	uint8_t M1432:1 ;
	uint8_t M1433:1 ;
	uint8_t M1434:1 ;
	uint8_t M1435:1 ;
	uint8_t M1436:1 ;
	uint8_t M1437:1 ;
	uint8_t M1438:1 ;
	uint8_t M1439:1 ;

	uint8_t M1440:1 ;
	uint8_t M1441:1 ;
	uint8_t M1442:1 ;
	uint8_t M1443:1 ;
	uint8_t M1444:1 ;
	uint8_t M1445:1 ;
	uint8_t M1446:1 ;
	uint8_t M1447:1 ;

	uint8_t M1448:1 ;
	uint8_t M1449:1 ;
	uint8_t M1450:1 ;
	uint8_t M1451:1 ;
	uint8_t M1452:1 ;
	uint8_t M1453:1 ;
	uint8_t M1454:1 ;
	uint8_t M1455:1 ;

	uint8_t M1456:1 ;
	uint8_t M1457:1 ;
	uint8_t M1458:1 ;
	uint8_t M1459:1 ;
	uint8_t M1460:1 ;
	uint8_t M1461:1 ;
	uint8_t M1462:1 ;
	uint8_t M1463:1 ;

}M_REGISTER_1400_1463st;

typedef union M_REGISTER8_UN
{
	uint8_t data[8];

	M_REGISTER_1400_1463st mRegister;
}M_REGISTER8_un;

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
//D0-D47
typedef union D0_47_REGISTER_UN
{
	int8_t data[(48)*2];

	struct
	{
		int16_t D0_47[48];
	}dRegister;

}D0_47_REGISTER_un;
// D100-D106 PLC时间校准
typedef union D100_106_UN
{
	int8_t data[(7)*2];
	struct
	{
		int16_t D100_106[7];
	}dRegister;
}D100_106_REGISTER_un;
//D200-D285	 设备控制参数 
typedef union D200_285_UN
{
	int8_t data[(86)*2];
	struct
	{
		int16_t D200_285[86];
	}dRegister;
}D200_285_REGISTER_un;
//D286-D297x  报警值设置
typedef union D286_297_UN
{
	int8_t data[(12)*2];
	struct
	{
		int16_t D286_297[12];
	}dRegister;	
}D286_297_REGISTER_un;
// D590-D595  限位开关数量定义
typedef union D590_595_UN
{
	int8_t data[(6)*2];
	struct
	{
		int16_t D590_595[6];
	}dRegister;
}D590_595_REGISTER_un;
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

	uint8_t rx_valuetype;// 分组1-12
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


extern void App_sheshisuo_PLC_Init_Var(void);
extern void App_sheshisuo_PLC_Loop(void);
extern void App_sheshisuo_PLC_DataReport_SensorBasic(char *SensorBasic_data_tx, char *SensorBasic_data_temp);

extern void App_sheshisuo_PLC_DataReport_SensorExtend(char *SensorBasic_data_tx, char *SensorBasic_data_temp) ;

extern void App_sheshisuo_PLC_Dinfo_Data_process(uint8_t pos,char* msg);
extern void PLC_TypeOccupiedValue_convertToAddress(uint8_t valuetype,uint8_t occupied,int32_t value);
extern int  App_sheshisuo_getValueFromType(uint8_t valuetype ,uint8_t occupied);//获取值
extern void App_sheshisuo_PLC_TimeCalibration(RTC_TimeTypeDef *sTime,RTC_DateTypeDef *sDate);

#endif
