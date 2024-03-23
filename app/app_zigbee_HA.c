#include "app_zigbee_HA.h"

#include "app.h"
#include "bsp.h"

_End_Device_data_type g_tZigbeeHAEndDevice1;// �ն�1
_End_Device_data_type g_tZigbeeHAEndDevice2;// �ն�2
_End_Device_data_type g_tZigbeeHAEndDevice3;// �ն�3
_End_Device_data_type g_tZigbeeHAEndDevice4;// �ն�4
_End_Device_data_type g_tZigbeeHAEndDevice5;// �ն�5
_End_Device_data_type g_tZigbeeHAEndDevice6;// �ն�6

 _End_Device_data_type g_tZigbeeHAEndDevice7;// �ն�7
 _End_Device_data_type g_tZigbeeHAEndDevice8;// �ն�8
 _End_Device_data_type g_tZigbeeHAEndDevice9;// �ն�9
 _End_Device_data_type g_tZigbeeHAEndDevice10;// �ն�10
 _End_Device_data_type g_tZigbeeHAEndDevice11;// �ն�11
 _End_Device_data_type g_tZigbeeHAEndDevice12;// �ն�12

 _End_Device_data_type * g_tZigbeeHAEndDevice_cur;// ��ǰ���ն�

 _End_Device_data_type * g_tZigbeeHAEndDevice_cmd_ASK;// ����ش���ն�
 u8  g_tZigbeeHAEndDevice_cmd_ASK_FLAG = 0;// ����ش���ն�

 _Coordinator_data_type g_tZigbeeHACoordinator;// Э���� Coordinator

 ZIGBEE_CONFIG_STRCUT g_tZigbeeHAConfig;//����Ĳ�����Ϣ

u8 rx_data_Coordinator[UART1_RX_BUF_SIZE];
u8 g_zigbee_count  = 0;


static u8 flash_data_buffer[ZIGBEE_SET_DATA_GESHU];

extern void App_SOCKET_STATE_ask(_End_Device_data_type * pEndDevice,uint8_t work_type);

/*
*********************************************************************************************************
*	�� �� ��: App_Config_Save_Into_Flash
*	����˵��:	  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_HA_Config_Save_Into_Flash(void)
{
	u8 l_sum = 0;
	u8* l_point = 0;
	u8 i = 0;
	_End_Device_data_type * pEndDevice;// �ն�


	l_point = &g_tZigbeeHAConfig.macAddrv[0][0];
	
	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(ZIGBEE_SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + l_point[i];
	}
	g_tZigbeeHAConfig.sum = l_sum;// �����ۼӺ͡�

	
	/*���浽spi flash����*/
	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
	sf_WriteBuffer(&g_tZigbeeHAConfig.macAddrv[0][0], SPI_FLASH_ZIGBEE_ADDR_VAR, ZIGBEE_SET_DATA_GESHU);
	Bsp_spiflash_power_off();	
	

	/* ��flash�п�����Ӧ����Ϣ����*/
	g_tZigbeeHACoordinator.End_Device_nums = g_tZigbeeHAConfig.End_Device_nums;	
	for(i=End_Device1;i<=End_Device6;i++)
	{
		pEndDevice = EndDeviceNums_To_EndDevice(i);

		memcpy((u8 *)&pEndDevice->macAddr , (u8 *)&g_tZigbeeHAConfig.macAddrv[i-1][0],ZIGBEE_MAC_ADDRESS_SIZE);
		
		pEndDevice->nwkAddr_L  =g_tZigbeeHAConfig.nwkAddr_L[i-1] ; 
		pEndDevice->nwkAddr_H =g_tZigbeeHAConfig.nwkAddr_H[i-1] ; 		
	}

	/* ��ӡ��ÿ����������Ϣ*/
	p_info("End_Device_nums:%d",g_tZigbeeHACoordinator.End_Device_nums);
	for(i=1;i<=g_tZigbeeHACoordinator.End_Device_nums;i++)
	{
		pEndDevice = EndDeviceNums_To_EndDevice(i);

		p_info("   macAddr:%02X,i=%d",pEndDevice->macAddr[0],i);
		p_info("nwkAddr_L:%02X,i=%d",pEndDevice->nwkAddr_L,i);		
		p_info("nwkAddr_H:%02X,i=%d",pEndDevice->nwkAddr_H,i);				
	}


}

/*
*********************************************************************************************************
*	�� �� ��: App_ZIGBEE_HA_Config_read_from_flash
*	����˵��:     ���浽spi flash�еĲ���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void App_ZIGBEE_HA_Config_read_from_flash(void)
{
	u8 i = 0;
	u8 l_sum = 0;

	memset(flash_data_buffer,0,ZIGBEE_SET_DATA_GESHU);

	Bsp_spiflash_power_on();//��gps��Դ
	bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);

	/*��ȡZIGBEE�Ĳ���*/
	sf_ReadBuffer(flash_data_buffer,SPI_FLASH_ZIGBEE_ADDR_VAR,ZIGBEE_SET_DATA_GESHU);/*�ӵ�ַ512��ȡ����*/
	Bsp_spiflash_power_off();


	/*����ǰ6�������ۼ�*/
	l_sum = 0;
	for(i=0;i<(ZIGBEE_SET_DATA_GESHU-1);i++)
	{
		l_sum = l_sum + flash_data_buffer[i];
	}
	/*�ж��ۼӺ��Ƿ���ȷ*/
	if((l_sum == flash_data_buffer[ZIGBEE_SET_DATA_GESHU-1])&&(flash_data_buffer[ZIGBEE_SET_DATA_GESHU-1] != 0xFF))
	{
		memcpy(&g_tZigbeeHAConfig.macAddrv[0][0],flash_data_buffer,ZIGBEE_SET_DATA_GESHU);
		p_info("zigbee�ۼӺ���ȷ  ");
		
	}
	else
	{
		memset(&g_tZigbeeHAConfig.macAddrv[0][0],0,ZIGBEE_SET_DATA_GESHU);
	
		g_tZigbeeHAConfig.End_Device_nums = 0;
			
		p_info("�ۼӺͲ���ȷ  new");
	}
	
	App_ZIGBEE_HA_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash


}

