
#include "app.h"

#include "app_update_4g.h"

//ָ��UPDATEģ��
typedef struct UPDATE_T * UPDATE_Handle;

#define UPDATE_SET_DATA_GESHU 16//spi flash data lenth
#define PCP_DATA_tx_count_max  3

#define UPDATE_DATA_PACKET_SIZE 1024//data_packet_size �������ݰ��Ĵ�С
#define UPDATE_RESULTCODE_OK (0X01)

static u8 flash_data_buffer[16];

UPDATE_4G_T gUpdate4G;

FTP_T gFTPtxData;//��������
FTP_T gFTPrxData;//��������

/**
 * 
 * @param ���У��
 * @param message У��ֵ
 * @return
 */
 static uint8_t do_xor(char* message,uint16_t length) 
 {
    char xor_reg = 0;
    
    for (uint16_t i = 0; i < length; i++) 
    {
        xor_reg ^= message[i];
    }
    return (uint8_t)xor_reg;
}



/*
*********************************************************************************************************
*   �� �� ��: app_update4G_configSaveIntoFlash
*   ����˵��:     
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_update4G_configSaveIntoFlash(void)
{
    u8 l_sum = 0;
    u8* l_point = 0;
    u8 i = 0;

    l_point = (u8 *)&gUpdate4G.safe_flag;
    
    /*����ǰ6�������ۼ�*/
    l_sum = 0;
    for(i=0;i<(UPDATE_SET_DATA_GESHU-1);i++)
    {
        l_sum = l_sum + l_point[i];
    }
    gUpdate4G.sum_flag = l_sum;// �����ۼӺ͡�

    /*���浽spi flash����*/
    /*���浽spi flash����*/
    Bsp_spiflash_power_on();//��gps��Դ
    bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
    sf_WriteBuffer((u8 *)&gUpdate4G.safe_flag, SPI_FLASH_START_ADDR_VAR, UPDATE_SET_DATA_GESHU);
    Bsp_spiflash_power_off();   

    /* ��flash�п�����Ӧ����Ϣ����*/

    /* ��ӡ��ÿ����������Ϣ*/
    p_info("safe_flag:0X%04X",gUpdate4G.safe_flag);

}

/*
*********************************************************************************************************
*   �� �� ��: app_update4G_configReadFromFlash
*   ����˵��:     ���浽spi flash�еĲ���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/

void app_update4G_configReadFromFlash(void)
{
    u8 i = 0;
    u8 l_sum = 0;

    memset(flash_data_buffer,0,UPDATE_SET_DATA_GESHU);

    /*��ȡUPDATE�Ĳ���*/
    Bsp_spiflash_power_on();//��gps��Դ
    bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);
    sf_ReadBuffer(flash_data_buffer,SPI_FLASH_START_ADDR_VAR,UPDATE_SET_DATA_GESHU);
    Bsp_spiflash_power_off();   
        
    /*����ǰ6�������ۼ�*/
    l_sum = 0;
    for(i=0;i<(UPDATE_SET_DATA_GESHU-1);i++)
    {
        l_sum = l_sum + flash_data_buffer[i];
    }
    /*�ж��ۼӺ��Ƿ���ȷ*/
    if((l_sum == flash_data_buffer[UPDATE_SET_DATA_GESHU-1])&&(flash_data_buffer[UPDATE_SET_DATA_GESHU-1] != 0xFF))
    {
        memcpy((u8 *)&gUpdate4G.safe_flag,flash_data_buffer,UPDATE_SET_DATA_GESHU);
        p_info("gUpdate4G�ۼӺ���ȷ  ");
    }
    else
    {   /*��ʼ���汾��*/
        memset((u8 *)&gUpdate4G.safe_flag,0,UPDATE_SET_DATA_GESHU);

        
        gUpdate4G.tx_data_count = 0;
        
        p_info("�ۼӺͲ���ȷ  new");
    }
    
    gUpdate4G.update_workstate = UPDATE_4G_INIT;
    app_update4G_configSaveIntoFlash();//�����ۼӺͣ����浽flash
}

