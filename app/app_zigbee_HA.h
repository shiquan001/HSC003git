
#ifndef _app_zigbee_HA_
#define _app_zigbee_HA_

#include "app.h"  

//#define ZIGBEE_SET_DATA_GESHU (12*10+2)// 6�������Ĳ���
#define ZIGBEE_SET_DATA_GESHU (6*10+2)// 6�������Ĳ���

#define ZIGBEE_MAC_ADDRESS_SIZE 	 0X08  // MAC 8 byte

#define ZIGBEE_HA_RX_HEADER 0X01 // ���յ�֡ͷ

#define ZIGBEE_HA_Turn_on_the_network 0xFF //0xFF - ���ô�����
#define ZIGBEE_HA_Turn_off_the_network 0x00//0x00 - �ر�����

/*
0x00 - �ص�0x01 - ����0x02 - ״̬�л�
*/
#define ZIGBEE_HA_Lamp_CMD_on_light 0x01 
#define ZIGBEE_HA_Lamp_CMD_off_light 0x00
#define ZIGBEE_HA_Lamp_CMD_switch_status_light 0x02 

/* ����˿ں� */
typedef enum
{
	End_Device1 = 1,	/* USART1  PA9, PA10 */
	End_Device2,
	End_Device3,
	End_Device4,
	End_Device5,
	End_Device6,
	End_Device7,
	End_Device8,
	End_Device9,
	End_Device10,
	End_Device11,	
	End_Device12,			
}End_Device_E;
	

/*
	  С�����Ĺ��ܣ�
	 1��װ��������1-4��
	 2��װ�ø߶ȣ�6-15��
	 3�����������ȣ�1-4����5000��1��2��3��
	 4�����������ȣ�1-4����5��10��15��20��
	 5��ˮƽ����ֵ��0 - 999��
	 6����ֱ����ֵ��0 - 999��
	 7��ˮƽ��ת�Ƕȡ���180-360��
*/
typedef struct
{

	u8 macAddrv[End_Device6][ZIGBEE_MAC_ADDRESS_SIZE];// �ն˵�mac
	u8 nwkAddr_L[End_Device6];// �ն˵ĵ�ַ
	u8 nwkAddr_H[End_Device6];// �ն˵ĵ�ַ	
	u8 End_Device_nums;
	u8 sum;
	
}
ZIGBEE_CONFIG_STRCUT;



/*	Э�����豸�Ľṹ��*/
typedef struct  
{	
	uint8_t work_type_update ;//������״̬����
	uint8_t work_type ;//������״̬

	u8 start;// 
	uint8_t flag_tx;       //���ͱ�־
	uint8_t count_tx;       //���ʹ���
	uint8_t work_state;       //״̬
	u8 power_flag;// 
	
	u8 version[3];	//2.0 �汾��Ϣ��
//	u8 devType;		//2.1 �豸����
	u8 PanIDH;		//2.2 ����ID
	u8 PanIDL;
	u8 channel;		//�ŵ�

	u8 nwkAddr0;	//0  2.4 �����ַ(��֧�֣�Э���������ַ�̶�Ϊ0x0000)
	u8 nwkAddr1;	// 0
	
//	u8 macAddr[8];// ������mac
//	u8 txPower;// ���书��
//	u8 baudRate;	// ������
	
//	u8 End_Device_macAddr[8];// �ն˵�mac
//	u8 End_Device_nwkAddr[2];

//	u8	End_Device_seqID;
//	u8 End_Device_feedback_state; //������ִ�н����:

//	u8 End_Device_offline_nwkAddr[2];// ���ߵ�ַ
//	u8 Remove_device_notifications_macAddr[8];
//	u8 End_Device_Rssi;
//	u8 groupNum;
//	u8 Group1_L;
//	u8 Group1_H;
	
//	u8 Group2_L;
//	u8 Group2_H;

	u8 rx_command_layer;//��
	u8 rx_command_subcommand;//������
	u8 rx_command_subcommand_type;//���������ͣ����ֽ�0x40��ʾ������Ϣ���أ���ʱ�޴���ACK����0x80��ʾ���յ���Զ��������Ϣ
	u8 rx_command_config_id;//����id
	u8 End_Device_nums;

	uint16_t loop_time;
	uint16_t report_time;//�ϱ�����״̬��ʱ��
	
	uint8_t	address_socket_cur;//��ǰ�Ĳ�����ַ
	uint8_t	rx_cmd_zigbee_network_onoff;//�������������翪������
}
_Coordinator_data_type; 	  

