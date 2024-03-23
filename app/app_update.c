
#include "app.h"

#include "app_update.h"

//ָ��UPDATEģ��
typedef struct UPDATE_T * UPDATE_Handle;


#define UPDATE_SET_DATA_GESHU 16//spi flash data lenth
static u8 flash_data_buffer[UPDATE_SET_DATA_GESHU];

UPDATE_T g_Update;

PCP_T g_PCP_TXData;//��������
PCP_T g_PCP_RXData;//��������


/**
* CRC-CCITT �㷨У����
* 
* @author amadowang
* @version [�汾��, Aug 29, 2011]
* @see [�����/����]
* @since [��Ʒ/ģ��汾]
* @date 2012-10-24
*/

/*
 * CCITT��׼CRC16(1021)������ CRC16-CCITT ISO HDLC, ITU X.25, x16+x12+x5+1 ����ʽ
 * ��λ����ʱ���ɶ���ʽ Gm=0x11021 ��λ����ʱ���ɶ���ʽ��Gm=0x8408 �������ø�λ����
 */
 static uint16_t crc16_ccitt_table[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a,
		0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
		0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528,
		0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
		0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e,
		0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf,
		0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec,
		0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd,
		0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2,
		0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3,
		0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
		0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691,
		0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
		0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37,
		0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64,
		0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55,
		0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };


/**
 * 
 * @param reg_init CRCУ��ʱ��ֵ
 * @param message У��ֵ
 * @return
 */
 static uint16_t do_crc(uint16_t reg_init, char* message,uint16_t length) 
 {
	uint16_t crc_reg = reg_init;
	
	for (uint16_t i = 0; i < length; i++) 
	{
		/* �̶���λ����0 data5 or data6 == 0x00*/
		if(i==(PCP_DATA_pos_crc_h+0))
		{
			crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ 0x00) & 0xff];
		}
		else if(i==(PCP_DATA_pos_crc_l+0))
		{
			crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ 0x00) & 0xff];
		}
		else
		{
			crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ message[i]) & 0xff];
		}
	}
	return crc_reg;
}



/*
*********************************************************************************************************
*	�� �� ��: App_Update_Config_Save_Into_Flash
*	����˵��:	  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Update_Config_Save_Into_Flash(void)
{
	u8 l_sum = 0;
	u8* l_point = 0;
	u8 i = 0;

	l_point = (u8 *)&g_Update.safe_flag;
	
	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(UPDATE_SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + l_point[i];
	}
	g_Update.sum_flag = l_sum;// �����ۼӺ͡�

	/*���浽spi flash����*/
	/*���浽spi flash����*/
	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_WriteBuffer((u8 *)&g_Update.safe_flag, SPI_FLASH_START_ADDR_VAR, UPDATE_SET_DATA_GESHU);
	Bsp_spiflash_power_off();	
		
	
	/* ��flash�п�����Ӧ����Ϣ����*/

	/* ��ӡ��ÿ����������Ϣ*/
	p_info("safe_flag:%02X",g_Update.safe_flag);

}

/*
*********************************************************************************************************
*	�� �� ��: App_Update_Config_read_from_flash
*	����˵��:     ���浽spi flash�еĲ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void App_Update_Config_read_from_flash(void)
{
	u8 i = 0;
	u8 l_sum = 0;

	memset(flash_data_buffer,0,UPDATE_SET_DATA_GESHU);

	/*��ȡUPDATE�Ĳ���*/
	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_ReadBuffer(flash_data_buffer,SPI_FLASH_START_ADDR_VAR,UPDATE_SET_DATA_GESHU);
	Bsp_spiflash_power_off();	
		

	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(UPDATE_SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + flash_data_buffer[i];
	}
	/*�ж��ۼӺ��Ƿ���ȷ*/
	if((l_sum == flash_data_buffer[UPDATE_SET_DATA_GESHU-1])&&(flash_data_buffer[UPDATE_SET_DATA_GESHU-1] != 0xFF))
	{
		memcpy((u8 *)&g_Update.safe_flag,flash_data_buffer,UPDATE_SET_DATA_GESHU);
		p_info("g_Update�ۼӺ���ȷ  ");
	}
	else
	{	/*��ʼ���汾��*/
		memset((u8 *)&g_Update.safe_flag,0,UPDATE_SET_DATA_GESHU);

		
		g_Update.tx_data_count = 0;
		
		p_info("�ۼӺͲ���ȷ  new");
	}
	
	g_Update.update_workstate = UPDATE_INIT;
	App_Update_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash


}

