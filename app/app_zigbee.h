
#ifndef _app_zigbee__
#define _app_zigbee__

#include "app.h"  
#if 0
#define DataSize_MAX (80+2) // 当次发送数据的总长度（当前版本 单帧数据透传的最大个数为80）

typedef enum 
{
	ZIGBEE_INIT_GetVersion=1,// 配置命令
	ZIGBEE_INIT_GetDEVICETYPE,
	ZIGBEE_INIT_GetPANID,
	ZIGBEE_INIT_GetCHANNEL,
	ZIGBEE_INIT_GetNET_ADDRESS,
	ZIGBEE_INIT_GetMAC_ADDRESS,
	ZIGBEE_INIT_GetTXPOWER,

	ZIGBEE_APP_OPEN_NETWORK, // 应用命令，打开网络
	ZIGBEE_LOOP,
		
	ZIGBEE_MAX,
}ENUM_ZIGBEE;


typedef struct 
{
	u8	DATALENGTH;					// 数据长度
	u8	DATABUF[DataSize_MAX]; 			// 数组
}DATA_BUF;

//----------设备的结构体
typedef struct  
{	

	uint8_t flag_tx;       //发送标志
	uint8_t count_tx;       //发送次数
	uint8_t work_state;       //状态
	
	u8 version[3];	//2.0 版本信息：
	u8 devType;		//2.1 设备类型
	u8 PanIDH;		//2.2 网络ID
	u8 PanIDL;
	u8 channel;		//信道

	u8 nwkAddr0;	//0  2.4 网络地址(不支持，协调器网络地址固定为0x0000)
	u8 nwkAddr1;	// 0
	
	u8 macAddr[8];// 本机的mac
	u8 txPower;// 发射功率
	u8 baudRate;	// 波特率
	
	u8 End_Device_macAddr[8];// 终端的mac
	u8 End_Device_nwkAddr[2];

	u8	End_Device_seqID;
	u8 End_Device_feedback_state; //反馈（执行结果）:

	u8 End_Device_offline_nwkAddr[2];// 离线地址
	u8 Remove_device_notifications_macAddr[8];
	u8 End_Device_Rssi;
	u8 groupNum;
	u8 Group1_L;
	u8 Group1_H;
	
	u8 Group2_L;
	u8 Group2_H;	
}
_Coordinator_data_type; 	  

/* zigbee 终端设备的结构体*/
typedef struct  
{	
	u8 macAddr[8];// 终端的mac
	u8 nwkAddr[2];

	u8 Remove_device_notifications_macAddr[8];
	u8 Rssi;
}_end_device_data_type; 	  

//==========R|W===========//
#define ZIGBEE_CMD_WRITE 0x01
#define ZIGBEE_CMD_READ  0x00


/* 
													2.协调器配置命令
配置ID	描述
0	查询版本号
1	配置设备类型
2	配置PANID
3	配置信道
4	配置网络地址
5	配置MAC地址
6	配置发送功率
7	配置波特率
8	恢复出厂配置并重启
9	重启设备
10	协调器的备份与恢复
*/
typedef enum 
{
	ZIGBEE_CMD_VERSION =0x00,
	ZIGBEE_CMD_CONFIG_DEVICE_TYPE,
	ZIGBEE_CMD_CONFIG_PANID,
	ZIGBEE_CMD_CONFIG_CHANNEL,
	ZIGBEE_CMD_CONFIG_NET_ADDRESS,//		2.4 网络地址(不支持，协调器网络地址固定为0x0000)
	ZIGBEE_CMD_CONFIG_MAC_ADDRESS,
	ZIGBEE_CMD_CONFIG_TXPOWER,
	ZIGBEE_CMD_CONFIG_BAUDRATE,
	ZIGBEE_CMD_CONFIG_REFACTORY,
	ZIGBEE_CMD_CONFIG_RESTART,
	ZIGBEE_CMD_CONFIG_BACKUP_RECOVERY,		
	ZIGBEE_CMD_MAX,
}ENUM_ZIGBEE_CMD_TYPE;