/* ZIGBEE_HA �ն��豸�Ľṹ��*/
typedef struct  
{	
	u8 macAddr[ZIGBEE_MAC_ADDRESS_SIZE];// �ն˵�mac
	u8 nwkAddr_L;// �ն˵ĵ�ַ
	u8 nwkAddr_H;// �ն˵ĵ�ַ
	
	u8 device_type;// �ն˵��豸����

	u8 flag_on_network;//������־
	u8 flag_endpoint_is_queryed;//�˵�
	u8 flag_tx;       //���ͱ�־
	u8 socket_state;//������״̬

	u8 endpoint_num;//�˵�
	u8 endpoint_Addr[8];//�˵�ĵ�ַ

	/*�����õĲ���*/
	u16 count_time;// ����1ms����һ��
	u16 on_time;
	u16 off_time;	
}_End_Device_data_type; 	  

/*	Э����״̬��*/
typedef enum 
{
	ZIGBEE_HA_OPEN=1, 
	ZIGBEE_HA_wait_net_ok,
	ZIGBEE_HA_INIT_GetVersion,// 1.1 �̼��汾��ѯ	
	//ZIGBEE_HA_INIT_GetDEVICETYPE,
	ZIGBEE_HA_INIT_GetPANID,
	ZIGBEE_HA_INIT_GetCHANNEL,// 1.4.2.1���ŵ�
	ZIGBEE_HA_INIT_Get_MAC_NET,// ����mac��ȡnet
	//ZIGBEE_HA_INIT_GetNET_ADDRESS,
	//ZIGBEE_HA_INIT_GetMAC_ADDRESS,
	//ZIGBEE_HA_INIT_GetTXPOWER,
	ZIGBEE_HA_APP_OPEN_NETWORK, // Ӧ�����������
	ZIGBEE_HA_LOOP,
	ZIGBEE_HA_APP_CLOSE_NETWORK, // Ӧ������ر�����
	ZIGBEE_HA_CLOSE,
	ZIGBEE_HA_MAX,
}ENUM_ZIGBEE_HA;



/* 
	1.ϵͳ����		System command

	���	��ID	����ID	��������	��ע
	1	1	0x80	Э����������ָ�����ɹ�	ÿ���ϵ綼���и���Ϣ
														The coordinator establishes or restores the network successfully
	2	1	0x01	��ѯЭ�����̼��汾��		Query coordinator firmware version number
	3	1	0x02	����Э����						Restart coordinator
	4	1	0x03	��Э�����ָ��������ò�����	Restore the coordinator to factory configuration and reboot
	5	1	0x04	����Э�����������	��������Ҫ�����Ķ�д����Configuring coordinator network parameters
	6	1	0x05	Э�����ı�����ָ�	Coordinator backup and recovery
*/
typedef enum 
{
	ZIGBEE_HA_System_CMD_Query_coordinator_firm_ver = 0x01,
	ZIGBEE_HA_System_CMD_Restart_coordinator ,
	ZIGBEE_HA_System_CMD_Restore_the_coordinator_to,
	ZIGBEE_HA_System_CMD_Conf_coordinator_netw,
	ZIGBEE_HA_System_CMD_Coordinator_backup_and_recovery,
	ZIGBEE_HA_System_CMD_MAX,
	ZIGBEE_HA_System_CMD_rest_network_succ= 0x80,	
}ENUM_ZIGBEE_HA_System_CMD_TYPE;

//==========R|W===========//
#define ZIGBEE_HA_CMD_WRITE 0x01
#define ZIGBEE_HA_CMD_READ  0x00

/* 
	1.ϵͳ����		System command
	5	1	0x04	����Э�����������	��������Ҫ�����Ķ�д����Configuring coordinator network parameters
	
	1.4.1.1 �����ŵ�Configuration channel
	1.4.1.2 ����PANID
	1.4.1.3 ����EPID
	1.4.1.3 ����ϵͳʱ��Configuring system time
*/

typedef enum 
{
	ZIGBEE_HA_System_CMD_Configuration_channel= 0x01,
	ZIGBEE_HA_System_CMD_Configuration_PANID ,
	ZIGBEE_HA_System_CMD_Configuration_EPID,
	ZIGBEE_HA_System_CMD_Configuring_system_time,
	ZIGBEE_HA_System_CMD_Configuring_MAX,
}ENUM_ZIGBEE_HA_System_CMD_Configuring_TYPE;