/*
*********************************************************************************************************
*	�� �� ��: App_Update_Init_Var
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Update_Init_Var(void)
{
	App_Update_Config_read_from_flash();/*�ӵ�ַ512��ȡ����*/
	

	g_Update.update_workstate = UPDATE_INIT;
	g_Update.tx_data_count = 0;

	g_Update.rx_data_ok = 0;
	g_Update.rx_data_not_ok= 0;


	//p_info("cur  version:%d,%d,%d��\r\n",g_Update.version_cur[0],g_Update.version_cur[1],g_Update.version_cur[2]);

}

/*
*********************************************************************************************************
*	�� �� ��: App_PCP_RXDataFrame_process
*	����˵��:    analyse rx data
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 update_rx_data[PCP_DATA_LENTH]; 

void App_PCP_RXDataFrame_analyse(uint16_t len,char* msg,PCP_T * p_PCP_RXData)
{
	uint8_t result_code ;// �����

	/*crc   */
	uint16_t crc_calu = 0x00;
	uint16_t crc_rx = 0x00;
	
	crc_calu = do_crc(0x0000,msg,len);
	/*crc_rx */
	crc_rx =(msg[4]<<8)|msg[5];
	if(crc_calu != crc_rx)
	{
		p_err("crc_rx is no ok! ");
		return;
	}

	p_PCP_RXData->starting_mark =(* (msg+0)<<8) |* (msg+1);
	p_PCP_RXData->version_number =*  (msg+2);
	p_PCP_RXData->message_code =(msgCode_e)*  (msg+3);
	p_PCP_RXData->check_code =(* (msg+4)<<8) |* (msg+5);
	p_PCP_RXData->Data_length =(* (msg+6)<<8) |* (msg+7);


	memset(&p_PCP_RXData->Data[0],0x00,PCP_DATA_LENTH);
	
	switch(p_PCP_RXData->message_code)
	{
		case MsgCode_Query_device_version:
		{

		}
			break;

		case MsgCode_New_version_notification:
		{
			memcpy(&p_PCP_RXData->new_version[0], &msg[PCP_HEADER_DATA_LENTH],PCP_DATA_version_lenth);
			
			p_PCP_RXData->upgrade_package_size =( (msg[24]<<8) |msg[25]);
			p_PCP_RXData->upgrade_package_total_number =( (msg[26]<<8) |msg[27]);
			p_PCP_RXData->upgrade_package_Check_Code =( (msg[28]<<8) |msg[29]);

			g_Update.data_packet_total = p_PCP_RXData->upgrade_package_total_number ;
			g_Update.data_packet_size = p_PCP_RXData->upgrade_package_size ;
			g_Update.data_packet_total_crc = p_PCP_RXData->upgrade_package_Check_Code ;

		}
			break;
		case MsgCode_Request_upgrade_package:
		{
			u16 data_packet_cur;	 //��ǰ�����ݰ�
			u16 update_rx_data_lenth = 0 ;
			u32 spi_flash_address = 0 ;
			u16 pos = 0;
			
			result_code =msg[PCP_HEADER_DATA_LENTH];
			p_PCP_RXData->result_code =result_code;
			data_packet_cur =msg[PCP_HEADER_DATA_LENTH+1];
			data_packet_cur = (data_packet_cur<<8)|(msg[PCP_HEADER_DATA_LENTH+2]);

			if(result_code == 0x00)
			{
				/* ��ͬ�����жϴ�0��ʼ*/
				if(g_Update.data_packet_cur == data_packet_cur)
				{			
					memset(update_rx_data,0xff,sizeof(update_rx_data));
					update_rx_data_lenth = 0 ;
					pos = PCP_HEADER_DATA_LENTH+3;
					while(pos < len)
					{					
						update_rx_data[update_rx_data_lenth] = msg[pos++];
						//printf("%02X ",update_rx_data[update_rx_data_lenth]);
						
						update_rx_data_lenth++;
					}
					/*��ȡUPDATE�Ĳ���*/
					Bsp_spiflash_power_on();//��gps��Դ
					bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);	
					
					spi_flash_address = SPI_FLASH_code_ADDR+g_Update.data_packet_size *(g_Update.data_packet_cur+0);//computer address
					if(sf_WriteBuffer(update_rx_data, spi_flash_address, g_Update.data_packet_size) == 1)
					{
						p_err("rx_data_ok  ���յ���%d��",g_Update.data_packet_cur);
					
						g_Update.rx_data_ok = 1;
						g_Update.data_packet_cur += 1;
					}	
					else
					{
						p_err("sf_WriteBuffer error ");
						g_Update.rx_data_not_ok = 1;
					}
					Bsp_spiflash_power_off();	
											
				}
				else
				{
					g_Update.rx_data_not_ok = 1;
				}
			}
			else if(result_code == 0x80)
			{
				p_err("result_code  error �������񲻴���");
			}
			else if(result_code == 0x81)
			{
				p_err("result_code  error ָ���ķ�Ƭ������");
			}			
		}
			break;
		case MsgCode_Escalation_package_download_status:
		{
			result_code =msg[PCP_HEADER_DATA_LENTH];
			p_PCP_RXData->result_code =result_code;

			if(result_code == 0x00)
			{
				p_info("result_code  ����ɹ�");
			}
			else
			{
				p_err("result_code  error �������񲻴���");
			}

		}
			break;
		case MsgCode_Perform_an_upgrade:
		{

		}
			break;
		case MsgCode_Report_upgrade_results:
		{
			result_code =msg[PCP_HEADER_DATA_LENTH];
			p_PCP_RXData->result_code =result_code;

			if(result_code == 0x00)
			{
				p_info("result_code  ����ɹ�");
			}
			else
			{
				p_err("result_code  error �������񲻴���");
			}

		}
			break;


		default:
			break;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: App_PCP_TXDataFrame_Init_Var
