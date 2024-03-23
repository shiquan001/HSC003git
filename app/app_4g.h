#ifndef _APP_4G_H__
#define _APP_4G_H__


#include "NB_Board.h"

/*
��Ϣ����	��������	��Ϣ��ʽ
0x01		��¼����		0x01 +deviceId_length��2�ֽڣ� +deviceId ? +password_length��2�ֽڣ� +password +version_length��2�ֽڣ� +version
0x02		�������ݱ���	0x02+���ݳ��ȣ�2�ֽڣ�+msgid(2�ֽ�)+serviceid��2�ֽڣ�+param1+params2+param3+����
0x82		����������Ӧ	0x82+���ݳ��ȣ�2�ֽڣ�+msgid(2�ֽ�)+����루2�ֽڣ�
0x03		�������ݱ���	0x03+���ݳ��ȣ�2�ֽڣ�+msgid(2�ֽ�)+serviceId��2�ֽڣ�+param1+param2+param3+����
0x83		����������Ӧ	0x83+���ݳ��ȣ�2�ֽڣ�+msgid(2�ֽ�)+����루2�ֽڣ�+serviceId��2�ֽڣ�+param1+param2+param3+����
0x04		����				0x04
0x05		��¼��Ӧ		0x05+����루2�ֽڣ�
0x06	������Ӧ	0x06
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
4 	 SensorBasic SensorBasic 				�����ϱ�   
9001 DEVICE_STATE_ask DEVICE_STATE_ask 	ָ���·���Ӧ
8001 DEVICE_STATE DEVICE_STATE			ָ���·�
1001 DeviceState DeviceState 				�¼��ϱ�
*/
// msgId  ����ID serviced
#define  APP4G_SERVICEID_SensorBasic  		4
#define  APP4G_SERVICEID_DeviceState  		1001
#define  APP4G_SERVICEID_DEVICESTATE  		8001
#define  APP4G_SERVICEID_DEVICESTATEask   9001
 

 
 //��¼APP4Gģ���ʼ������˳��
#define  APP4G_SEQ_INIT        0x01
#define  APP4G_SEQ_UDP_CR      0X02
#define  APP4G_SEQ_UDP_REG     0x04
#define  APP4G_SEQ_COAP_SERVER 0x08


 
typedef enum
{
  APP4G_NONE,
  APP4G_INIT,          // ��ʼ������
  APP4G_MODULE,        // ��ȡAPP4Gģ�鳧�̼��̼���Ƶ�ε���Ϣ
  APP4G_SIGN,
  APP4G_UDP_CR,        // ����UDP
  APP4G_UDP_CL,        // �ر�UDP
  APP4G_UDP_REG,       // ��״̬����ע�������ƽ̨��Ϣ��������������ƽ̨ͨ�ţ��ɺ��ԣ�
  APP4G_UDP_ST,        // �����Ѿ�������UDP��������
  APP4G_UDP_RE,        // UDP������Ϣ
  APP4G_CoAP_SEVER,    // CoAPԶ�̵�ַ�������ȡ
  APP4G_CoAP_ST,       // ����CoAP������Ϣ
  APP4G_CoAP_RE,       // CoAP������Ϣ
  APP4G_RESET,         // ��λAPP4G
  APP4G_END
}APP4G_STATE_e;


typedef enum 
{
	APP4G_STATE_INIT=0,// ��ʼ��̬
	

	APP4G_STATE_LOOP,
	APP4G_STATE_TIMER_OUT,
		
	APP4G_STATE_MAX,
}ENUM_APP4G;


typedef struct
{
	u8 work_state;
	u8 start_flag;// ��ʼ��־
	u8 update_flag;
	
}APP4G_STRCUT;


int  app_4G_msgReportCb(char* msg,uint16 len);

void app_4G_initVar(void);
void app_4G_initStart(void);
void app_4G_loop(void);
void app_4G_sendData(char * p_byte_array, uint16_t * p_size);

#endif
