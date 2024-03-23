#ifndef _APP_UPDATE_H_
#define _APP_UPDATE_H_


#define SPI_FLASH_UPDATE_NEEDED 0x5aa5
#define SPI_FLASH_UPDATE_IS_OK 0xAAAA
#define SPI_FLASH_UPDATE_IS_init 0xFFFF

/*
spi flash  ���ݵķֲ�ͼ

��1ҳ		0��ʼ 512��ʼ

��2ҳ		1024��ʼ
��3ҳ		1024��ʼ
��4ҳ		1024��ʼ

*/
#define SPI_FLASH_EACH_PAGE_SIZE 1024

#define SPI_FLASH_START_ADDR_VAR 0 // ���������Ĵ洢��ַ
#define SPI_FLASH_onoff_ADDR_VAR 256 // ���Բ����Ĵ洢��ַ��g_Indicat����λԭ��

/*  spi flash address*/
#define SPI_FLASH_ZIGBEE_ADDR_VAR 512 // ZIGBEE�����Ĵ洢��ַ

#define SPI_FLASH_code_ADDR  1024 // ��������Ĵ洢��ַ������bin��λ��

/*
�ϱ���ǰ����汾��
Report_current_software_version_number.
�°汾֪ͨ
New_version_notification
����������
Request_upgrade_package
�ϱ�����������״̬
Escalation_package_download_status
ִ������
Perform_an_upgrade
�ϱ��������
Report_upgrade_results
*/

typedef enum 
{
	UPDATE_INIT=0,// ��ʼ��̬
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
Զ������	
��������״̬				0x93	1	0-2	0��δ����  1������   2--������
���������µĹ̼��汾	0x94	3		
�̼��汾�Ĵ�С			0x95	4		�̼�����Ĵ�С�������ֽ���
ÿһ���̼�����Ĵ�С	0x96	2		ÿ�δ��͹̶��Ĵ�С������500�ֽڡ�
�̼�������ʼ����		0x97	1	0-1	0��ֹͣ�������� 
						1����ʼ��������   
*/
typedef struct
{
	u16 safe_flag;// 0x5a 0xa5

	//u32 version_size;//�̼��汾�Ĵ�С		
	
	u16 data_packet_total;//�ܰ��� 500������
	u16 data_packet_cur; //��ǰ��
	u16 data_packet_size; //ÿ���Ĵ�С 500
	
	u16 data_packet_total_crc; //�ܰ���crc

	u8  update_workstate;//������������״̬
	
	u8  rx_data_ok;//������������״̬
	u8  rx_data_not_ok;//������������״̬
	
	//u8  update_start_on;//��������״̬
	//u8  update_state;//��������״̬


	u8  rx_data_in_flag;//rx data 
	u8  tx_data_count;//tx data 


	u8 sum_flag;// �ۼӺ�
	
	/*cpu  flash address*/
	u32 cpu_flash_all_total_chars; 	//�ܵ��ֽ�����
	//u16 cpu_flash_each_page_chars;  //ÿ���Ĵ�С2048	
	u16 cpu_flash_cur_page_num; 	//��ǰpage
	//u16 cpu_flash_cur_address; //��ǰcpu flash address
	/*spi  flash address*/
	//u16 spi_flash_cur_page; //��ǰspi flash address
	u16 spi_flash_cur_page_start_address; //��ǰspi flash address��ʼ��ַ
	//u16 spi_flash_cur_page_size; //��ǰspi flash address ��С


}
UPDATE_T;
/*
��Ϣ�붨�壺
��Ϣ��	����
0-18	ƽ̨Ԥ��
19	��ѯ�豸�汾
20	�°汾֪ͨ
21	����������
22	�ϱ�����������״̬
23	ִ������
24	�ϱ��������
25-127	ƽ̨Ԥ��
*/

typedef enum
{
  MsgCode_Platform_reservation = 18,//ƽ̨Ԥ��
  MsgCode_Query_device_version ,//0x13
  MsgCode_New_version_notification ,//0x14
  MsgCode_Request_upgrade_package ,//0x15
  MsgCode_Escalation_package_download_status ,//0x16
  MsgCode_Perform_an_upgrade,//0x17
  MsgCode_Report_upgrade_results,//0x18
  MsgCode_Platform_reservation2	//ƽ̨Ԥ��
}msgCode_e;


#define PCP_DATA_LENTH 512

#define PCP_HEADER_DATA_LENTH 8 //�̶���֡���ݳ���

#define PCP_DATA_starting_mark1  0XFF
#define PCP_DATA_starting_mark2  0XFE

#define PCP_DATA_pos_crc_h  4
#define PCP_DATA_pos_crc_l  5

#define PCP_DATA_version_lenth  16


#define PCP_DATA_tx_count_max  3

/*
Զ������	
��������״̬				0x93	1	0-2	0��δ����  1������   2--������
���������µĹ̼��汾	0x94	3		
�̼��汾�Ĵ�С			0x95	4		�̼�����Ĵ�С�������ֽ���
ÿһ���̼�����Ĵ�С	0x96	2		ÿ�δ��͹̶��Ĵ�С������500�ֽڡ�
�̼�������ʼ����		0x97	1	0-1	0��ֹͣ�������� 
						1����ʼ��������   
*/
typedef struct
{
	uint16_t starting_mark;// ��ʼ��ʶ���̶�Ϊ0XFFFE
	uint8_t version_number ;// �汾��
	msgCode_e message_code;//��Ϣ��		
	uint16_t check_code ;//У����
	
	uint16_t Data_length ;///����������
	char Data[PCP_DATA_LENTH];//������  n�ֽڵ�ʮ��������

	uint8_t result_code ;// �����

	char cur_version[PCP_DATA_version_lenth];
	char new_version[PCP_DATA_version_lenth];
	
	uint16_t upgrade_package_size;//ÿ����Ƭ�Ĵ�С
	uint16_t upgrade_package_total_number;//��������Ƭ����
	uint16_t upgrade_package_Check_Code ;//��������Ƭ����
		
}
PCP_T;


extern UPDATE_T g_Update;
extern PCP_T g_PCP_TXData;//��������
extern PCP_T g_PCP_RXData;//��������
extern u8 update_rx_data[PCP_DATA_LENTH]; 

extern void App_Update_Init_Var(void);
extern void App_Update_Loop(void);
extern void App_PCP_RXDataFrame_analyse(uint16_t len,char* msg,PCP_T * p_PCP_RXData);
extern void App_Update_Check_update_or_goto_app(void);
extern void BootLoad_Jump(void);

void App_Update_Config_Save_Into_Flash(void);
void App_Update_Config_read_from_flash(void);

#endif