*	����˵��:    ��ʼ�����͵�����֡
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_PCP_TXDataFrame_Init_Var(msgCode_e message_code)
{
	strcpy(g_PCP_TXData.cur_version, VERSION_WENSHI_cur);

	g_PCP_TXData.starting_mark = (PCP_DATA_starting_mark1<<8)|PCP_DATA_starting_mark2;
	g_PCP_TXData.version_number = 0x01;

	g_PCP_TXData.message_code = message_code;
	g_PCP_TXData.check_code = 0x00;

	memset(&g_PCP_TXData.Data[0],0x00,PCP_DATA_LENTH);
	switch(message_code)
	{
		case MsgCode_Query_device_version:
		{
			g_PCP_TXData.result_code = 0x00;
			
			g_PCP_TXData.Data[0] = g_PCP_TXData.result_code;
			strcpy(&g_PCP_TXData.Data[1],g_PCP_TXData.cur_version);		
			g_PCP_TXData.Data_length = 1+PCP_DATA_version_lenth;
		}
		break;

		case MsgCode_New_version_notification:
		{
			g_PCP_TXData.result_code = 0x00;
			
			g_PCP_TXData.Data[0] = g_PCP_TXData.result_code;
			g_PCP_TXData.Data_length = 1;

		}
		break;
		case MsgCode_Request_upgrade_package:
		{
			strcpy(&g_PCP_TXData.Data[0],g_PCP_RXData.new_version);		
			g_PCP_TXData.Data_length =PCP_DATA_version_lenth;
			g_PCP_TXData.Data[PCP_DATA_version_lenth] = g_Update.data_packet_cur<<8;
			g_PCP_TXData.Data[PCP_DATA_version_lenth+1] =(uint8_t) g_Update.data_packet_cur&0x00ff;
			g_PCP_TXData.Data_length =PCP_DATA_version_lenth+2;
		}
			break;
		case MsgCode_Escalation_package_download_status:
		{
			g_PCP_TXData.result_code = 0x00;
			
			g_PCP_TXData.Data[0] = g_PCP_TXData.result_code;
			g_PCP_TXData.Data_length = 1;

		}
			break;
		case MsgCode_Perform_an_upgrade:
		{
			g_PCP_TXData.result_code = 0x00;
			
			g_PCP_TXData.Data[0] = g_PCP_TXData.result_code;
			g_PCP_TXData.Data_length = 1;

		}
			break;
		case MsgCode_Report_upgrade_results:
		{
			g_PCP_TXData.result_code = 0x00;
			
			g_PCP_TXData.Data[0] = g_PCP_TXData.result_code;
			g_PCP_TXData.Data_length = 1;
			strcpy(&g_PCP_TXData.Data[g_PCP_TXData.Data_length ],g_PCP_RXData.new_version);		
			g_PCP_TXData.Data_length = 1+PCP_DATA_version_lenth;
		}
			break;


		default:
			break;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: App_Update_TX_data
*	����˵��:  	�����������ݸ�ƽ̨
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
char json_buf[512];
uint16_t json_len= 0;


void App_Update_TX_data( json_info_t *p_json_info,PCP_T * p_PCP_TXData)
{
	u16 lenth = 0;

	/* zero data*/
	memset(&json_buf[0],'0',sizeof(json_buf));
	json_len = lenth;
	
	/*starting_mark copy*/
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->starting_mark>>8);
	lenth= lenth + 1;
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->starting_mark&0x00ff);
	lenth= lenth + 1;	
	/*version_number copy*/
	json_buf[lenth] = p_PCP_TXData->version_number ;
	lenth= lenth + 1;	
	/*message_code copy*/
	json_buf[lenth] = p_PCP_TXData->message_code ;
	lenth= lenth + 1;	

	/*check_code copy*/
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->check_code>>8);
	lenth= lenth + 1;
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->check_code&0x00ff);
	lenth= lenth + 1;	
	
	/*Data_length copy*/
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->Data_length>>8);
	lenth= lenth + 1;
	json_buf[lenth] = (uint8_t)(p_PCP_TXData->Data_length&0x00ff);
	lenth= lenth + 1;	

	/*data copy*/
	memcpy(&json_buf[lenth], &p_PCP_TXData->Data[0],p_PCP_TXData->Data_length);		
	lenth= lenth + p_PCP_TXData->Data_length;		

	json_len = lenth;

	/*crc  data copy*/
	uint16_t crc_return = 0x00;
	crc_return = do_crc(0x0000,json_buf,json_len);
	/*check_code copy*/
	json_buf[PCP_DATA_pos_crc_h] = (uint8_t)(crc_return>>8);
	json_buf[PCP_DATA_pos_crc_l] = (uint8_t)(crc_return&0x00ff);


