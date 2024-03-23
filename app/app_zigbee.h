
#ifndef _app_zigbee__
#define _app_zigbee__

#include "app.h"  
#if 0
#define DataSize_MAX (80+2) // ���η������ݵ��ܳ��ȣ���ǰ�汾 ��֡����͸����������Ϊ80��

typedef enum 
{
	ZIGBEE_INIT_GetVersion=1,// ��������
	ZIGBEE_INIT_GetDEVICETYPE,
	ZIGBEE_INIT_GetPANID,
	ZIGBEE_INIT_GetCHANNEL,
	ZIGBEE_INIT_GetNET_ADDRESS,
	ZIGBEE_INIT_GetMAC_ADDRESS,
	ZIGBEE_INIT_GetTXPOWER,

	ZIGBEE_APP_OPEN_NETWORK, // Ӧ�����������
	ZIGBEE_LOOP,
		
	ZIGBEE_MAX,
}ENUM_ZIGBEE;


typedef struct 
{
	u8	DATALENGTH;					// ���ݳ���
	u8	DATABUF[DataSize_MAX]; 			// ����
}DATA_BUF;

//----------�豸�Ľṹ��
typedef struct  
{	

	uint8_t flag_tx;       //���ͱ�־
	uint8_t count_tx;       //���ʹ���
	uint8_t work_state;       //״̬
	
	u8 version[3];	//2.0 �汾��Ϣ��
	u8 devType;		//2.1 �豸����
	u8 PanIDH;		//2.2 ����ID
	u8 PanIDL;
	u8 channel;		//�ŵ�

	u8 nwkAddr0;	//0  2.4 �����ַ(��֧�֣�Э���������ַ�̶�Ϊ0x0000)
	u8 nwkAddr1;	// 0
	
	u8 macAddr[8];// ������mac
	u8 txPower;// ���书��
	u8 baudRate;	// ������
	
	u8 End_Device_macAddr[8];// �ն˵�mac
	u8 End_Device_nwkAddr[2];

	u8	End_Device_seqID;
	u8 End_Device_feedback_state; //������ִ�н����:

	u8 End_Device_offline_nwkAddr[2];// ���ߵ�ַ
	u8 Remove_device_notifications_macAddr[8];
	u8 End_Device_Rssi;
	u8 groupNum;
	u8 Group1_L;
	u8 Group1_H;
	
	u8 Group2_L;
	u8 Group2_H;	
}
_Coordinator_data_type; 	  

/* zigbee �ն��豸�Ľṹ��*/
typedef struct  
{	
	u8 macAddr[8];// �ն˵�mac
	u8 nwkAddr[2];

	u8 Remove_device_notifications_macAddr[8];
	u8 Rssi;
}_end_device_data_type; 	  

//==========R|W===========//
#define ZIGBEE_CMD_WRITE 0x01
#define ZIGBEE_CMD_READ  0x00


/* 
													2.Э������������
����ID	����
0	��ѯ�汾��
1	�����豸����
2	����PANID
3	�����ŵ�
4	���������ַ
5	����MAC��ַ
6	���÷��͹���
7	���ò�����
8	�ָ��������ò�����
9	�����豸
10	Э�����ı�����ָ�
*/
typedef enum 
{
	ZIGBEE_CMD_VERSION =0x00,
	ZIGBEE_CMD_CONFIG_DEVICE_TYPE,
	ZIGBEE_CMD_CONFIG_PANID,
	ZIGBEE_CMD_CONFIG_CHANNEL,
	ZIGBEE_CMD_CONFIG_NET_ADDRESS,//		2.4 �����ַ(��֧�֣�Э���������ַ�̶�Ϊ0x0000)
	ZIGBEE_CMD_CONFIG_MAC_ADDRESS,
	ZIGBEE_CMD_CONFIG_TXPOWER,
	ZIGBEE_CMD_CONFIG_BAUDRATE,
	ZIGBEE_CMD_CONFIG_REFACTORY,
	ZIGBEE_CMD_CONFIG_RESTART,
	ZIGBEE_CMD_CONFIG_BACKUP_RECOVERY,		
	ZIGBEE_CMD_MAX,
}ENUM_ZIGBEE_CMD_TYPE;

