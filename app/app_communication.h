#ifndef _APP_COMMUNICATION_H__
#define _APP_COMMUNICATION_H__


#include "bsp.h"

/* 开关的状态，报警的状态*/
typedef enum
{
	FIRE_ON = 0x01,
	FIRE_OFF = 0x00,
} ENUM_STATE_ON_OFF;
typedef enum 
{
    BIT0 = 0X01,
    BIT1 = 0X02,
	BIT2 = 0X04,
	BIT3 = 0X08,
	BIT4 = 0X10,
	BIT5 = 0X20,
    BIT6 = 0X40,
    BIT7 = 0X80,
    BITS_MAX,
}BITS_ENUM;


typedef enum 
{
	COMMUNICATE_TYPE_CHAXUN = 0X01,// 查询帧
	COMMUNICATE_TYPE_CHAXUN_ASK  ,// 查询帧应答
	
	COMMUNICATE_TYPE_CMD   ,	//命令帧
	COMMUNICATE_TYPE_CMD_ASK   ,	//命令帧应答
	
	COMMUNICATE_TYPE_ERROR   ,	//错误帧
		
	COMMUNICATE_TYPE_MAX,
}ENUM_COMMUNICATE_TYPE;

#if 0
typedef enum 
{
	PAOTOU_CONTRL_MODE_AUTO = 0X06,// 自动模式
	PAOTOU_CONTRL_MODE_MANUL = 0X05,//手动模式
		
	PAOTOU_CONTRL_MODE_MAX,
}ENUM_PAOTOU_CONTRL_MODE;
#endif

typedef struct
{

	//    命令数据，状态数据，属性数据
	u8 work_state ;
	u8 contrl_mode ;
	u8 contrl_mode_bak ;

	ENUM_STATE_ON_OFF Fire_On_State;
	ENUM_STATE_ON_OFF Value_State;
	ENUM_STATE_ON_OFF Value_State_return;//炮头实际返回的状态
	ENUM_STATE_ON_OFF Value_State_qingtai;//琴台实际判断的逻辑结果
	ENUM_STATE_ON_OFF Value_State_xianchang;//现场的命令
	
	ENUM_STATE_ON_OFF Pump_State; //水泵的状态
	u8 Selfcheck_State;
	u8 Selfcheck_State_rx_485;
	
	u16 fire_location_Vertical_StepNumber ;//扫描定位之后 ，步数记忆值

	u8 move_start;
	u8 move_position_state;
	u8 move_position_cmd;
	u8 move_position_75_ready;
	
	u8 move_direction;
	
	/* 现场和远程控制切换标志 */
	u8 xianchang_yuancheng_contrl_mode ;
	u8 xianchang_yuancheng_contrl_mode_bak ;//备份寄存器
	
	/* 通信故障 */
	u8 com_error ;
	
	/* 控制主机 */
	u8 master_manchine ;
	u8 Fire_yujing_flag;// 火灾 预警
	u8 Fire_yujing_flag_bak;// 火灾 预警
	
	/* 状态的备份，用于故障的记录 */
	ENUM_STATE_ON_OFF Fire_On_State_bak;
	u8 Value_State_return_bak;//炮头实际返回的状态
	u8 Pump_State_bak; //水泵的状态
	u8 Selfcheck_State_bak;
	u8 com_error_bak ;/* 通信故障 */
	float Vertical_angle_current;//当前的垂直角度
	float Horizontal_angle_current;//当前的水平角度

	u8 beep_manul_mute_flag ;//蜂鸣器手动的消音标志位
	
}
PAOTOU_STRCUT;


/*采用小端模式，低字节在前，高字节在后。*/
/*
case 0://帧头
case 1://地址
case 2://键值
case 3://校验和
case 4://帧尾
*/


#define FRAME_HEAD (0xaa)
#define FRAME_END  (0x55)

#define FRAME_POS_HEAD      0
#define FRAME_POS_address 	1 //數值的下標

#define FRAME_POS_TYPE 		2 //帧类型
#define FRAME_POS_WORK_MODE 		2 //工作模式 手动自动

#define FRAME_POS_FIRE_ON_STATE	 	3 //报警状态
#define FRAME_POS_FIRE_YUJING_STATE	 	3 //预警状态
#define FRAME_POS_VALUE_STATE 		3 //阀门状态
#define FRAME_POS_PUMP_STATE 		3 //水泵状态
#define FRAME_POS_SELFCHECK_STATE 	3 //自检状态

