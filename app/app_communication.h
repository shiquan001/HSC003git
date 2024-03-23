#ifndef _APP_COMMUNICATION_H__
#define _APP_COMMUNICATION_H__


#include "bsp.h"

/* ���ص�״̬��������״̬*/
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
	COMMUNICATE_TYPE_CHAXUN = 0X01,// ��ѯ֡
	COMMUNICATE_TYPE_CHAXUN_ASK  ,// ��ѯ֡Ӧ��
	
	COMMUNICATE_TYPE_CMD   ,	//����֡
	COMMUNICATE_TYPE_CMD_ASK   ,	//����֡Ӧ��
	
	COMMUNICATE_TYPE_ERROR   ,	//����֡
		
	COMMUNICATE_TYPE_MAX,
}ENUM_COMMUNICATE_TYPE;

#if 0
typedef enum 
{
	PAOTOU_CONTRL_MODE_AUTO = 0X06,// �Զ�ģʽ
	PAOTOU_CONTRL_MODE_MANUL = 0X05,//�ֶ�ģʽ
		
	PAOTOU_CONTRL_MODE_MAX,
}ENUM_PAOTOU_CONTRL_MODE;
#endif

typedef struct
{

	//    �������ݣ�״̬���ݣ���������
	u8 work_state ;
	u8 contrl_mode ;
	u8 contrl_mode_bak ;

	ENUM_STATE_ON_OFF Fire_On_State;
	ENUM_STATE_ON_OFF Value_State;
	ENUM_STATE_ON_OFF Value_State_return;//��ͷʵ�ʷ��ص�״̬
	ENUM_STATE_ON_OFF Value_State_qingtai;//��̨ʵ���жϵ��߼����
	ENUM_STATE_ON_OFF Value_State_xianchang;//�ֳ�������
	
	ENUM_STATE_ON_OFF Pump_State; //ˮ�õ�״̬
	u8 Selfcheck_State;
	u8 Selfcheck_State_rx_485;
	
	u16 fire_location_Vertical_StepNumber ;//ɨ�趨λ֮�� ����������ֵ

	u8 move_start;
	u8 move_position_state;
	u8 move_position_cmd;
	u8 move_position_75_ready;
	
	u8 move_direction;
	
	/* �ֳ���Զ�̿����л���־ */
	u8 xianchang_yuancheng_contrl_mode ;
	u8 xianchang_yuancheng_contrl_mode_bak ;//���ݼĴ���
	
	/* ͨ�Ź��� */
	u8 com_error ;
	
	/* �������� */
	u8 master_manchine ;
	u8 Fire_yujing_flag;// ���� Ԥ��
	u8 Fire_yujing_flag_bak;// ���� Ԥ��
	
	/* ״̬�ı��ݣ����ڹ��ϵļ�¼ */
	ENUM_STATE_ON_OFF Fire_On_State_bak;
	u8 Value_State_return_bak;//��ͷʵ�ʷ��ص�״̬
	u8 Pump_State_bak; //ˮ�õ�״̬
	u8 Selfcheck_State_bak;
	u8 com_error_bak ;/* ͨ�Ź��� */
	float Vertical_angle_current;//��ǰ�Ĵ�ֱ�Ƕ�
	float Horizontal_angle_current;//��ǰ��ˮƽ�Ƕ�

	u8 beep_manul_mute_flag ;//�������ֶ���������־λ
	
}
PAOTOU_STRCUT;


/*����С��ģʽ�����ֽ���ǰ�����ֽ��ں�*/
/*
case 0://֡ͷ
case 1://��ַ
case 2://��ֵ
case 3://У���
case 4://֡β
*/


#define FRAME_HEAD (0xaa)
#define FRAME_END  (0x55)

#define FRAME_POS_HEAD      0
#define FRAME_POS_address 	1 //��ֵ����

#define FRAME_POS_TYPE 		2 //֡����
#define FRAME_POS_WORK_MODE 		2 //����ģʽ �ֶ��Զ�

#define FRAME_POS_FIRE_ON_STATE	 	3 //����״̬
#define FRAME_POS_FIRE_YUJING_STATE	 	3 //Ԥ��״̬
#define FRAME_POS_VALUE_STATE 		3 //����״̬
#define FRAME_POS_PUMP_STATE 		3 //ˮ��״̬
#define FRAME_POS_SELFCHECK_STATE 	3 //�Լ�״̬

#define FRAME_POS_HOR_ANGLE 		4
#define FRAME_POS_VER_ANGLE 		5
#define FRAME_POS_CMD 		6//5

#define FRAME_POS_SUM       7//6
#define FRAME_POS_END       7

#define FRAME_LENTH       8
#define FRAME_SUM_LENTH      6// 7//5

/*
�ֶ�ģʽ 0X55
�Զ�ģʽ0X66
case 1://���� 
case 8:// ��    
case 3:// �Լ�  
case 4:// ���ط�    
case 11://��   
case 6://������ 		
           
case 2://  ��   
case 9://  ��/�ر�   
case 10://���� 
case 5://��     
case 12:// ��λ   
*/

#define FRAME_CMD_MANUL  (0X01)
#define FRAME_CMD_AUTO   (0X00)


#define FRAME_CMD_XIANGCHANG  (0X01)// �ֳ�����
#define FRAME_CMD_YUANCHEN   (0X00)	// Զ�̿���


#define FRAME_CMD_MASTER_MANCHINE  (0X01)// 1��ʾ��ǰ����������
#define FRAME_CMD_SALVE_MANCHINE   (0X00)//0��ʾ����

#define FRAME_CMD_COM_ERROR  (0X01)// 1��ʾ��ǰ����������
#define FRAME_CMD_COM_OK   (0X00)//0��ʾ����








#define FRAME_CMD_MOVE_LOCK   	(1)
#define FRAME_CMD_MOVE_STOP     (7)

#define FRAME_CMD_MOVE_DOWN   	(8)

/*�Լ�����*/
#define FRAME_CMD_SELF_CHECK_START    (3)


/*
��̨-�ֳ�	�Լ������0
				�Լ쿪ʼ��1
				
�ֳ�-��̨	�ֳ��Լ�״̬ok��1
				�ֳ��Լ�״̬notok��0
				
�ֳ�-��ͷ	�Լ������0
				�Լ쿪ʼ��1
				
��ͷ-�ֳ�	��ͷ�Լ�״̬ok��1
				��ͷ�Լ�״̬notok��0
*/
#define  	SELF_CHECK_START 		0X01
#define 	SELF_CHECK_OVER 		0X00

#define 	SELF_CHECK_PAOTOU_OK 	0X01
#define 	SELF_CHECK_PAOTOU_NOT_OK 	0X00

#define 	SELF_CHECK_XIANCHANG_OK 0X01
#define 	SELF_CHECK_XIANCHANG_NOT_OK 0X00

/*��������*/
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
	Communication_INIT=0,// ��ʼ��̬

	Communication_LOOP,
	Communication_TIMER_OUT,
		
	Communication_MAX,
}ENUM_Communication;

typedef struct
{
	u16 loop_time;
	u8 work_state ;	
}Communication_STRCUT;

#define READY 1 // λ�õ���
#define READY_NO 0 // λ�õ���

#define Communication_ASK_OK 3 //�յ���Ӧ��
#define Communication_ASK_NOT_OK 4 //�յ���Ӧ��

extern Communication_STRCUT g_Communication;
//extern PAOTOU_STRCUT g_Paotou;
extern PAOTOU_STRCUT * g_tXiangchang_address_paotou_master_CAN ;//���ص� _CAN
extern PAOTOU_STRCUT * g_tXiangchang_address_paotou_cur_CAN ;//��ǰͨѶ�� CAN


/*��1����*/
extern PAOTOU_STRCUT g_Paotou11;
extern PAOTOU_STRCUT g_Paotou12;
extern PAOTOU_STRCUT g_Paotou13;
extern PAOTOU_STRCUT g_Paotou14;
/*��2����*/
extern PAOTOU_STRCUT g_Paotou21;
extern PAOTOU_STRCUT g_Paotou22;
extern PAOTOU_STRCUT g_Paotou23;
extern PAOTOU_STRCUT g_Paotou24;
/*��3����*/
extern PAOTOU_STRCUT g_Paotou31;
extern PAOTOU_STRCUT g_Paotou32;
extern PAOTOU_STRCUT g_Paotou33;
extern PAOTOU_STRCUT g_Paotou34;
/*��4����*/
extern PAOTOU_STRCUT g_Paotou41;
extern PAOTOU_STRCUT g_Paotou42;
extern PAOTOU_STRCUT g_Paotou43;
extern PAOTOU_STRCUT g_Paotou44;

/*��5����*/
extern PAOTOU_STRCUT g_Paotou51;
extern PAOTOU_STRCUT g_Paotou52;
extern PAOTOU_STRCUT g_Paotou53;
extern PAOTOU_STRCUT g_Paotou54;
/*��6����*/
extern PAOTOU_STRCUT g_Paotou61;
extern PAOTOU_STRCUT g_Paotou62;
extern PAOTOU_STRCUT g_Paotou63;
extern PAOTOU_STRCUT g_Paotou64;
/*��7����*/
extern PAOTOU_STRCUT g_Paotou71;
extern PAOTOU_STRCUT g_Paotou72;
extern PAOTOU_STRCUT g_Paotou73;
extern PAOTOU_STRCUT g_Paotou74;
/*��8����*/
extern PAOTOU_STRCUT g_Paotou81;
extern PAOTOU_STRCUT g_Paotou82;
extern PAOTOU_STRCUT g_Paotou83;
extern PAOTOU_STRCUT g_Paotou84;
/*��9����*/
extern PAOTOU_STRCUT g_Paotou91;
extern PAOTOU_STRCUT g_Paotou92;
extern PAOTOU_STRCUT g_Paotou93;
extern PAOTOU_STRCUT g_Paotou94;
/*��10����*/
extern PAOTOU_STRCUT g_Paotou101;
extern PAOTOU_STRCUT g_Paotou102;
extern PAOTOU_STRCUT g_Paotou103;
extern PAOTOU_STRCUT g_Paotou104;
/*��11����*/
extern PAOTOU_STRCUT g_Paotou111;
extern PAOTOU_STRCUT g_Paotou112;
extern PAOTOU_STRCUT g_Paotou113;
extern PAOTOU_STRCUT g_Paotou114;
/*��12����*/
extern PAOTOU_STRCUT g_Paotou121;
extern PAOTOU_STRCUT g_Paotou122;
extern PAOTOU_STRCUT g_Paotou123;
extern PAOTOU_STRCUT g_Paotou124;
/*��13����*/
extern PAOTOU_STRCUT g_Paotou131;
extern PAOTOU_STRCUT g_Paotou132;
extern PAOTOU_STRCUT g_Paotou133;
extern PAOTOU_STRCUT g_Paotou134;
/*��14����*/
extern PAOTOU_STRCUT g_Paotou141;
extern PAOTOU_STRCUT g_Paotou142;
extern PAOTOU_STRCUT g_Paotou143;
extern PAOTOU_STRCUT g_Paotou144;
/*��15����*/
extern PAOTOU_STRCUT g_Paotou151;
extern PAOTOU_STRCUT g_Paotou152;
extern PAOTOU_STRCUT g_Paotou153;
extern PAOTOU_STRCUT g_Paotou154;
/*��16����*/
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