/* 
													1.Ӧ������
���	����ID	��������												��ע
0	0	�������豸��Ϣ													�����ַ��mac��ַ���豸����
		����Mac��ַ��ѯ�����ַ	
		���������ַ��ѯmac��ַ	
1	1	��������͸��	
2	2	�������豸��Ϣ֪ͨ											�豸���߼�⹦��
3	3	��/�ر�����	
4	4	�ָ�Զ���豸��Ĭ������״̬	
5	5	����Զ���豸														����ڵ��豸�յ��������30���ڽ�����������
6	6	�Ƴ�Զ���豸														��Ŀ��ڵ��豸��������ɾ��
7	7	�����е��ӽڵ��豸����֪ͨ	
8	8	�������豸��ѯ�����豸���ź�ǿ��	
9	9	Э������ѯ�����豸���ź�ǿ��	
10	10	�����	
11	11	ɾ����	
12	12	��ѯ��	
*/
typedef enum 
{
	ZIGBEE_APPLICAITON_CMD_Device_Information = 0x00,//�豸��ϢDevice Information// 0x00�� ��ʾ ��ѯָ��mac��ַ�豸�������ַ��
	ZIGBEE_APPLICAITON_CMD_transparent_data = 0x01,// ����͸������Serial port transparent data // 0x01����ʾ ���д���͸������
	
	ZIGBEE_APPLICAITON_CMD_Offline_device_notification,//�����豸֪ͨOffline device notification
	ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network,//��/�ر�����Turn off the network
	ZIGBEE_APPLICAITON_CMD_Remote_factory_reset,//		Զ�ָ̻���������Remote factory reset
	ZIGBEE_APPLICAITON_CMD_Remote_restart_command,//Զ����������Remote restart command
	ZIGBEE_APPLICAITON_CMD_Remove_device,//2.2.6 �Ƴ��豸Remove device
	ZIGBEE_APPLICAITON_CMD_Remove_device_notifications,//2.2.7 �Ƴ��豸֪ͨRemove device notifications
	ZIGBEE_APPLICAITON_CMD_Query_signal_quality_of_the_local_device,//2.2.8 ��ѯ�������豸���ص��ź���������·���豸���ն��豸ʹ�õ����Query the signal quality of the local device?

	ZIGBEE_APPLICAITON_CMD_The_coordinator_queries_the_signal_quality_of_the_remote,//2.2.9 Э������ѯԶ���豸���ź�����The coordinator queries the signal quality of the remote device
	ZIGBEE_APPLICAITON_CMD_Add_group,		//2.2.10 �����Add group
	ZIGBEE_APPLICAITON_CMD_Delete_group,//2.2.11 ɾ����Delete group
	ZIGBEE_APPLICAITON_CMD_Query_group,//2.2.12 ��ѯ��Query group
	
	ZIGBEE_APPLICAITON_CMD_MAX,
}ENUM_ZIGBEE_APPLICAITON_CMD_TYPE;
#define FEEDBACK_OK 0X00  // ����ִ�н��ok

/*
						application 2.1 ֡��ʽ˵��								
����									
SOF	LEN	CMD	AddrMode	nwkAddr0	nwkAddr1	CMD	seqID	Data0	Data1	Data2
0xFE	0x09	0x01	0x02	0x34	0x12	0x01	0xFE	0x31	0x32	0x33

SOF��	֡ͷ ����Ӧ�ù���֡ͷΪ 0xFE						
LEN��	���η������ݵ��ܳ��ȣ���ǰ�汾 ��֡����͸����������Ϊ80��		

CMD��	Ӧ�ù���ѡ�����							
	0x00�� ��ʾ ��ѯָ��mac��ַ�豸�������ַ��				
	0x01����ʾ ���д���͸������	
	
Mode��	���ݷ��ͷ�ʽ��							
	0x01 - �鲥��ʽ�������ַΪĿ����Id					
	0x02 - ������ʽ								
	0x0F - �㲥��ʽ���㲥�����ַΪ0xFFFF			
	
nwkAddr�������ַ����2���ֽڣ����ֽ���ǰ����ʾ������ʾ�����ַΪ0x1234	
seqID��	ʱ����������û���������Ҫ���������ظ����ݰ�������ÿ����һ�������ۼ�1	
Data��	͸�����û��Զ�������	
*/

/*		���ù���_Э����   �Ĵ�		*/
#define ZIGBEE_DATA_SOF_CONFIG_Coordinator 0xFC   // ���ù���_Э����
#define ZIGBEE_DATA_SOF_APPLICAITON 0xFE   // ֡ͷ ����Ӧ�ù���֡ͷΪ 0xFE
#define ZIGBEE_DATA_LENTH_POS 1

/*
	CMD��  Ӧ�ù���ѡ�����	   
	0x00�� ��ʾ ��ѯָ��mac��ַ�豸�������ַ�� 	   
	0x01����ʾ ���д���͸������ 	   
*/
#define ZIGBEE_CMDtoModule 0x00//0x00�� ��ʾ ��ѯָ��mac��ַ�豸�������ַ�� 		   
#define ZIGBEE_DATAtoModule 0x01//0x01����ʾ ���д���͸������    

/*
	Mode��	���ݷ��ͷ�ʽ��		
	0x01 - �鲥��ʽ�������ַΪĿ����Id		
	0x02 - ������ʽ		
	0x0F - �㲥��ʽ���㲥�����ַΪ0xFFFF		
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
