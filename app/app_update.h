#ifndef _APP_UPDATE_H_
#define _APP_UPDATE_H_


#define SPI_FLASH_UPDATE_NEEDED 0x5aa5
#define SPI_FLASH_UPDATE_IS_OK 0xAAAA
#define SPI_FLASH_UPDATE_IS_init 0xFFFF

/*
spi flash  数据的分布图

第1页		0开始 512开始

第2页		1024开始
第3页		1024开始
第4页		1024开始

*/
#define SPI_FLASH_EACH_PAGE_SIZE 1024

#define SPI_FLASH_START_ADDR_VAR 0 // 升级参数的存储地址
#define SPI_FLASH_onoff_ADDR_VAR 256 // 调试参数的存储地址；g_Indicat；复位原因

/*  spi flash address*/
#define SPI_FLASH_ZIGBEE_ADDR_VAR 512 // ZIGBEE参数的存储地址

#define SPI_FLASH_code_ADDR  1024 // 升级程序的存储地址；代码bin的位置

/*
上报当前软件版本号
Report_current_software_version_number.
新版本通知
New_version_notification
请求升级包
Request_upgrade_package
上报升级包下载状态
Escalation_package_download_status
执行升级
Perform_an_upgrade
上报升级结果
Report_upgrade_results
*/

typedef enum 
{
	UPDATE_INIT=0,// 初始化态
	UPDATE_Report_current_software_version_number,
	UPDATE_New_version_notification,
	UPDATE_Request_upgrade_package,
	UPDATE_Escalation_package_download_status,
	UPDATE_Perform_an_upgrade,
	UPDATE_Report_upgrade_results,
	
	UPDATE_WAIT_START,
	UPDATE_SEND_CMD,
	UPDATE_WAIT_ACK,
	UPDATE_OK_RESTART,
		
	UPDATE_MAX,
}ENUM_UPDATE;


/*
远程升级	
程序升级状态				0x93	1	0-2	0—未升级  1—升级   2--升级中
服务器最新的固件版本	0x94	3		
固件版本的大小			0x95	4		固件程序的大小，多少字节数
每一包固件程序的大小	0x96	2		每次传送固定的大小，例如500字节。
固件升级开始命令		0x97	1	0-1	0—停止升级命令 
						1—开始升级命令   
*/
typedef struct
{
	u16 safe_flag;// 0x5a 0xa5

	//u32 version_size;//固件版本的大小		
	
	u16 data_packet_total;//总包数 500的数量
	u16 data_packet_cur; //当前包
	u16 data_packet_size; //每包的大小 500
	
	u16 data_packet_total_crc; //总包的crc

	u8  update_workstate;//程序升级工作状态
	
	u8  rx_data_ok;//程序升级工作状态
	u8  rx_data_not_ok;//程序升级工作状态
	
	//u8  update_start_on;//程序升级状态
	//u8  update_state;//程序升级状态


	u8  rx_data_in_flag;//rx data 
	u8  tx_data_count;//tx data 


	u8 sum_flag;// 累加和
	
	/*cpu  flash address*/
	u32 cpu_flash_all_total_chars; 	//总的字节数量
	//u16 cpu_flash_each_page_chars;  //每包的大小2048	
	u16 cpu_flash_cur_page_num; 	//单前page
	//u16 cpu_flash_cur_address; //单前cpu flash address
	/*spi  flash address*/
	//u16 spi_flash_cur_page; //单前spi flash address
	u16 spi_flash_cur_page_start_address; //单前spi flash address开始地址
	//u16 spi_flash_cur_page_size; //单前spi flash address 大小


}
UPDATE_T;
/*
消息码定义：
消息码	描述
0-18	平台预留
19	查询设备版本
20	新版本通知
21	请求升级包
22	上报升级包下载状态
23	执行升级
24	上报升级结果
25-127	平台预留
*/

typedef enum
{
  MsgCode_Platform_reservation = 18,//平台预留
  MsgCode_Query_device_version ,//0x13
  MsgCode_New_version_notification ,//0x14
  MsgCode_Request_upgrade_package ,//0x15
  MsgCode_Escalation_package_download_status ,//0x16
  MsgCode_Perform_an_upgrade,//0x17
  MsgCode_Report_upgrade_results,//0x18
  MsgCode_Platform_reservation2	//平台预留
}msgCode_e;


#define PCP_DATA_LENTH 512

#define PCP_HEADER_DATA_LENTH 8 //固定的帧数据长度

#define PCP_DATA_starting_mark1  0XFF
#define PCP_DATA_starting_mark2  0XFE

#define PCP_DATA_pos_crc_h  4
#define PCP_DATA_pos_crc_l  5

#define PCP_DATA_version_lenth  16


#define PCP_DATA_tx_count_max  3

/*
远程升级	
程序升级状态				0x93	1	0-2	0—未升级  1—升级   2--升级中
服务器最新的固件版本	0x94	3		
固件版本的大小			0x95	4		固件程序的大小，多少字节数
每一包固件程序的大小	0x96	2		每次传送固定的大小，例如500字节。
固件升级开始命令		0x97	1	0-1	0—停止升级命令 
						1—开始升级命令   
*/
typedef struct
{
	uint16_t starting_mark;// 起始标识，固定为0XFFFE
	uint8_t version_number ;// 版本号
	msgCode_e message_code;//消息码		
	uint16_t check_code ;//校验码
	
	uint16_t Data_length ;///数据区长度
	char Data[PCP_DATA_LENTH];//数据区  n字节的十六进制数

	uint8_t result_code ;// 结果码

	char cur_version[PCP_DATA_version_lenth];
	char new_version[PCP_DATA_version_lenth];
	
	uint16_t upgrade_package_size;//每个分片的大小
	uint16_t upgrade_package_total_number;//升级包分片总数
	uint16_t upgrade_package_Check_Code ;//升级包分片总数
		
}
PCP_T;


extern UPDATE_T g_Update;
extern PCP_T g_PCP_TXData;//发送数据
extern PCP_T g_PCP_RXData;//接收数据
extern u8 update_rx_data[PCP_DATA_LENTH]; 

extern void App_Update_Init_Var(void);
extern void App_Update_Loop(void);
extern void App_PCP_RXDataFrame_analyse(uint16_t len,char* msg,PCP_T * p_PCP_RXData);
extern void App_Update_Check_update_or_goto_app(void);
extern void BootLoad_Jump(void);

void App_Update_Config_Save_Into_Flash(void);
void App_Update_Config_read_from_flash(void);

#endif






