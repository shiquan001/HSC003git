#include "app_caiji_PLC.h"

#include "bsp.h"

#include "app.h"
#define PLC_RX_DATA_LENTH 256

#define OCCUPIED_SHFT_MASK 2//occupied  6BITS,    SHIFT 2 BIT
#define VALUE_SHFT_MASK 0x00000003//occupied  6BITS,    SHIFT 2 BIT

plc_t g_plc;
plc_t g_plc_copy;

static uint8_t rx_data_PLC[PLC_RX_DATA_LENTH];


/*
*********************************************************************************************************
*	�� �� ��: App_caiji_PLC_Init_Var
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_PLC_Init_Var(void)
{	
	memset(&g_plc,0,sizeof(g_plc));	

	g_plc.motor[0].Motor_remote = DISCONNECT;
	
	g_plc.PLC_start = TRUE;
	g_plc.PLC_work_state = CAIJI_PLC_OPEN;

	memcpy(&g_plc_copy,&g_plc,sizeof(g_plc)); 
}
#define REGISTER_ADDRESS_MOTOR_01 500
uint8_t buffer[16];

/**************************************************************************************                      
* ��    App_485_tx_PLC_cmd
* ��    ��:
* ��ڲ�����
	address:   ��ַ	
	function:       ������
* ���ڲ�������
* ˵    �����ɼ�PLC
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_caiji_tx_PLC_read_cmd(uint8_t address ,uint8_t function)
{
	u8 buffer_lenth=0;
	u16 crc1 = 0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = address;//��ַ��
	buffer_lenth++;	
	
	buffer[buffer_lenth] = function;//������
	buffer_lenth++;	
	
	buffer[buffer_lenth] = (REGISTER_ADDRESS_MOTOR_01-1)>>8;//�Ĵ�����ʼ��ַ 2�ֽ�
	buffer_lenth++;			
	buffer[buffer_lenth] = (uint8_t)(REGISTER_ADDRESS_MOTOR_01-1);// 500
	buffer_lenth++;			

	buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
	buffer_lenth++;			
	buffer[buffer_lenth] = REGISTER_NUM_all;	//24
	buffer_lenth++;			

	crc1 = CRC16_Modbus(buffer, buffer_lenth);

	buffer[buffer_lenth] = crc1>>8;//	 
	buffer_lenth++;		
	buffer[buffer_lenth] = (uint8_t)crc1;//	 
	buffer_lenth++;	
	
	//RS485_SendBuf(buffer, buffer_lenth);

	if (g_tUart5.SendBefor != 0)
	{
		g_tUart5.SendBefor();		/* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
	}
	HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	RS485_SendOver();
}


/**************************************************************************************                      
* ��    ��: App_caiji_tx_PLC_write_cmd
* ��    ��:
* ��ڲ�����
	address:   ��ַ	
	function:       ������06
* ���ڲ�������
* ˵    ���� 
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_caiji_tx_PLC_write_cmd(uint8_t address ,uint8_t function,uint16_t reg_address,uint16_t reg_num,uint8_t *data)
{

	u8 buffer_lenth=0;
	u16 crc1 = 0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = address;//��ַ��
	buffer_lenth++;	
	
	buffer[buffer_lenth] = function;//������
	buffer_lenth++;	
	
	buffer[buffer_lenth] = (reg_address-1)>>8;//�Ĵ�����ʼ��ַhigh address
	buffer_lenth++;			
	buffer[buffer_lenth] = (uint8_t)(reg_address-1)>>0;// 
	buffer_lenth++;			
#if 0
	buffer[buffer_lenth] = reg_num>>8;//�Ĵ������� 2�ֽ�
	buffer_lenth++;			
	buffer[buffer_lenth] = (uint8_t)reg_num>>0;	//
	buffer_lenth++;			
#endif
	for(uint8_t i = 0;i < (reg_num*2); i++)
	{
		buffer[buffer_lenth] = *data++; //
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
		g_tUart5.SendBefor();		/* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
	}
	HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	RS485_SendOver();
}

#if 0
uint8_t App_caiji_PLCdata_report_process(void)
{

	if( g_wenshi.m_co2!=g_wenshi.m_co2_last)
	{
		p_info("CO2 ���ݱ仯�ϱ�");				
		return TRUE;
	}		
	return FALSE;

}
#endif
/**/
uint16_t register_address_data[4][3]=
{
	{602,600,601},
	{605,603,604},
	{608,606,607},
	{611,609,610},
};
uint8_t occupied_data[]={2,8,14,20};