#define FRAME_POS_HOR_ANGLE 		4
#define FRAME_POS_VER_ANGLE 		5
#define FRAME_POS_CMD 		6//5

#define FRAME_POS_SUM       7//6
#define FRAME_POS_END       7

#define FRAME_LENTH       8
#define FRAME_SUM_LENTH      6// 7//5

/*
手动模式 0X55
自动模式0X66
case 1://锁定 
case 8:// 下    
case 3:// 自检  
case 4:// 开关阀    
case 11://右   
case 6://键盘锁 		
           
case 2://  上   
case 9://  启/关泵   
case 10://消声 
case 5://左     
case 12:// 复位   
*/

#define FRAME_CMD_MANUL  (0X01)
#define FRAME_CMD_AUTO   (0X00)


#define FRAME_CMD_XIANGCHANG  (0X01)// 现场控制
#define FRAME_CMD_YUANCHEN   (0X00)	// 远程控制


#define FRAME_CMD_MASTER_MANCHINE  (0X01)// 1表示当前控制主机；
#define FRAME_CMD_SALVE_MANCHINE   (0X00)//0表示不是

#define FRAME_CMD_COM_ERROR  (0X01)// 1表示当前控制主机；
#define FRAME_CMD_COM_OK   (0X00)//0表示不是








#define FRAME_CMD_MOVE_LOCK   	(1)
#define FRAME_CMD_MOVE_STOP     (7)

#define FRAME_CMD_MOVE_DOWN   	(8)

/*自检命令*/
#define FRAME_CMD_SELF_CHECK_START    (3)


/*
琴台-现场	自检结束：0
				自检开始：1
				
现场-琴台	现场自检状态ok：1
				现场自检状态notok：0
				
现场-炮头	自检结束：0
				自检开始：1
				
炮头-现场	炮头自检状态ok：1
				炮头自检状态notok：0
*/
#define  	SELF_CHECK_START 		0X01
#define 	SELF_CHECK_OVER 		0X00

#define 	SELF_CHECK_PAOTOU_OK 	0X01
#define 	SELF_CHECK_PAOTOU_NOT_OK 	0X00

#define 	SELF_CHECK_XIANCHANG_OK 0X01
#define 	SELF_CHECK_XIANCHANG_NOT_OK 0X00

/*消声命令*/
#define FRAME_CMD_MUTE    (10)


#define FRAME_CMD_SELF_VALVE_OPEN   (4)
#define FRAME_CMD_VALVE_CLOSE   (44)

#define FRAME_CMD_MOVE_RIGHT    (11)
#define FRAME_CMD_MOVE_UP   	(2)
#define FRAME_CMD_MOVE_LEFT   	(5)
#define FRAME_CMD_MOVE_RESET    (12)

#define FRAME_CMD_KEYPAD_LOCK   (6)


#define FRAME_CMD_PUMP_OPEN    (45)
#define FRAME_CMD_PUMP_CLOSE   (46)


typedef enum 
{
	Communication_INIT=0,// 初始化态

	Communication_LOOP,
	Communication_TIMER_OUT,
		
	Communication_MAX,
}ENUM_Communication;

typedef struct
{
	u16 loop_time;
	u8 work_state ;	
}Communication_STRCUT;

#define READY 1 // 位置到了
#define READY_NO 0 // 位置到了

#define Communication_ASK_OK 3 //收到了应答
#define Communication_ASK_NOT_OK 4 //收到了应答

extern Communication_STRCUT g_Communication;
//extern PAOTOU_STRCUT g_Paotou;
extern PAOTOU_STRCUT * g_tXiangchang_address_paotou_master_CAN ;//主控的 _CAN
extern PAOTOU_STRCUT * g_tXiangchang_address_paotou_cur_CAN ;//当前通讯的 CAN


