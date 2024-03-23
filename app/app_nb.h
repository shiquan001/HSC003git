#ifndef _APP_NB_H__
#define _APP_NB_H__


#include "NB_Board.h"
#include "NB_BC95.h"
#include "NB_Board_Cfg.h"
  //记录NB模块初始化流程顺序
#define  SIGNAL_STRENGTH_3        -91
#define  SIGNAL_STRENGTH_2      -103
#define  SIGNAL_STRENGTH_1     -113

 
 //记录NB模块初始化流程顺序
#define  NB_SEQ_INIT        0x01
#define  NB_SEQ_UDP_CR      0X02
#define  NB_SEQ_UDP_REG     0x04
#define  NB_SEQ_COAP_SERVER 0x08


#define JSON_BUF_LEN    512
  
 //管理json数据
 typedef struct
 {
	 char json_buf[JSON_BUF_LEN];  //用于存放json格式数据
	 uint16_t json_len; 		   //有效数据长度
 }json_info_t;

 

typedef enum
{
  NB_NONE,
  NB_INIT,          // 初始化操作
  NB_MODULE,        // 获取NB模块厂商及固件，频段等信息
  NB_SIGN,
  NB_UDP_CR,        // 创建UDP
  NB_UDP_CL,        // 关闭UDP
  NB_UDP_REG,       // 此状态用于注册谷雨云平台信息（如果不与谷雨云平台通信，可忽略）
  NB_UDP_ST,        // 利用已经创建的UDP发送数据
  NB_UDP_RE,        // UDP接收信息
  NB_CoAP_SEVER,    // CoAP远程地址设置与获取
  NB_CoAP_ST,       // 利用CoAP发送消息
  NB_CoAP_RE,       // CoAP返回信息
  NB_RESET,         // 复位NB
  NB_END
}NB_STATE_e;


typedef enum 
{
	NB_STATE_INIT=0,// 初始化态
	

	NB_STATE_LOOP,
	NB_STATE_TIMER_OUT,
		
	NB_STATE_MAX,
}ENUM_NB;


typedef struct
{
	u8 work_state;
	u8 start_flag;// 开始标志
	u8 update_flag;
	
}NB_STRCUT;


extern volatile NB_STATE_e  APP_STATE ;
extern json_info_t json_info;

extern uint8_t seq_record ;

int NB_MsgreportCb(msg_types_t,int ,char*);
void App_nb_Init_Var(void);
void App_nb_Loop(void);
void App_nb_test(void);
void App_setReport(void);
void App_nb_Init_start(void);
uint8_t App_nb_get_APPSTATE(void);


#endif
