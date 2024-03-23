#ifndef _APP_UPDATE_4G_H_
#define _APP_UPDATE_4G_H_


#define FTP_DATA_startingMark1  0X55
#define FTP_DATA_startingMark2  0XFC
#define FTP_DATA_startingMark3  0XAA

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
    UPDATE_4G_INIT=0,// ��ʼ��̬
    UPDATE_4G_ReportUpgradeResults,// �ϱ��������
    UPDATE_4G_ReportCurrentDoftwareVersionNumber,//�ϱ�����汾��

    UPDATE_4G_WAIT_START,//�ȴ���������
/*
1������FTP����ģʽ����
2������FTPĿ��IP������˿�
3������FTP�û�����������
4�������ļ�·�����������ݻ�ȡ
5�����ݻ�ȡ
6���˳�����ģʽ����
7���ϱ��¼�������״̬

1.EnterFTPUpgradeMode
2.SetIPAndPort
3.SetFTPNameAndPassword
4.SetFilePathAndDownloadData
5. DataAcquisition
6. ExitUpgradeMode
7. ReportEventAndDownloadStatus

*/
    UPDATE_4G_EnterFTPUpgradeMode,
    UPDATE_4G_SetIPAndPort,
    UPDATE_4G_SetFTPNameAndPassword,
    UPDATE_4G_SetFilePathAndDownloadData,
    UPDATE_4G_DataAcquisition,
    UPDATE_4G_ExitUpgradeMode,
    UPDATE_4G_ReportEventAndDownloadStatus,
    UPDATE_4G_RESTART,
    UPDATE_4G_MAX,
}ENUM_UPDATE_4G;


/*
Զ������    
��������״̬              0x93    1   0-2 0��δ����  1������   2--������
���������µĹ̼��汾  0x94    3       
�̼��汾�Ĵ�С           0x95    4       �̼�����Ĵ�С�������ֽ���
ÿһ���̼�����Ĵ�С  0x96    2       ÿ�δ��͹̶��Ĵ�С������500�ֽڡ�
�̼�������ʼ����        0x97    1   0-1 0��ֹͣ�������� 
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
    
    char ipAddress[32];
    char port[32];
    char name[32];
    char passWord[32];
    char root[32];
    u8 updateStart;//������ʼ 
}
UPDATE_4G_T;
/*
��Ϣ�붨�壺
��Ϣ��   ����
���� FTP ����ģʽ����=0xA0
�˳� FTP ����ģʽ����=0XAF
���� FTP Ŀ�� IP ��������˿�=0XA1
���� FTP �û�������=0XA2
���������ļ�·��������=0XA3
���ݻ�ȡ=0XA4
Enter FTP upgrade mode command=0xA0
Exit FTP upgrade mode command=0XAF
Set FTP destination IP or domain name and port=0XA1
Set FTP username and password=0XA2
Set the download file path and download=0XA3
Data acquisition=0XA4

1.EnterFTPUpgradeMode
2.SetIPAndPort
3.SetFTPNameAndPassword
4.SetFilePathAndDownloadData
5. DataAcquisition
6. ExitUpgradeMode
*/
typedef enum
{
    MSGCODE_4G_ExitUpgradeMode= 0XAF,// �˳� FTP ����ģʽ����=0XAF
    MSGCODE_4G_EnterFTPUpgradeMode = 0xA0,// ���� FTP ����ģʽ����=0xA0
    MSGCODE_4G_SetIPAndPort = 0xA1,// 
    MSGCODE_4G_SetFTPNameAndPassword = 0xA2,// 
    MSGCODE_4G_SetFilePathAndDownloadData= 0xA3,
    MSGCODE_4G_DataAcquisition= 0xA4// 
}MSGCODE_4G_E;


/*
  ���ؽ���� 5 �ֽ���ɣ���һ���ֽ��Ƿ��ؽ���������ֽڸ����ͬ��;����Ҫ���ڱ�ʾ�ļ����Ȼ��߰����кţ�  ���ֽ� ��ǰ��
  ����ظ�����Ƴ��Ȼ������кŵĻ����� 4 �ֽڲ��ô���
����
0x01 0xXX 0xXX 0xXX 0xXX ����ִ�гɹ���
0x02 0xXX 0xXX 0xXX 0xXX ����ִ��ʧ�ܣ�
0x03 0xXX 0xXX 0xXX 0xXX ����У�����
0x04 0xXX 0xXX 0xXX 0xXX ������ڣ�
���ϣ�  0X02 ��ͷ�ķ��ؽ����ζ������ִ��ʧ�� �����һ���ֽ������������ִ��ʧ�ܵľ������ԭ��
0x01 �������޷�����
0x02 �û������������
0x03 �������쳣�Ͽ�
0x04 ����ͨ����ʧ��
0x05 ������������ʧ��
0x06 ��ȡ����ͨ����Ϣʧ��
0x07 ����������󳤶ȣ���� 2048 ÿ��
0x08 ������������  ������>ʵ���ļ���С/ÿ����С
0x09 ǰ�ò���δִ��
0x0A �����ڴ�ռ�ʧ��

*/
typedef struct
{
    uint8_t starting_mark[3];// �̶�Ϊ 0x55 0xFC 0xAA  ��ͷ(3)
    uint16_t dataLength ;///���������� 	����(2)	
    uint8_t version_number ;// �汾�� Э��汾(1)	
    MSGCODE_4G_E message_code;//��Ϣ��      ����(1)	
    char Data[512];//������  n�ֽڵ�ʮ��������  ����(n)	
    uint16_t DataLength ;// DataLength
    uint8_t check_code ;//У���� У��(1) 

    uint8_t resultCode[5];//0x01 0xXX 0xXX 0xXX 0xXX ����ִ�гɹ���
    uint16_t upgradeTotalSize;//�ļ���С�� 2000 �ֽ�
    
    char cur_version[16];
    char new_version[16];
    
    uint16_t upgrade_package_size;//ÿ����Ƭ�Ĵ�С
    uint16_t upgrade_package_total_number;//��������Ƭ����
    uint16_t upgrade_package_Check_Code ;//��������Ƭ����
        
}
FTP_T;


extern UPDATE_4G_T gUpdate4G;
extern FTP_T gFTPtxData;//��������
extern FTP_T gFTPrxData;//��������

extern void app_update4G_initVar(void);
extern void app_update4G_loop(void);
extern void app_update4G_rxDataFrameAnalyse(uint16_t len,char* msg,FTP_T * pFTPrxData);
extern u8 app_update4G_paramCheck(void);

#endif