/*
*********************************************************************************************************
*   �� �� ��: ��������У��
*   ����˵��:  true  is ok
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
u8 app_update4G_paramCheck(void)
{
    uint16 len = 0;
    len = strlen(gUpdate4G.ipAddress);
    if((len>=11 )&&(len<= 32))
    {

    }
    else
    {
        return FALSE;
    }

    len = strlen(gUpdate4G.port);
    if((len>=2 )&&(len<= 32))
    {

    }
    else
    {
        return FALSE;
    }    

    len = strlen(gUpdate4G.name);
    if((len>=2 )&&(len<= 32))
    {

    }
    else
    {
        return FALSE;
    }        

    len = strlen(gUpdate4G.passWord);
    if((len>=2 )&&(len<= 32))
    {

    }
    else
    {
        return FALSE;
    }            

    len = strlen(gUpdate4G.root);
    if((len>=4)&&(len<= 32))
    {

    }
    else
    {
        return FALSE;
    }            
    return TRUE;
}

/*
*********************************************************************************************************
*   �� �� ��: app_update4G_initVar
*   ����˵��:  
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_update4G_initVar(void)
{
    app_update4G_configReadFromFlash();/*�ӵ�ַ512��ȡ����*/
    
    gUpdate4G.update_workstate = UPDATE_4G_INIT;
    gUpdate4G.tx_data_count = 0;

    gUpdate4G.rx_data_ok = 0;
    gUpdate4G.rx_data_not_ok= 0;

    gUpdate4G.data_packet_size = UPDATE_DATA_PACKET_SIZE;//��Ƭ����С��500�ֽڣ�
    gUpdate4G.data_packet_cur = 1;//�ӵ�һ����ʼ

    strcpy(gUpdate4G.ipAddress,"192.144.233.218");
    strcpy(gUpdate4G.port,"1921");
    strcpy(gUpdate4G.name,"HSC3");
    strcpy(gUpdate4G.passWord,"hsc3_20200610#");
    strcpy(gUpdate4G.root,"root/zhihuiwenshiV3.bin");
}

