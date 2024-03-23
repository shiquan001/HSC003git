#ifndef _APP_CAMERA_H__
#define _APP_CAMERA_H__
#include "app.h"

/*****************发送摄像头的日志信息到串口*************************/
#ifndef USING_CAMERA_DEBUGE
#define	USING_CAMERA_DEBUGE		1
#endif

#define HEADER_TX 0X56
#define HEADER_RX 0X76

#define CMD_RESET 0X26//4.1 复位指令：56 00 26 00 返回：76 00 26 00 +DSP 版本信息
#define CMD_START_CAMERA 		0X36//4.2 拍照指令：56 00 36 01 00 返回：76 00 36 00 00
#define CMD_READ_DATAS_LENTH  	0X34//4.3 读取所拍图片长度指令：56 00 34 01 00
#define CMD_READ_DATAS 			0X32//4.4 读取所拍图片数据指令：56 00 32 0C 00 0A 00 00 XX XX 00 00 YY YY 00 FF
#define CMD_CLEAR_DATAS 0X36//4.5 清空图片缓存指令：56 00 36 01 03 返回：76 00 36 00 00

#define CMD_set_compress_cmd 0X31//4.6 设置拍照图片压缩率指令：56 00 31 05 01 01 12 04 XX 返回：76 00 31 00 00
#define CMD_photo_size_cmd 0X31//4.6 设置拍照图片压缩率指令：56 00 31 05 01 01 12 04 XX 返回：76 00 31 00 00

#define CMD_set_address_cmd 0X31//4.10 修改摄像头序号 56 YY 31 05 04 01 00 06 ZZ


/*
56 00 31 05 04 01 00 19 11 （320*240） 返回：76 00 31 00 00
56 00 31 05 04 01 00 19 00 （640*480）
56 00 31 05 04 01 00 19 22 （160*120）
*/
#define CMD_photo_size_640480 0x00
#define CMD_photo_size_320240 0x11
#define CMD_photo_size_160120 0x22

#define CMD_compress_rate_36  36
#define CAMERA_DATA_LENTH (512+256)  // 

typedef struct
{
	uint8_t flag_tx;       //发送标志
	uint8_t count_tx;       //发送次数

	u8 start_state;//开始
	u8 work_state;
	u8 update_flag;
	
	u16 datas_lenth;// 数据长度
	u16 all_packages;//
	u16 data_lenth_each_packages;//
	
	u16 rx_datas_count;//接收数据的次数；512*10
//	u16 rx_datas_size;// 512
	u8 server_ask_ok;
	u8 poweroffpic;
	u8 getpicpack;
	u16 getpicpack_count;
	char data[CAMERA_DATA_LENTH];
}CAMERA_STRCUT;

/*
（1）发送拍照指令 
（2）发送读取所拍图片长度指令 
（3）发送读取所拍图片数据指令 
（4）发送清空图片缓存指令
*/
typedef enum 
{
	CAMERA_INIT=1,//
	CAMERA_delay,//2.5s
	CAMERA_set_size,			 
	CAMERA_reset,	//设置之后 复位
	CAMERA_reset_delay,//2.5s
	
	CAMERA_start_camera,			//（1）发送拍照指令
	CAMERA_read_datas_lenth,		//（2）发送读取所拍图片长度指令
	CAMERA_read_datas,				//（3）发送读取所拍图片数据指令
	CAMERA_tx_datas,				// send to server

	CAMERA_poweroffpic_getpicpack,				//
	
	CAMERA_clear_datas,				//（4）发送清空图片缓存指令
	
	CAMERA_TIMER_OUT,
		
	CAMERA_MAX,
}
ENUM_CAMERA;

extern CAMERA_STRCUT	  g_Camera;
extern void App_camera_Init_Var(void);
void App_camera_tx_set_address_cmd(uint8_t address, uint8_t new_address );
void App_camera_tx_clear_datas_cmd(uint8_t address  );
void App_camera_tx_photo_size_cmd(uint8_t address ,u8 size_type );
void App_camera_tx_compress_rate_cmd(uint8_t address ,u8 compress_rate );
void App_camera_tx_read_datas_cmd(uint8_t address ,u16 start_address,u16 lenth );
void App_camera_tx_read_datas_lenth_cmd(uint8_t address  );
void App_camera_tx_start_camera_cmd(uint8_t address  );
void App_DataReport_SensorExtend(void);
void App_camera_Loop(void);


#endif