/*
*********************************************************************************************************
*	�� �� ��: ComToUart
*	����˵��: ��COM�˿ں�ת��ΪUARTָ��
*	��    ��: _ucPort: �˿ں�(COM1 - COM6)
*	�� �� ֵ: uartָ��
*********************************************************************************************************
*/
_End_Device_data_type *EndDeviceNums_To_EndDevice(uint8_t DeviceNums)
{
	if (DeviceNums == End_Device1)
	{
		return &g_tZigbeeHAEndDevice1;
	}
	else if (DeviceNums == End_Device2)
	{
		return &g_tZigbeeHAEndDevice2;
	}
	else if (DeviceNums == End_Device3)
	{
		return &g_tZigbeeHAEndDevice3;
	}
	else if (DeviceNums == End_Device4)
	{
		return &g_tZigbeeHAEndDevice4;
	}
	else if (DeviceNums == End_Device5)
	{
		return &g_tZigbeeHAEndDevice5;
	}
	else if (DeviceNums == End_Device6)
	{
		return &g_tZigbeeHAEndDevice6;
	}	
	else if (DeviceNums == End_Device7)
	{
		return &g_tZigbeeHAEndDevice7;
	}
	else if (DeviceNums == End_Device8)
	{
		return &g_tZigbeeHAEndDevice8;
	}
	else if (DeviceNums == End_Device9)
	{
		return &g_tZigbeeHAEndDevice9;
	}
	else if (DeviceNums == End_Device10)
	{
		return &g_tZigbeeHAEndDevice10;
	}
	else if (DeviceNums == End_Device11)
	{
		return &g_tZigbeeHAEndDevice11;
	}
	else if (DeviceNums == End_Device12)
	{
		return &g_tZigbeeHAEndDevice12;
	}			
	else
	{
		/* �����κδ��� */
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: App_ZIGBEE_HA_Init_Var
*	����˵��:  
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_HA_Init_Var(void)
{
	g_tZigbeeHACoordinator.start = FALSE;
	g_tZigbeeHACoordinator.power_flag = FALSE;

	g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_OPEN;
	g_tZigbeeHACoordinator.flag_tx = FALSE;
	
	g_tZigbeeHACoordinator.count_tx = 0;// ++

	g_tZigbeeHACoordinator.loop_time =TMR_DELAY_0_3S ;//0.5����ѭ����ѯһ�Ρ�6������
	g_tZigbeeHACoordinator.report_time =TMR_DELAY_30S ;//
	g_tZigbeeHACoordinator.address_socket_cur =0 ;

	g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = FALSE;// Ĭ�Ϲر�
			
	g_tZigbeeHACoordinator.version[0]= 0;
	g_tZigbeeHACoordinator.version[1]= 0;
	g_tZigbeeHACoordinator.version[2]= 0;

	g_tZigbeeHACoordinator.nwkAddr0 = 0x00;
	g_tZigbeeHACoordinator.nwkAddr1 = 0x00;


	g_tZigbeeHACoordinator.rx_command_layer = 0;
	g_tZigbeeHACoordinator.rx_command_subcommand= 0;
	g_tZigbeeHACoordinator.rx_command_subcommand_type =0;
		
	g_tZigbeeHACoordinator.rx_command_config_id= 0;

	g_tZigbeeHACoordinator.End_Device_nums = 0;

	memset(&g_tZigbeeHAEndDevice1,0x0,sizeof(g_tZigbeeHAEndDevice1));
	memset(&g_tZigbeeHAEndDevice2,0x0,sizeof(g_tZigbeeHAEndDevice2));
	memset(&g_tZigbeeHAEndDevice3,0x0,sizeof(g_tZigbeeHAEndDevice3));
	memset(&g_tZigbeeHAEndDevice4,0x0,sizeof(g_tZigbeeHAEndDevice4));
	memset(&g_tZigbeeHAEndDevice5,0x0,sizeof(g_tZigbeeHAEndDevice5));
	memset(&g_tZigbeeHAEndDevice6,0x0,sizeof(g_tZigbeeHAEndDevice6));

	g_tZigbeeHAEndDevice1.on_time = TMR_DELAY_3S;
	g_tZigbeeHAEndDevice1.off_time= TMR_DELAY_10S;
	g_tZigbeeHAEndDevice1.count_time = 0;

	g_tZigbeeHAEndDevice1.endpoint_num  = 1;
	g_tZigbeeHAEndDevice1.endpoint_Addr[0] = 0x01;//C0 A9
	
	g_tZigbeeHAEndDevice1.nwkAddr_H  = 0xA9;
	g_tZigbeeHAEndDevice1.nwkAddr_L = 0xC0;//C0 A9


	g_tZigbeeHAEndDevice2.endpoint_num  = 1;
	g_tZigbeeHAEndDevice2.endpoint_Addr[0] = 0x01;

	g_tZigbeeHAEndDevice3.endpoint_num  = 1;
	g_tZigbeeHAEndDevice3.endpoint_Addr[0] = 0x01;

	g_tZigbeeHAEndDevice4.endpoint_num  = 1;
	g_tZigbeeHAEndDevice4.endpoint_Addr[0] = 0x01;	

	g_tZigbeeHAEndDevice5.endpoint_num  = 1;
	g_tZigbeeHAEndDevice5.endpoint_Addr[0] = 0x01;

	g_tZigbeeHAEndDevice6.endpoint_num  = 1;
	g_tZigbeeHAEndDevice6.endpoint_Addr[0] = 0x01;

	App_ZIGBEE_HA_Config_read_from_flash();

}





/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_1_3_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/

void App_ZIGBEE_HA_tx_system_1_3_cmd(u8  sub_cmd ,u8 config_id,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;


	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_System;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = sub_cmd;//CMD	
	buffer_lenth++;			

	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = 0x00;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	�˵�
	buffer_lenth++;		

		(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	
	buffer_lenth++;	

	if(sub_cmd == ZIGBEE_HA_System_CMD_Restore_the_coordinator_to)
	{
		buffer[buffer_lenth] = 0x03;//	
		buffer_lenth++; 
	}
	
	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}





/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_4_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_system_4_cmd(u8  sub_cmd ,u8 read_write,u8 config_id,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_System;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = sub_cmd;//CMD	
	buffer_lenth++;			

	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = 0x00;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	�˵�
	buffer_lenth++;		

		(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	

	
	buffer[buffer_lenth] = read_write;//	��/д
	buffer_lenth++;	


	buffer[buffer_lenth] = config_id;//	����ID
	buffer_lenth++;	

	
	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_4_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_Network_cmd_Turn_on_off_the_network(u8  on_off_the_network,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_Network_Management;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = ZIGBEE_HA_Network_CMD_Turn_on_off_the_network;//subCMD	
	buffer_lenth++;			
	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = 0x00;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	�˵�
	buffer_lenth++;		

	(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	
	
	buffer[buffer_lenth] = on_off_the_network;//
	buffer_lenth++;	

	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}


/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_4_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_Network_cmd_Querying_device_endpoint(u8 nwkAddr_L,u8 nwkAddr_H,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_Network_Management;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = ZIGBEE_HA_Network_CMD_Querying_device_endpoint_information;//subCMD	
	buffer_lenth++;			
	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = nwkAddr_L;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = nwkAddr_H;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	�˵�
	buffer_lenth++;		

		(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	
	
	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}



/**************************************************************************************                      
* ��    App_ZIGBEE_HA_tx_Network_cmd_Get_the_nwkAddr_according_MACAddr
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_Network_cmd_Get_the_nwkAddr_according_MACAddr(u8 *mac,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_Network_Management;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = ZIGBEE_HA_Network_CMD_Get_the_nwkAddr_according_MACAddr;//subCMD	
	buffer_lenth++;			
	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_MAC_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = 0x00;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	�˵�
	buffer_lenth++;		

		(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	

	/*mac*/
	for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
	{
		buffer[buffer_lenth] =*mac;//	mac
		mac++;
		buffer_lenth++; 
	}

	buffer[buffer_lenth] = 0x00;//	��������
	buffer_lenth++;
	buffer[buffer_lenth] = 0x00;//	��ʼ����
	buffer_lenth++;

	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);
}


/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_4_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_Light_cmd_on_off(u8 nwkAddr_L,u8 nwkAddr_H,u8 end_ponit,u8 on_off,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_Lamp;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = ZIGBEE_HA_Lamp_CMD_on_off_light;//subCMD	
	buffer_lenth++;			
	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = nwkAddr_L;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = nwkAddr_H;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = end_ponit;//	�˵�
//	0x0A��OnOff����   0x08 �ǵ��⹦�ܣ�Ӳ��δ������
	buffer_lenth++;		

	(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ

	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	

	buffer[buffer_lenth] = on_off;//	��������
	buffer_lenth++;	
		
	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}




/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_tx_system_4_cmd
* ��    ��:
* ��ڲ�����
	sub_cmd��	������			
	����ID:����ID

* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_tx_Light_chaxun_state(u8 nwkAddr_L,u8 nwkAddr_H,u8 end_ponit,u8 * zigbee_count)
{

	u8 l_sum = 0;
	u8 i = 0;

	u8 buffer[16];
	u8 buffer_lenth=0;
	
	memset(buffer,0x00,sizeof(buffer));
	
	buffer[buffer_lenth] = ZIGBEE_HA_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_HA_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = ZIGBEE_HA_CMD_Lamp;//CMD	0x01
	buffer_lenth++;		
	
	buffer[buffer_lenth] = ZIGBEE_HA_Lamp_CMD_Query_lamp_switch_status;//subCMD	
	buffer_lenth++;			
	
	buffer[buffer_lenth] = ZIGBEE_HA_AddrMode_Network_address_unicast;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = nwkAddr_L;//nwkAddr0
	buffer_lenth++;	
	
	buffer[buffer_lenth] = nwkAddr_H;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = end_ponit;//	�˵�
	buffer_lenth++;		

	(*zigbee_count) =(*zigbee_count) + 1 ;// ��һ
	buffer[buffer_lenth] = *zigbee_count;//	֡���
	buffer_lenth++;	

	buffer[buffer_lenth] = 0x00;//	
	buffer_lenth++;	
	
	buffer[buffer_lenth] = 0x00;//	
	buffer_lenth++;	
	
	buffer[buffer_lenth] = l_sum;//	���У���
	buffer_lenth++;	

	buffer[ZIGBEE_HA_DATA_LENTH_POS] = buffer_lenth;//	LEN

	/* ����ǰ9�����ݵ��ɻ��*/
	l_sum = 0;
	for(i=0;i<(buffer_lenth-1);i++)
	{
		l_sum = l_sum ^ buffer[i];
	}
	buffer[buffer_lenth-1] = l_sum;

	
	HAL_UART_Transmit(&ZIGBEE_HANDLE_UART, buffer, buffer_lenth, 0xFFFF);

}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_rx_data_pro_CMD_System
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
uint8_t App_ZIGBEE_HA_rx_data_pro_CMD_System(uint8_t Control_command_subcommand,uint16_t cur_pos)
{
	uint8_t ret = 0xff ;
	uint8_t Configuring_ID = 0xff ;
	
	Configuring_ID =  rx_data_Coordinator[10+cur_pos];//	����ID
	g_tZigbeeHACoordinator.rx_command_config_id = Configuring_ID;
	
	switch(g_tZigbeeHACoordinator.rx_command_subcommand)
	{
		case ZIGBEE_HA_System_CMD_Query_coordinator_firm_ver:
		{
			g_tZigbeeHACoordinator.version[0] = rx_data_Coordinator[9+cur_pos];
			g_tZigbeeHACoordinator.version[1] = rx_data_Coordinator[10+cur_pos];
			g_tZigbeeHACoordinator.version[2] = rx_data_Coordinator[11+cur_pos];

		}
			break;
		case ZIGBEE_HA_System_CMD_Restart_coordinator:
		{
		}
			break;
		case ZIGBEE_HA_System_CMD_Restore_the_coordinator_to:
		{
		}
			break;
			
		case ZIGBEE_HA_System_CMD_Conf_coordinator_netw://	1.4 ����Э�����������
		{
			switch(g_tZigbeeHACoordinator.rx_command_config_id)
			{
				case ZIGBEE_HA_System_CMD_Configuration_channel:
				{			
					g_tZigbeeHACoordinator.channel= rx_data_Coordinator[12+cur_pos];
					p_info("�ŵ�:%d!",g_tZigbeeHACoordinator.channel);
				}
					break;
				case ZIGBEE_HA_System_CMD_Configuration_PANID:
				{
					g_tZigbeeHACoordinator.PanIDL= rx_data_Coordinator[12+cur_pos];
					g_tZigbeeHACoordinator.PanIDH= rx_data_Coordinator[13+cur_pos];
					p_info("PanID:%x%x!",g_tZigbeeHACoordinator.PanIDH,g_tZigbeeHACoordinator.PanIDL);
				}
					break;
				case ZIGBEE_HA_System_CMD_Configuration_EPID:
				{
				
				}
					break;
				case ZIGBEE_HA_System_CMD_Configuring_system_time:
				{
				
				}
					break;
				default:
					break;

			}
			
		}
			break;

		case ZIGBEE_HA_System_CMD_Coordinator_backup_and_recovery:
		{
		}
			break;
		case ZIGBEE_HA_System_CMD_rest_network_succ:
		{
			g_tZigbeeHACoordinator.version[0] = rx_data_Coordinator[9+cur_pos];
			g_tZigbeeHACoordinator.version[1] = rx_data_Coordinator[10+cur_pos];
			g_tZigbeeHACoordinator.version[2] = rx_data_Coordinator[11+cur_pos];
		}
			break;

		default:
			break;
			
	}

	ret = Control_command_subcommand;
	return ret;
}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_rx_data_pro_CMD_System
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
uint8_t App_ZIGBEE_HA_rx_data_pro_Network_CMD(uint8_t Control_command_subcommand,uint16_t cur_pos)
{
	uint8_t ret = 0xff ;
	uint8_t i = 0 ;
	uint8_t j = 0 ;
	_End_Device_data_type * pEndDevice;// �ն�
	int return_value = 1;//��������ͬ�ĶԱȽ��

	switch(Control_command_subcommand)
	{
		case ZIGBEE_HA_Network_CMD_Turn_on_off_the_network:
		{

		}
			break;
		case ZIGBEE_HA_Network_CMD_Device_address_information:
		{
			/* 0x80��ʾ���յ���Զ��������Ϣ*/
			if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{			

				/* ��6���ն��� ƥ�䵽��Ӧ������ָ��*/
				for(i=End_Device1;i<=g_tZigbeeHACoordinator.End_Device_nums;i++)
				{
					pEndDevice = EndDeviceNums_To_EndDevice(i);
					return_value = (memcmp(pEndDevice->macAddr,&rx_data_Coordinator[11+cur_pos],ZIGBEE_MAC_ADDRESS_SIZE));
					if(return_value == 0)// ��ͬ��mac�������network address
					{
						p_info("����������ͬ��mac return");
						break;
						//return Control_command_subcommand;
					}
					else
					{
						p_info("��������bu��ͬ��mac,��������");
					}
				}
				/*����������ͬ��mac,�򲻼�1*/
				if(return_value == 0)
				{
					/*	��������bu��ͬ��mac��ַ�����ظ����ԣ������µĲ�����*/
					g_tZigbeeHAConfig.nwkAddr_L[i-End_Device1] =  rx_data_Coordinator[9+cur_pos];// network address
					g_tZigbeeHAConfig.nwkAddr_H[i-End_Device1] =  rx_data_Coordinator[10+cur_pos];//	
					
					for(j=0; j<ZIGBEE_MAC_ADDRESS_SIZE; j++)
						g_tZigbeeHAConfig.macAddrv[i-End_Device1][j] = rx_data_Coordinator[j+11+cur_pos];// mac
				}
				else/*�������ڲ���ͬ��mac,���1*/
				{
					/*	��������bu��ͬ��mac��ַ�����ظ����ԣ������µĲ�����*/
					g_tZigbeeHAConfig.nwkAddr_L[g_tZigbeeHAConfig.End_Device_nums] =  rx_data_Coordinator[9+cur_pos];// network address
					g_tZigbeeHAConfig.nwkAddr_H[g_tZigbeeHAConfig.End_Device_nums] =  rx_data_Coordinator[10+cur_pos];//	
					
					for(j=0; j<ZIGBEE_MAC_ADDRESS_SIZE; j++)
						g_tZigbeeHAConfig.macAddrv[g_tZigbeeHAConfig.End_Device_nums][j] = rx_data_Coordinator[j+11+cur_pos];// mac
				
					g_tZigbeeHAConfig.End_Device_nums ++;
				}

				/* ���浽spi flash �У�ͬʱ���µ�����1-6�У���ӡ����*/
				App_ZIGBEE_HA_Config_Save_Into_Flash();
			}
			
		}
			break;
		case ZIGBEE_HA_Network_CMD_Querying_device_endpoint_information:
		{
			/* 0x80��ʾ���յ���Զ��������Ϣ*/
			if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{				
				g_tZigbeeHAEndDevice1.endpoint_num =  rx_data_Coordinator[10+cur_pos];//	
				if(g_tZigbeeHAEndDevice1.endpoint_num >= 8)
					g_tZigbeeHAEndDevice1.endpoint_num = 8;
				
				/* �˵�����*/
				for(i=0; i<g_tZigbeeHAEndDevice1.endpoint_num; i++)
					g_tZigbeeHAEndDevice1.endpoint_Addr[i] =  rx_data_Coordinator[i+11+cur_pos];//	mac
				
				g_tZigbeeHAEndDevice1.flag_endpoint_is_queryed	 = TRUE;
			}

		}
			break;
			
		case ZIGBEE_HA_Network_CMD_Query_device_type_and_function_information:
		{
			
		}
			break;

		case ZIGBEE_HA_Network_CMD_Get_the_nwkAddr_according_MACAddr:
		{
			/* 0x80��ʾ���յ���Զ��������Ϣ*/
			if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{
			g_tZigbeeHAConfig.nwkAddr_L[End_Device1-1] =  rx_data_Coordinator[5+cur_pos];// network address
			g_tZigbeeHAConfig.nwkAddr_H[End_Device1-1] =  rx_data_Coordinator[6+cur_pos];//	
			/* ���浽spi flash �У�ͬʱ���µ�����1-6�У���ӡ����*/
			App_ZIGBEE_HA_Config_Save_Into_Flash();
			}
			
		}
			break;
		case ZIGBEE_HA_Network_CMD_Get_the_MACAddr_according_nwkAddr:
		{

		}
			break;

		case ZIGBEE_HA_Network_CMD_binding:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Unbind:
		{

		}
			break;


		case ZIGBEE_HA_Network_CMD_binding_table_query:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Switching_network_device_channel:
		{

		}
			break;		
		case ZIGBEE_HA_Network_CMD_Query_device_neighbor_table:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Query_device_routing_table:
		{
		
		}
			break;		

		case ZIGBEE_HA_Network_CMD_network_scan:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Configuring_remote_device_network_parameters:
		{
		
		}
			break;		
		case ZIGBEE_HA_Network_CMD_Restart_remote_device:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_configuration_key:
		{
		
		}
			break;		

		case ZIGBEE_HA_Network_CMD_update_network_key:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Switch_network_key:
		{
		
		}
			break;		
		case ZIGBEE_HA_Network_CMD_Device_Online_Monitoring:
		{
		}
			break;
		case ZIGBEE_HA_Network_CMD_Get_device_signal_strength:
		{
		
		}
			break;		
		case ZIGBEE_HA_Network_CMD_Off_grid_device_information_reporting:
		{
		
		}
			break;		

			
		default:
			break;
			
	}

	ret = Control_command_subcommand;
	return ret;
}

/**************************************************************************************                      
* ��    App_ZIGBEE_HA_rx_data_pro_Lamp_CMD
* ��    �ܣ�ZIGBE
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ  
* ���÷�������
****************************************************************************************/
uint8_t App_ZIGBEE_HA_rx_data_pro_Lamp_CMD(uint8_t Control_command_subcommand,uint16_t cur_pos)
{
	uint8_t ret = 0xff ;
	
	switch(Control_command_subcommand)
	{
		case ZIGBEE_HA_Lamp_CMD_on_off_light:
		{
			#if 0
			bsp_StartTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER,g_tZigbeeHACoordinator.loop_time);
			#else
			bsp_StartTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER,g_tZigbeeHACoordinator.loop_time);
			#endif
			bsp_StartTimer(TMR_ID_ZIGBEE_HA_report_socke_state,g_tZigbeeHACoordinator.report_time);//ֹͣ�ϱ�
			g_tZigbeeHAEndDevice_cmd_ASK_FLAG = 1;

		}
			break;

		case ZIGBEE_HA_Lamp_CMD_Query_lamp_switch_status:
		{
			uint8_t nwkAddr_H = 0;
			uint8_t nwkAddr_L = 0;
			uint8_t socket_state = 0;
			nwkAddr_L =  rx_data_Coordinator[5+cur_pos];//	network address
			nwkAddr_H =  rx_data_Coordinator[6+cur_pos];// 
			socket_state =  rx_data_Coordinator[10+cur_pos];// 
			
			/* 0x80��ʾ���յ���Զ��������Ϣ*/
			if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Local_information_return)
			{
				p_info("ģ��Ӧ��" );
			}
			else if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{		
				p_info("����Ӧ��" );
			
				if((g_tZigbeeHAEndDevice_cur->nwkAddr_L== nwkAddr_L)&&(g_tZigbeeHAEndDevice_cur->nwkAddr_H== nwkAddr_H))
				{
				 	g_tZigbeeHAEndDevice_cur->socket_state = socket_state;					
					g_tZigbeeHAEndDevice_cur->flag_on_network = TRUE;
					p_info("���ߣ�״̬%d",g_tZigbeeHAEndDevice_cur->socket_state );
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);				
				}
			}
		}
			break;			
			
		default:
			break;
			
	}

	ret = Control_command_subcommand;
	return ret;
}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_loop
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
uint8_t App_ZIGBEE_HA_rx_data_pro(void)
{
	uint8_t ucData;
	uint16_t cur_pos = 0;
	uint16_t pos = 0;
	uint8_t ret = 0xff ;
	uint8_t rx_cmd = 0xff;

	uint8_t i;	
	uint8_t data_lenth = 0;	
	uint8_t data_crc = 0;
	uint8_t data_crc_ok = FALSE;
	uint8_t Control_command_layer = 0x00;//��ID Control command - layer
	uint8_t Control_command_subcommand = 0x00;//Control command��subcommand
	
//	uint8_t AddrMode = 0;
	
	if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_UART_DATAIN))
	{
		p_info("ZIGBEE_HA_UART_DATAIN");
		memset(rx_data_Coordinator,0,sizeof(rx_data_Coordinator));
		
		while(1)
		{
		
			if(comGetChar(ZIGBEE_HA_UART1, &ucData))
			{			
				rx_data_Coordinator[pos++] = ucData;
				printf("%02X",ucData);
			}
			else
				break;

			if(pos >= DataSize_MAX)
				break;
		}
		/* log */
		if(pos <= 4)
		{
			ret = 250;
			return ret; 
		}
		//p_info("L=%d",pos);
		/* ��һ��������һ��֡*/
		while(pos>0)
		{
			pos--;
			/*����֡ͷ*/
			if((rx_data_Coordinator[cur_pos] == ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator))//||(rx_data_Coordinator[cur_pos] == 0x00) )
			{
				data_lenth =rx_data_Coordinator[cur_pos+1];//֡����

				/*�������У���*/
				data_crc = 0;
				for(i =0 ;i<(data_lenth-1);i++)
				{
					data_crc ^=rx_data_Coordinator[cur_pos+i];
				}
				if(rx_data_Coordinator[cur_pos+(data_lenth-1)] == data_crc)
				{
					data_crc_ok = TRUE;
					//p_info("crc 1");
				}
				//p_info("break 1");
				break;//У���ͨ����
			}
			else
			{
				cur_pos++;
			}		
		}		
		
		/*  Э������������ظ�*/
		if((rx_data_Coordinator[0+cur_pos] == ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator)&&(data_crc_ok == TRUE))//&&(rx_data_Coordinator[7+cur_pos] == ZIGBEE_HA_CMD_READ))
		{
			ret = ZIGBEE_HA_RX_HEADER; // ��֡ͷ
			
			/*��������ձ���*/
			g_tZigbeeHACoordinator.rx_command_layer = 0;
			g_tZigbeeHACoordinator.rx_command_subcommand = 0;
			g_tZigbeeHACoordinator.rx_command_subcommand_type = 0;
			
			g_tZigbeeHACoordinator.rx_command_config_id= 0;		
			
			/* ������*/
			Control_command_layer = rx_data_Coordinator[2+cur_pos];// cmd
			g_tZigbeeHACoordinator.rx_command_layer	 = Control_command_layer;

			/* ������*/
			rx_cmd = rx_data_Coordinator[3+cur_pos];// cmd
			
			/*���У������ֽ� �� 0x40��ʾ������Ϣ���ش���ACK�������ֽ� �� 0x80��ʾ���յ���Զ��������Ϣ*/
			if((rx_cmd&ZIGBEE_HA_SUB_CMD_Local_information_return) == ZIGBEE_HA_SUB_CMD_Local_information_return)//0x40|CMD //2.2.0.1 ���������ַ ��ѯ macAddr//2.2.1 ����͸������
			{			
				Control_command_subcommand  = rx_cmd&(~ZIGBEE_HA_SUB_CMD_Local_information_return);// CMD&0x80
				g_tZigbeeHACoordinator.rx_command_subcommand_type = ZIGBEE_HA_SUB_CMD_Local_information_return;				
			}
			else if((rx_cmd&ZIGBEE_HA_SUB_CMD_Remote_wireless_message) == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{
				Control_command_subcommand  = rx_cmd&(~ZIGBEE_HA_SUB_CMD_Remote_wireless_message);// CMD&0x80
				g_tZigbeeHACoordinator.rx_command_subcommand_type = ZIGBEE_HA_SUB_CMD_Remote_wireless_message;
			}
			g_tZigbeeHACoordinator.rx_command_subcommand	 = Control_command_subcommand;

			/* ��������߼�*/
			switch(g_tZigbeeHACoordinator.rx_command_layer)
			{
				case ZIGBEE_HA_CMD_System:
				{
					App_ZIGBEE_HA_rx_data_pro_CMD_System(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;
				case ZIGBEE_HA_CMD_Network_Management:
				{
					App_ZIGBEE_HA_rx_data_pro_Network_CMD(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;
				case ZIGBEE_HA_CMD_Standard_HA:
				{
				
				}
					break;
				case ZIGBEE_HA_CMD_No_Standard:
				{

				}
					break;
				case ZIGBEE_HA_CMD_Lamp:
				{
					App_ZIGBEE_HA_rx_data_pro_Lamp_CMD(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;

				default:
					break;
					
			}
		}

		/*  �ٴμ����ڶ�����ͷFE*/
		cur_pos=cur_pos+1;//ƫ��7���ֽ�
		while(pos>0)
		{
			pos--;
			/*����֡ͷ*/
			if((rx_data_Coordinator[cur_pos] == ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator))//||(rx_data_Coordinator[cur_pos] == 0x00) )
			{
				data_lenth =rx_data_Coordinator[cur_pos+1];//֡����

				/*�������У���*/
				data_crc = 0;
				data_crc_ok = FALSE;				
				for(i =0 ;i<(data_lenth-1);i++)
				{
					data_crc ^=rx_data_Coordinator[cur_pos+i];
				}
				if(rx_data_Coordinator[cur_pos+(data_lenth-1)] == data_crc)
				{
					data_crc_ok = TRUE;				
					//p_info("crc 2");
				}
				//p_info("break 2=%d",cur_pos);				
				break;//У���ͨ����
			}
			else
			{
				cur_pos++;
			}		
		}		

		
		/*  Э������������ظ�*/
		if(((rx_data_Coordinator[0+cur_pos] == ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator)&&(data_crc_ok == TRUE)))//&&(rx_data_Coordinator[7+cur_pos] == ZIGBEE_HA_CMD_READ))
		{
			ret = ZIGBEE_HA_RX_HEADER; // ��֡ͷ
			
			/*��������ձ���*/
			g_tZigbeeHACoordinator.rx_command_layer = 0;
			g_tZigbeeHACoordinator.rx_command_subcommand = 0;
			g_tZigbeeHACoordinator.rx_command_subcommand_type = 0;
			
			g_tZigbeeHACoordinator.rx_command_config_id= 0;		
			
			/* ������*/
			Control_command_layer = rx_data_Coordinator[2+cur_pos];// cmd
			g_tZigbeeHACoordinator.rx_command_layer	 = Control_command_layer;

			/* ������*/
			rx_cmd = rx_data_Coordinator[3+cur_pos];// cmd
			
			/*���У������ֽ� �� 0x40��ʾ������Ϣ���ش���ACK�������ֽ� �� 0x80��ʾ���յ���Զ��������Ϣ*/
			if((rx_cmd&ZIGBEE_HA_SUB_CMD_Local_information_return) == ZIGBEE_HA_SUB_CMD_Local_information_return)//0x40|CMD //2.2.0.1 ���������ַ ��ѯ macAddr//2.2.1 ����͸������
			{			
				Control_command_subcommand  = rx_cmd&(~ZIGBEE_HA_SUB_CMD_Local_information_return);// CMD&0x80
				g_tZigbeeHACoordinator.rx_command_subcommand_type = ZIGBEE_HA_SUB_CMD_Local_information_return;				
			}
			else if((rx_cmd&ZIGBEE_HA_SUB_CMD_Remote_wireless_message) == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
			{
				Control_command_subcommand  = rx_cmd&(~ZIGBEE_HA_SUB_CMD_Remote_wireless_message);// CMD&0x80
				g_tZigbeeHACoordinator.rx_command_subcommand_type = ZIGBEE_HA_SUB_CMD_Remote_wireless_message;
			}
			g_tZigbeeHACoordinator.rx_command_subcommand	 = Control_command_subcommand;

			/* ��������߼�*/
			switch(g_tZigbeeHACoordinator.rx_command_layer)
			{
				case ZIGBEE_HA_CMD_System:
				{
					App_ZIGBEE_HA_rx_data_pro_CMD_System(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;
				case ZIGBEE_HA_CMD_Network_Management:
				{
					App_ZIGBEE_HA_rx_data_pro_Network_CMD(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;
				case ZIGBEE_HA_CMD_Standard_HA:
				{
				
				}
					break;
				case ZIGBEE_HA_CMD_No_Standard:
				{

				}
					break;
				case ZIGBEE_HA_CMD_Lamp:
				{
					App_ZIGBEE_HA_rx_data_pro_Lamp_CMD(g_tZigbeeHACoordinator.rx_command_subcommand,cur_pos);
				}
					break;

				default:
					break;
					
			}
		}
		
    }

    return ret;
}  

/*
*********************************************************************************************************
*	�� �� ��: App_communication_address_change_pro
*	����˵��:  ѭ��ɨ�� ��������ͷ��ַ��
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_HA_address_change_pro(void)
{
	/* ��ǰͨ�ŵ�ַ���л�*/
	g_tZigbeeHACoordinator.address_socket_cur++;
	if(g_tZigbeeHACoordinator.address_socket_cur > g_tZigbeeHACoordinator.End_Device_nums)
	{
		g_tZigbeeHACoordinator.address_socket_cur= End_Device1 ;
	}

	/* ��ǰͨ�� ָ��ı�� */
	g_tZigbeeHAEndDevice_cur = EndDeviceNums_To_EndDevice(g_tZigbeeHACoordinator.address_socket_cur);
	p_info("����=%d",g_tZigbeeHACoordinator.address_socket_cur);
}
/*
*********************************************************************************************************
*	�� �� ��: App_ZIGBEE_HA_chaxun_socket_state
*	����˵��:  ѭ��ɨ�� ������״̬
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_HA_chaxun_socket_state(void)
{
	
	if(bsp_CheckTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER))
	{
		bsp_StartTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER,g_tZigbeeHACoordinator.loop_time);

		if(g_tZigbeeHAEndDevice_cmd_ASK_FLAG == 1)
		{
			g_tZigbeeHAEndDevice_cmd_ASK_FLAG = 0;
			
			g_tZigbeeHAEndDevice_cur = g_tZigbeeHAEndDevice_cmd_ASK;// 
			App_ZIGBEE_HA_tx_Light_chaxun_state(g_tZigbeeHAEndDevice_cur->nwkAddr_L,g_tZigbeeHAEndDevice_cur->nwkAddr_H,g_tZigbeeHAEndDevice_cur->endpoint_Addr[0],&g_zigbee_count);
			
		}
		else
		{
			//p_info("TMR_ID_ZIGBEE_HA_CHAXUN_TIMER ��ʼ");
			if( g_tZigbeeHACoordinator.End_Device_nums != 0)
			{
				App_ZIGBEE_HA_address_change_pro();// �����ַ
				App_ZIGBEE_HA_tx_Light_chaxun_state(g_tZigbeeHAEndDevice_cur->nwkAddr_L,g_tZigbeeHAEndDevice_cur->nwkAddr_H,g_tZigbeeHAEndDevice_cur->endpoint_Addr[0],&g_zigbee_count);
			}
		}


		bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_5S); 	/* ����500ms	 ��ʱ */		
			
	}
	/* ���Ͳ�ѯ�����ʱ�ˣ��������������״̬ */
	if(bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
	{
		g_tZigbeeHAEndDevice_cur->flag_on_network = FALSE;
		p_info("flag_on_network ����");
		
	}
	
}
/*
*********************************************************************************************************
*	�� �� ��: App_ZIGBEE_HA_report_socket_state
*	����˵��:    �ϱ�������״̬
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_HA_report_socket_state(void)
{
	
	if(bsp_CheckTimer(TMR_ID_ZIGBEE_HA_report_socke_state))
	{
		bsp_StartTimer(TMR_ID_ZIGBEE_HA_report_socke_state,g_tZigbeeHACoordinator.report_time);
#if 0
		if(App_SocketStatusReport_01() == TRUE)
		{
			APP_STATE = NB_CoAP_ST;   //��������
			p_info("�ϱ�����״̬");
		}
		else
		{
			//p_info("������ͬ�����ϱ�����״̬");
		}					 
#else
	App_SocketStatusReport_01();
	{
		APP_STATE = NB_CoAP_ST;   //��������
		p_info("�ϱ�����״̬");
	}
#endif
	}
	
}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_endpoint_process_loop
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_endpoint_process_loop(void )
{	
	/* �Ѿ����˶˵����˳�*/
	if((g_tZigbeeHAEndDevice1.flag_on_network == TRUE)&&(g_tZigbeeHAEndDevice1.flag_endpoint_is_queryed == FALSE))
	{
		/* ��һ�η��ͣ���ѯ�汾������*/
		if(g_tZigbeeHAEndDevice1.flag_tx == FALSE)
		{
			g_tZigbeeHAEndDevice1.flag_tx = TRUE ; 

			App_ZIGBEE_HA_tx_Network_cmd_Querying_device_endpoint(g_tZigbeeHAEndDevice1.nwkAddr_L ,g_tZigbeeHAEndDevice1.nwkAddr_H,&g_zigbee_count);
			bsp_StartTimer(TMR_ID_ZIGBEE_HA_ENDPOINT_TIMEROUT, 500); 	/* ����500ms	 ��ʱ */		
		}
		
		/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
		if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_ENDPOINT_TIMEROUT))
		{
			g_tZigbeeHAEndDevice1.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
		}		
	}
	else
	{
		g_tZigbeeHAEndDevice1.flag_tx = FALSE;	
		bsp_StopTimer(TMR_ID_ZIGBEE_HA_ENDPOINT_TIMEROUT);// ֹͣ��ʱ��
	}
}


/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_light_switch_process_loop
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_light_switch_process_loop(void )
{	
	/* �Ѿ����˶˵����˳�*/
	//if((g_tZigbeeHAEndDevice1.flag_on_network == TRUE)&&(g_tZigbeeHAEndDevice1.flag_endpoint_is_queryed == TRUE))
	if((g_tZigbeeHAEndDevice1.flag_on_network == TRUE))
	{
		g_tZigbeeHAEndDevice1.count_time++;

		g_tZigbeeHAEndDevice1.on_time = TMR_DELAY_3S;// ��1����
		g_tZigbeeHAEndDevice1.off_time= TMR_DELAY_10S;//Ϩ��3-1=2����
		
		if(g_tZigbeeHAEndDevice1.count_time == g_tZigbeeHAEndDevice1.on_time )
		{
			App_ZIGBEE_HA_tx_Light_cmd_on_off(g_tZigbeeHAEndDevice1.nwkAddr_L,g_tZigbeeHAEndDevice1.nwkAddr_H,g_tZigbeeHAEndDevice1.endpoint_Addr[0],ZIGBEE_HA_Lamp_CMD_on_light,&g_zigbee_count);
		}
		else if(g_tZigbeeHAEndDevice1.count_time == g_tZigbeeHAEndDevice1.off_time )
		{
			App_ZIGBEE_HA_tx_Light_cmd_on_off(g_tZigbeeHAEndDevice1.nwkAddr_L,g_tZigbeeHAEndDevice1.nwkAddr_H,g_tZigbeeHAEndDevice1.endpoint_Addr[0],ZIGBEE_HA_Lamp_CMD_off_light,&g_zigbee_count);
		}
		else if(g_tZigbeeHAEndDevice1.count_time >( g_tZigbeeHAEndDevice1.off_time + g_tZigbeeHAEndDevice1.on_time ) )
		{
			g_tZigbeeHAEndDevice1.count_time = 0;
		}					
	}

}

/**************************************************************************************                      
* ��    �ƣ�App_ZIGBEE_HA_loop
* ��    �ܣ�
* ��ڲ�����
* ���ڲ�������
* ˵    ����
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_ZIGBEE_HA_loop(void )
{	
	/*	*/
	if(g_tZigbeeHACoordinator.start == FALSE)
	{
		return ;
	}
	
	switch(g_tZigbeeHACoordinator.work_state)
	{
		case ZIGBEE_HA_OPEN :
		{				
			comClearRxFifo(ZIGBEE_HA_UART1);//ZIGBEE_HA_UART1
			g_tZigbeeHACoordinator.power_flag = TRUE;
			Bsp_zigbee_power_on();
			
			MX_USART1_UART_Init();
			g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_wait_net_ok;
			p_info("ZIGBEE_HA_OPEN!");
		}
			break;
		case ZIGBEE_HA_wait_net_ok :
		{	
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_System)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_System_CMD_Query_coordinator_firm_ver))
				{
					if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Local_information_return)
					{
						g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_INIT_GetVersion;
						g_tZigbeeHACoordinator.flag_tx = FALSE;
						g_tZigbeeHACoordinator.count_tx = 0;// ++
						bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
						
						p_info("goto  ZIGBEE_HA_INIT_GetVersion");
					}
				}
			}					
		}
			break;		
		case ZIGBEE_HA_INIT_GetVersion :
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				App_ZIGBEE_HA_tx_system_1_3_cmd(ZIGBEE_HA_System_CMD_Query_coordinator_firm_ver,0x00,&g_zigbee_count);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S);		/* ����500ms	 ��ʱ */		
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
				
					p_info("TMR_ID_ZIGBEE_HA_TIMEROUT");
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_System)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_System_CMD_Query_coordinator_firm_ver))
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_INIT_GetPANID;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��

					p_info("goto  ZIGBEE_HA_INIT_GetPANID");
				}
			}
			
			break;
		}

		#if 1
		case ZIGBEE_HA_INIT_GetPANID :
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				App_ZIGBEE_HA_tx_system_4_cmd(ZIGBEE_HA_System_CMD_Conf_coordinator_netw,ZIGBEE_HA_CMD_READ,ZIGBEE_HA_System_CMD_Configuration_PANID,&g_zigbee_count);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S);		/* ����500ms	 ��ʱ */		
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_System)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_System_CMD_Conf_coordinator_netw)
					&&(g_tZigbeeHACoordinator.rx_command_config_id== ZIGBEE_HA_System_CMD_Configuration_PANID))
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_INIT_GetCHANNEL;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
					p_info("goto  ZIGBEE_HA_INIT_GetCHANNEL");					
					
				}
			}
			
			break;
		}		
		case ZIGBEE_HA_INIT_GetCHANNEL :
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				App_ZIGBEE_HA_tx_system_4_cmd(ZIGBEE_HA_System_CMD_Conf_coordinator_netw,ZIGBEE_HA_CMD_READ,ZIGBEE_HA_System_CMD_Configuration_channel,&g_zigbee_count);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S);		/* ����500ms	 ��ʱ */		
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_System)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_System_CMD_Conf_coordinator_netw)
					&&(g_tZigbeeHACoordinator.rx_command_config_id== ZIGBEE_HA_System_CMD_Configuration_channel))
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_INIT_Get_MAC_NET;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
					p_info("goto  ZIGBEE_HA_INIT_Get_MAC_NET");					
					
				}
			}
			
			break;
		}

		case ZIGBEE_HA_INIT_Get_MAC_NET :
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				if( g_tZigbeeHACoordinator.End_Device_nums != 0)
				{
					App_ZIGBEE_HA_tx_Network_cmd_Get_the_nwkAddr_according_MACAddr(&g_tZigbeeHAConfig.macAddrv[End_Device1-1][0],&g_zigbee_count);
					bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S); 	/* ����500ms	 ��ʱ */		
				}	
				else
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_APP_OPEN_NETWORK;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
					p_info("goto  ZIGBEE_HA_APP_OPEN_NETWORK");										
				}
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_Network_Management)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_Network_CMD_Get_the_nwkAddr_according_MACAddr))
				{
					//if(g_tZigbeeHACoordinator.rx_command_subcommand_type == ZIGBEE_HA_SUB_CMD_Remote_wireless_message)
					{
						g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_APP_OPEN_NETWORK;
						g_tZigbeeHACoordinator.flag_tx = FALSE;
						g_tZigbeeHACoordinator.count_tx = 0;// ++
						bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
						p_info("goto  ZIGBEE_HA_APP_OPEN_NETWORK");										
					}
				}
			}
			
			break;
		}		
		#endif
		case ZIGBEE_HA_APP_OPEN_NETWORK :
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				App_ZIGBEE_HA_tx_Network_cmd_Turn_on_off_the_network(ZIGBEE_HA_Turn_on_the_network,&g_zigbee_count);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S);		/* ����500ms	 ��ʱ */		
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_Network_Management)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_Network_CMD_Turn_on_off_the_network))
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_LOOP;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
					
					bsp_StartTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER,g_tZigbeeHACoordinator.loop_time);
					bsp_StartTimer(TMR_ID_ZIGBEE_HA_report_socke_state,g_tZigbeeHACoordinator.report_time);

					g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//��
					bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
					
					p_info("goto  ZIGBEE_HA_LOOP");										
				}
			}			
			break;
		}


		case ZIGBEE_HA_LOOP :
		{		

			/* ������û�п��������ն����С�����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				/* �µ��豸��������*/
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_Network_Management)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_Network_CMD_Device_address_information))
				{
					//g_tZigbeeHACoordinator.End_Device_nums ++;
				}
			}		

		
			if(g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff == TRUE)
			{

				if(bsp_CheckTimer(TMR_ID_ZIGBEE_HA_cmd_socke_state_ask))
				{
					App_SOCKET_STATE_ask( g_tZigbeeHAEndDevice_cmd_ASK,null);// ����������Ӧ
					APP_STATE = NB_CoAP_ST;   //��������
					p_info("App_SOCKET_STATE_ask��������");									
				}
					/* 1s ��ѯ������״̬ */
				App_ZIGBEE_HA_chaxun_socket_state(); // 
				
				/* �ϱ�������״̬ */
				App_ZIGBEE_HA_report_socket_state();
				
				//App_ZIGBEE_HA_endpoint_process_loop();// ��ѯ�˵���Ϣ
			}
			else
			{
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_CHAXUN_TIMER,g_tZigbeeHACoordinator.loop_time);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_report_socke_state,g_tZigbeeHACoordinator.report_time);
			}

			/* 5���ӵ���ʱ�Զ��رգ�*/
			if(bsp_CheckTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;
				
				g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_APP_CLOSE_NETWORK;
				g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = FALSE;
				p_info("�Զ��ر��ϱ�  TMR_ID_ZIGBEE_HA_OFF_TIMER"); 									
			}			
					
			/* �����Ŀ����͹ر�*/
			if(g_tZigbeeHACoordinator.work_type_update == TRUE)
			{
				g_tZigbeeHACoordinator.work_type_update =FALSE;
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_cmd_socke_state_ask,TMR_DELAY_2S);//��ѯ���ϱ� 		
				if(g_tZigbeeHACoordinator.work_type  == SOCKET_OPEN)
				{		
					App_ZIGBEE_HA_tx_Light_cmd_on_off(g_tZigbeeHAEndDevice_cmd_ASK->nwkAddr_L,g_tZigbeeHAEndDevice_cmd_ASK->nwkAddr_H,g_tZigbeeHAEndDevice_cmd_ASK->endpoint_Addr[0],ZIGBEE_HA_Lamp_CMD_on_light,&g_zigbee_count);
					p_info("��%d:SOCKET_OPEN",1);
				}
				else// if(work_type == SOCKET_CLOSE)
				{
					App_ZIGBEE_HA_tx_Light_cmd_on_off(g_tZigbeeHAEndDevice_cmd_ASK->nwkAddr_L,g_tZigbeeHAEndDevice_cmd_ASK->nwkAddr_H,g_tZigbeeHAEndDevice_cmd_ASK->endpoint_Addr[0],ZIGBEE_HA_Lamp_CMD_off_light,&g_zigbee_count);
					p_info("��%d:SOCKET_CLOSE",1);
				}				
			}
		}
			break;
		
		case ZIGBEE_HA_APP_CLOSE_NETWORK:
		{				
			/* ��һ�η��ͣ���ѯ�汾������*/
			if(g_tZigbeeHACoordinator.flag_tx == FALSE)
			{
				g_tZigbeeHACoordinator.flag_tx = TRUE ; 
				g_tZigbeeHACoordinator.count_tx++;// ++
				if(g_tZigbeeHACoordinator.count_tx >= 255)
					g_tZigbeeHACoordinator.count_tx = 255;
				
				App_ZIGBEE_HA_tx_Network_cmd_Turn_on_off_the_network(ZIGBEE_HA_Turn_off_the_network,&g_zigbee_count);
				bsp_StartTimer(TMR_ID_ZIGBEE_HA_TIMEROUT, TMR_DELAY_0_25S);		/* ����500ms	 ��ʱ */		
			}
			
			/* 500ms ��ʱ���ٴη��Ͳ�ѯ����*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_HA_TIMEROUT))
			{
				g_tZigbeeHACoordinator.flag_tx = FALSE;// �ٴη��Ͱ汾�ţ���ѯ����
			}		
			
			/* ����Э��֡ͷ����*/
			if(App_ZIGBEE_HA_rx_data_pro() == ZIGBEE_HA_RX_HEADER)
			{
				if((g_tZigbeeHACoordinator.rx_command_layer == ZIGBEE_HA_CMD_Network_Management)&&(g_tZigbeeHACoordinator.rx_command_subcommand == ZIGBEE_HA_Network_CMD_Turn_on_off_the_network))
				{
					g_tZigbeeHACoordinator.work_state = ZIGBEE_HA_CLOSE;
					g_tZigbeeHACoordinator.flag_tx = FALSE;
					g_tZigbeeHACoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_HA_TIMEROUT);// ֹͣ��ʱ��
					
					p_info("goto  ZIGBEE_HA_CLOSE");					
				}
			}			
			break;
		}

		case ZIGBEE_HA_CLOSE :
		{				
			g_tZigbeeHACoordinator.power_flag = FALSE;
			
			HAL_UART_DeInit(&huart1);
			Bsp_zigbee_power_off();
			
			g_tZigbeeHACoordinator.start = FALSE;						
		}
			break;
				
		default:
			break;
	}
	
}



