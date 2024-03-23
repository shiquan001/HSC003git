#ifndef  _APP_FAULT_ANA_
#define _APP_FAULT_ANA_

/*
//	printf("reboot error:Low-power!\r\n");				//�͹��Ĺ���
//}else
	if(RCC->CSR & 1<<30){
	printf("reboot error:Window watchdog!\r\n");		//���ڿ��Ź�
}else if(RCC->CSR & 1<<29){
	printf("reboot error:Independent watchdog!\r\n");	//�������Ź�
}else if(RCC->CSR & 1<<28){
	printf("reboot error:Software!\r\n");				//�����λ
}else if(RCC->CSR & 1<<27){
	printf("reboot error:POR/PDR!\r\n");				//por/pdr��λ
}else if(RCC->CSR & 1<<26){
	printf("reboot error:PIN!\r\n");					//NRST���Ÿ�λ
}else if(RCC->CSR & 1<<25){
	printf("reboot error:BOR!\r\n");					//���д��RMVFλ����
}

*/
typedef enum
{
	REBOOT_FLAG_REMOTE=				(0x0200),//Զ������
	REBOOT_FLAG_BOR=				(0x0100),
	REBOOT_FLAG_PIN=				(0x0080),
	REBOOT_FLAG_POR=				(0x0040),
	REBOOT_FLAG_Software=			(0x0020),
	REBOOT_FLAG_Independentwatchdog=(0x0010),
	REBOOT_FLAG_Windowwatchdog=		(0x0008),
	REBOOT_FLAG_SERVER=				(0x0004),//����10�β�����������
	REBOOT_FLAG_KEY_POWEROFF_TOON = (0x0002),
	REBOOT_FLAG_NB_INIT = 			(0x0001),	//1Сʱ����һ��
		
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
}Indicate_t;                                                                    //����״̬



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
