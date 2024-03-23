
#ifndef _app_zigbee_HA_
#define _app_zigbee_HA_

#include "app.h"  

//#define ZIGBEE_SET_DATA_GESHU (12*10+2)// 6个插座的参数
#define ZIGBEE_SET_DATA_GESHU (6*10+2)// 6个插座的参数

#define ZIGBEE_MAC_ADDRESS_SIZE 	 0X08  // MAC 8 byte

#define ZIGBEE_HA_RX_HEADER 0X01 // 接收到帧头

#define ZIGBEE_HA_Turn_on_the_network 0xFF //0xFF - 永久打开网络
#define ZIGBEE_HA_Turn_off_the_network 0x00//0x00 - 关闭网络

/*
0x00 - 关灯0x01 - 开灯0x02 - 状态切换
*/
#define ZIGBEE_HA_Lamp_CMD_on_light 0x01 
#define ZIGBEE_HA_Lamp_CMD_off_light 0x00
#define ZIGBEE_HA_Lamp_CMD_switch_status_light 0x02 

/* 定义端口号 */
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
	  小按键的功能：
	 1、装置数量（1-4）
	 2、装置高度（6-15）
	 3、红外灵敏度（1-4）（5000、1万、2万、3万）
	 4、紫外灵敏度（1-4）（5、10、15、20）
	 5、水平补偿值（0 - 999）
	 6、垂直补偿值（0 - 999）
	 7、水平回转角度、（180-360）
*/
typedef struct
{

	u8 macAddrv[End_Device6][ZIGBEE_MAC_ADDRESS_SIZE];// 终端的mac
	u8 nwkAddr_L[End_Device6];// 终端的地址
	u8 nwkAddr_H[End_Device6];// 终端的地址	
	u8 End_Device_nums;
	u8 sum;
	
}
ZIGBEE_CONFIG_STRCUT;



/*	协调器设备的结构体*/
typedef struct  
{	
	uint8_t work_type_update ;//插座的状态更新
	uint8_t work_type ;//插座的状态

	u8 start;// 
	uint8_t flag_tx;       //发送标志
	uint8_t count_tx;       //发送次数
	uint8_t work_state;       //状态
	u8 power_flag;// 
	
	u8 version[3];	//2.0 版本信息：
//	u8 devType;		//2.1 设备类型
	u8 PanIDH;		//2.2 网络ID
	u8 PanIDL;
	u8 channel;		//信道

	u8 nwkAddr0;	//0  2.4 网络地址(不支持，协调器网络地址固定为0x0000)
	u8 nwkAddr1;	// 0
	
//	u8 macAddr[8];// 本机的mac
//	u8 txPower;// 发射功率
//	u8 baudRate;	// 波特率
	
//	u8 End_Device_macAddr[8];// 终端的mac
//	u8 End_Device_nwkAddr[2];

//	u8	End_Device_seqID;
//	u8 End_Device_feedback_state; //反馈（执行结果）:

//	u8 End_Device_offline_nwkAddr[2];// 离线地址
//	u8 Remove_device_notifications_macAddr[8];
//	u8 End_Device_Rssi;
//	u8 groupNum;
//	u8 Group1_L;
//	u8 Group1_H;
	
//	u8 Group2_L;
//	u8 Group2_H;

	u8 rx_command_layer;//层
	u8 rx_command_subcommand;//子命令
	u8 rx_command_subcommand_type;//子命令类型，该字节0x40表示本地信息返回（此时无串口ACK），0x80表示接收到的远端无线消息
	u8 rx_command_config_id;//配置id
	u8 End_Device_nums;

	uint16_t loop_time;
	uint16_t report_time;//上报插座状态的时间
	
	uint8_t	address_socket_cur;//当前的插座地址
	uint8_t	rx_cmd_zigbee_network_onoff;//服务器给的网络开关命令
}
_Coordinator_data_type; 	  

/* ZIGBEE_HA 终端设备的结构体*/
typedef struct  
{	
	u8 macAddr[ZIGBEE_MAC_ADDRESS_SIZE];// 终端的mac
	u8 nwkAddr_L;// 终端的地址
	u8 nwkAddr_H;// 终端的地址
	
	u8 device_type;// 终端的设备类型

	u8 flag_on_network;//入网标志
	u8 flag_endpoint_is_queryed;//端点
	u8 flag_tx;       //发送标志
	u8 socket_state;//插座的状态

	u8 endpoint_num;//端点
	u8 endpoint_Addr[8];//端点的地址

	/*调试用的参数*/
	u16 count_time;// 计数1ms运行一次
	u16 on_time;
	u16 off_time;	
}_End_Device_data_type; 	  