/*
*********************************************************************************************************
*	�� �� ��:  PLC_value_convert
*	����˵��:   ����ռλ��ֵȷ��plc�Ĵ����ĵ�ַ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t PLC_occupied_value_convert(uint8_t occupied,uint8_t value)
{
	uint16_t reg_address = 0;

	if((value>=MOTOR_STATE_STOP)&&(value<=MOTOR_STATE_reversal))
	{}
	else
	{
		p_err("value:%d outof range!",value);
	}

	if(occupied == occupied_data[MOTOR_01])
		reg_address = register_address_data[MOTOR_01][value];
	else if(occupied == occupied_data[MOTOR_02])
		reg_address = register_address_data[MOTOR_02][value];
	else if(occupied == occupied_data[MOTOR_03])
		reg_address = register_address_data[MOTOR_03][value];
	else if(occupied == occupied_data[MOTOR_04])
		reg_address = register_address_data[MOTOR_04][value];	
	else
	{
		p_err("occupied:%d outof range!",occupied);
	}
	return reg_address;
}
#if 0
typedef enum
{
	VERIFICATION_SHORT_LENTH =0,
	VERIFICATION_ADDRESS ,	
	VERIFICATION_CRC ,
	VERIFICATION_NO_DATA,
	VERIFICATION_OK
}
ENMU_VERIFICATION;
#endif
#define CMD_RETRY_COUNT 3
/*
*********************************************************************************************************
*	�� �� ��:  PLC_485FIFO_Protocolverification
*	����˵��:   PLC
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t PLC_485FIFO_Protocolverification(uint8_t * start_index,uint8_t address)
{
	uint8_t ucData = 0;
	uint16_t pos = 0;
	uint8_t ret = 0;
	
	uint16_t crc1 = 0;
	uint16_t rx_crc = 0;

	uint16_t i = 0;

	/*�ȴ������Ч��PLC_TH����	 */ 		
	if (bsp_CheckTimer(TMR_ID_485_UART))
	{
		memset(rx_data_PLC,0,sizeof(rx_data_PLC));
		
		p_info("plc 485 rx_data");
		while(1)
		{				
			if(comGetChar(Reserve_485_uart, &ucData))
			{			
				rx_data_PLC[pos++] = ucData;
				printf("%02X",ucData);
			}
			else
				break;
	
			if(pos >= (PLC_RX_DATA_LENTH))
				break;
		}				
		printf("\r\n");
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
		/* ���ݳ���̫���� ����*/
		if((pos-i) <= 4)
		{
			ret = VERIFICATION_ADDRESS;
			return ret;
		}	
		
		/* ����CRCУ��� */
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
*	�� �� ��: App_caiji_PLC_Loop
*	����˵��: �ɼ� PLC
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_PLC_Loop(void)
{
#ifdef ENABLE_TIANJING_PLC

	if(g_plc.PLC_start == TRUE)
	{
	}
	else
	{
		return;
	}

	switch(g_plc.PLC_work_state)
	{
		case CAIJI_PLC_OPEN :
		{	
			Bsp_12V_power_on();	//12 ���硣				
			Bsp_sensor_power_on(); //sensor���硣	
			//MX_UART5_Init();//��ʼ������5��ֹ������ 20211007
			g_plc.PLC_work_state = CAIJI_PLC_DELAY;

			bsp_SetTimer(TMR_ID_caiji_PLC_refresh);			
			bsp_StartAutoTimer(TMR_ID_caiji_PLC_cmd_chaxun,10);//10ms

			g_wenshi.m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//���͸����������ݴ������ı�־

			p_info("g_plc �򿪵�ԴPLC");				
		}		
			break;	
		case CAIJI_PLC_DELAY:
		{		
			/* 5S   ����һ�� ���Ȳ�ѯ�Ƿ���Ҫ����*/
			if(bsp_CheckTimer(TMR_ID_caiji_PLC_refresh))
			{
				g_plc.PLC_work_state = CAIJI_PLC_TX;
				p_info("CAIJI_PLC_DELAY");				
			}
			else
			{								
				/* 5S	����һ�� */
				if(bsp_CheckTimer(TMR_ID_caiji_PLC_ask_server))
				{
					App_caiji_PLC_writer_ask();
					p_info("CAIJI_PLC_DELAY ask");				
				}	
				/* 10mS	��ʱ��ѯ�����ƽ��� */
				if(bsp_CheckTimer(TMR_ID_caiji_PLC_cmd_chaxun))
				{
					uint32_t CmdCode = 0;
					CmdCode = bsp_GetCmd(); 		
					if(CmdCode != Cmd_NONE)
					{
						g_plc.PLC_work_state = CMD_PLC_TX;					
						g_plc.reg_retry_tx_count = 0;//clear 0
						
						g_plc.rx_data = CmdCode;	
						g_plc.rx_occupied = (uint8_t)(CmdCode>>OCCUPIED_SHFT_MASK)&0x000003f;					
						g_plc.rx_value= (uint8_t)CmdCode&VALUE_SHFT_MASK;
						g_plc.tx_mid = CmdCode>>8;
						
						g_plc.reg_address = PLC_occupied_value_convert(g_plc.rx_occupied,g_plc.rx_value);		
						g_plc.reg_data[0] = 0;
						g_plc.reg_data[1] = CONNECT;

						/*fifoû��cmd ����ȥ��ѯ��Ӧ��*/
						if(bsp_GetFIFOState_IsNull())
						{
							bsp_SetTimer(TMR_ID_caiji_PLC_refresh);//cmd֮�󣬿��ٲ�ѯplc״̬
							bsp_SetTimer(TMR_ID_caiji_PLC_ask_server);//�ٷ���Ӧ��
						}
					}
				}
			}
					

			/* ����3�Σ�����*/
			if(	g_plc.plc_not_ask_count > 3)
			{
				g_plc.plc_not_ask_count = 0;
				p_err("PLC_TX %d����Ӧ�𣬵���",10);	
				g_wenshi.m_sensor_TXflag &= ~SENSOR_TX_SERVER_PLC_TIANJING; 	
				if(g_plc.plc_not_ask_state_changed == 0)
				{
					g_plc.plc_not_ask_state_changed = 1;
					if((g_tConfig.report_changed))
					{
						App_caiji_report_start();//
						p_info("g_caiji plc ���ݱ仯�ϱ�"); 			
					}
					g_plc.tx_count++;
				}
			}
			/* ����3�Σ�����*/
			if(	g_plc.turang_not_ask_count> 3)
			{
				p_err("TURANG_TH %d����Ӧ�𣬵���",g_plc.turang_not_ask_count);	
				g_wenshi.m_sensor_TXflag &= ~SENSOR_TX_SERVER_TURANG_TH; 	
				g_plc.turang_not_ask_count= 0;
			}	
			/* ����3�Σ�����*/
			if(	g_plc.led_not_ask_count> 3)
			{
				p_err("LED_DISPLAY %d����Ӧ�𣬵���",g_plc.led_not_ask_count);	
				g_wenshi.m_sensor_TXflag &= ~SENSOR_TX_SERVER_LED_DISPLAY; 	
				g_plc.led_not_ask_count= 0;
			}				
		}		
			break;				
		case CAIJI_PLC_TX :
		{	
			comClearRxFifo(COM5);//COM5
		
			/*��ʼ  �ɼ� */ 	
			App_caiji_tx_PLC_read_cmd(ADDRESS_PLC_201,FUNCTION_CODE_03);//
			
			bsp_StartTimer(TMR_ID_caiji_PLC_TimerOutError , TMR_DELAY_0_3S);//5���Ӳɼ�������̫��ᵼ������
			g_plc.PLC_work_state = CAIJI_PLC_RX;

			p_info("g_plc ���Ͳ�ѯ����PLC ");												
		}		
			break;	
			
		case CAIJI_PLC_RX :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_PLC_201);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_PLC_201)&&(rx_data_PLC[start_index+1]==FUNCTION_CODE_03)
					&&(rx_data_PLC[start_index+2]==(REGISTER_NUM_all*2)))
				{				
					p_info("g_plc PLC ok");					
					#if 1
					for(uint8_t j =0;j<NUM_MOTOR;j++)
					{
						g_plc.motor[j].Motor_remote  			= rx_data_PLC[start_index+4+j*REGISTER_NUM*2];
						g_plc.motor[j].Motor_positive			= rx_data_PLC[start_index+6+j*REGISTER_NUM*2];
						g_plc.motor[j].Motor_reversal			= rx_data_PLC[start_index+8+j*REGISTER_NUM*2];
						g_plc.motor[j].Motor_fault   			= rx_data_PLC[start_index+10+j*REGISTER_NUM*2];
						
						g_plc.motor[j].limit_Positive_turn_limit= rx_data_PLC[start_index+12+j*REGISTER_NUM*2];
						g_plc.motor[j].limit_Reverse_turn_limit = rx_data_PLC[start_index+14+j*REGISTER_NUM*2];
					}
					g_wenshi.m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_TIANJING;//���͸����������ݴ������ı�־
					g_plc.plc_not_ask_count = 0;
					if(g_plc.plc_not_ask_state_changed == 1)//����������
					{
						if((g_tConfig.report_changed))
						{
							App_caiji_report_start();//
							p_info("g_caiji plc ���ݱ仯�ϱ�"); 			
						}
						g_plc.tx_count++;
					}
					g_plc.plc_not_ask_state_changed = 0;
					
					//App_caiji_sensor_address();//��ַ����
					
					#else					
					memcpy(&g_plc.motor[0].Motor_remote,&rx_data_PLC[i+3],sizeof(motor_t)*NUM_MOTOR);
					#endif

					/* �Ա������Ƿ��б仯 */
					if(NULL != memcmp(&g_plc,&g_plc_copy,REGISTER_NUM_all))
					{												
						if((g_tConfig.report_changed))
						{
							App_caiji_report_start();//
							p_info("g_caiji plc ���ݱ仯�ϱ�"); 			
						}
						g_plc.tx_count++;
					}
					memcpy(&g_plc_copy,&g_plc,sizeof(g_plc)); //����				
				}
				else
				{
					p_err("g_plc g_caiji RX error ");	
				}
				g_plc.PLC_work_state = CAIJI_TRTH_TX;
				
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("PLC_485FIFO_Protocolverification:%d",return_data);	
			}	
				
			/*PLC_TH 3s ��ʱ�˳����ж�	 */
			if(bsp_CheckTimer(TMR_ID_caiji_PLC_TimerOutError))
			{
				g_plc.PLC_work_state = CAIJI_TRTH_TX;

				p_err("TMR_ID_caiji_PLC_TimerOutError");	
				g_plc.plc_not_ask_count++;
				
			}
		}		
			break;	

		case CAIJI_TRTH_TX :
		{	
			comClearRxFifo(COM5);//COM5
		
			/*��ʼ  �ɼ� */ 	
			App_485_tx_turang_T_H_cmd(ADDRESS_TURANG_TH,FUNCTION_CODE_03);// 
			bsp_DelayMS(10);//30ms
						
			bsp_StartTimer(TMR_ID_caiji_PLC_TimerOutError , TMR_DELAY_0_3S);//5���Ӳɼ�������̫��ᵼ������
			g_plc.PLC_work_state = CAIJI_TRTH_RX;

			p_info("g_plc TRTH_TX��ѯ����");												
		}		
			break;	
		case CAIJI_TRTH_RX :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_TURANG_TH);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_TURANG_TH)&&(rx_data_PLC[start_index+1]==FUNCTION_CODE_03))
				{				
					p_info("g_plc CAIJI_TRTH_RX ok"); 				

					g_wenshi.m_soil_H = ((uint16_t)rx_data_PLC[start_index+3] << 8 | rx_data_PLC[start_index+4]);
					
					int16_t m_soil_T = 0;
					m_soil_T = (rx_data_PLC[start_index+5] << 8 | rx_data_PLC[start_index+6]);
					if(m_soil_T >=0.0)
					{
						g_wenshi.m_soil_Symbol=1;	
						g_wenshi.m_soil_T = m_soil_T;
					}
					else
					{
						g_wenshi.m_soil_Symbol=0;
						g_wenshi.m_soil_T = -m_soil_T;
					}
					
					g_wenshi.m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_TH;//���͸����������ݴ������ı�־
					g_plc.turang_not_ask_count = 0;
					p_info("����, Symbol:%c;�¶�:%d;ʪ��:%d;",(char)g_wenshi.m_soil_Symbol,g_wenshi.m_soil_T,g_wenshi.m_soil_H);
					
					g_caiji.soil_TH_work_state = CAIJI_soil_LUX_DELAY;					
					p_info("g_caiji soil_TH ok");			
				}
				else
				{
					p_err("g_plc CAIJI_TRTH_RX   error ");	
				}
				g_plc.PLC_work_state = CAIJI_LED_TX;
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("CAIJI_TRTH_RX PLC_485FIFO_Protocolverification:%d",return_data);	
			}	
				
			/*PLC_TH 3s ��ʱ�˳����ж�	 */
			if(bsp_CheckTimer(TMR_ID_caiji_PLC_TimerOutError))
			{
				g_plc.turang_not_ask_count++;
				g_plc.PLC_work_state = CAIJI_LED_TX;
				p_err("CAIJI_TRTH_RX TMR_ID_caiji_PLC_TimerOutError");					
			}
		}		
			break;	
			
		case CAIJI_LED_TX :
		{	
			comClearRxFifo(COM5);//COM5
		
			App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// ˢ������			
			bsp_StartTimer(TMR_ID_caiji_PLC_TimerOutError , TMR_DELAY_0_3S);//5���Ӳɼ�������̫��ᵼ������
			g_plc.PLC_work_state = CAIJI_LED_RX;
		
			p_info("g_plc CAIJI_LED_TX ��ѯ����");												
		}		
			break;	
		case CAIJI_LED_RX :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_LED_DISPLAY);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_LED_DISPLAY)&&(rx_data_PLC[start_index+1]==FUNCTION_CODE_10))
				{				
					g_wenshi.m_led_display=1;									
					g_wenshi.m_sensor_TXflag |= SENSOR_TX_SERVER_LED_DISPLAY;//���͸����������ݴ������ı�־					
					g_plc.led_not_ask_count= 0;
					p_info("g_caiji CAIJI_LED_RX ok");			
				}
				else
				{
					g_wenshi.m_led_display = 0;				
					p_err("g_plc CAIJI_LED_RX	 error ");	
				}
				p_info(" m_led_display:%d;;",g_wenshi.m_led_display);	
				g_plc.PLC_work_state = CAIJI_PLC_DELAY;
				bsp_StartTimer(TMR_ID_caiji_PLC_refresh , TMR_DELAY_2S);//2���Ӳɼ�������̫��ᵼ������ 												
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("CAIJI_TRTH_RX PLC_485FIFO_Protocolverification:%d",return_data); 
			}	
				
			/*PLC_TH 3s ��ʱ�˳����ж�	 */
			if(bsp_CheckTimer(TMR_ID_caiji_PLC_TimerOutError))
			{
				g_plc.led_not_ask_count++;
				g_plc.PLC_work_state = CAIJI_PLC_DELAY;
				bsp_StartTimer(TMR_ID_caiji_PLC_refresh , TMR_DELAY_2S);//2���Ӳɼ�������̫��ᵼ������ 				
				p_err("CAIJI_TRTH_RX TMR_ID_caiji_PLC_TimerOutError");					
			}
		}		
			break;	

			
		case CMD_PLC_TX :
		{	
			/* ����3�Σ��˳�*/
			if(	g_plc.reg_retry_tx_count> CMD_RETRY_COUNT)
			{
				g_plc.PLC_work_state = CAIJI_PLC_DELAY;
				p_err("CMD_PLC_TX����3�Σ��˳�");	
				
				App_caiji_report_start();//
				p_info("  CMD_PLC_TX����3�Σ��˳��� ���ݱ仯�ϱ�"); 				
			}
			else
			{
				comClearRxFifo(COM5);//COM5					
				App_caiji_tx_PLC_write_cmd(ADDRESS_PLC_201,FUNCTION_CODE_06,g_plc.reg_address,1,g_plc.reg_data);//				
				bsp_StartTimer(TMR_ID_caiji_PLC_TimerOutError , TMR_DELAY_0_3S);//5���Ӳɼ�������̫��ᵼ������
				g_plc.PLC_work_state = CMD_PLC_RX;				
				p_info("g_plc ���� CMD_PLC_TX  ");	
			}														
		}		
			break;	
			
		case CMD_PLC_RX :
		{	
			uint8_t start_index = 0;
			uint8_t return_data = 0;
			return_data = PLC_485FIFO_Protocolverification(&start_index,ADDRESS_PLC_201);
			if(return_data == VERIFICATION_OK)
			{
				if((rx_data_PLC[start_index+0]==ADDRESS_PLC_201)&&(rx_data_PLC[start_index+1]==FUNCTION_CODE_06))
				{
					p_info("g_plc CMD_PLC_RX  ok  ");	
					g_plc.PLC_work_state = CAIJI_PLC_DELAY;					
				}
				else
				{
					p_err("g_plc CMD_PLC_RX error ");	
					//g_plc.PLC_work_state = CMD_PLC_TX;//�ط�
					//g_plc.reg_retry_tx_count++;// 
				}
			}
			else if(return_data == VERIFICATION_NO_DATA)
			{	}				
			else //error
			{
				p_err("PLC_485FIFO_Protocolverification:%d",return_data);												
				//g_plc.PLC_work_state = CMD_PLC_TX;//�ط�
				//g_plc.reg_retry_tx_count++;// 
			}	

			/*PLC_TH 3s ��ʱ�˳����ж�	 */
			if(bsp_CheckTimer(TMR_ID_caiji_PLC_TimerOutError))
			{
				g_plc.PLC_work_state = CMD_PLC_TX;
				g_plc.reg_retry_tx_count++;// 
				p_err("CMD_PLC_RX TMR_ID_caiji_PLC_TimerOutError");		
				g_plc.plc_not_ask_count++;
				
			}			
		}		
			break;

			
		case CAIJI_PLC_CLOSE :
		{	
			bsp_StopTimer(TMR_ID_caiji_PLC_refresh);//
			
			//HAL_UART_DeInit(&huart5);
			
			Bsp_12V_power_off();
			Bsp_sensor_power_off(); //sensor���硣	
			
			g_plc.PLC_start = FALSE;
			g_plc.PLC_work_state = CAIJI_PLC_MAX;
				
			/* :  ���ݱ仯�ϱ� */			
			p_info("g_plc �ر�PLC ��Դ");		
 		}		
			break;				
		default:
			break;
	}
	
	