#if 0
	//��������
	APP_STATE = NB_CoAP_ST;   
	p_info("update tx data ��ʼ��������");
#else
	app_fifo_NB_CoapST_Put(&json_buf[0],&json_len);

#endif

}


/*
*********************************************************************************************************
*	�� �� ��: App_PCP_TXData_process
*	����˵��:    ��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_PCP_TXData_process(msgCode_e message_code)
{
	App_PCP_TXDataFrame_Init_Var(message_code); 


	App_Update_TX_data(&json_info,&g_PCP_TXData);
	g_Update.rx_data_in_flag = FALSE;
}
#define TIMER_UPDATE_10SECONDER TMR_DELAY_10S
#define TIMER_UPDATE_60SECONDER TMR_DELAY_60S
/*
*********************************************************************************************************
*	�� �� ��: App_Update_Loop
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_Update_Loop(void)
{
	if(seq_record & NB_SEQ_INIT)
	{
		//p_info("REPORT_CAIJI_LOOP ��ʼ��������"); 	
	}
	else
	{
		//p_err("App_Update_Loop,������");
		return;
	}	

	switch(g_Update.update_workstate)
	{
		case UPDATE_INIT :
		{				
			#if 0
			bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_2min);
			#else
			bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_4min);//4min֮�� ���ϱ�������״̬
			#endif
			if(g_Update.safe_flag == SPI_FLASH_UPDATE_IS_OK)
			{
				g_Update.update_workstate = UPDATE_Report_upgrade_results;
			}
			else
			{
				g_Update.update_workstate = UPDATE_Report_current_software_version_number;
				//g_Update.update_workstate = UPDATE_Escalation_package_download_status;
			}
			p_info("App_Update_Loop UPDATE_INIT");
			
		}
			break;		
		case UPDATE_Report_upgrade_results :
		{		
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_60SECONDER);
			
				App_PCP_TXData_process(MsgCode_Report_upgrade_results); 
				g_Update.tx_data_count++;
				p_info("tx MsgCode_Report_upgrade_results");
			}
			/* ��ʱ3�β����ϱ���*/
			if(g_Update.tx_data_count >= PCP_DATA_tx_count_max)
			{
				g_Update.update_workstate = UPDATE_Report_current_software_version_number;
				g_Update.safe_flag = SPI_FLASH_UPDATE_IS_init;
				App_Update_Config_Save_Into_Flash();//save					
				p_err(" MsgCode_Report_upgrade_results tx_data_count max");
			}			
			if(g_Update.rx_data_in_flag == TRUE)
			{
				g_Update.rx_data_in_flag = FALSE;
				if(g_PCP_RXData.message_code == MsgCode_Report_upgrade_results)
				{
					bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
					g_Update.update_workstate = UPDATE_Report_current_software_version_number;
									/*�������֮�󣬱��ֱ�־��flash��*/
					g_Update.safe_flag = SPI_FLASH_UPDATE_IS_init;
					App_Update_Config_Save_Into_Flash();//save												

				}
			}

		}
			break;		
		case UPDATE_Report_current_software_version_number :
		{		
			#if 1
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				//bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_30S);
			
				App_PCP_TXData_process(MsgCode_Query_device_version); 
				p_info("tx current_software_version_number");
			}
			#endif
			if(g_Update.rx_data_in_flag == TRUE)
			{
				g_Update.rx_data_in_flag = FALSE;
				if(g_PCP_RXData.message_code == MsgCode_New_version_notification)
				{
					bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
					g_Update.update_workstate = UPDATE_New_version_notification;
				}
				else if(g_PCP_RXData.message_code == MsgCode_Query_device_version)
				{
					App_PCP_TXData_process(MsgCode_Query_device_version); 
					p_info("tx current_software_version_number");
				}
				
			}
		}
			break;
		case UPDATE_New_version_notification :
		{
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_10SECONDER);
			
				App_PCP_TXData_process(MsgCode_New_version_notification); 
				p_info("tx MsgCode_New_version_notification");
				g_Update.update_workstate = UPDATE_Request_upgrade_package;
				g_Update.data_packet_cur = 0;// ��0����ʼ	
			}
		}
			break;	
		case UPDATE_Request_upgrade_package :
		{
			/*��ʱ��������ط�*/
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_10SECONDER);
			
				App_PCP_TXData_process(MsgCode_Request_upgrade_package); 
				g_Update.rx_data_ok = 0;
				g_Update.rx_data_not_ok= 0;
				p_info("tx MsgCode_New_version_notification����ȡ��%d��",g_Update.data_packet_cur);
			}
			if(g_Update.rx_data_ok)
			{
				g_Update.rx_data_ok = 0;
			
				bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
				/*�Ѿ���д��spi flahs*/

				/*�������ݰ� �������������*/
				if(g_Update.data_packet_cur >= g_Update.data_packet_total)
				{
					/*�������֮�󣬱��ֱ�־��flash��*/
					g_Update.safe_flag = SPI_FLASH_UPDATE_NEEDED;
					App_Update_Config_Save_Into_Flash();//save 												
					
					g_Update.update_workstate = UPDATE_Escalation_package_download_status;
					bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
					//p_info("version_new;%d;%d;%d !",g_Update.version_new[0],g_Update.version_new[1],g_Update.version_new[2]);
					//p_info("version_cur;%d;%d;%d !",g_Update.version_cur[0],g_Update.version_cur[1],g_Update.version_cur[2]);
					p_info("down load over !");
				}
			}
			/*���յ����ݰ���������Ų��ԣ������·�������*/
			if(g_Update.rx_data_not_ok)
			{
				g_Update.rx_data_not_ok= 0;
			
				bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
				p_info("App_Update_Loop rx_data_not_ok ! ");
			}
		}				
			break;
		case UPDATE_Escalation_package_download_status :
		{
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_10SECONDER);
			
				App_PCP_TXData_process(MsgCode_Escalation_package_download_status); 
				p_info("tx MsgCode_Escalation_package_download_status");
			}
			if(g_Update.rx_data_in_flag == TRUE)
			{
				g_Update.rx_data_in_flag = FALSE;
				if(g_PCP_RXData.message_code == MsgCode_Escalation_package_download_status)
				{
					//bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
					g_Update.update_workstate = UPDATE_Perform_an_upgrade;
				}
			}			
		}
			break;	
		case UPDATE_Perform_an_upgrade :
		{
			if(g_Update.rx_data_in_flag == TRUE)
			{
				g_Update.rx_data_in_flag = FALSE;
				if(g_PCP_RXData.message_code == MsgCode_Perform_an_upgrade)
				{
					bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_10SECONDER);
					g_Update.update_workstate = UPDATE_OK_RESTART;
					
					App_PCP_TXData_process(MsgCode_Perform_an_upgrade); 
					p_info("tx MsgCode_Perform_an_upgrade");
				}
			}
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_60SECONDER);
			
				App_PCP_TXData_process(MsgCode_Perform_an_upgrade); 
				p_info("tx MsgCode_Perform_an_upgrade");

			}			
		}
			break;			
		case UPDATE_OK_RESTART:
		{		
			if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
			{
				p_info("UPDATE_OK_RESTART");
				#if 0
				App_Update_Init_Var();
				#else
				while(1)
				{
					__set_FAULTMASK(1); 	 // �ر������ж�							
					NVIC_SystemReset();//�����λ
				}
				#endif
			}		
			break;
		}
		
		default:
			break;
	}

	

}

