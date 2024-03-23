#ifndef  _APP_FAULT_ANA_
#define _APP_FAULT_ANA_

/*
//	printf("reboot error:Low-power!\r\n");				//低功耗管理
//}else
	if(RCC->CSR & 1<<30){
	printf("reboot error:Window watchdog!\r\n");		//窗口看门狗
}else if(RCC->CSR & 1<<29){
	printf("reboot error:Independent watchdog!\r\n");	//独立看门狗
}else if(RCC->CSR & 1<<28){
	printf("reboot error:Software!\r\n");				//软件复位
}else if(RCC->CSR & 1<<27){
	printf("reboot error:POR/PDR!\r\n");				//por/pdr复位
}else if(RCC->CSR & 1<<26){
	printf("reboot error:PIN!\r\n");					//NRST引脚复位
}else if(RCC->CSR & 1<<25){
	printf("reboot error:BOR!\r\n");					//软件写入RMVF位清零
}

*/
typedef enum
{
	REBOOT_FLAG_REMOTE=				(0x0200),//远程重启
	REBOOT_FLAG_BOR=				(0x0100),
	REBOOT_FLAG_PIN=				(0x0080),
	REBOOT_FLAG_POR=				(0x0040),
	REBOOT_FLAG_Software=			(0x0020),
	REBOOT_FLAG_Independentwatchdog=(0x0010),
	REBOOT_FLAG_Windowwatchdog=		(0x0008),
	REBOOT_FLAG_SERVER=				(0x0004),//连续10次不正常，重启
	REBOOT_FLAG_KEY_POWEROFF_TOON = (0x0002),
	REBOOT_FLAG_NB_INIT = 			(0x0001),	//1小时重启一次
		
}ENUM_REBOOT_FLAG;


typedef union _INDICATE
{  
    struct
    {
        uint16_t POR_RST:1;
        uint16_t PUC_RST:1;//
        uint16_t WDT_RST:1;//watch dog		
        uint16_t EOS_OFF:1;
		
        uint16_t PC_OFF:1;
        uint16_t SCH_OFF:1;
        uint16_t AMP_OFF:1;
        uint16_t REBOOT_FLAG_BOR:1;
		
        uint16_t REBOOT_FLAG_PIN:1;
        uint16_t REBOOT_FLAG_POR:1;
        uint16_t REBOOT_FLAG_Software:1;
        uint16_t REBOOT_FLAG_Independentwatchdog:1;
		
        uint16_t REBOOT_FLAG_Windowwatchdog:1;
        uint16_t REBOOT_FLAG_SERVER:1;
        uint16_t REBOOT_FLAG_KEY_POWEROFF_TOON:1;
        uint16_t REBOOT_FLAG_NB_INIT:1;        
    }bIndicate;
	
    uint16_t wIndicate;        
}Indicate_t;                                                                    //错误状态



typedef struct _FAULT_ANALYSIS_t
{		
	uint8_t	res0;
	uint8_t	res1;
	Indicate_t Indicat ;	
	uint16_t	reboot_cout;
	uint16_t	res2;
	uint8_t	res3;
	uint8_t sum;	
}
FAULT_ANALYSIS_t;
extern FAULT_ANALYSIS_t g_Indicat ;

void App_fault_analysis_Init_Var(void);
void App_fault_analysis_Clear_reboot_cout(void);

void App_fault_analysis_Set_Value(uint16_t value);
void App_fault_analysis_Clear_Value(void);
uint16_t App_fault_analysis_Get_Value(void);

void print_reboot_flag(void);
void clear_reboot_flag(void);

#endif