/*
*********************************************************************************************************
*   �� �� ��: App_PCP_RXDataFrame_process
*   ����˵��:    analyse rx data
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static u8 updateRxData[UPDATE_DATA_PACKET_SIZE]; 

void app_update4G_rxDataFrameAnalyse(uint16_t len,char* msg,FTP_T * pFTPrxData)
{
    uint8_t result_code ;// �����

    /*xor   */
    uint8_t xor_calu = 0x00;
    uint8_t xor_rx = 0x00;
    
    xor_calu = do_xor(msg,len-1);
    /*xor_rx */
    xor_rx = msg[len-1];
    if(xor_calu != xor_rx)
    {
        p_err("xor is no ok! ");
        return;
    }

    pFTPrxData->starting_mark[0] =* (msg+0);
    pFTPrxData->starting_mark[1] =* (msg+1);
    pFTPrxData->starting_mark[2] =* (msg+2);
    
    pFTPrxData->dataLength =(* (msg+3)<<8) |* (msg+4);
    pFTPrxData->version_number =* (msg+5);
    pFTPrxData->message_code =(MSGCODE_4G_E)* (msg+6);
    
    pFTPrxData->check_code =* (msg+len-1);


    memset(&pFTPrxData->Data[0],0x00,512);
    
    switch(pFTPrxData->message_code)
    {
        case MSGCODE_4G_EnterFTPUpgradeMode:
        {
            memcpy(&pFTPrxData->resultCode[0],(msg+7),5);
        }
            break;
        case MSGCODE_4G_SetIPAndPort:
        {
            memcpy(&pFTPrxData->resultCode[0],(msg+7),5);
        }
            break;
        case MSGCODE_4G_SetFTPNameAndPassword:
        {
            memcpy(&pFTPrxData->resultCode[0],(msg+7),5);
        }
            break;
        case MSGCODE_4G_SetFilePathAndDownloadData:
        {
            pFTPrxData->resultCode[0] = * (msg+7);
            pFTPrxData->upgradeTotalSize = (* (msg+10)<<8)|msg[11];//�ļ��ܵĴ�С 
        }
            break;

//        case MsgCode_New_version_notification:
//        {
//            memcpy(&pFTPrxData->new_version[0], &msg[PCP_HEADER_DATA_LENTH],PCP_DATA_version_lenth);
//            
//            pFTPrxData->upgrade_package_size =( (msg[24]<<8) |msg[25]);
//            pFTPrxData->upgrade_package_total_number =( (msg[26]<<8) |msg[27]);
//            pFTPrxData->upgrade_package_Check_Code =( (msg[28]<<8) |msg[29]);
//
//            gUpdate4G.data_packet_total = pFTPrxData->upgrade_package_total_number ;
//            gUpdate4G.data_packet_size = pFTPrxData->upgrade_package_size ;
//            gUpdate4G.data_packet_total_crc = pFTPrxData->upgrade_package_Check_Code ;
//
//        }
//            break;
        case MSGCODE_4G_DataAcquisition:
        {
            pFTPrxData->resultCode[0] = * (msg+7);
            
            u16 data_packet_cur;     //��ǰ�����ݰ�
            u16 updateRxData_lenth = 0 ;
            u32 spi_flash_address = 0 ;
            u16 pos = 0;
            
            result_code =pFTPrxData->resultCode[0];
            gUpdate4G.data_packet_total = (msg[8]<<8)|(msg[9]);
            data_packet_cur = (msg[10]<<8)|(msg[11]);

            if(result_code == 0x01)
            {
                /* ��ͬ�����жϴ�0��ʼ*/
                if(gUpdate4G.data_packet_cur == data_packet_cur)
                {           
                    memset(updateRxData,0xff,sizeof(updateRxData));
                    updateRxData_lenth = 0 ;
                    pos = 12;
                    for(uint16_t len=0;len<gUpdate4G.data_packet_size ;len++)
                    {                   
                        updateRxData[updateRxData_lenth] = msg[pos++];
                        //printf("%02X ",updateRxData[updateRxData_lenth]);
                        
                        updateRxData_lenth++;
                    }
                    /*��ȡUPDATE�Ĳ���*/
                    Bsp_spiflash_power_on();//��gps��Դ
                    bsp_DelayMS(SPI_FLASH_POWER_ON_DELAY);  
                    
                    spi_flash_address = SPI_FLASH_code_ADDR+gUpdate4G.data_packet_size *(gUpdate4G.data_packet_cur-1);//computer address
                    if(sf_WriteBuffer(updateRxData, spi_flash_address, gUpdate4G.data_packet_size) == 1)
                    {
                        p_err("rx_data_ok  ���յ���%d��",gUpdate4G.data_packet_cur);
                    
                        gUpdate4G.rx_data_ok = 1;
                        gUpdate4G.data_packet_cur += 1;
                    }   
                    else
                    {
                        p_err("sf_WriteBuffer error ");
                        gUpdate4G.rx_data_not_ok = 1;
                    }
                    Bsp_spiflash_power_off();   
                                            
                }
                else
                {
                    gUpdate4G.rx_data_not_ok = 1;
                }
            }
            else if(result_code == 0x80)
            {
                p_err("result_code  error �������񲻴���");
            }
            else if(result_code == 0x81)
            {
                p_err("result_code  error ָ���ķ�Ƭ������");
            }           
        }
            break;

        default:
            break;
    }

}

