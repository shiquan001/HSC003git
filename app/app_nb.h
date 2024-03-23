#ifndef _APP_NB_H__
#define _APP_NB_H__


#include "NB_Board.h"
#include "NB_BC95.h"
#include "NB_Board_Cfg.h"
  //��¼NBģ���ʼ������˳��
#define  SIGNAL_STRENGTH_3        -91
#define  SIGNAL_STRENGTH_2      -103
#define  SIGNAL_STRENGTH_1     -113

 
 //��¼NBģ���ʼ������˳��
#define  NB_SEQ_INIT        0x01
#define  NB_SEQ_UDP_CR      0X02
#define  NB_SEQ_UDP_REG     0x04
#define  NB_SEQ_COAP_SERVER 0x08


#define JSON_BUF_LEN    512
  
 //����json����
 typedef struct
 {
	 char json_buf[JSON_BUF_LEN];  //���ڴ��json��ʽ����
	 uint16_t json_len; 		   //��Ч���ݳ���
 }json_info_t;

 

typedef enum
{
  NB_NONE,
  NB_INIT,          // ��ʼ������
  NB_MODULE,        // ��ȡNBģ�鳧�̼��̼���Ƶ�ε���Ϣ
  NB_SIGN,
  NB_UDP_CR,        // ����UDP
  NB_UDP_CL,        // �ر�UDP
  NB_UDP_REG,       // ��״̬����ע�������ƽ̨��Ϣ��������������ƽ̨ͨ�ţ��ɺ��ԣ�
  NB_UDP_ST,        // �����Ѿ�������UDP��������
  NB_UDP_RE,        // UDP������Ϣ
  NB_CoAP_SEVER,    // CoAPԶ�̵�ַ�������ȡ
  NB_CoAP_ST,       // ����CoAP������Ϣ
  NB_CoAP_RE,       // CoAP������Ϣ
  NB_RESET,         // ��λNB
  NB_END
}NB_STATE_e;


typedef enum 
{
	NB_STATE_INIT=0,// ��ʼ��̬
	

	NB_STATE_LOOP,
	NB_STATE_TIMER_OUT,
		
	NB_STATE_MAX,
}ENUM_NB;


typedef struct
{
	u8 work_state;
	u8 start_flag;// ��ʼ��־
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