/* 
	2.�����������		Network management command

	���	��ID	����ID	��������	��ע
	1	2	0x01	��/�ر�����								Turn on/off the network
	2	2	0x02	�豸��ַ��Ϣ	�豸�����������ϱ�macAddr&nwkAddr
										Device address information
	3	2	0x03	��ѯ�豸�˵���Ϣ	�����ж��Ӧ�ö˿�
												Querying device endpoint information There may be multiple application ports
	4	2	0x04	��ѯ�豸���ͼ�������Ϣ	�����ض��˿ڵĲ�ѯ
			Query device type and function information				
	5	2	0x05	����MACAddr ��ȡ���nwkAddr		Get the set nwkAddr according to MACAddr
	
	6	2	0x06	����nwkAddr ��ȡ�豸��macAddr		Get the device's macAddr according to nwkAddr
	7	2	0x07	������	zigbee�����ϵİ�	binding command 
	8	2	0x08	�������								Unbind command
	9	2	0x09	�󶨱��ѯ								binding table query
	10	2	0x0A	�л������豸�ŵ�	�������豸֧�֣���Ҫ�豸����һЩ����
			Switching network device channel If the sleep device supports it, you need to do some work on the device side.
			
	11	2	0x0B	��ѯ�豸���ڱ�	�������豸��һ��֧��
			Query device neighbor table Third-party device does not necessarily support
	12	2	0x0C	��ѯ�豸·�ɱ�	�������豸��һ��֧��
			Query device routing table Third-party devices do not necessarily support
	13	2	0x0D	����ɨ��	��������		network scan  
	14	2	0x0E	����Զ���豸�������	�������豸��һ��֧��
																				Configuring remote device network parameters
	15	2	0x0F	����Զ���豸	�������豸��һ��֧��			Restart remote device 
	
	16	2	0x10	������Կ											configuration key
	17	2	0x11	����������Կ										update network key
	18	2	0x12	�л�������Կ										Switch network key
	19	2	0x13	�豸���߼��	"���°汾Э��֧����ع��ܣ�ͨ���������ض˲�ѯ�豸�Ƿ�����"
																			 Device Online Monitoring
	20	2	0x14	��ȡ�豸�ź�ǿ��	��������				Get device signal strength	
	
	21	2	0x15	�����豸��Ϣ�ϱ�	�������������		Off-grid device information reporting 	
*/
typedef enum 
{
	ZIGBEE_HA_Network_CMD_Turn_on_off_the_network = 0x01,
	ZIGBEE_HA_Network_CMD_Device_address_information,//�豸��ַ��Ϣ
	ZIGBEE_HA_Network_CMD_Querying_device_endpoint_information,//�豸�˵���Ϣ
	ZIGBEE_HA_Network_CMD_Query_device_type_and_function_information,	
	ZIGBEE_HA_Network_CMD_Get_the_nwkAddr_according_MACAddr,
	
	ZIGBEE_HA_Network_CMD_Get_the_MACAddr_according_nwkAddr,
	ZIGBEE_HA_Network_CMD_binding,
	ZIGBEE_HA_Network_CMD_Unbind,
	ZIGBEE_HA_Network_CMD_binding_table_query,
	ZIGBEE_HA_Network_CMD_Switching_network_device_channel ,

	ZIGBEE_HA_Network_CMD_Query_device_neighbor_table,
	ZIGBEE_HA_Network_CMD_Query_device_routing_table,
	ZIGBEE_HA_Network_CMD_network_scan,
	ZIGBEE_HA_Network_CMD_Configuring_remote_device_network_parameters,
	ZIGBEE_HA_Network_CMD_Restart_remote_device ,

	ZIGBEE_HA_Network_CMD_configuration_key,
	ZIGBEE_HA_Network_CMD_update_network_key,
	ZIGBEE_HA_Network_CMD_Switch_network_key,
	ZIGBEE_HA_Network_CMD_Device_Online_Monitoring,
	ZIGBEE_HA_Network_CMD_Get_device_signal_strength	 ,
	
	ZIGBEE_HA_Network_CMD_Off_grid_device_information_reporting  ,

	ZIGBEE_HA_Network_CMD_MAX,
}
ENUM_ZIGBEE_HA_Network_CMD_TYPE;