/*
*********************************************************************************************************
*   �� �� ��: app_FTP_tXDataFrameInitVar
*   ����˵��:    ��ʼ�����͵�����֡
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_FTP_tXDataFrameInitVar(MSGCODE_4G_E message_code)
{
    strcpy(gFTPtxData.cur_version, VERSION_WENSHI_cur);

    gFTPtxData.starting_mark[0] = FTP_DATA_startingMark1;
    gFTPtxData.starting_mark[1] = FTP_DATA_startingMark2;
    gFTPtxData.starting_mark[2] = FTP_DATA_startingMark3;
    gFTPtxData.dataLength = 5;
    gFTPtxData.version_number = 0x01;

    gFTPtxData.message_code = message_code;
    gFTPtxData.check_code = 0x00;

    memset(&gFTPtxData.Data[0],0,512);
    switch(message_code)
    {
        case MSGCODE_4G_ExitUpgradeMode:
        {
            gFTPtxData.DataLength = 0;
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;
        case MSGCODE_4G_EnterFTPUpgradeMode:
        {
            gFTPtxData.DataLength = 0;
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;        
        case MSGCODE_4G_SetIPAndPort:
        {
            uint16 dataLen = 0;
            strcpy(gFTPtxData.Data , gUpdate4G.ipAddress);
            dataLen = strlen(gFTPtxData.Data);
            gFTPtxData.Data[dataLen] = 0x3A;// IP ��ַ�������� ASCII ��ʽ,�˿�Ҳ�� ASCII �룬  �˿��� IP �м��ã�    (0x3A)  ������
            strcpy(&gFTPtxData.Data[dataLen+1] , gUpdate4G.port);
            dataLen = strlen(gUpdate4G.ipAddress)+strlen(gUpdate4G.port)+1;
            gFTPtxData.DataLength = dataLen;
            
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;
        case MSGCODE_4G_SetFTPNameAndPassword:
        {
            uint16 dataLen = 0;
            strcpy(gFTPtxData.Data , gUpdate4G.name);
            dataLen = strlen(gFTPtxData.Data);
            gFTPtxData.Data[dataLen] = 0x00;//�˺ź������� ASCII ����ʽ��ʾ,�˺������� 0x00 �ָ���
            strcpy(&gFTPtxData.Data[dataLen+1] , gUpdate4G.passWord);
            dataLen = strlen(gUpdate4G.name)+strlen(gUpdate4G.passWord)+1;
            gFTPtxData.DataLength = dataLen;
            
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;
        case MSGCODE_4G_SetFilePathAndDownloadData:
        {
            uint16 dataLen = 0;
            strcpy(gFTPtxData.Data , gUpdate4G.root);
            dataLen = strlen(gUpdate4G.root);
            gFTPtxData.DataLength = dataLen;
            
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;
        case MSGCODE_4G_DataAcquisition:
        {
            uint16 dataLen = 0;
            gFTPtxData.Data[dataLen] = gUpdate4G.data_packet_size>>8;       dataLen++;
            gFTPtxData.Data[dataLen] = (char)gUpdate4G.data_packet_size>>0; dataLen++;
            gFTPtxData.Data[dataLen] = gUpdate4G.data_packet_cur>>8;        dataLen++;
            gFTPtxData.Data[dataLen] = (char)gUpdate4G.data_packet_cur>>0;  dataLen++;
            
            gFTPtxData.DataLength = dataLen;
            
            gFTPtxData.dataLength = gFTPtxData.dataLength + gFTPtxData.DataLength ;
        }
            break;        
        default:
            break;
    }
}

/*
*********************************************************************************************************
*   �� �� ��: app_update4G_txDataUart
*   ����˵��:   �����������ݸ�ƽ̨
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static char json_buf[512];
static uint16_t json_len= 0;


void app_update4G_txDataUart( json_info_t *p_json_info,FTP_T * p_PCP_TXData)
{
    u16 lenth = 0;

    /* zero data*/
    memset(&json_buf[0],0,sizeof(json_buf));
    json_len = lenth;
    
    /*starting_mark copy*/
    json_buf[lenth] = (uint8_t)(p_PCP_TXData->starting_mark[0]);
    lenth= lenth + 1;
    json_buf[lenth] = (uint8_t)(p_PCP_TXData->starting_mark[1]);
    lenth= lenth + 1;   
    json_buf[lenth] = (uint8_t)(p_PCP_TXData->starting_mark[2]);
    lenth= lenth + 1;   
    
    /*dataLength copy*/
    json_buf[lenth] = (uint8_t)(p_PCP_TXData->dataLength>>8);
    lenth= lenth + 1;
    json_buf[lenth] = (uint8_t)(p_PCP_TXData->dataLength&0x00ff);
    lenth= lenth + 1;   
    
    /*version_number copy*/
    json_buf[lenth] = p_PCP_TXData->version_number ;
    lenth= lenth + 1;   
    
    /*message_code copy*/
    json_buf[lenth] = p_PCP_TXData->message_code ;
    lenth= lenth + 1;   
    
    /*data copy*/
    memcpy(&json_buf[lenth], &p_PCP_TXData->Data[0],p_PCP_TXData->DataLength);     
    lenth= lenth + p_PCP_TXData->DataLength;       
    
    /*check_code copy*/
    p_PCP_TXData->check_code = do_xor(&json_buf[0],lenth);
    json_buf[lenth] = p_PCP_TXData->check_code;
    lenth= lenth + 1;

    json_len = lenth;
    app_4G_sendData(&json_buf[0],&json_len);
}


