#ifndef _APP_4G_H__
#define _APP_4G_H__


#include "NB_Board.h"

/*
消息类型	类型描述	消息格式
0x01		登录请求		0x01 +deviceId_length（2字节） +deviceId ? +password_length（2字节） +password +version_length（2字节） +version
0x02		上行数据报文	0x02+数据长度（2字节）+msgid(2字节)+serviceid（2字节）+param1+params2+param3+……
0x82		上行数据响应	0x82+数据长度（2字节）+msgid(2字节)+结果码（2字节）
0x03		下行数据报文	0x03+数据长度（2字节）+msgid(2字节)+serviceId（2字节）+param1+param2+param3+……
0x83		下行数据响应	0x83+数据长度（2字节）+msgid(2字节)+结果码（2字节）+serviceId（2字节）+param1+param2+param3+……
0x04		心跳				0x04
0x05		登录响应		0x05+结果码（2字节）
0x06	心跳响应	0x06
*/
#define  APP4G_MSG_LOGIN        	0x01
#define  APP4G_MSG_LOGIN_ASK      0X05
#define  APP4G_MSG_DATAUP     		0x02
#define  APP4G_MSG_DATAUP_ASK     0x82
#define  APP4G_MSG_DATADOWN     	0x03
#define  APP4G_MSG_DATADOWN_ASK   0x83
#define  APP4G_MSG_HEARTBEAT     	0x04
#define  APP4G_MSG_HEARTBEAT_ASK  0x06

/*
4 	 SensorBasic SensorBasic 				数据上报   
9001 DEVICE_STATE_ask DEVICE_STATE_ask 	指令下发响应
8001 DEVICE_STATE DEVICE_STATE			指令下发
1001 DeviceState DeviceState 				事件上报
*/
// msgId  服务ID serviced
#define  APP4G_SERVICEID_SensorBasic  		4
#define  APP4G_SERVICEID_DeviceState  		1001
#define  APP4G_SERVICEID_DEVICESTATE  		8001
#define  APP4G_SERVICEID_DEVICESTATEask   9001
 

 
 //记录APP4G模块初始化流程顺序
#define  APP4G_SEQ_INIT        0x01
#define  APP4G_SEQ_UDP_CR      0X02
#define  APP4G_SEQ_UDP_REG     0x04
#define  APP4G_SEQ_COAP_SERVER 0x08


 
typedef enum
{
  APP4G_NONE,
  APP4G_INIT,          // 初始化操作
  APP4G_MODULE,        // 获取APP4G模块厂商及固件，频段等信息
  APP4G_SIGN,
  APP4G_UDP_CR,        // 创建UDP
  APP4G_UDP_CL,        // 关闭UDP
  APP4G_UDP_REG,       // 此状态用于注册谷雨云平台信息（如果不与谷雨云平台通信，可忽略）
  APP4G_UDP_ST,        // 利用已经创建的UDP发送数据
  APP4G_UDP_RE,        // UDP接收信息
  APP4G_CoAP_SEVER,    // CoAP远程地址设置与获取
  APP4G_CoAP_ST,       // 利用CoAP发送消息
  APP4G_CoAP_RE,       // CoAP返回信息
  APP4G_RESET,         // 复位APP4G
  APP4G_END
}APP4G_STATE_e;


typedef enum 
{
	APP4G_STATE_INIT=0,// 初始化态
	

	APP4G_STATE_LOOP,
	APP4G_STATE_TIMER_OUT,
		
	APP4G_STATE_MAX,
}ENUM_APP4G;


typedef struct
{
	u8 work_state;
	u8 start_flag;// 开始标志
	u8 update_flag;
	
}APP4G_STRCUT;


int  app_4G_msgReportCb(char* msg,uint16 len);

void app_4G_initVar(void);
void app_4G_initStart(void);
void app_4G_loop(void);
void app_4G_sendData(char * p_byte_array, uint16_t * p_size);

#endif