#if 1
uint8_t BootLoad_Erase(uint32_t start)
{
	uint32_t NbrOfPages = 0;
	uint32_t PageError = 0;
	FLASH_EraseInitTypeDef pEraseInit;
	HAL_StatusTypeDef status = HAL_OK;
	
	/* Unlock the Flash to enable the flash control register access *************/ 
	HAL_FLASH_Unlock();
	
	/* Get the number of page to  erase */
	NbrOfPages = (FLASH_START_ADRESS + FLASH_SIZE);
	NbrOfPages = (NbrOfPages - start) / FLASH_PAGE_SIZE;
	
	if(NbrOfPages > FLASH_PAGE_NBPERBANK)
	{
	  pEraseInit.Banks = FLASH_BANK_1;
	  pEraseInit.NbPages = NbrOfPages % FLASH_PAGE_NBPERBANK;
	  pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
	  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	  status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
	
	  NbrOfPages = FLASH_PAGE_NBPERBANK;
	}
	
	if(status == HAL_OK)
	{
	  pEraseInit.Banks = FLASH_BANK_2;
	  pEraseInit.NbPages = NbrOfPages;// bank2  ȫ��������
	  pEraseInit.Page = FLASH_PAGE_NBPERBANK - pEraseInit.NbPages;
	  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	  status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
	}
	
	/* Lock the Flash to disable the flash control register access (recommended
	   to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();
	
	if (status != HAL_OK)
	{
	  /* Error occurred while page erase */
	  return FLASHIF_ERASEKO;
	}
	
	return FLASHIF_OK;

}