/*	协调器状态机*/
typedef enum 
{
	ZIGBEE_HA_OPEN=1, 
	ZIGBEE_HA_wait_net_ok,
	ZIGBEE_HA_INIT_GetVersion,// 1.1 固件版本查询	
	//ZIGBEE_HA_INIT_GetDEVICETYPE,
	ZIGBEE_HA_INIT_GetPANID,
	ZIGBEE_HA_INIT_GetCHANNEL,// 1.4.2.1读信道
	ZIGBEE_HA_INIT_Get_MAC_NET,// 根据mac获取net
	//ZIGBEE_HA_INIT_GetNET_ADDRESS,
	//ZIGBEE_HA_INIT_GetMAC_ADDRESS,
	//ZIGBEE_HA_INIT_GetTXPOWER,
	ZIGBEE_HA_APP_OPEN_NETWORK, // 应用命令，打开网络
	ZIGBEE_HA_LOOP,
	ZIGBEE_HA_APP_CLOSE_NETWORK, // 应用命令，关闭网络
	ZIGBEE_HA_CLOSE,
	ZIGBEE_HA_MAX,
}ENUM_ZIGBEE_HA;



/* 
	1.系统命令		System command

	序号	层ID	命令ID	功能描述	备注
	1	1	0x80	协调器建立或恢复网络成功	每次上电都会有该消息
														The coordinator establishes or restores the network successfully
	2	1	0x01	查询协调器固件版本号		Query coordinator firmware version number
	3	1	0x02	重启协调器						Restart coordinator
	4	1	0x03	将协调器恢复出厂配置并重启	Restore the coordinator to factory configuration and reboot
	5	1	0x04	配置协调器网络参数	对网络主要参数的读写操作Configuring coordinator network parameters
	6	1	0x05	协调器的备份与恢复	Coordinator backup and recovery
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
	1.系统命令		System command
	5	1	0x04	配置协调器网络参数	对网络主要参数的读写操作Configuring coordinator network parameters
	
	1.4.1.1 配置信道Configuration channel
	1.4.1.2 配置PANID
	1.4.1.3 配置EPID
	1.4.1.3 配置系统时间Configuring system time
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
	2.网络管理命令		Network management command

	序号	层ID	命令ID	功能描述	备注
	1	2	0x01	打开/关闭网络								Turn on/off the network
	2	2	0x02	设备地址信息	设备入网后主动上报macAddr&nwkAddr
										Device address information
	3	2	0x03	查询设备端点信息	可能有多个应用端口
												Querying device endpoint information There may be multiple application ports
	4	2	0x04	查询设备类型及功能信息	基于特定端口的查询
			Query device type and function information				
	5	2	0x05	根据MACAddr 获取设的nwkAddr		Get the set nwkAddr according to MACAddr
	
	6	2	0x06	根据nwkAddr 获取设备的macAddr		Get the device's macAddr according to nwkAddr
	7	2	0x07	绑定命令	zigbee概念上的绑定	binding command 
	8	2	0x08	解绑命令								Unbind command
	9	2	0x09	绑定表查询								binding table query
	10	2	0x0A	切换网络设备信道	若休眠设备支持，需要设备端做一些工作
			Switching network device channel If the sleep device supports it, you need to do some work on the device side.
			
	11	2	0x0B	查询设备相邻表	第三方设备不一定支持
			Query device neighbor table Third-party device does not necessarily support
	12	2	0x0C	查询设备路由表	第三方设备不一定支持
			Query device routing table Third-party devices do not necessarily support
	13	2	0x0D	网络扫描	保留功能		network scan  
	14	2	0x0E	配置远端设备网络参数	第三方设备不一定支持
																				Configuring remote device network parameters
	15	2	0x0F	重启远端设备	第三方设备不一定支持			Restart remote device 
	
	16	2	0x10	配置秘钥											configuration key
	17	2	0x11	更新网络秘钥										update network key
	18	2	0x12	切换网络秘钥										Switch network key
	19	2	0x13	设备在线监测	"最新版本协议支持相关功能；通用做法网关端查询设备是否在线"
																			 Device Online Monitoring
	20	2	0x14	获取设备信号强度	保留功能				Get device signal strength	
	
	21	2	0x15	离网设备信息上报	部分情况下有用		Off-grid device information reporting 	
*/
typedef enum 
{
	ZIGBEE_HA_Network_CMD_Turn_on_off_the_network = 0x01,
	ZIGBEE_HA_Network_CMD_Device_address_information,//设备地址信息
	ZIGBEE_HA_Network_CMD_Querying_device_endpoint_information,//设备端点信息
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
	5.HA灯类标准命令			Lamp standard command		
		序号	层ID	命令ID	功能描述	备注
		1	5	0x01	开/关灯 					 on/off light
		2	5	0x02	调光灯					dimming light
		3	5	0x03	调彩灯					Lantern
		4	5	0x04	调色温灯				color temperature lamp
		
		5	5	0x05	查询灯的开关状态			Query lamp switch status
		6	5	0x06	查询灯的亮度					Query the brightness of the lamp
		7	5	0x07	查询灯的彩度 和 饱和度	Query the chroma and saturation of the lamp
		8	5	0x08	查询灯的色温					Query the color temperature of the lamp
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
帧头：串口帧数据的起始字节，固定为0xFE												
帧长：整帧数据的总长度，长度区间在0x09 - 0x64												
控制命令—层：该命令帧的属于哪一层的命令，												
	    其中：0x01-系统命令； 											
	          0x02-网络管理命令； 											
	          0x03-标准的HA命令； 											
	          0x04-非标准命令   											
控制命令—子命令：对应层中的具体命令，如开灯命令；												
	         其中，该字节0x40表示本地信息返回（此时无串口ACK），
	         0x80表示接收到的远端无线消息											
地址模式：该帧命令的发送方式，												
	其中：0x01-组播； 											
	      0x02-网络地址单播； 											
	      0x03-MAC地址单播； 											
	      0x0F-广播；   											
目标设备网络地址：网络地址的低字节在前，高字节在后，如目标设备网络地址为0x1234，则网络地址_L = 0x34、网络地址_H = 0x12；												
端点：目标设备端点，类似TCP/IP中的端口号；												
帧序号：该命令帧的ID标识；												
数据：包含特定应用的命令数据，如调色灯中的颜色属性数据												
校验字节：前面所有字节的异或校验结果												
*/

/*		帧头：串口帧数据的起始字节，固定为0xFE	*/
#define ZIGBEE_HA_DATA_SOF_CONFIG_Coordinator 0xFE   // 配置功能_协调器
#define ZIGBEE_HA_DATA_SOF_APPLICAITON 0xFE   // 帧头 ，如应用功能帧头为 0xFE
#define ZIGBEE_HA_DATA_LENTH_POS 1

/*		
	帧长：整帧数据的总长度，长度区间在0x09 - 0x64												
*/
#define DataSize_MIN (0X09) 
#define DataSize_MAX (0x64)

/*
	控制命令—层：该命令帧的属于哪一层的命令，												
			其中：0x01-系统命令；	System command										
				  0x02-网络管理命令；	- Network Management Command										
				  0x03-标准的HA命令；Standard HA command;											
				  0x04-非标准命令			Non-standard command
*/
#define ZIGBEE_HA_CMD_System   				0x01		   	
#define ZIGBEE_HA_CMD_Network_Management    0x02		   
#define ZIGBEE_HA_CMD_Standard_HA    		0x03		   
#define ZIGBEE_HA_CMD_No_Standard   		0x04		   
#define ZIGBEE_HA_CMD_Lamp   				0x05 // 灯的命令		   

/*
	控制命令—子命令：对应层中的具体命令，如开灯命令；												
				 其中，该字节0x40表示本地信息返回（此时无串口ACK），Local information return
				 0x80表示接收到的远端无线消息		Remote wireless message									
*/
#define ZIGBEE_HA_SUB_CMD_Local_information_return   0x40		   	
#define ZIGBEE_HA_SUB_CMD_Remote_wireless_message    0x80	

/*
	地址模式：该帧命令的发送方式，												
		其中：
			  0x01-组播；						Multicast					
			  0x02-网络地址单播；		Network address unicast									
			  0x03-MAC地址单播；			MAC address unicast;								
			  0x0F-广播；						broadcast	
*/
#define ZIGBEE_HA_AddrMode_Multicast 				0x01
#define ZIGBEE_HA_AddrMode_Network_address_unicast  0x02 
#define ZIGBEE_HA_AddrMode_MAC_address_unicast  	0x03 
#define ZIGBEE_HA_AddrMode_Boardcast 				0x0F    


/*
	状态编码:0x00-success 0x01-fail 0x02-???
*/
#define ZIGBEE_HA_FEEDBACK_success 	 0X00  // 反馈执行结果ok
#define ZIGBEE_HA_FEEDBACK_fail 	 0X01  // 反馈执行结果ok

extern u8 g_zigbee_count ;
extern _End_Device_data_type g_tZigbeeHAEndDevice1;// 终端1
extern _End_Device_data_type g_tZigbeeHAEndDevice2;// 终端2
extern _End_Device_data_type g_tZigbeeHAEndDevice3;// 终端3
extern _End_Device_data_type g_tZigbeeHAEndDevice4;// 终端4
extern _End_Device_data_type g_tZigbeeHAEndDevice5;// 终端3
extern _End_Device_data_type g_tZigbeeHAEndDevice6;// 终端4
extern _End_Device_data_type * g_tZigbeeHAEndDevice_cur;// 当前的终端
extern _End_Device_data_type * g_tZigbeeHAEndDevice_cmd_ASK;// 命令回答的终端
extern u8	g_tZigbeeHAEndDevice_cmd_ASK_FLAG;// 命令回答的终端

extern _Coordinator_data_type g_tZigbeeHACoordinator;// 协调器 Coordinator

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
