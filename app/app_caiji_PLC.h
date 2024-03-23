
#ifndef  _app_caiji_PLC_H_
#define  _app_caiji_PLC_H_

#include "stm32l4xx_hal.h"



#define  DISCONNECT 0  
#define  CONNECT 	  1		//1=接通；0=断开

typedef enum 
{		
	CAIJI_PLC_OPEN=0x00,	
		
	CAIJI_PLC_DELAY,//延时5秒钟，发送采集命令
		
	CAIJI_PLC_TX,
	CAIJI_PLC_RX, 

	CAIJI_TRTH_TX,//土壤温湿度
	CAIJI_TRTH_RX, 

	CAIJI_LED_TX,//led 屏幕
	CAIJI_LED_RX, 

	CMD_PLC_TX,
	CMD_PLC_RX,

	CMD_PLC_SERVER_ASK,//CMD ASK 
	
	CAIJI_PLC_CLOSE,
	
	CAIJI_PLC_MAX, 
}ENUM_CAIJI_PLC;

typedef enum
{
	PLC_NULL = 0,
	PLC_TIANJIN = 11,// 天津
	PLC_SHESHISUO =22,	//设施所
}PLC_TYPE;

typedef enum 
{		
	MOTOR_01=0x00,	
	MOTOR_02,
	MOTOR_03,
	MOTOR_04,
}ENUM_MOTOR;
typedef enum
{
	MOTOR_STATE_STOP =0,
	MOTOR_STATE_positive,
	MOTOR_STATE_reversal,
}
ENUM_MOTOR_STATE;

#define NUM_MOTOR 4
#define REGISTER_NUM  6
#define REGISTER_NUM_all  NUM_MOTOR*REGISTER_NUM
typedef struct MOTOR
{
	uint8_t  Motor_remote;//Motor remote indication
	uint8_t  Motor_positive;//Motor positive transfer indication  Motor reversal indication
	uint8_t  Motor_reversal;// Motor reversal indication   
	uint8_t  Motor_fault;//    Motor fault indication    

	uint8_t limit_Positive_turn_limit;// 正转限位指示Positive turn limit indication
	uint8_t limit_Reverse_turn_limit;// 反转限位指示Reverse limit indication	
}motor_t;

typedef struct PLC
{
	motor_t motor[NUM_MOTOR];
	
	ENUM_CAIJI_PLC PLC_work_state; 
	uint8_t PLC_start; 		
	uint8_t tx_count;//发给服务器的次数 	

	uint32_t rx_data;
	uint8_t rx_occupied;//[NUM_MOTOR];//占位
	uint8_t rx_value;//值

	uint8_t tx_occupied[NUM_MOTOR];//应答的数据
	uint16_t tx_mid;
	
	uint16_t reg_address;//PLC 	地址（只写）
	uint8_t reg_data[2];//PLC 	地址（只写）
	uint8_t reg_retry_tx_count; //重发的计数	
	
	uint16_t plc_not_ask_count;//断线的检测3次
	
	uint16_t plc_not_ask_state_changed; //掉线、上报
	uint16_t turang_not_ask_count;//断线的检测3次
	uint16_t led_not_ask_count;//断线的检测3次

	
}plc_t;


extern plc_t g_plc;
void PLC_motor_state_convert(void);

void App_caiji_PLC_Init_Var(void);
void App_caiji_tx_PLC_cmd(uint8_t address ,uint8_t function);
void App_caiji_PLC_Loop(void);
void App_plc_Dinfo_Data_process(uint8_t pos,char* msg);
void App_caiji_PLC_writer_ask(void);


#endif