/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);

//#pragma pack(8) 
__align(8)     uint8_t data_spi_flash[1024];

//#pragma pack() 

pFunction JumpToApplication;
uint32_t JumpAddress;

uint8_t BootLoad_Program(void)
{
	uint32_t BaseAddress=APPLICATION_ADDRESS;//APP��ַ
	uint32_t i = 0;

	uint32_t flashdestination;

	
	//DISABLE_INT();// 
	FLASH_If_Init();
	
	/*ȫ�� ����bank 2��flash*/
	if(BootLoad_Erase(BaseAddress) !=FLASHIF_OK)//����Flash
	{
	
		//ENABLE_INT();// 
		return FALSE;
	}
	
	/* ��ʼ��flash����׵�ַ */
	flashdestination = APPLICATION_ADDRESS; 
	
	HAL_FLASH_Unlock();//����Flash
	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	DISABLE_INT();// 
	#if 0
	for(i=0; i<g_Update.data_packet_total; i++)
	{
		/*��flash�ж�������data buffer */	
		sf_ReadBuffer(data_flash, (i+1)*1024, 1024);
		/* --- ι�� */
		HAL_IWDG_Refresh(&hiwdg) ;
		
		/* �����յ������ݱ�̵�flash */			
		if (Flash_If_Write( data_flash, flashdestination,(uint32_t) g_Update.data_packet_size)  == 0)
		{
			flashdestination = flashdestination+ g_Update.data_packet_size;
		}
		/* ��̹����в������� */
		else 
		{
			/* ��ֹ���� */
			printf("FLASH_If_Write error!");
		}	
		/* --- ι�� */
		HAL_IWDG_Refresh(&hiwdg) ;
		
	}
	#endif

	g_Update.cpu_flash_all_total_chars = g_Update.data_packet_size*g_Update.data_packet_total;
	g_Update.cpu_flash_cur_page_num = g_Update.cpu_flash_all_total_chars/SPI_FLASH_EACH_PAGE_SIZE;

	
	g_Update.cpu_flash_cur_page_num = g_Update.cpu_flash_all_total_chars/SPI_FLASH_EACH_PAGE_SIZE;
	if( g_Update.cpu_flash_all_total_chars != g_Update.cpu_flash_cur_page_num*SPI_FLASH_EACH_PAGE_SIZE)
	{
		g_Update.cpu_flash_cur_page_num = g_Update.cpu_flash_cur_page_num+1;
	}
	//g_Update.cpu_flash_cur_address = 0;
	g_Update.spi_flash_cur_page_start_address = SPI_FLASH_code_ADDR;
	for(i=0; i<g_Update.cpu_flash_cur_page_num; i++)
	{
		/* --- ι�� */
		HAL_IWDG_Refresh(&hiwdg) ;
			
		/*��flash�ж�������data buffer */	
		sf_ReadBuffer(data_spi_flash, g_Update.spi_flash_cur_page_start_address+i*SPI_FLASH_EACH_PAGE_SIZE, SPI_FLASH_EACH_PAGE_SIZE);

		/* �����յ������ݱ�̵�flash */			
		if (Flash_If_Write( data_spi_flash, flashdestination,(uint32_t) SPI_FLASH_EACH_PAGE_SIZE)  == 0)
		//if (Flash_If_Write( data_spi_flash, flashdestination,(uint32_t) g_Update.data_packet_size)  == 0)
		{
			flashdestination = flashdestination+ SPI_FLASH_EACH_PAGE_SIZE;
		}
		/* ��̹����в������� */
		else 
		{
			/* ��ֹ���� */
			printf("FLASH_If_Write error!");
		}	
		/* --- ι�� */
		HAL_IWDG_Refresh(&hiwdg) ;
		
	}

	
	ENABLE_INT();// 
	Bsp_spiflash_power_off();	
	HAL_FLASH_Lock();//����Flash
	return TRUE;
}
void BootLoad_delay(void)
{
	u32 i = 0;
	u32 j = 0;
	for(i=0;i<100;i++)
	{
		for(j=0;j<100;j++)
		{
		
		
		}


	}

}

