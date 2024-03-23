#ifndef _APP_UPDATE_4G_H_
#define _APP_UPDATE_4G_H_


#define FTP_DATA_startingMark1  0X55
#define FTP_DATA_startingMark2  0XFC
#define FTP_DATA_startingMark3  0XAA

/*
上报当前软件版本号
Report_current_software_version_number.
新版本通知
New_version_notification
请求升级包
Request_upgrade_package
上报升级包下载状态
Escalation_package_download_status
执行升级
Perform_an_upgrade
上报升级结果
Report_upgrade_results
*/

typedef enum 
{
    UPDATE_4G_INIT=0,// 初始化态
    UPDATE_4G_ReportUpgradeResults,// 上报升级结果
    UPDATE_4G_ReportCurrentDoftwareVersionNumber,//上报软件版本号

    UPDATE_4G_WAIT_START,//等待升级命令
/*
1、进入FTP升级模式命令
2、设置FTP目标IP域名与端口
3、设置FTP用户名密码设置
4、下载文件路径并下载数据获取
5、数据获取
6、退出升级模式命令
7、上报事件，下载状态

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
远程升级    
程序升级状态              0x93    1   0-2 0—未升级  1—升级   2--升级中
服务器最新的固件版本  0x94    3       
固件版本的大小           0x95    4       固件程序的大小，多少字节数
每一包固件程序的大小  0x96    2       每次传送固定的大小，例如500字节。
固件升级开始命令        0x97    1   0-1 0—停止升级命令 
                        1—开始升级命令   
*/
typedef struct
{
    u16 safe_flag;// 0x5a 0xa5

    //u32 version_size;//固件版本的大小      
    
    u16 data_packet_total;//总包数 500的数量
    u16 data_packet_cur; //当前包
    u16 data_packet_size; //每包的大小 500
    
    u16 data_packet_total_crc; //总包的crc

    u8  update_workstate;//程序升级工作状态
    
    u8  rx_data_ok;//程序升级工作状态
    u8  rx_data_not_ok;//程序升级工作状态
    
    //u8  update_start_on;//程序升级状态
    //u8  update_state;//程序升级状态


    u8  rx_data_in_flag;//rx data 
    u8  tx_data_count;//tx data 


    u8 sum_flag;// 累加和
    
    char ipAddress[32];
    char port[32];
    char name[32];
    char passWord[32];
    char root[32];
    u8 updateStart;//升级开始 
}
UPDATE_4G_T;
/*
消息码定义：
消息码   描述
进入 FTP 升级模式命令=0xA0
退出 FTP 升级模式命令=0XAF
设置 FTP 目标 IP 或域名与端口=0XA1
设置 FTP 用户名密码=0XA2
设置下载文件路径并下载=0XA3
数据获取=0XA4
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
    MSGCODE_4G_ExitUpgradeMode= 0XAF,// 退出 FTP 升级模式命令=0XAF
    MSGCODE_4G_EnterFTPUpgradeMode = 0xA0,// 进入 FTP 升级模式命令=0xA0
    MSGCODE_4G_SetIPAndPort = 0xA1,// 
    MSGCODE_4G_SetFTPNameAndPassword = 0xA2,// 
    MSGCODE_4G_SetFilePathAndDownloadData= 0xA3,
    MSGCODE_4G_DataAcquisition= 0xA4// 
}MSGCODE_4G_E;


/*
  返回结果由 5 字节组成，第一个字节是返回结果，其余字节各命令不同用途，主要用于表示文件长度或者包序列号，  高字节 在前，
  如果回复不设计长度或者序列号的话，该 4 字节不用处理。
例：
0x01 0xXX 0xXX 0xXX 0xXX 命令执行成功；
0x02 0xXX 0xXX 0xXX 0xXX 命令执行失败；
0x03 0xXX 0xXX 0xXX 0xXX 命令校验出错；
0x04 0xXX 0xXX 0xXX 0xXX 命令不存在；
如上，  0X02 开头的返回结果意味着命令执行失败 ，最后一个字节则表明了命令执行失败的具体错误原因：
0x01 服务器无法连接
0x02 用户名或密码错误
0x03 服务器异常断开
0x04 数据通道打开失败
0x05 设置数据类型失败
0x06 获取数据通道信息失败
0x07 超过最大请求长度，最大 2048 每包
0x08 超过最大包数，  即包数>实际文件大小/每包大小
0x09 前置步骤未执行
0x0A 申请内存空间失败

*/
typedef struct
{
    uint8_t starting_mark[3];// 固定为 0x55 0xFC 0xAA  包头(3)
    uint16_t dataLength ;///数据区长度 	长度(2)	
    uint8_t version_number ;// 版本号 协议版本(1)	
    MSGCODE_4G_E message_code;//消息码      命令(1)	
    char Data[512];//数据区  n字节的十六进制数  参数(n)	
    uint16_t DataLength ;// DataLength
    uint8_t check_code ;//校验码 校验(1) 

    uint8_t resultCode[5];//0x01 0xXX 0xXX 0xXX 0xXX 命令执行成功；
    uint16_t upgradeTotalSize;//文件大小有 2000 字节
    
    char cur_version[16];
    char new_version[16];
    
    uint16_t upgrade_package_size;//每个分片的大小
    uint16_t upgrade_package_total_number;//升级包分片总数
    uint16_t upgrade_package_Check_Code ;//升级包分片总数
        
}
FTP_T;


extern UPDATE_4G_T gUpdate4G;
extern FTP_T gFTPtxData;//发送数据
extern FTP_T gFTPrxData;//接收数据

extern void app_update4G_initVar(void);
extern void app_update4G_loop(void);
extern void app_update4G_rxDataFrameAnalyse(uint16_t len,char* msg,FTP_T * pFTPrxData);
extern u8 app_update4G_paramCheck(void);

#endif






