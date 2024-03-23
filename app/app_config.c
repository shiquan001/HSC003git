#include "app.h"
#include "bsp.h"

#include "app_config.h"
#define SET_DATA_GESHU (30+2)

CONFIG_STRCUT g_tConfig;
char rx_data_address[256];
char address[3][10]=
{
	"set_f1",
	"set_f3",
	"set_f4",
};

/*
*********************************************************************************************************
*	�� �� ��: App_Config_Save_Into_Flash
*	����˵��:	  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Config_Save_Into_Flash(void)
{
	u8 l_sum = 0;
	u8* l_point = 0;
	u8 i = 0;
	__align(8) uint8_t aPacketData[SET_DATA_GESHU];

	l_point = &g_tConfig.power_onoff;
	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + l_point[i];
	}
	g_tConfig.sum = l_sum;// �����ۼӺ͡�

#if 1
		memcpy(aPacketData,&g_tConfig.power_onoff,SET_DATA_GESHU);
		bsp_WriteCpuFlash(STM32_FLASH_BASE_FLAG_SET,aPacketData,SET_DATA_GESHU);
#else
		/*��ȡUPDATE�Ĳ���*/
		Bsp_spiflash_power_on();//��gps��Դ
		MX_SPI3_Init();
		bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
		sf_WriteBuffer((u8 *)&g_tConfig.clock_hour,SPI_FLASH_onoff_ADDR_VAR,SET_DATA_GESHU);
		Bsp_spiflash_power_off();	
		
#endif

}

/*
*********************************************************************************************************
*	�� �� ��: App_Config_Save_Into_Flash
*	����˵��:     
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8  App_Config_RS485_Com(void)
{
	uint8_t ucData;
	uint16_t pos = 0;
	uint8_t ret;
	
	int year =0;
	int mon =0;
	int day =0;
	int hour =0;
	int min =0;
	int sec =0;

	int f3 =0;
	int f4 =0;
	//char enter_char[] ={0x0D,0x0A};// �س�
	
    	//u8 flag_address_changed = 0;// ��־
	
	char *pAddress = NULL;// ����ĸ��ַ

	if (bsp_CheckTimer(TMR_ID_485_UART))
	{
		memset(rx_data_address,0,sizeof(rx_data_address));
		
		p_info("485 rx_data");
		while(1)
		{
		
			if(comGetChar(Reserve_485_uart, &ucData))
			{			
				rx_data_address[pos++] = ucData;
				printf("%c",ucData);
			}
			else
				break;

			if(pos >= (256))
				break;
		}
		/* log */
		if(pos <= 4)
		{
			ret = 4;
			return ret;
		}		
		
		/*���ҹؼ��� set_f1 19-10-05 09:20:55*/
		pAddress = strstr(rx_data_address, &address[0][0]);
		if(pAddress != NULL)
		{
			sscanf(pAddress+6,"%d-%d-%d %d:%d:%d",&year,&mon,&day,&hour,&min,&sec);	
			p_info("%d-%d-%d, %d:%d:%d",year,mon,day,hour,min,sec);
			
			MX_RTC_Set(year,mon,day,hour,min,sec);
			
			HAL_UART_Transmit(&debug_handle, (uint8_t *)&rx_data_address,pos, 0xFFFF);
			//RS485_SendBuf((uint8_t *)&rx_data_address,pos);// �ش���ַ��Ϣ
		}
		/*���ҹؼ��� */
		pAddress = strstr(rx_data_address, &address[1][0]);
		if(pAddress != NULL)
		{
			sscanf(pAddress+6,"%d",&f3);				
			g_tConfig.power_onoff = f3;
			App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash			
			HAL_UART_Transmit(&debug_handle, (uint8_t *)&rx_data_address,pos, 0xFFFF);
		}
		/*���ҹؼ��� */
		pAddress = strstr(rx_data_address, &address[2][0]);
		if(pAddress != NULL)
		{
			sscanf(pAddress+6,"%d",&f4);				
			g_tConfig.setAcqTimer = f4;
			App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash			
			HAL_UART_Transmit(&debug_handle, (uint8_t *)&rx_data_address,pos, 0xFFFF);
		}		

		ret = 1;
	}
	else
		ret = 0;
	
	return ret;
}

/*
*********************************************************************************************************
*	�� �� ��: App_Config_Init_Var
*	����˵��:     
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void App_Config_Init_Var(void)
{
	u8 writer_data_buffer[SET_DATA_GESHU];
	u8 i = 0;
	u8 l_sum = 0;
	u8 lenth = 0;

	lenth = sizeof(CONFIG_STRCUT);
	p_info("CONFIG_STRCUT lenth:%d",lenth);
	
	memset(writer_data_buffer,0,SET_DATA_GESHU);

	bsp_ReadCpuFlash(STM32_FLASH_BASE_FLAG_SET,writer_data_buffer,SET_DATA_GESHU);

	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + writer_data_buffer[i];
	}
	/*�ж��ۼӺ��Ƿ���ȷ*/
	if((l_sum == writer_data_buffer[SET_DATA_GESHU-1])&&(writer_data_buffer[SET_DATA_GESHU-1] != 0xFF))
	{
		memcpy(&g_tConfig.power_onoff,writer_data_buffer,SET_DATA_GESHU);
		p_info("App_Config_Init_Var  ok");
	}
	else
	{
		g_tConfig.power_onoff= TRUE;// 
		g_tConfig.setAcqTimer = 5;// 5 ���Ӳɼ�һ�Σ�һ���Զ�ȡ5�����ݣ�

		strcpy(g_tConfig.Name,"�չ�����");
		g_tConfig.T_down= 13;
		g_tConfig.T_up = 28;
		
		g_tConfig.H_down= 35;
		g_tConfig.H_up= 85;
		
		g_tConfig.soil_T_down= 15;
		g_tConfig.soil_T_up= 25;
		
		g_tConfig.soil_H_down= 40;
		g_tConfig.soil_H_up= 50;

		g_tConfig.PLC_Type = PLC_NULL;

		g_tConfig.Lux_down= 0;
		g_tConfig.Lux_up= 38000;
		g_tConfig.report_second = 10*60;//10min
		g_tConfig.report_changed = 1;//���ݱ仯�ϱ�
		
		p_info("App_Config_Init_Var  new");
	}

	g_wenshi.power_onoff = g_tConfig.power_onoff;
	
	App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash
	
	p_info("Version:%d,PLC:%d!",g_wenshi.version_number,g_tConfig.PLC_Type);

}


/*
*********************************************************************************************************
*	�� �� ��: App_Config_output_log
*	����˵��:     �����־
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Config_output_log(void)
{
	p_info("timer_outfire:%d,timer_delay_start:%d,",g_tConfig.power_onoff,g_tConfig.setAcqTimer);
}
#if 0
/*
*********************************************************************************************************
*	�� �� ��: App_Config_Loop
*	����˵��:  UPDATE  AND 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 App_Config_Loop(void)
{	
	
	
	switch(g_tConfig.work_state)
	{
		case CONFIG_INIT :
		{						
			
			g_tConfig.work_state = CONFIG_LOOP;
		}
			break;
			
		case CONFIG_LOOP :
		{
			App_Config_RS485_Com();//
						
		}		
			break;	
			

		
		default:
			break;
	}



	return 0;


}



/*
*********************************************************************************************************
*	�� �� ��: App_Config_Test
*	����˵��:  UPDATE  AND 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Config_Test(void)
{
	#ifdef MODE_TEST_App_Config_Test

	while(1)
	{

		App_Config_Loop();
	
	}
	#endif
}




#endif





