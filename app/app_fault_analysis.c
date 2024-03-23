#include "app.h"

#include "app_fault_analysis.h"



FAULT_ANALYSIS_t g_Indicat ;

/*��ӡ�жϱ�־��*/
void print_reboot_flag(void)
{
	uint16 lFlag = 0;
	
	printf("reboot flag :0x%04X\r\n",RCC->CSR);	//
	/*�жϲ���ӡ����ԭ��*/
	//if(RCC->CSR & 1<<31){
	//	printf("reboot error:Low-power!\r\n");				//�͹��Ĺ���
	//}else
	if(RCC->CSR & 1<<30)
	{
		printf("reboot error:Window watchdog!\r\n");		//���ڿ��Ź�
		lFlag |= REBOOT_FLAG_Windowwatchdog;
	}
	else if(RCC->CSR & 1<<29)
	{
		printf("reboot error:Independent watchdog!\r\n");	//�������Ź�
		lFlag |= REBOOT_FLAG_Independentwatchdog;
	}
	else if(RCC->CSR & 1<<28)
	{
		printf("reboot error:Software!\r\n");				//�����λ
		lFlag |= REBOOT_FLAG_Software;
	}
	else if(RCC->CSR & 1<<27)
	{
		printf("reboot error:POR/PDR!\r\n");				//por/pdr��λ
		lFlag |= REBOOT_FLAG_POR;
	}
	else if(RCC->CSR & 1<<26)
	{
		printf("reboot error:PIN!\r\n");					//NRST���Ÿ�λ
		lFlag |= REBOOT_FLAG_PIN;
		
	}
	else if(RCC->CSR & 1<<25)
	{
		printf("reboot error:BOR!\r\n");					//���д��RMVFλ����
		lFlag |= REBOOT_FLAG_BOR;
	}
	App_fault_analysis_Set_Value(lFlag);
	
}
/*����жϱ�־��*/
void clear_reboot_flag(void)
{
	RCC->CSR |= 1<<24;	//���
}



/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_save_to_flash_Var
*	����˵��:   �����ۼӺͣ����浽flash��
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
void App_fault_analysis_save_to_flash_Var(void)
{	
	u8 l_sum = 0;
	u8* l_point = 0;
	u8 i = 0;
	u8 geshu = 0;

	geshu = sizeof(g_Indicat);
	l_point = (u8*)&g_Indicat.res0;
	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(geshu-1);i++)
	{
		l_sum = l_sum + l_point[i];
	}
	g_Indicat.sum = l_sum;// �����ۼӺ͡�	
	
	/*���浽spi flash����*/
	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_WriteBuffer((u8 *)&g_Indicat.res0, SPI_FLASH_onoff_ADDR_VAR, sizeof(g_Indicat));
	Bsp_spiflash_power_off();		
}

/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_Init_Var
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
void App_fault_analysis_Init_Var(void)
{	
	u8 writer_data_buffer[sizeof(g_Indicat)];
	u8 i = 0;
	u8 l_sum = 0;
	u8 geshu = 0;

	geshu = sizeof(g_Indicat);
	p_info("g_Indicat lenth:%d",geshu);
	
	memset(writer_data_buffer,0,geshu);

	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_ReadBuffer(writer_data_buffer, SPI_FLASH_onoff_ADDR_VAR, geshu);

	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(geshu-1);i++)
	{
		l_sum = l_sum + writer_data_buffer[i];
	}
	/*�ж��ۼӺ��Ƿ���ȷ*/
	if((l_sum == writer_data_buffer[geshu-1])&&(writer_data_buffer[geshu-1] != 0xFF))
	{
		memcpy((u8 *)&g_Indicat.res0,writer_data_buffer,geshu);
		p_info("App_fault_analysis_Init_Var  ok");
	}
	else
	{		
		memset((u8 *)&g_Indicat.res0,0,sizeof(g_Indicat));		
		p_info("App_fault_analysis_Init_Var  new");
	}
	/*  �Ӽ�reboot_cout */
	g_Indicat.reboot_cout ++;	
	printf("App_fault reboot_cout:%d",g_Indicat.reboot_cout);
	
	App_fault_analysis_save_to_flash_Var();
}




/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_Set_Value
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
void App_fault_analysis_Set_Value(uint16_t value)
{	
	printf("App_fault code1:%d",g_Indicat.Indicat.wIndicate);
	g_Indicat.Indicat.wIndicate|= value;	
	printf("App_fault code2:%d",g_Indicat.Indicat.wIndicate);
	App_fault_analysis_save_to_flash_Var();		
}

/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_Clear_Value
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
void App_fault_analysis_Clear_Value(void)
{	
	static uint8_t lTxflag = 0;

	/* ֻ���һ��*/
	if(lTxflag == 0)
	{
		lTxflag = 1;
		g_Indicat.Indicat.wIndicate = 0;	
		App_fault_analysis_save_to_flash_Var();			
	}			
}


/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_Clear_reboot_cout
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
void App_fault_analysis_Clear_reboot_cout(void)
{	
	g_Indicat.reboot_cout = 0;	
	App_fault_analysis_save_to_flash_Var();					
}

/*
*********************************************************************************************************
*	�� �� ��: App_fault_analysis_Get_Value
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��

*********************************************************************************************************
*/
uint16_t App_fault_analysis_Get_Value(void)
{	
	return g_Indicat.Indicat.wIndicate;
}




