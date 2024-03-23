#include "app.h"

#include "app_fault_analysis.h"



FAULT_ANALYSIS_t g_Indicat ;

/*打印中断标志字*/
void print_reboot_flag(void)
{
	uint16 lFlag = 0;
	
	printf("reboot flag :0x%04X\r\n",RCC->CSR);	//
	/*判断并打印重启原因*/
	//if(RCC->CSR & 1<<31){
	//	printf("reboot error:Low-power!\r\n");				//低功耗管理
	//}else
	if(RCC->CSR & 1<<30)
	{
		printf("reboot error:Window watchdog!\r\n");		//窗口看门狗
		lFlag |= REBOOT_FLAG_Windowwatchdog;
	}
	else if(RCC->CSR & 1<<29)
	{
		printf("reboot error:Independent watchdog!\r\n");	//独立看门狗
		lFlag |= REBOOT_FLAG_Independentwatchdog;
	}
	else if(RCC->CSR & 1<<28)
	{
		printf("reboot error:Software!\r\n");				//软件复位
		lFlag |= REBOOT_FLAG_Software;
	}
	else if(RCC->CSR & 1<<27)
	{
		printf("reboot error:POR/PDR!\r\n");				//por/pdr复位
		lFlag |= REBOOT_FLAG_POR;
	}
	else if(RCC->CSR & 1<<26)
	{
		printf("reboot error:PIN!\r\n");					//NRST引脚复位
		lFlag |= REBOOT_FLAG_PIN;
		
	}
	else if(RCC->CSR & 1<<25)
	{
		printf("reboot error:BOR!\r\n");					//软件写入RMVF位清零
		lFlag |= REBOOT_FLAG_BOR;
	}
	App_fault_analysis_Set_Value(lFlag);
	
}
/*清除中断标志字*/
void clear_reboot_flag(void)
{
	RCC->CSR |= 1<<24;	//清除
}



/*
*********************************************************************************************************
*	函 数 名: App_fault_analysis_save_to_flash_Var
*	功能说明:   计算累加和，保存到flash中
*	形    参：无
*	返 回 值: 无

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
	/*计算前6个数的累加*/
	l_sum = 0;
	for(i=0;i<(geshu-1);i++)
	{
		l_sum = l_sum + l_point[i];
	}
	g_Indicat.sum = l_sum;// 保存累加和。	
	
	/*保存到spi flash、中*/
	Bsp_spiflash_power_on();//打开gps电源
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_WriteBuffer((u8 *)&g_Indicat.res0, SPI_FLASH_onoff_ADDR_VAR, sizeof(g_Indicat));
	Bsp_spiflash_power_off();		
}

/*
*********************************************************************************************************
*	函 数 名: App_fault_analysis_Init_Var
*	功能说明:  
*	形    参：无
*	返 回 值: 无

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

	Bsp_spiflash_power_on();//打开gps电源
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_ReadBuffer(writer_data_buffer, SPI_FLASH_onoff_ADDR_VAR, geshu);

	/*计算前6个数的累加*/
	l_sum = 0;
	for(i=0;i<(geshu-1);i++)
	{
		l_sum = l_sum + writer_data_buffer[i];
	}
	/*判断累加和是否正确*/
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
	/*  加加reboot_cout */
	g_Indicat.reboot_cout ++;	
	printf("App_fault reboot_cout:%d",g_Indicat.reboot_cout);
	
	App_fault_analysis_save_to_flash_Var();
}




/*
*********************************************************************************************************
*	函 数 名: App_fault_analysis_Set_Value
*	功能说明:  
*	形    参：无
*	返 回 值: 无

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
*	函 数 名: App_fault_analysis_Clear_Value
*	功能说明:  
*	形    参：无
*	返 回 值: 无

*********************************************************************************************************
*/
void App_fault_analysis_Clear_Value(void)
{	
	static uint8_t lTxflag = 0;

	/* 只清除一次*/
	if(lTxflag == 0)
	{
		lTxflag = 1;
		g_Indicat.Indicat.wIndicate = 0;	
		App_fault_analysis_save_to_flash_Var();			
	}			
}


/*
*********************************************************************************************************
*	函 数 名: App_fault_analysis_Clear_reboot_cout
*	功能说明:  
*	形    参：无
*	返 回 值: 无

*********************************************************************************************************
*/
void App_fault_analysis_Clear_reboot_cout(void)
{	
	g_Indicat.reboot_cout = 0;	
	App_fault_analysis_save_to_flash_Var();					
}

/*
*********************************************************************************************************
*	函 数 名: App_fault_analysis_Get_Value
*	功能说明:  
*	形    参：无
*	返 回 值: 无

*********************************************************************************************************
*/
uint16_t App_fault_analysis_Get_Value(void)
{	
	return g_Indicat.Indicat.wIndicate;
}