/* 
													1.应用命令
序号	命令ID	功能描述												备注
0	0	入网新设备信息													网络地址、mac地址、设备类型
		根据Mac地址查询网络地址	
		根据网络地址查询mac地址	
1	1	串口数据透传	
2	2	不在线设备信息通知											设备在线监测功能
3	3	打开/关闭网络	
4	4	恢复远端设备至默认配置状态	
5	5	重启远端设备														网络节点设备收到该命令后，30秒内进行重启动作
6	6	移除远端设备														将目标节点设备从网络中删除
7	7	网络中的子节点设备离网通知	
8	8	已入网设备查询自身父设备的信号强度	
9	9	协调器查询入网设备的信号强度	
10	10	添加组	
11	11	删除组	
12	12	查询组	
*/
typedef enum 
{
	ZIGBEE_APPLICAITON_CMD_Device_Information = 0x00,//设备信息Device Information// 0x00： 表示 查询指定mac地址设备的网络地址；
	ZIGBEE_APPLICAITON_CMD_transparent_data = 0x01,// 串口透传数据Serial port transparent data // 0x01：表示 进行串口透传功能
	
	ZIGBEE_APPLICAITON_CMD_Offline_device_notification,//离线设备通知Offline device notification
	ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network,//打开/关闭网络Turn off the network
	ZIGBEE_APPLICAITON_CMD_Remote_factory_reset,//		远程恢复出厂设置Remote factory reset
	ZIGBEE_APPLICAITON_CMD_Remote_restart_command,//远程重启命令Remote restart command
	ZIGBEE_APPLICAITON_CMD_Remove_device,//2.2.6 移除设备Remove device
	ZIGBEE_APPLICAITON_CMD_Remove_device_notifications,//2.2.7 移除设备通知Remove device notifications
	ZIGBEE_APPLICAITON_CMD_Query_signal_quality_of_the_local_device,//2.2.8 查询已入网设备本地的信号质量（仅路由设备和终端设备使用的命令）Query the signal quality of the local device?

	ZIGBEE_APPLICAITON_CMD_The_coordinator_queries_the_signal_quality_of_the_remote,//2.2.9 协调器查询远端设备的信号质量The coordinator queries the signal quality of the remote device
	ZIGBEE_APPLICAITON_CMD_Add_group,		//2.2.10 添加组Add group
	ZIGBEE_APPLICAITON_CMD_Delete_group,//2.2.11 删除组Delete group
	ZIGBEE_APPLICAITON_CMD_Query_group,//2.2.12 查询组Query group
	
	ZIGBEE_APPLICAITON_CMD_MAX,
}ENUM_ZIGBEE_APPLICAITON_CMD_TYPE;
#define FEEDBACK_OK 0X00  // 反馈执行结果ok

/*
						application 2.1 帧格式说明								
请求：									
SOF	LEN	CMD	AddrMode	nwkAddr0	nwkAddr1	CMD	seqID	Data0	Data1	Data2
0xFE	0x09	0x01	0x02	0x34	0x12	0x01	0xFE	0x31	0x32	0x33

SOF：	帧头 ，如应用功能帧头为 0xFE						
LEN：	当次发送数据的总长度（当前版本 单帧数据透传的最大个数为80）		

CMD：	应用功能选择命令；							
	0x00： 表示 查询指定mac地址设备的网络地址；				
	0x01：表示 进行串口透传功能	
	
Mode：	数据发送方式：							
	0x01 - 组播方式，网络地址为目标组Id					
	0x02 - 单播方式								
	0x0F - 广播方式，广播网络地址为0xFFFF			
	
nwkAddr：网络地址，共2个字节，低字节在前，如示例中所示网络地址为0x1234	
seqID：	时间戳计数，用户产生，主要用于区分重复数据包，建议每发送一次数据累加1	
Data：	透传的用户自定义数据	
*/

/*		配置功能_协调器   寄存		*/
#define ZIGBEE_DATA_SOF_CONFIG_Coordinator 0xFC   // 配置功能_协调器
#define ZIGBEE_DATA_SOF_APPLICAITON 0xFE   // 帧头 ，如应用功能帧头为 0xFE
#define ZIGBEE_DATA_LENTH_POS 1

/*
	CMD：  应用功能选择命令；	   
	0x00： 表示 查询指定mac地址设备的网络地址； 	   
	0x01：表示 进行串口透传功能 	   
*/
#define ZIGBEE_CMDtoModule 0x00//0x00： 表示 查询指定mac地址设备的网络地址； 		   
#define ZIGBEE_DATAtoModule 0x01//0x01：表示 进行串口透传功能    

/*
	Mode：	数据发送方式：		
	0x01 - 组播方式，网络地址为目标组Id		
	0x02 - 单播方式		
	0x0F - 广播方式，广播网络地址为0xFFFF		
*/
#define ZIGBEE_AddrMode_Multicast 0x01
#define ZIGBEE_AddrMode_Signacast 0x02 
#define ZIGBEE_AddrMode_03  0x03 

#define ZIGBEE_AddrMode_Boardcast 0x0F    


void App_zigbee_Init_Var(void);
void App_zigbee_tx_cmd(u8  cmd );

void App_zigbee_config_read_cmd(u8  cmd );
void App_zigbee_loop(void );
void App_zigbee_app_TurnOn_Off_network_cmd(u8  cmd ,u8 AddrMode,u8 OnOffnetwork);
#endif
#endif