#endif
}




/*
*********************************************************************************************************
*	�� �� ��: App_plc_Dinfo_Data_process
*	����˵��: 
*	��	  �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_plc_Dinfo_Data_process(u8 pos,char* msg)
{
#ifdef ENABLE_TIANJING_PLC

	char *p=0;
	int lenth = 0;
	
	int i=0,j=0,n=0,len=0;	
	int a[16];//16 ������ 	
	char str[64];
	char str1[10];
	#if 0
	memset(name,0,sizeof(name))	;		
	p = strstr(&msg[pos],"plcw,");				
	lenth = strlen("plcw,");
	ask = sscanf(p+lenth,"%[^,]",name);
	p = strstr(&msg[pos],name);	
	lenth = strlen(name);	
	strcpy(str,p+lenth+1);
#endif
	p = strstr(&msg[pos],"plcw,");	
	lenth = strlen("plcw,");	
	strcpy(str,p+lenth);

	len=strlen(str);
	j=0;
	for(i=0;i<len;i++)
	{
		if(str[i]!=',')
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

	if(a[0] != SENSOR_ID_PLC_TIANJING)
	{
		p_err("SENSOR_ID_PLC_TIANJING: %d!����",a[0]);
		return ;
	}
	uint32_t _CmdCode = 0;
	uint8_t  count= n/2;//������

	//memset(&g_plc.rx_occupied[0],0,4);
	//g_plc.tx_mid = g_mid;
	
	for(i=0;i<count;i++)
	{
		_CmdCode = (a[i*2+1]<<(OCCUPIED_SHFT_MASK)|(a[i*2+2]&VALUE_SHFT_MASK));//��6bit��ռλ����2bit������
		_CmdCode |= (g_mid<<8);
		bsp_PutCmd(_CmdCode);
		//g_plc.rx_occupied[i] = a[i*2+1];
	}
#endif
}


/*
*********************************************************************************************************
*	�� �� ��:  PLC_motor_state_convert
*	����˵��:   PLC
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PLC_motor_state_convert(void)
{
	for(uint8_t i =MOTOR_01;i<NUM_MOTOR;i++)
	{
		if((g_plc.motor[i].Motor_positive == CONNECT)&&(g_plc.motor[i].Motor_reversal== DISCONNECT))
		{
			g_plc.tx_occupied[i]=MOTOR_STATE_positive;
		}
		else if((g_plc.motor[i].Motor_positive == DISCONNECT)&&(g_plc.motor[i].Motor_reversal== CONNECT))
		{
			g_plc.tx_occupied[i]=MOTOR_STATE_reversal;
		}
		else if((g_plc.motor[i].Motor_positive == CONNECT)&&(g_plc.motor[i].Motor_reversal== CONNECT))
		{
			p_err("%d���״̬����Motor_positive:1��Motor_reversal:1",i);
		}	
		else
		{
			g_plc.tx_occupied[i]=MOTOR_STATE_STOP;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: App_caiji_PLC_writer_ask
*	����˵��: 
*	��	  �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_PLC_writer_ask(void)
{
	if((g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_TIANJING) != SENSOR_TX_SERVER_PLC_TIANJING)
	{
		p_err("App_caiji_PLC_writer_ask ����");	
		return;///�����ߣ���Ӧ��
	}
	u16 lenth = 0;
	u16 sensor_data_lenth = 0;
	
	unsigned long l_long = 0;
	unsigned long l_long_s = 0;
	
	char json_buf[JSON_BUF_LEN];  //���ڴ��json��ʽ����
	uint16_t json_len  =0;


	/* ������֡*/
	memset(&json_buf,'0',sizeof(json_buf));
	json_len = 0;
	
	json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//����������	SensorBasic 0x0002
	lenth= lenth + 1;
	json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
	lenth= lenth + 1;
	
	//�豸ִ�����������ִ�н���ϱ��е�midҪ���յ������е�mid����һ�£�����ƽ̨����ˢ�¶�Ӧ�����״̬
	json_buf[lenth] = g_plc.tx_mid>>8;//mid	2
	lenth= lenth + 1;
	json_buf[lenth] = (u8)g_plc.tx_mid;
	lenth= lenth + 1;	
 
	json_buf[lenth] = 0x00;//errcode	1
	lenth= lenth + 1;
	
	/*ʱ��*/
	l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
	l_long_s = mktime_second(18,1,1,0,0,0);
	if(l_long>=l_long_s)
	{
		json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
	}
	else
	{
		json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
		json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
		lenth= lenth + 1;
	}

	/*sensor_data_lenth*/
	//lenth= lenth + 1;
	//lenth= lenth + 1;

	PLC_motor_state_convert();//״̬�任
	sprintf(&json_buf[lenth+2],"%d,2,%d,8,%d,14,%d,20,%d",SENSOR_ID_PLC_TIANJING,
	g_plc.tx_occupied[MOTOR_01],g_plc.tx_occupied[MOTOR_02],g_plc.tx_occupied[MOTOR_03],g_plc.tx_occupied[MOTOR_04]);
	
	sensor_data_lenth = strlen(&json_buf[lenth+2]);

	json_buf[lenth] =sensor_data_lenth>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
	lenth= lenth + 1;
	json_buf[lenth] = sensor_data_lenth;// ���ֽ��ڵ�λ��ַ�����ģʽ��
	lenth= lenth + 1;

//	strcpy(&json_buf[lenth],"dtime");// ���ֽ��ڵ�λ��ַ�����ģʽ��
	lenth= lenth + sensor_data_lenth;
	
	json_len = lenth;

	app_fifo_NB_CoapST_Put(json_buf,&json_len);
}

