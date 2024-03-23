#include "bsp.h"
#include "app.h"

#include "app_4g.h"
#include "app_update_4g.h"

APP4G_STRCUT    g_4G;

/*
*********************************************************************************************************
*   函 数 名: app4G_DataRx_process
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
uint8_t app_4G_dataRx_process(int len,char* msg)
{
    uint8_t ret = 0;
    uint8_t i = 0;

    u8 pos = 0;
    u8 messageId = 0;
    uint16_t rxMid = 0;
    uint16_t rxServiceId = 0;
    uint16_t rxDataLenth = 0;
    uint16_t rxResult = 0;
    unsigned long gmt_time_server = 0;

    /* log */


    /*帧类型的判断*/
    messageId = msg[pos];
    pos++;
    p_info("messageId=%02X",messageId);

    switch(messageId)
    {
        case(APP4G_MSG_LOGIN_ASK)://登录响应        0x05+结果码（2字节）
        {
            uint16 result = 0;
            result = (msg[pos]<<8)|(msg[pos+1]<<0);//登录结果：0-成功；1-未知错误；2-设备未注册；3-设备认证失败
            
            p_info("APP4G_MSG_LOGIN_ASK:%d",result);
            if(result == 0)
            {
                bsp_SetTimer(TMR_ID_4G_HeartBeat);
                seq_record = NB_SEQ_INIT;
                bsp_StopTimer(TMR_ID_server_ok);//恢复正常  则停止定时器
            }
            else
            {
                seq_record = 0;
                bsp_StartTimer(TMR_ID_server_ok,TMR_DELAY_30min);//1小时重启一次
            }
        }
            break;
        case(APP4G_MSG_DATAUP_ASK)://0x82       上行数据响应  0x82+数据长度（2字节）+msgid(2字节)+结果码（2字节）
        {
            pos++;pos++;
            /*mid的判断*/
            rxMid = msg[pos];
            pos++;  
            rxMid = (rxMid<<8)|msg[pos];
            pos++;  
            
            p_info("APP4G rxMid=%02X",rxMid);
            /* 结果码：0-成功、1-报文解析失败 */
            rxResult = msg[pos];
            pos++;  
            rxResult = (rxResult<<8)|msg[pos];
            pos++;  
            if(rxResult == 0)
            {
                p_info("APP4G_MSG_DATAUP_ASK ok");
                App_fault_analysis_Clear_Value();//只清除一次

                g_caiji.sensor_basic_server_ask_ok=TRUE;
                g_wenshi.server_ok_count=0;//只要收到数据就清0；
                p_info("messageId_SensorBasic 数据清0");                                     
            }
            else
            {
                p_err("APP4G_MSG_DATAUP_ASK err");
            }
        }
            break;
        case(APP4G_MSG_DATADOWN)://下行数据报文   0x03+数据长度（2字节）+msgid(2字节)+serviceId（2字节）+param1+param2+param3+……
        {
            pos++;pos++;
            /*mid的判断*/
            g_mid = msg[pos];
            pos++;  
            g_mid = (g_mid<<8)|msg[pos];
            pos++;  
            /*rxServiceId 的判断*/
            rxServiceId = msg[pos];
            pos++;  
            rxServiceId = (rxServiceId<<8)|msg[pos];
            pos++;  
            if(rxServiceId == APP4G_SERVICEID_DEVICESTATE)
            {
                /* lenth  2byte*/
                rxDataLenth |= msg[pos]<<8;
                pos++;  
                rxDataLenth|= msg[pos]<<0;
                pos++;  
                if(strstr(&msg[pos],"dtime") == null)
                {
                    if(strstr(&msg[pos],"getpic,"))
                    {
                        if(g_Camera.start_state == FALSE)
                        {                
                            g_Camera.start_state = TRUE;
                            p_info("接收到 getpic");                 
                        }               
                    }
                    if(strstr(&msg[pos],"poweroffpic"))
                    {
                        if(g_Camera.start_state == TRUE)
                        {                
                            g_Camera.poweroffpic = 1;
                            p_info("接收到 poweroffpic");                    
                        }               
                    }
                    if(strstr(&msg[pos],"getpicpack,"))
                    {
                        char *p=0;
                        int getpicpack_count = 0;
                        
                        p = strstr(&msg[pos],"getpicpack,5,");
                        if(g_Camera.start_state == TRUE)
                        {                
                            g_Camera.getpicpack = 1;
                            sscanf(p+13,"%d",&getpicpack_count);//将字bai符串里的数字按du%d读取到i中
                            g_Camera.getpicpack_count = getpicpack_count ;

                            p_info("接收到 getpicpack");                 
                        }               
                    }
                    if(strstr(&msg[pos],"dreport,"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        char *p=0;
                        int report_second  =0;
                        int report_changed  =0;
                        p = strstr(&msg[pos],"dreport,");                
                        sscanf(p+8,"%d,%d",&report_second,&report_changed);
                        if(report_second != 0)
                            g_tConfig.report_second = (u16)report_second;
                        g_tConfig.report_changed = (u8)report_changed;
                        App_Config_Save_Into_Flash();//计算累加和，保存到flash

                        p_info("接收到 dreport");    
                        g_caiji.report_server_ask_ok =TRUE;
                        
                    }   
                    if(strstr(&msg[pos],"setreport,"))
                    {
                        char *p=0;
                        int report_second  =0;
                        int report_changed  =0;
                        p = strstr(&msg[pos],"setreport,");              
                        sscanf(p+10,"%d,%d",&report_second,&report_changed);
                        if(report_second != 0)
                            g_tConfig.report_second = (u16)report_second;
                        g_tConfig.report_changed = (u8)report_changed;
                        App_Config_Save_Into_Flash();//计算累加和，保存到flash

                        p_info("接收到 setreport");      
                        App_DeviceState_cmdAsk(CMD_ASK_setReport);
                        p_info("App_DeviceState_setreport_ask发送数据");                    
                        
                    }           
                    if(strstr(&msg[pos],"dinfo,"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        App_Dinfo_Data_process(pos,msg);

                        p_info("接收到 dinfo");                                  
                        g_caiji.info_server_ask_ok =TRUE;
                    }
                    if(strstr(&msg[pos],"plcw,"))
                    {
                        #ifdef ENABLE_TIANJING_PLC          
                        App_plc_Dinfo_Data_process(pos,msg);
                        p_info("接收到 plcw");                                   
                        g_caiji.info_server_ask_ok =TRUE;
                        #endif

                        #ifdef ENABLE_SHESHISUO_PLC         
                        App_sheshisuo_PLC_Dinfo_Data_process(pos,msg);
                        p_info("接收到 plcw");                                   
                        g_caiji.info_server_ask_ok =TRUE;
                        #endif              
                    }   
                    if(strstr(&msg[pos],"setplctype,"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        char *p=0;
                        int plc_type  =0;
                        p = strstr(&msg[pos],"setplctype,");                 
                        sscanf(p+strlen("setplctype,"),"%d",&plc_type);
                        if((plc_type == PLC_NULL)||(plc_type == PLC_TIANJIN)||(plc_type == PLC_SHESHISUO))
                        {
                            g_tConfig.PLC_Type = (u8)plc_type;
                            App_Config_Save_Into_Flash();//计算累加和，保存到flash
                            p_info("接收到 setplctype:%d,ok",plc_type);      
                        }
                        else
                        {
                            p_err("接收到 setplctype,no ok!");       
                        }
                        App_caiji_report_start();//立即上报             
                    }   
                    /* remote reboot                */
                    if(strstr(&msg[pos],"rebootmcu"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        App_fault_analysis_Set_Value(REBOOT_FLAG_REMOTE);                           
                        printf("服务器 remote 重启" );
                        bsp_DelayMS(10);
                        HAL_NVIC_SystemReset();//重启
                    }       
                    /* clearcount                   */
                    if(strstr(&msg[pos],"clearcount"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        App_fault_analysis_Clear_reboot_cout();
                        App_caiji_report_start();//立即上报
                    }   
                    /* reportbasic                  */
                    if(strstr(&msg[pos],"reportbasic"))
                    {
                        App_DeviceState_cmdAsk(CMD_ASK_Ok);//ok
                        App_caiji_report_start();//立即上报
                    }               
                    /* setAcqTimer,60                  */
                    if(strstr(&msg[pos],"setAcqTimer"))
                    {
                        char *p=0;
                        int acqTimer  =0;
                        p = strstr(&msg[pos],"setAcqTimer,");                 
                        sscanf(p+strlen("setAcqTimer,"),"%d",&acqTimer);
                        if(( acqTimer<=255 )&&( acqTimer>=1))
                        {
                            g_tConfig.setAcqTimer = (u8)acqTimer;
                            App_Config_Save_Into_Flash();//计算累加和，保存到flash
                            p_info("接收到 acqTimer:%d,ok",acqTimer);      
                        }
                        else
                        {
                            p_err("接收到 acqTimer,no ok!");       
                        }
                        App_DeviceState_cmdAsk(CMD_ASK_setAcqTimer);//ok ();
                        App_caiji_report_start();//立即上报             
                    }               
                    if(strstr(&msg[pos],"updateSetParam"))
                    {
                        char *p=0;
                        p = strstr(&msg[pos],"updateSetParam,");                 
                        sscanf(p+strlen("updateSetParam,"),"%[^,],%[^,],%[^,],%[^,],%[^,]",gUpdate4G.ipAddress,gUpdate4G.port,
                            gUpdate4G.name,gUpdate4G.passWord,gUpdate4G.root);
                        p_info("接收到 updateSetParam:%s,%s,%s,%s,%s",gUpdate4G.ipAddress,gUpdate4G.port,
                            gUpdate4G.name,gUpdate4G.passWord,gUpdate4G.root);
                        App_DeviceState_cmdAsk(CMD_ASK_updateSetParam);
                    }
                    else if(strstr(&msg[pos],"updateGetParam"))
                    {
                        p_info("接收到 updateGetParam");
                        App_DeviceState_cmdAsk(CMD_ASK_updateGetParam);
                    }
                    else if(strstr(&msg[pos],"updateStart"))
                    {
                        p_info("接收到 updateStart");
                        if(app_update4G_paramCheck())//true is  ok
                        {
                            App_DeviceState_cmdAsk(CMD_ASK_updateStart);
                            gUpdate4G.updateStart = TRUE;
                        }
                        else
                        {
                            App_DeviceState_cmdAsk(CMD_ASK_error);
                        }
                    }
                    return 0;
                }
                else
                {
                    char data[16] = {0};//中间变量
                    /* 时间service_time timer提取   dtime,81204459*/
                    pos++;  pos++;  pos++;  
                    pos++;  pos++;  pos++;  
                    
                    memset(data,0x00,sizeof(data));
                    for(i=0;i<(rxDataLenth-6);i++)
                    {
                        data[i] = msg[pos];
                        pos++;  
                    }
                    
                    gmt_time_server=atoi(data);//81204459

                    /* 时间service_time   int32s  4   是 服务器时间,从2018年1月1日0时0分0秒开始的unix时间戳      提取*/      
                    App_server_gettime(gmt_time_server);

                    /*先调用读时间函数，再调用读日期函数，即可解决问题。*/
                    if (HAL_RTC_GetTime(&hrtc, &g_Time, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        /* HAL Error */
                        Error_Handler();
                    }
                    /* Get the RTC current Time */
                    if (HAL_RTC_GetDate(&hrtc, &g_Date, RTC_FORMAT_BIN) != HAL_OK)
                    {
                        /* HAL Error */
                        Error_Handler();
                    }   

                    App_DeviceState_cmdAsk(CMD_ASK_setDtime);
                    g_caiji.timer_server_ask_ok =TRUE;
                    p_info("App_DeviceState_gettime_ask发送数据");

                    ret = 0;
                }                
            }

        }
            break;
        case(APP4G_MSG_HEARTBEAT_ASK)://0x06    心跳响应    0x06
        {
            p_info("APP4G_MSG_HEARTBEAT_ASK");
        }
            break;
        
        default:
            break;
    }

    return ret;
}

//******************************************************************************
// fn : app4G_msgReportCb
//
// brief : APP4G模块消息上报回调
//
// param : static void bc95_receCb(char* buf, uint16 len)
//
// return : none
//******************************************************************************
int  app_4G_msgReportCb(char* msg,uint16 len)
{
    p_info("MSG_COAP_RECE");
    
    if((msg[0]==FTP_DATA_startingMark1)&&(msg[1]==FTP_DATA_startingMark2)&&(msg[2]==FTP_DATA_startingMark3))
    {
        app_update4G_rxDataFrameAnalyse(len,msg,&gFTPrxData);
        gUpdate4G.rx_data_in_flag = TRUE;
    }
    else
    {
        app_4G_dataRx_process(len,msg);
    }
    return 0;
}


/*
*********************************************************************************************************
*   函 数 名: app4G_initVar
*   功能说明:   
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void app_4G_initVar(void)
{
    g_4G.start_flag = FALSE;
    g_4G.work_state = APP4G_STATE_MAX;
}



/*
*********************************************************************************************************
*   函 数 名: app4G_initStart
*   功能说明:   
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void app_4G_initStart(void)
{
    g_4G.start_flag = TRUE;
    g_4G.work_state = APP4G_STATE_INIT;
    
}
/*
*********************************************************************************************************
*   函 数 名: app_4G_sendData
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void app_4G_sendData(char * p_byte_array, uint16_t * p_size)
{
    {
        HAL_UART_NB_Write(p_byte_array,* p_size);
        bsp_DelayMS(52);//1》时间触发模式，出厂默认 50ms。
    }
    memset(&json_info,'0',sizeof(json_info));//发送完成，清除数据
    json_info.json_len = 0;                                     
}
char heartBeart[2]={0x04,0x04};
uint16_t heartBeartLenth =1;

/*
*********************************************************************************************************
*   函 数 名: app_4G_loop
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void app_4G_loop(void)
{
    if(g_4G.start_flag == TRUE)
    {

    }
    else
    {
        return ;
    }

    switch(g_4G.work_state)
    {
        case APP4G_STATE_INIT :
        {               
            // register 
            bsp_StartTimer(TMR_ID_4G_HeartBeat , TMR_DELAY_3min);//3min 心跳一次
            g_4G.work_state = APP4G_STATE_LOOP;
        }
            break;  

        case APP4G_STATE_LOOP :// 没有经历过5min，直接人工退出
        {   
            if(bsp_CheckTimer(TMR_ID_4G_HeartBeat))
            {
                bsp_StartTimer(TMR_ID_4G_HeartBeat , TMR_DELAY_3min);//
                if(gUpdate4G.updateStart == FALSE)//升级过程中，不再发送数据给4g模块
                    app_4G_sendData(&heartBeart[0],&heartBeartLenth);
                p_info("g_4G TMR_ID_4G_HeartBeat");                 
            }   

        }       
            break;  

            
        default:
            break;
    }
    
    switch(APP_STATE)
    {
        case NB_NONE:
        {
            //wait for key
            APP_STATE = NB_INIT;
        }
        break;
        case NB_INIT:
        {
            //printf("\r\n<----BC95 Init---->\r\n");
            //LCD_Print("Init start...",NULL);
            //seq_record = 0;
            //NBModule_Init(&nb_config);// 初始化
            APP_STATE = NB_END;
        }
        break;

        case NB_CoAP_ST:
        {
            if(json_info.json_len)
            {
                /*  发送数据成功*/
                if(gUpdate4G.updateStart == FALSE)//升级过程中，不再发送数据给4g模块
                    app_4G_sendData(&json_info.json_buf[0], &json_info.json_len);

                p_info("4g 发送数据 success");
                memset(&json_info,'0',sizeof(json_info));//发送完成，清除数据
                json_info.json_len = 0;                                     
            }
            APP_STATE = NB_END;
        }
        break;

        default:
        {

        }
        break;
    }   




}