/*第1分区*/
extern PAOTOU_STRCUT g_Paotou11;
extern PAOTOU_STRCUT g_Paotou12;
extern PAOTOU_STRCUT g_Paotou13;
extern PAOTOU_STRCUT g_Paotou14;
/*第2分区*/
extern PAOTOU_STRCUT g_Paotou21;
extern PAOTOU_STRCUT g_Paotou22;
extern PAOTOU_STRCUT g_Paotou23;
extern PAOTOU_STRCUT g_Paotou24;
/*第3分区*/
extern PAOTOU_STRCUT g_Paotou31;
extern PAOTOU_STRCUT g_Paotou32;
extern PAOTOU_STRCUT g_Paotou33;
extern PAOTOU_STRCUT g_Paotou34;
/*第4分区*/
extern PAOTOU_STRCUT g_Paotou41;
extern PAOTOU_STRCUT g_Paotou42;
extern PAOTOU_STRCUT g_Paotou43;
extern PAOTOU_STRCUT g_Paotou44;

/*第5分区*/
extern PAOTOU_STRCUT g_Paotou51;
extern PAOTOU_STRCUT g_Paotou52;
extern PAOTOU_STRCUT g_Paotou53;
extern PAOTOU_STRCUT g_Paotou54;
/*第6分区*/
extern PAOTOU_STRCUT g_Paotou61;
extern PAOTOU_STRCUT g_Paotou62;
extern PAOTOU_STRCUT g_Paotou63;
extern PAOTOU_STRCUT g_Paotou64;
/*第7分区*/
extern PAOTOU_STRCUT g_Paotou71;
extern PAOTOU_STRCUT g_Paotou72;
extern PAOTOU_STRCUT g_Paotou73;
extern PAOTOU_STRCUT g_Paotou74;
/*第8分区*/
extern PAOTOU_STRCUT g_Paotou81;
extern PAOTOU_STRCUT g_Paotou82;
extern PAOTOU_STRCUT g_Paotou83;
extern PAOTOU_STRCUT g_Paotou84;
/*第9分区*/
extern PAOTOU_STRCUT g_Paotou91;
extern PAOTOU_STRCUT g_Paotou92;
extern PAOTOU_STRCUT g_Paotou93;
extern PAOTOU_STRCUT g_Paotou94;
/*第10分区*/
extern PAOTOU_STRCUT g_Paotou101;
extern PAOTOU_STRCUT g_Paotou102;
extern PAOTOU_STRCUT g_Paotou103;
extern PAOTOU_STRCUT g_Paotou104;
/*第11分区*/
extern PAOTOU_STRCUT g_Paotou111;
extern PAOTOU_STRCUT g_Paotou112;
extern PAOTOU_STRCUT g_Paotou113;
extern PAOTOU_STRCUT g_Paotou114;
/*第12分区*/
extern PAOTOU_STRCUT g_Paotou121;
extern PAOTOU_STRCUT g_Paotou122;
extern PAOTOU_STRCUT g_Paotou123;
extern PAOTOU_STRCUT g_Paotou124;
/*第13分区*/
extern PAOTOU_STRCUT g_Paotou131;
extern PAOTOU_STRCUT g_Paotou132;
extern PAOTOU_STRCUT g_Paotou133;
extern PAOTOU_STRCUT g_Paotou134;
/*第14分区*/
extern PAOTOU_STRCUT g_Paotou141;
extern PAOTOU_STRCUT g_Paotou142;
extern PAOTOU_STRCUT g_Paotou143;
extern PAOTOU_STRCUT g_Paotou144;
/*第15分区*/
extern PAOTOU_STRCUT g_Paotou151;
extern PAOTOU_STRCUT g_Paotou152;
extern PAOTOU_STRCUT g_Paotou153;
extern PAOTOU_STRCUT g_Paotou154;
/*第16分区*/
extern PAOTOU_STRCUT g_Paotou161;
extern PAOTOU_STRCUT g_Paotou162;
extern PAOTOU_STRCUT g_Paotou163;
extern PAOTOU_STRCUT g_Paotou164;

void App_communication_Init_Var(void);
void App_communication_tx_chaxun_data_pro(u8 input_CMD,u8 input_type);
void App_communication_tx_cmd_data_pro(u8 input_CMD,u8 input_type);

u8 App_communication_Loop(void);
u8 App_communication_tx_data_Retry(u8 input_CMD);
uint8_t App_communication_rx_data_pro(void);

void App_communication_address_change_pro(void);

void App_communication_Test(void);
void App_communication_address_point_change_CAN(u8 address_fengqu_cur,u8  address_paotou_cur);
void App_communication_address_change_to_zero(void);

#endif