/* 
	5.HA�����׼����			Lamp standard command		
		���	��ID	����ID	��������	��ע
		1	5	0x01	��/�ص� 					 on/off light
		2	5	0x02	�����					dimming light
		3	5	0x03	���ʵ�					Lantern
		4	5	0x04	��ɫ�µ�				color temperature lamp
		
		5	5	0x05	��ѯ�ƵĿ���״̬			Query lamp switch status
		6	5	0x06	��ѯ�Ƶ�����					Query the brightness of the lamp
		7	5	0x07	��ѯ�ƵĲʶ� �� ���Ͷ�	Query the chroma and saturation of the lamp
		8	5	0x08	��ѯ�Ƶ�ɫ��					Query the color temperature of the lamp
*/
typedef enum 
{
	ZIGBEE_HA_Lamp_CMD_on_off_light = 0x01,
	ZIGBEE_HA_Lamp_CMD_dimming_light ,
	ZIGBEE_HA_Lamp_CMD_Lantern,
	ZIGBEE_HA_Lamp_CMD_color_temperature_lamp,
	
	ZIGBEE_HA_Lamp_CMD_Query_lamp_switch_status,
	ZIGBEE_HA_Lamp_CMD_Query_the_brightness_of_the_lamp,
	ZIGBEE_HA_Lamp_CMD_Query_the_chroma_and_saturation_of_the_lamp,
	ZIGBEE_HA_Lamp_CMD_Query_the_color_temperature_of_the_lamp,
}ENUM_ZIGBEE_HA_Lamp_CMD_Configuring_TYPE;


/*
֡ͷ������֡���ݵ���ʼ�ֽڣ��̶�Ϊ0xFE												
֡������֡���ݵ��ܳ��ȣ�����������0x09 - 0x64												
��������㣺������֡��������һ������												
	    ���У�0x01-ϵͳ��� 											
	          0x02-���������� 											
	          0x03-��׼��HA��� 											
	          0x04-�Ǳ�׼����   											
��������������Ӧ���еľ�������翪�����												
	         ���У����ֽ�0x40��ʾ������Ϣ���أ���ʱ�޴���ACK����
	         0x80��ʾ���յ���Զ��������Ϣ											
��ַģʽ����֡����ķ��ͷ�ʽ��												
	���У�0x01-�鲥�� 											
	      0x02-�����ַ������ 											
	      0x03-MAC��ַ������ 											
	      0x0F-�㲥��   											
Ŀ���豸�����ַ�������ַ�ĵ��ֽ���ǰ�����ֽ��ں���Ŀ���豸�����ַΪ0x1234���������ַ_L = 0x34�������ַ_H = 0x12��												
�˵㣺Ŀ���豸�˵㣬����TCP/IP�еĶ˿ںţ�												
֡��ţ�������֡��ID��ʶ��												
���ݣ������ض�Ӧ�õ��������ݣ����ɫ���е���ɫ��������												
У���ֽڣ�ǰ�������ֽڵ����У����												
*/

/*		֡ͷ������֡���ݵ���ʼ�ֽڣ��̶�Ϊ0xFE	*/
#define ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator 0xFE   // ���ù���_Э����
#define ZIGBEE_HA_DATA_SOF_APPLICAITON 0xFE   // ֡ͷ ����Ӧ�ù���֡ͷΪ 0xFE
#define ZIGBEE_HA_DATA_LENTH_POS 1

/*		
	֡������֡���ݵ��ܳ��ȣ�����������0x09 - 0x64												
*/
#define DataSize_MIN (0X09) 
#define DataSize_MAX (0x64)

/*
	��������㣺������֡��������һ������												
			���У�0x01-ϵͳ���	System command										
				  0x02-����������	- Network Management Command										
				  0x03-��׼��HA���Standard HA command;											
				  0x04-�Ǳ�׼����			Non-standard command
*/
#define ZIGBEE_HA_CMD_System   				0x01		   	
#define ZIGBEE_HA_CMD_Network_Management    0x02		   
#define ZIGBEE_HA_CMD_Standard_HA    		0x03		   
#define ZIGBEE_HA_CMD_No_Standard   		0x04		   
#define ZIGBEE_HA_CMD_Lamp   				0x05 // �Ƶ�����		   