/*
*********************************************************************************************************
*   �� �� ��: app_update4G_txDataUart
*   ����˵��:    ��������
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_update4G_FTPtxData(MSGCODE_4G_E message_code)
{
    app_FTP_tXDataFrameInitVar(message_code); 
    app_update4G_txDataUart(&json_info,&gFTPtxData);
    gUpdate4G.rx_data_in_flag = FALSE;
}
#define TIMER_UPDATE_2SECONDER TMR_DELAY_2S
#define TIMER_UPDATE_60SECONDER TMR_DELAY_60S
/*
*********************************************************************************************************
*   �� �� ��: app_update4G_errExit
*   ����˵��:  
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_update4G_errExit(void)
{
    /* ��ʱ3�β����ϱ���*/
    if(gUpdate4G.tx_data_count >= 3)
    {
        gUpdate4G.tx_data_count = 0;
        gUpdate4G.update_workstate = UPDATE_4G_ExitUpgradeMode;
        p_err(" app_update4G_errExit");
        bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
    }           
}
/*
*********************************************************************************************************
*   �� �� ��: app_update4G_loop
*   ����˵��:  
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_update4G_loop(void)
{
    if(seq_record & NB_SEQ_INIT)
    {
        //p_info("REPORT_CAIJI_LOOP ��ʼ��������");   
    }
    else
    {
        //p_err("App_Update_Loop,������");
        return;
    }   

    switch(gUpdate4G.update_workstate)
    {
        case UPDATE_4G_INIT :
        {               
            bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_60S);//1min֮�� ���ϱ�������״̬
            if(gUpdate4G.safe_flag == SPI_FLASH_UPDATE_IS_OK)
            {
                gUpdate4G.update_workstate = UPDATE_4G_ReportUpgradeResults;
            }
            else
            {
                gUpdate4G.update_workstate = UPDATE_4G_ReportCurrentDoftwareVersionNumber;
            }
            p_info("App_Update_Loop UPDATE_INIT");
            
        }
            break;      
        case UPDATE_4G_ReportUpgradeResults :
        {       
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, 1);
            
                app_deviceState_eventReport(EVENT_REPORT_UPDATERESULT); // �ϱ��������
                gUpdate4G.tx_data_count++;
                p_info("tx MsgCode_Report_upgrade_results");
            }
            /* ��ʱ2�β����ϱ���*/
            if(gUpdate4G.tx_data_count >= 2)
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, 1);
                gUpdate4G.update_workstate = UPDATE_4G_ReportCurrentDoftwareVersionNumber;
                gUpdate4G.safe_flag = SPI_FLASH_UPDATE_IS_init;
                app_update4G_configSaveIntoFlash();//save                  
                p_err(" MsgCode_Report_upgrade_results tx_data_count max");
            }           

        }
            break;      
        case UPDATE_4G_ReportCurrentDoftwareVersionNumber :
        {       
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                app_deviceState_eventReport(EVENT_REPORT_VERSION); // �ϱ�version
                p_info("tx EVENT_REPORT_VERSION");
                gUpdate4G.update_workstate = UPDATE_4G_WAIT_START;
            }
        }
            break;
        case UPDATE_4G_WAIT_START ://                           �ȴ���������
        {
            if(gUpdate4G.updateStart == TRUE)
            {
                bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                gUpdate4G.update_workstate = UPDATE_4G_EnterFTPUpgradeMode;
                
                gUpdate4G.data_packet_size = UPDATE_DATA_PACKET_SIZE;//��Ƭ����С
                gUpdate4G.data_packet_cur = 1;//�ӵ�һ����ʼ
                
                gUpdate4G.tx_data_count = 0;
            }
        }
            break;
        case UPDATE_4G_EnterFTPUpgradeMode :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_2SECONDER);
                app_update4G_FTPtxData(MSGCODE_4G_EnterFTPUpgradeMode); // MSGCODE_4G_EnterFTPUpgradeMode
                p_info("tx  MSGCODE_4G_EnterFTPUpgradeMode");
                
                gUpdate4G.tx_data_count++;
            }
            if(gUpdate4G.rx_data_in_flag == TRUE)
            {
                gUpdate4G.rx_data_in_flag = FALSE;
                if((gFTPrxData.message_code == MSGCODE_4G_EnterFTPUpgradeMode)&&(gFTPrxData.resultCode[0] == UPDATE_RESULTCODE_OK))
                {
                    bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                    gUpdate4G.update_workstate = UPDATE_4G_SetIPAndPort;
                    gUpdate4G.tx_data_count = 0;
                }
            }
            /* ��ʱ error ����*/
            app_update4G_errExit();
        }
            break;
        case UPDATE_4G_SetIPAndPort :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_20S);
                app_update4G_FTPtxData(MSGCODE_4G_SetIPAndPort); // MSGCODE_4G_SetIPAndPort
                p_info("tx UPDATE_4G_SetIPAndPort");
                gUpdate4G.tx_data_count++;
            }
            if(gUpdate4G.rx_data_in_flag == TRUE)
            {
                gUpdate4G.rx_data_in_flag = FALSE;
                if((gFTPrxData.message_code == MSGCODE_4G_SetIPAndPort)&&(gFTPrxData.resultCode[0] == UPDATE_RESULTCODE_OK))
                {
                    bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                    gUpdate4G.update_workstate = UPDATE_4G_SetFTPNameAndPassword;
                    gUpdate4G.tx_data_count = 0;
                }
            }
            /* ��ʱ error ����*/
            app_update4G_errExit();
        }
            break;
        case UPDATE_4G_SetFTPNameAndPassword :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_20S);
                app_update4G_FTPtxData(MSGCODE_4G_SetFTPNameAndPassword); // MSGCODE_4G_SetFTPNameAndPassword
                p_info("tx UPDATE_4G_SetFTPNameAndPassword");
                gUpdate4G.tx_data_count++;
            }
            if(gUpdate4G.rx_data_in_flag == TRUE)
            {
                gUpdate4G.rx_data_in_flag = FALSE;
                if((gFTPrxData.message_code == MSGCODE_4G_SetFTPNameAndPassword)&&(gFTPrxData.resultCode[0] == UPDATE_RESULTCODE_OK))
                {
                    bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                    gUpdate4G.update_workstate = UPDATE_4G_SetFilePathAndDownloadData;
                    gUpdate4G.tx_data_count = 0;
                }
            }
            /* ��ʱ error ����*/
            app_update4G_errExit();
        }
            break;
        case UPDATE_4G_SetFilePathAndDownloadData :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_20S);
                app_update4G_FTPtxData(MSGCODE_4G_SetFilePathAndDownloadData); // MSGCODE_4G_SetFilePathAndDownloadData
                p_info("tx UPDATE_4G_SetFilePathAndDownloadData");
                gUpdate4G.tx_data_count++;
            }
            if(gUpdate4G.rx_data_in_flag == TRUE)
            {
                gUpdate4G.rx_data_in_flag = FALSE;
                if((gFTPrxData.message_code == MSGCODE_4G_SetFilePathAndDownloadData)&&(gFTPrxData.resultCode[0] == UPDATE_RESULTCODE_OK))
                {
                    bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_5S);// ��������Ӧ���� 0xA3 ���������� 5s ��ʱ�䣬��֤�ļ�������ɡ�
                    gUpdate4G.update_workstate = UPDATE_4G_DataAcquisition;
                    gUpdate4G.tx_data_count = 0;
                }
            }
            /* ��ʱ error ����*/
            app_update4G_errExit();
        }
            break;
        case UPDATE_4G_DataAcquisition :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_2SECONDER);
                app_update4G_FTPtxData(MSGCODE_4G_DataAcquisition); // MSGCODE_4G_DataAcquisition
                gUpdate4G.rx_data_ok = 0;
                gUpdate4G.rx_data_not_ok= 0;
                p_info("tx UPDATE_4G_DataAcquisition%d��",gUpdate4G.data_packet_cur);
            }
            if(gUpdate4G.rx_data_ok)
            {
                gUpdate4G.rx_data_ok = 0;
            
                bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                /*�Ѿ���д��spi flahs*/

                /*�������ݰ� �������������*/
                if(gUpdate4G.data_packet_cur > gUpdate4G.data_packet_total)
                {
                    /*�������֮�󣬱��ֱ�־��flash��*/
                    gUpdate4G.safe_flag = SPI_FLASH_UPDATE_NEEDED;
                    app_update4G_configSaveIntoFlash();//save                                              
                    
                    gUpdate4G.update_workstate = UPDATE_4G_ExitUpgradeMode;
                    bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                    p_info("down load over !");
                    gUpdate4G.tx_data_count = 0;
                }
            }
            /*���յ����ݰ���������Ų��ԣ������·�������*/
            if(gUpdate4G.rx_data_not_ok)
            {
                gUpdate4G.rx_data_not_ok= 0;
            
                bsp_SetTimer(TMR_ID_UPDATE_TX_DATA);
                p_info("App_Update_Loop rx_data_not_ok ! ");
            }            
        }
            break;        
        case UPDATE_4G_ExitUpgradeMode :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TIMER_UPDATE_2SECONDER);
                app_update4G_FTPtxData(MSGCODE_4G_ExitUpgradeMode); // 
                p_info("tx UPDATE_4G_ExitUpgradeMode");
                gUpdate4G.tx_data_count++;
            }
            if(gUpdate4G.rx_data_in_flag == TRUE)
            {
                gUpdate4G.rx_data_in_flag = FALSE;
                if((gFTPrxData.message_code == MSGCODE_4G_ExitUpgradeMode)&&(gFTPrxData.resultCode[0] == UPDATE_RESULTCODE_OK))
                {
                    bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_2S);
                    gUpdate4G.update_workstate = UPDATE_4G_ReportEventAndDownloadStatus;
                }
            }
            /* ��ʱ error ����*/
            /* ��ʱ3�β����ϱ���*/
            if(gUpdate4G.tx_data_count >= 3)
            {
                gUpdate4G.tx_data_count = 0;
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_2S);
                gUpdate4G.update_workstate = UPDATE_4G_ReportEventAndDownloadStatus;
            }
        }
            break;
        case UPDATE_4G_ReportEventAndDownloadStatus :
        {
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                bsp_StartTimer(TMR_ID_UPDATE_TX_DATA, TMR_DELAY_1S);
                app_deviceState_eventReport(EVENT_REPORT_DownloadStatus); //��������������¼��ϱ� updateDownload,0
                gUpdate4G.update_workstate = UPDATE_4G_RESTART;
                gUpdate4G.updateStart = FALSE;
                p_info("tx UPDATE_4G_ReportEventAndDownloadStatus");
            }
        }
            break;        
        case UPDATE_4G_RESTART:
        {       
            if(bsp_CheckTimer(TMR_ID_UPDATE_TX_DATA))
            {
                p_info("UPDATE_OK_RESTART");
                while(1)
                {
                    __set_FAULTMASK(1);      // �ر������ж�                          
                    NVIC_SystemReset();//�����λ
                }
            }       
            break;
        }
        default:
            break;

    }
}