void BootLoad_Jump(void)
{
	/* Check Vector Table: Test if user code is programmed starting from address 
	"APPLICATION_ADDRESS" */
	//printfhex32((*(__IO uint32_t*)APPLICATION_ADDRESS));
	//printf("\n");
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
	{
		
		HAL_DeInit();// ��ת֮ǰ�����踴λ��
		DISABLE_INT();// 	
		//printf("BootLoad_Jump ok!");
	
		/* execute the new program */
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		/* Jump to user application */
		JumpToApplication = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		//BootLoad_delay();
	
		JumpToApplication();  

	}
	else
	{		
		//printf("BootLoad_Jump error!");
	}
}

/*
*********************************************************************************************************
*	�� �� ��: App_Update_Check_update_or_goto_app
*	����˵��: 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void App_Update_Check_update_or_goto_app(void)
{

	App_Update_Init_Var();// update

	/* ��־ok*/
	if(g_Update.safe_flag == SPI_FLASH_UPDATE_NEEDED)
	{
		printf("start update!");
		printf("\r\n");


		if(BootLoad_Program())//�Ƿ�д��ɹ�
		{			
			/*�������֮�󣬱��ֱ�־��flash��*/
			g_Update.safe_flag = SPI_FLASH_UPDATE_IS_OK;
			App_Update_Config_Save_Into_Flash();
			printf("update ok\n");

			BootLoad_Jump();//��ת����
		}
		else
		{
			//HAL_FLASH_Lock();//����Flash
			printf("update fail\n");
			//f_close(&File);//�ر������ļ�
			BootLoad_Jump();//��ת����
		}	

	}
	else
	{
		BootLoad_Jump();//��ת����
	}

}

#endif