/*
	��������������Ӧ���еľ�������翪�����												
				 ���У����ֽ�0x40��ʾ������Ϣ���أ���ʱ�޴���ACK����Local information return
				 0x80��ʾ���յ���Զ��������Ϣ		Remote wireless message									
*/
#define ZIGBEE_HA_SUB_CMD_Local_information_return   0x40		   	
#define ZIGBEE_HA_SUB_CMD_Remote_wireless_message    0x80	

/*
	��ַģʽ����֡����ķ��ͷ�ʽ��												
		���У�
			  0x01-�鲥��						Multicast					
			  0x02-�����ַ������		Network address unicast									
			  0x03-MAC��ַ������			MAC address unicast;								
			  0x0F-�㲥��						broadcast	
*/
#define ZIGBEE_HA_AddrMode_Multicast 				0x01
#define ZIGBEE_HA_AddrMode_Network_address_unicast  0x02 
#define ZIGBEE_HA_AddrMode_MAC_address_unicast  	0x03 
#define ZIGBEE_HA_AddrMode_Boardcast 				0x0F    


/*
	״̬����:0x00-success 0x01-fail 0x02-???
*/
#define ZIGBEE_HA_FEEDBACK_success 	 0X00  // ����ִ�н��ok
#define ZIGBEE_HA_FEEDBACK_fail 	 0X01  // ����ִ�н��ok

extern u8 g_zigbee_count ;
extern _End_Device_data_type g_tZigbeeHAEndDevice1;// �ն�1
extern _End_Device_data_type g_tZigbeeHAEndDevice2;// �ն�2
extern _End_Device_data_type g_tZigbeeHAEndDevice3;// �ն�3
extern _End_Device_data_type g_tZigbeeHAEndDevice4;// �ն�4
extern _End_Device_data_type g_tZigbeeHAEndDevice5;// �ն�3
extern _End_Device_data_type g_tZigbeeHAEndDevice6;// �ն�4
extern _End_Device_data_type * g_tZigbeeHAEndDevice_cur;// ��ǰ���ն�
extern _End_Device_data_type * g_tZigbeeHAEndDevice_cmd_ASK;// ����ش���ն�
extern u8	g_tZigbeeHAEndDevice_cmd_ASK_FLAG;// ����ش���ն�

extern _Coordinator_data_type g_tZigbeeHACoordinator;// Э���� Coordinator

extern void App_ZIGBEE_HA_Config_Save_Into_Flash(void);
extern void App_ZIGBEE_HA_Config_read_from_flash(void);

extern void App_ZIGBEE_HA_Init_Var(void);

extern void App_ZIGBEE_HA_loop(void );

extern void App_ZIGBEE_HA_tx_system_1_3_cmd(u8  sub_cmd ,u8 config_id,u8 * zigbee_count);

extern void App_ZIGBEE_HA_tx_system_4_cmd(u8  sub_cmd ,u8 read_write,u8 config_id,u8 * zigbee_count);
extern void App_ZIGBEE_HA_tx_Network_cmd_Turn_on_off_the_network(u8  on_off_the_network,u8 * zigbee_count);
extern uint8_t App_ZIGBEE_HA_rx_data_pro_Network_CMD(uint8_t Control_command_subcommand,uint16_t cur_pos);
extern void App_ZIGBEE_HA_tx_Network_cmd_Querying_device_endpoint(u8 nwkAddr_L,u8 nwkAddr_H,u8 * zigbee_count);
extern uint8_t App_ZIGBEE_HA_rx_data_pro_CMD_System(uint8_t Control_command_subcommand,uint16_t cur_pos);
extern void App_ZIGBEE_HA_tx_Light_cmd_on_off(u8 nwkAddr_L,u8 nwkAddr_H,u8 end_ponit,u8 on_off,u8 * zigbee_count);
extern void App_ZIGBEE_HA_light_switch_process_loop(void );
extern void App_ZIGBEE_HA_tx_Light_chaxun_state(u8 nwkAddr_L,u8 nwkAddr_H,u8 end_ponit,u8 * zigbee_count);
extern uint8_t App_ZIGBEE_HA_rx_data_pro_Lamp_CMD(uint8_t Control_command_subcommand,uint16_t cur_pos);
extern _End_Device_data_type *EndDeviceNums_To_EndDevice(uint8_t DeviceNums);
extern void App_ZIGBEE_HA_tx_Network_cmd_Get_the_nwkAddr_according_MACAddr(u8 *mac,u8 * zigbee_count);

#endif
