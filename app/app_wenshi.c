#include "app_wenshi.h"
#include "debug.h"


#include "bsp.h"

#include "app.h"

#define CTWING_DATA_UP (0X02)// 上行数据报文
#define CTWING_DATALENTH_INDEX1 (1)// 上行数据报文长度地址1
#define CTWING_DATALENTH_INDEX2 (2)// 上行数据报文长度地址2

struct WENSHI g_wenshi ;
uint8_t g_key_press = 0;
uint16_t g_mid = 0;
uint16_t msgid = 0;//上行msgid

extern void App_SOCKET_STATE_ask(_End_Device_data_type * pEndDevice,uint8_t work_type);

/*
*********************************************************************************************************
*   函 数 名: App_server_gettime
*   功能说明:  解析服务器的时间
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_server_gettime(unsigned long gmt_time_server)
{   
    unsigned long gmt_time = 0;
    unsigned long gmt_time_2018 = 0;
    unsigned int year[6];//int year =0;     int mon =0;     int day =0;     int hour =0;        int min =0;     int sec =0;

    /*时间 秒钟*/
    gmt_time_2018 = mktime_second(2018,1,1,0,0,0);
    gmt_time =gmt_time_2018+ gmt_time_server; 
    
    GMT_toLocalTime(gmt_time,&year[0],&year[1],&year[2],&year[3],&year[4],&year[5]);
    p_info("1时间更新%d-%d-%d, %d:%d:%d",year[0],year[1],year[2],year[3],year[4],year[5]);

    year[0] = year[0] - 2000;//Min_Data = 0 and Max_Data = 99；

    if(MX_RTC_Set(year[0],year[1],year[2],year[3],year[4],year[5])==0)      
    {
        App_caiji_report_gettime_ok();//停止发送校时命令

    }
    else
    {
        return ;
    }

}


//******************************************************************************
// fn : App_NB_HexStrToNum
//
// brief : 将十六进制字符串转成数字
//
// param : 
//         str -> 十六进制字符串内容
//
// return : 地址
uint16_t App_NB_HexStrToNum(char* str)
{
  uint16_t i = 0;
  uint8_t tmp = 0;
  uint8_t tmp1 = 0;
  
  uint16_t len = strlen(str);
  if(len == 0)
  {
    return 0;
  }
 
  for( i = 0 ; i < len ; i++)
  {
    if(str[i] >= '0' && str[i] <= '9')
    {
      tmp1 = str[i] - '0';
    }
    else if(str[i] >= 'A' && str[i] <= 'F')
    {
     tmp1 = str[i] - 'A' + 10; 
    }
    else if(str[i] >= 'a' && str[i] <= 'f')
    {
      tmp1 = str[i] - 'a' + 10;
    }
    else
    {
      tmp1 = 0;
    }
    
    if((i%2) == 0)
    {
      tmp = tmp1; 
    }
    else
    {
      tmp <<= 4;
      tmp += tmp1;
      str[i>>1] = tmp;
    }
    
  }
  
  str[i >> 1] = 0;
  
  return (i >> 1);
}

/*
*********************************************************************************************************
*   函 数 名: ChangeNum
*   功能说明: 十六进制字符串 转整数
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/

int ChangeNum(char* str, int length)
{
    char revstr[16] = { 0 };// ?//根据十六进制字符串的长度，这里注意数组不要越界 ?
//  int num[16] = { 0 };
    int count = 1;
    int result = 0;
    int tmp = 0;
    
    strcpy(revstr, str);

    for (int i = length - 1; i >= 0; i--)
    {
        if ((revstr[i] >= '0') && (revstr[i] <= '9'))
        //num[i] = revstr[i] - 48;//字符0的ASCII值为48?
            tmp = revstr[i] - 48;//字符0的ASCII值为48?
        else if ((revstr[i] >= 'a') && (revstr[i] <= 'f'))
        //num[i] = revstr[i] - 'a' + 10;
            tmp = revstr[i] - 'a' + 10;
        else if ((revstr[i] >= 'A') && (revstr[i] <= 'F'))
        //num[i] = revstr[i] - 'A' + 10;
            tmp = revstr[i] - 'A' + 10;
        else
        //num[i] = 0;
            tmp = 0;
        //result = result + num[i] * count;
        result = result + tmp * count;
        count = count * 16;//十六进制(如果是八进制就在这里乘以8) ? ? ?
    }
    return result;
}


/*
*********************************************************************************************************
*   函 数 名: App_Dinfo_Data_process
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_Dinfo_Data_process(u8 pos,char* msg)
{
    char *p=0;
    char name[32];//临时缓存 扩大到16个汉字
    int lenth = 0;
    
    int i,j,n=0,len;
    
    int a[16];//10 个参数 
    
    char str[64];
    char str1[10];
    
    memset(name,0,sizeof(name)) ;       
    p = strstr(&msg[pos],"dinfo,");             
    lenth = strlen("dinfo,");
     sscanf(p+lenth,"%[^,]",name);
        
    p = strstr(&msg[pos],name); 
    lenth = strlen(name);   
    strcpy(str,p+lenth+1);
        
    len=strlen(str);
    j=0;
    for(i=0;i<len;i++)
    {
        if(str[i]!=',')
            str1[j++]=str[i];
        else
        {
            str1[j]=0;
            if(j>0)
            {
                a[n++]=atoi(str1);
            }
            j=0;
        }
    }
    
    if(j>0)
    {
        str1[j]=0;
        a[n++]=atoi(str1);
    }
    //strcpy(&g_tConfig.Name[0],name);
    memcpy(&g_tConfig.Name[0],name,12);//只复制12个字符。避免溢出
    
    g_tConfig.T_down = a[0];
    g_tConfig.T_up= a[1];
    
    g_tConfig.H_down = a[2];
    g_tConfig.H_up= a[3];

    g_tConfig.soil_T_down = a[4];
    g_tConfig.soil_T_up= a[5];

    g_tConfig.soil_H_down = a[6];
    g_tConfig.soil_H_up= a[7];

    g_tConfig.Lux_down = a[8];
    g_tConfig.Lux_up= a[9];

    App_Config_Save_Into_Flash();

}

/*
*********************************************************************************************************
*   函 数 名: App_DataRx_process
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
uint8_t App_DataRx_process(int len,char* msg)
{
    uint8_t ret = 0;
    uint8_t i = 0;

    u8 messageId_h = 0;
    u8 pos = 0;
    u16 messageId = 0;

    unsigned long gmt_time_server = 0;

    /* log */

    if(len <= 4)
    {
        p_err("(len <= 4):%d",len);
    }

    /*帧类型的判断*/
    messageId_h = msg[pos];
    pos++;
    messageId = (messageId_h<<8)|msg[pos];
    pos++;  
    /*mid的判断*/
    g_mid = msg[pos];
    pos++;  
    g_mid = (g_mid<<8)|msg[pos];
    pos++;  

    p_info("messageId=%02X",messageId);

     /*  命令帧 02*/
    if((messageId == messageId_SOCKET_STATE))/*  插座命令   SOCKET_STATE    0xa005 , 0xa006 命令  yess */
    {
        /*  摄像头在运行的时候，不能进行插座的控制5分钟*/
        if(g_Camera.start_state == TRUE)
        {       
            return ret;
        }
        _End_Device_data_type * pEndDevice;// 终端
        u8 rx_macAddr[ZIGBEE_MAC_ADDRESS_SIZE];// 接收到的终端的mac
        char addres[3] = {0};//中间变量
        int return_value = 0;
        
        App_SocketStatusReport_clear_bak();//清理备份

        /* 时间service_time   int32s  4   是 服务器时间,从2018年1月1日0时0分0秒开始的unix时间戳      提取*/
        gmt_time_server|= msg[pos]<<24;
        pos++;  
        gmt_time_server|= msg[pos]<<16;
        pos++;  
        gmt_time_server|= msg[pos]<<8;
        pos++;  
        gmt_time_server|= msg[pos]<<0;
        pos++;  
        
        //App_server_gettime(gmt_time_server);      
        
        /* mac地址提取*/
        for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
        {
            memset(addres,0x00,sizeof(addres));
            addres[0] = msg[pos+0*2];
            pos++;  
            addres[1] = msg[pos+0*2];
            pos++;  
            rx_macAddr[i]=ChangeNum(addres,2);
            p_info("rx_macAddr%d:%02X",i,rx_macAddr[i]);
            
        }
        /*zigbee 开启网络*/
        bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
        if(g_tZigbeeHACoordinator.start == FALSE)
        {
            App_ZIGBEE_HA_Init_Var();// zigbee 初始化
            g_tZigbeeHACoordinator.start = TRUE;//              
            g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//开
        }       
        /* 从6个终端中 匹配到对应插座的指针*/

        for(i=1;i<=g_tZigbeeHACoordinator.End_Device_nums;i++)
        {
            pEndDevice = EndDeviceNums_To_EndDevice(i);
            return_value = (memcmp(pEndDevice->macAddr,rx_macAddr,ZIGBEE_MAC_ADDRESS_SIZE));
            if(return_value == 0)
            {
                g_tZigbeeHAEndDevice_cmd_ASK = pEndDevice;// 
                p_info("rx控制插座=%d",i);

                /* 插座的开启和关闭*/
                g_tZigbeeHACoordinator.work_type_update = TRUE;
                g_tZigbeeHACoordinator.work_type = msg[pos];
                pos++;  
                break;
            }
            else
            {
                p_info("rx控制插座，不是=%d",i);
            }
        }
        ret = 0;
    }
    else if((messageId == messageId_ZIGBEE_NETWORK))/* 设备网络控制命令 ZIGBEE_NETWORK  0xa003 , 0xa004 命令  yes*/
    {
        uint8_t rx_cmd_zigbee_network_onoff = 0;
        uint8_t report_time = 0;

        App_SocketStatusReport_clear_bak();//清理备份
        /* 时间service_time   int32s  4   是 服务器时间,从2018年1月1日0时0分0秒开始的unix时间戳      提取*/
        pos++;  
        pos++;  
        pos++;  
        pos++;
    
        rx_cmd_zigbee_network_onoff =  msg[pos];
        pos++;
        if(rx_cmd_zigbee_network_onoff == TRUE)
        {
            bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
            if(g_tZigbeeHACoordinator.start == FALSE)
            {
                App_ZIGBEE_HA_Init_Var();// zigbee 初始化
                g_tZigbeeHACoordinator.start = TRUE;//              
                g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//开
            }           
            p_info("打开ZigBee网络");
        }
        else
        {
            g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = FALSE;
            p_info("关闭ZigBee网络");
        }

        App_ZIGBEE_NETWORK_ask(g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff);     
        APP_STATE = NB_CoAP_ST;   //发送数据
        p_info("App_ZIGBEE_NETWORK_ask 发送数据");                  

        /*  上报时间的限定为4秒钟         */
        report_time =msg[pos];
        pos++;
        if(report_time <= 3)
            report_time = 3;
        
        if(rx_cmd_zigbee_network_onoff == TRUE)
        {
            p_info("打开ZigBee网络:%dS",g_tZigbeeHACoordinator.report_time );
        }
        else
        {
            p_info("关闭ZigBee网络:%dS",g_tZigbeeHACoordinator.report_time );
        }

        ret = 0;
    }
    else if((messageId == messageId_DEVICE_STATE))/* 设备网络控制命令   ZIGBEE_NETWORK  0xa003 , 0xa004 命令  yes*/
    {
        uint8_t rx_data_lenth = 0;
        char data[16] = {0};//中间变量

        App_SocketStatusReport_clear_bak();//清理备份

        /* lenth  2byte*/
        rx_data_lenth |= msg[pos]<<8;
        pos++;  
        rx_data_lenth|= msg[pos]<<0;
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
                APP_STATE = NB_CoAP_ST;   //发送数据
                p_info("App_DeviceState_setreport_ask发送数据");                    
                
            }           
            if(strstr(&msg[pos],"dinfo,"))
            {
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
                App_fault_analysis_Set_Value(REBOOT_FLAG_REMOTE);                           
                printf("服务器 remote 重启" );
                bsp_DelayMS(10);
                HAL_NVIC_SystemReset();//重启
            }       
            /* clearcount                   */
            if(strstr(&msg[pos],"clearcount"))
            {
                App_fault_analysis_Clear_reboot_cout();
                App_caiji_report_start();//立即上报
            }   
            /* reportbasic                  */
            if(strstr(&msg[pos],"reportbasic"))
            {
                App_caiji_report_start();//立即上报
            }               
            return 0;
        }

        /* 时间service_time timer提取   dtime,81204459*/
        pos++;  pos++;  pos++;  
        pos++;  pos++;  pos++;  
        
        memset(data,0x00,sizeof(data));
        for(i=0;i<(rx_data_lenth-6);i++)
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

        APP_STATE = NB_CoAP_ST;   //发送数据
        p_info("App_DeviceState_gettime_ask发送数据");                  

        ret = 0;
    }   
    else if((messageId == messageId_DeviceState))/*  0x0001  */
    {
        g_caiji.rx_data_in_flag =TRUE;
        p_info("rx messageId_DeviceState ok");                  
    }
    else if((messageId == messageId_SensorBasic))/*  0x0002  */
    {
        App_fault_analysis_Clear_Value();//只清除一次

        g_caiji.sensor_basic_server_ask_ok=TRUE;
        g_wenshi.server_ok_count=0;//只要收到数据就清0；
        p_info("messageId_SensorBasic 数据清0");                     
    }   
    else if((messageId == messageId_SensorExtend))/*  0x0003  */
    {
        g_Camera.server_ask_ok = 1;//  
        p_info("rx server_ask_ok!");                    
    }   
    return ret;
}



json_info_t json_info_bak;// 备份

/*
*********************************************************************************************************
*   函 数 名: App_SocketStatusReport_clear_bak
*   功能说明:  清理数据上报的备份
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_SocketStatusReport_clear_bak(void)
{   
    memset(&json_info_bak,0,sizeof(json_info_bak)); 
}

/*
*********************************************************************************************************
*   函 数 名: App_SocketStatusReport_01
*   功能说明: 
*   形   参：无
*   返 回 值: 无

    字段  数据类型    长度  默认值   偏移值
1   messageId   Int8u   1   1   0-1
2   mac01   String  16      1-17
3   net01   Int8u   1       17-18
4   work01  Int8u   1       18-19
5   mac02   String  16      19-35
6   net02   Int8u   1       35-36
7   work02  Int8u   1       36-37
8   mac03   String  16      37-53
9   net03   Int8u   1       53-54
10  work03  Int8u   1       54-55
11  mac04   String  16      55-71
12  net04   Int8u   1       71-72
13  work04  Int8u   1       72-73
14  mac05   String  16      73-89
15  net05   Int8u   1       89-90
16  Work05  Int8u   1       90-91
17  mac06   String  16      91-107
18  net06   Int8u   1       107-108
19  work06  Int8u   1       108-109
    响应字段    AAAA0000            

*********************************************************************************************************
*/
uint8_t App_SocketStatusReport_01(void)
{
    u16 lenth = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t return_value = 0;

    _End_Device_data_type * pEndDevice;// 终端

    
    /*  发送之前，替换掉last 上次的数值    */


    /* */
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    /* message id */
    json_info.json_buf[lenth] = messageId_Socket>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_Socket;
    lenth= lenth + 1;

    /*socket_data_length    int16u  2   是 长度 */
    lenth= lenth + 1;
    lenth= lenth + 1;


    /* 6个插座的组码 */
    for(j=End_Device1;j<=End_Device6;j++)
    {
        pEndDevice = EndDeviceNums_To_EndDevice(j);

        /* 插座mac地址*/
        for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
        {
            sprintf(&json_info.json_buf[lenth],"%02X", pEndDevice->macAddr[i]);     
            lenth= lenth + 2;       
        }
        /* ,*/ 
        json_info.json_buf[lenth] = ',';
        lenth= lenth + 1;       
        
        /* 插座网络状态*/ 
        sprintf(&json_info.json_buf[lenth],"%X", pEndDevice->flag_on_network);  
        lenth= lenth + 1;   
        /* ,*/ 
        json_info.json_buf[lenth] = ',';
        lenth= lenth + 1;   
        
        /* 插座开关状态*/ 
        sprintf(&json_info.json_buf[lenth],"%X", pEndDevice->socket_state);     
        lenth= lenth + 1;   
        /* ,*/ 
        json_info.json_buf[lenth] = ';';
        lenth= lenth + 1;       
    }           

    /* 插座数量上报*/ 
    sprintf(&json_info.json_buf[lenth],"%03d", g_tZigbeeHACoordinator.End_Device_nums); 
    lenth= lenth + 3;   
    /* ,*/ 
    json_info.json_buf[lenth] = ',';
    lenth= lenth + 1;

    /*socket_data_length    int16u  2   是 长度 */
    json_info.json_buf[2] = (lenth-4)>>8;
    json_info.json_buf[3] = (lenth-4)>>0;

    json_info.json_len = lenth;


    return_value = memcmp(&json_info_bak,&json_info,sizeof(json_info_t));
    if(return_value == 0)
    {
        //p_info("上报的插座数据相同，不上报");
        return FALSE;
    }
    else
    {
        memcpy(&json_info_bak,&json_info,sizeof(json_info_t));
        p_info("上报的插座数据不相同，上报");
        return TRUE;
    }
                    
    
}


/*
*********************************************************************************************************
*   函 数 名: App_DataReport_04
*   功能说明: 
*   形   参：无
*   返 回 值: 无

字段  数据类型    长度  偏移值
messageId   Int8u   1               0-1
air_temperature String  6               1-7
air_humidity    Int8u   1               7-8
light_intensity Int16u  2               8-10
soil_temperature    String  6           10-16
soil_moisture   Int8u   1               16-17
co2_concentration   Int8u   1   17-18
longitude   String              10  18-28
latitude    String                  9   28-37
electricity Int8u                   1   37-38
fault_number    Int8u   1   38-39
version_number  Int16u  2   39-41
repot_time  String  14  41-55

*********************************************************************************************************
*/
void App_DataReport_04(void)
{
    u16 lenth = 0;

    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DataReport_04");
        return;
    }
    /*  发送之前，替换掉last 上次的数值    */
    g_wenshi.m_Symbol_last = g_wenshi.m_Symbol;
    g_wenshi.m_T_last = g_wenshi.m_T;
    g_wenshi.m_H_last = g_wenshi.m_H;
    
    g_wenshi.m_Lux_last = g_wenshi.m_Lux;
    
    g_wenshi.m_soil_Symbol_last = g_wenshi.m_soil_Symbol;
    g_wenshi.m_soil_T_last = g_wenshi.m_soil_T;
    g_wenshi.m_soil_H_last = g_wenshi.m_soil_H;
    
    g_wenshi.m_voltage_level_last = g_wenshi.m_voltage_level;


    /* */

    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = MessageId_DataReport>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)MessageId_DataReport;
    lenth= lenth + 1;   
    /*空气温度*/
    if(g_wenshi.m_Symbol >= 0x01)
    {
        json_info.json_buf[lenth] = '+';
        lenth= lenth + 1;       
        sprintf(&json_info.json_buf[lenth],"%03d.%d",g_wenshi.m_T/10,g_wenshi.m_T%10);
        lenth= lenth + 5;       
    }
    else
    {
        json_info.json_buf[lenth] = '-';
        lenth= lenth + 1;       
        sprintf(&json_info.json_buf[lenth],"%03d.%d",g_wenshi.m_T/10,g_wenshi.m_T%10);
        lenth= lenth + 5;       
    }
    /*空气湿度*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*空气光照*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    
    /*土壤温度*/
    if(g_wenshi.m_soil_Symbol>= 0x01)
    {
        json_info.json_buf[lenth] = '+';
        lenth= lenth + 1;       
        sprintf(&json_info.json_buf[lenth],"%03d.%d",g_wenshi.m_soil_T/10,g_wenshi.m_soil_T%10);
        lenth= lenth + 5;               
    }
    else
    {
        json_info.json_buf[lenth] = '-';
        lenth= lenth + 1;       
        sprintf(&json_info.json_buf[lenth],"%03d.%d",g_wenshi.m_soil_T/10,g_wenshi.m_soil_T%10);        
        lenth= lenth + 5;               
    }   
    /*土壤湿度*/
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       

    /*co2*/
    json_info.json_buf[lenth] = g_wenshi.airTH;
    lenth= lenth + 1;       

    if(g_wenshi.m_flag_gps)
    {
        /*经度*/          
        sprintf(&json_info.json_buf[lenth],"%03.06f", g_wenshi.m_longtitude);       
        lenth= lenth + 10;      
        
        /*纬度*/      
        sprintf(&json_info.json_buf[lenth],"%02.06f", g_wenshi.m_latitude);         
        lenth= lenth + 9;               

    }
    else
    {
        /*经度*/          
        sprintf(&json_info.json_buf[lenth],"%03.08f", g_wenshi.m_longtitude);       
        lenth= lenth + 10;      
        /*纬度*/      
        sprintf(&json_info.json_buf[lenth],"%02.06f", g_wenshi.m_latitude);         
        lenth= lenth + 9;       
        
    }   
    
    /*电池电量*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    lenth= lenth + 1;   

    /*故障代码*/
    json_info.json_buf[lenth] =   g_wenshi.fault_number;    
    lenth= lenth + 1;   
    /*版本号*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*时间*/
    sprintf(&json_info.json_buf[lenth],"20%02d%02d%02d%02d%02d%02d", g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);      
    lenth= lenth + 14;      
    
    json_info.json_len = lenth;
}

#define LENTH_TEMP (256u)
static char SensorBasic_data_tx[482];
static char SensorBasic_data_temp[LENTH_TEMP];

#ifdef ONLY_8SAME_AIRTHSENSOR // 8传感器
extern uint8_t addressAirTh[8];
/*
*********************************************************************************************************
*   函 数 名: App_DataReport_SensorBasic
*   功能说明: 
*   形   参：无
*   返 回 值: 无

SensorBasic
设备基础传感器数据
属性名称            数据类型    长度  是否必选    备注
electricity int 0 ~ 100 是 电量
fault_number    int 0 ~ 100 是 故障号
version_number  int 1000 ~ 60000    是 版本号
report_time string  14  是 上报时间
*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    
    /*  发送之前，替换掉last 上次的数值    */
    g_wenshi.m_Symbol_last = g_wenshi.m_Symbol;
    g_wenshi.m_T_last = g_wenshi.m_T;
    g_wenshi.m_H_last = g_wenshi.m_H;
    
    g_wenshi.m_Lux_last = g_wenshi.m_Lux;
    
    g_wenshi.m_soil_Symbol_last = g_wenshi.m_soil_Symbol;
    g_wenshi.m_soil_T_last = g_wenshi.m_soil_T;
    g_wenshi.m_soil_H_last = g_wenshi.m_soil_H;
    g_wenshi.m_voltage_level_last = g_wenshi.m_voltage_level;

    g_wenshi.m_soil_LUX_last = g_wenshi.m_soil_LUX;
    g_wenshi.m_soil_N_last = g_wenshi.m_soil_N[0];
    g_wenshi.m_soil_P_last = g_wenshi.m_soil_N[1];
    g_wenshi.m_soil_K_last = g_wenshi.m_soil_N[2];
    g_wenshi.m_soil_Water_last = g_wenshi.m_soil_Water;
    g_wenshi.m_camera_last = g_wenshi.m_camera;

    g_wenshi.m_led_display_last = g_wenshi.m_led_display;
    g_wenshi.m_pt100_last =         g_wenshi.m_pt100;
    g_wenshi.m_pt100_value_last = g_wenshi.m_pt100_value;
    g_wenshi.m_TH_LUX_last =        g_wenshi.m_TH_LUX;
    
    g_wenshi.m_TH_LUX_t_value_last = g_wenshi.m_TH_LUX_t_value;
    g_wenshi.m_TH_LUX_h_value_last = g_wenshi.m_TH_LUX_h_value;
    g_wenshi.m_TH_LUX_lux_value_last = g_wenshi.m_TH_LUX_lux_value;
    
    g_wenshi.m_UV_last =        g_wenshi.m_UV;
    g_wenshi.m_UV_value_last= g_wenshi.m_UV_value;
    
    g_wenshi.airTHvalueH_last= g_wenshi.airTHvalueH;
    g_wenshi.airTH_last= g_wenshi.airTH;
    g_wenshi.airTHvalueT_last= g_wenshi.airTHvalueT;
    g_wenshi.airTHvalueTwet_last= g_wenshi.airTHvalueTwet;
    g_wenshi.airTHvalueLevel_last= g_wenshi.airTHvalueLevel;

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_SensorBasic>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = APP4G_SERVICEID_SensorBasic;
    lenth= lenth + 1;   
    /*故障代码  fault_analysis 2 */
    /*复位计数  reboot_cout 2*/
    /*电池电量*/    
    /*充电状态复用一个字段  1、0*/
    /*故障代码 信号强度*/
    /*版本号*/
    /*空气温度*/
    if(g_wenshi.m_Symbol >= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_T);
        lenth= lenth + 1;       
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_T);
        lenth= lenth + 1;       
    }       
    /*空气湿度*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*光照强度*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    
    /*土壤温度*/
    #if 0
    if(g_wenshi.m_soil_Symbol>= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T);
        lenth= lenth + 1;           
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T);
        lenth= lenth + 1;                   
    }   
    #else
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value()>>8;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value();
    lenth= lenth + 1;                   
    #endif
    /*土壤湿度*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*经度*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        /*经度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;   
        
    }   
    
    /*电池电量*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*充电状态复用一个字段  1、0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*故障代码 信号强度*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*版本号*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }   
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    
	/*扩展传感器 PH\  NPK\co2*/

    /*扩展传感器 AIRTH 0- 7 */
    for(uint8_t n=0;n<8;n++)
    {
        if(g_wenshi.m_sensor_TXflag&(0X0001<<n))
        {
            memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
            sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",SENSOR_ID_AIRTH_SHESHISUO,addressAirTh[n],airThSensor[n].airTHvalueH,airThSensor[n].airTHvalueT,
            airThSensor[n].airTHvalueTwet,airThSensor[n].airTHvalueLevel,airThSensor[n].airTHvalueHcal);  
            strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
        }   
    }


    /*扩展传感器 有数据就上传 PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    //if(sensor_data_lenth >0)
    {
        json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
        lenth= lenth + 1;
    
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
        lenth= lenth + sensor_data_lenth;   
    }
    
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    //app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    if(gUpdate4G.updateStart == FALSE)//升级过程中，不再发送数据给4g模块
        app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


/**************************************          ALL_SENSORS     ****************************************************/

#ifdef ALL_SENSORS // 原来的传感器
/*
*********************************************************************************************************
*   函 数 名: App_DataReport_SensorBasic
*   功能说明: 
*   形   参：无
*   返 回 值: 无

SensorBasic
设备基础传感器数据
属性名称            数据类型    长度  是否必选    备注
electricity int 0 ~ 100 是 电量
fault_number    int 0 ~ 100 是 故障号
version_number  int 1000 ~ 60000    是 版本号
report_time string  14  是 上报时间
*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    
    /*  发送之前，替换掉last 上次的数值    */
    g_wenshi.m_Symbol_last = g_wenshi.m_Symbol;
    g_wenshi.m_T_last = g_wenshi.m_T;
    g_wenshi.m_H_last = g_wenshi.m_H;
    
    g_wenshi.m_Lux_last = g_wenshi.m_Lux;
    
    g_wenshi.m_soil_Symbol_last = g_wenshi.m_soil_Symbol;
    g_wenshi.m_soil_T_last = g_wenshi.m_soil_T;
    g_wenshi.m_soil_H_last = g_wenshi.m_soil_H;
    g_wenshi.m_voltage_level_last = g_wenshi.m_voltage_level;

    g_wenshi.m_soil_LUX_last = g_wenshi.m_soil_LUX;
    g_wenshi.m_soil_N_last = g_wenshi.m_soil_N[0];
    g_wenshi.m_soil_P_last = g_wenshi.m_soil_N[1];
    g_wenshi.m_soil_K_last = g_wenshi.m_soil_N[2];
    g_wenshi.m_soil_Water_last = g_wenshi.m_soil_Water;
    g_wenshi.m_camera_last = g_wenshi.m_camera;

    g_wenshi.m_led_display_last = g_wenshi.m_led_display;
    g_wenshi.m_pt100_last =         g_wenshi.m_pt100;
    g_wenshi.m_pt100_value_last = g_wenshi.m_pt100_value;
    g_wenshi.m_TH_LUX_last =        g_wenshi.m_TH_LUX;
    
    g_wenshi.m_TH_LUX_t_value_last = g_wenshi.m_TH_LUX_t_value;
    g_wenshi.m_TH_LUX_h_value_last = g_wenshi.m_TH_LUX_h_value;
    g_wenshi.m_TH_LUX_lux_value_last = g_wenshi.m_TH_LUX_lux_value;
    
    g_wenshi.m_UV_last =        g_wenshi.m_UV;
    g_wenshi.m_UV_value_last= g_wenshi.m_UV_value;
    
    g_wenshi.airTHvalueH_last= g_wenshi.airTHvalueH;
    g_wenshi.airTH_last= g_wenshi.airTH;
    g_wenshi.airTHvalueT_last= g_wenshi.airTHvalueT;
    g_wenshi.airTHvalueTwet_last= g_wenshi.airTHvalueTwet;
    g_wenshi.airTHvalueLevel_last= g_wenshi.airTHvalueLevel;

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_SensorBasic>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = APP4G_SERVICEID_SensorBasic;
    lenth= lenth + 1;   
    /*故障代码  fault_analysis 2 */
    /*复位计数  reboot_cout 2*/
    /*电池电量*/    
    /*充电状态复用一个字段  1、0*/
    /*故障代码 信号强度*/
    /*版本号*/
    /*空气温度*/
    if(g_wenshi.m_Symbol >= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_T);
        lenth= lenth + 1;       
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_T);
        lenth= lenth + 1;       
    }       
    /*空气湿度*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*光照强度*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    
    /*土壤温度*/
    #if 0
    if(g_wenshi.m_soil_Symbol>= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T);
        lenth= lenth + 1;           
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T);
        lenth= lenth + 1;                   
    }   
    #else
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value()>>8;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value();
    lenth= lenth + 1;                   
    #endif
    /*土壤湿度*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*经度*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        /*经度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;   
        
    }   
    
    /*电池电量*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*充电状态复用一个字段  1、0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*故障代码 信号强度*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*版本号*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }   
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    
//    /*控制器设备4G版本*/
//    /*时间*/
//    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
//    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
//    l_long_s = mktime_second(18,1,1,0,0,0);
//    if(l_long>=l_long_s)
//    {
//        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",10201,g_Indicat.Indicat.wIndicate,g_Indicat.reboot_cout,
//            g_wenshi.m_voltage_level,g_wenshi.m_nb_signal_level,g_wenshi.version_number,(l_long-l_long_s)); 
//    }
//    else
//    {
//        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",10201,g_Indicat.Indicat.wIndicate,g_Indicat.reboot_cout,
//            g_wenshi.m_voltage_level,g_wenshi.m_nb_signal_level,g_wenshi.version_number,0); 
//    }
//    strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    /*扩展传感器 气象站*/
    if(g_wenshi.weatherStationExisted == 1)// 存在过 气象站，需要上报 在线 还是 离线；
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",SENSOR_ID_WEATHER_STATION,ADDRESS_WEATHER_STATION,g_wenshi.weatherStationOnline,
            g_wenshi.weatherStation[0],g_wenshi.weatherStation[1],g_wenshi.weatherStation[2],g_wenshi.weatherStation[3],
            g_wenshi.weatherStation[4],g_wenshi.weatherStation[5],g_wenshi.weatherStation[6],g_wenshi.weatherStation[7]);
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
        
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d,%d;",
            g_wenshi.weatherStation[8],g_wenshi.weatherStation[9],g_wenshi.weatherStation[10],g_wenshi.weatherStation[11],
            g_wenshi.weatherStation[12],g_wenshi.weatherStation[13],g_wenshi.weatherStation[14],g_wenshi.weatherStation[15]);        
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);

    }       
    else
    {

    }
	/*扩展传感器 PH\  NPK\co2*/
	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_PH)
	{
		memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
		sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_SOIL_PH,ADDRESS_TURANG_PH,g_wenshi.m_soil_PH);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);
	}
	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_NPK)
	{
		memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d;",SENSOR_ID_SOIL_NPK,ADDRESS_TURANG_NPK,g_wenshi.m_soil_N[0],g_wenshi.m_soil_N[1],g_wenshi.m_soil_N[2]);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);			
	}
	if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_CO2)
	{
		memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
		sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",SENSOR_ID_CO2_LUX,ADDRESS_CO2,g_wenshi.m_co2,g_wenshi.m_co2_value);	
		strcat(SensorBasic_data_tx,SensorBasic_data_temp);	
	}

    /*扩展传感器 土壤*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_TH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        /*土壤温度*/
        if(g_wenshi.m_soil_Symbol>= 0x01)
        {
            sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d;",SENSOR_ID_SOIL_TH,ADDRESS_TURANG_TH,g_wenshi.m_soil_T,g_wenshi.m_soil_H,g_wenshi.m_soil_EC);    
        }
        else
        {
            sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d;",SENSOR_ID_SOIL_TH,ADDRESS_TURANG_TH,g_wenshi.m_soil_T,g_wenshi.m_soil_H,g_wenshi.m_soil_EC);    
        }       
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    }
    /*扩展传感器 LUX*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_LUX)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_SOIL_LUX,ADDRESS_TURANG_LUX,g_wenshi.m_soil_LUX);   
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    }
    /*扩展传感器 AIRTH*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_AIRTH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",SENSOR_ID_AIRTH_SHESHISUO,ADDRESS_AIR_TH,g_wenshi.airTHvalueH,g_wenshi.airTHvalueT,
        g_wenshi.airTHvalueTwet,g_wenshi.airTHvalueLevel,g_wenshi.airTHvalueHcal);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }   
    /*   LED屏幕 */
    if((g_wenshi.mLedFlag == 1))// 存在过LED 在线 离线 null 
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_LED_DISPLAY,ADDRESS_LED_DISPLAY,g_wenshi.m_led_display);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    else // 不存在 不发送null
    {

    }

    /*   PLC 屏幕 */
    if(g_wenshi.mPlcFlag == 1)// 存在过 PLC 在线 离线 null 
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,1,%d,2,%d;",SENSOR_ID_PLC_SHESHISUO2,ADDRESS_PLC_DISPLAY,
            g_wenshi.mPlcdisplay,g_wenshi.mPlcStateInsulation,g_wenshi.mPlcStateRoofVentilation);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }       
    else
    {

    }
    /*扩展传感器 有数据就上传 PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    //if(sensor_data_lenth >0)
    {
        json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
        lenth= lenth + 1;
    
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
        lenth= lenth + sensor_data_lenth;   
    }
    
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    //app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    if(gUpdate4G.updateStart == FALSE)//升级过程中，不再发送数据给4g模块
        app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


#if 0
/*
*********************************************************************************************************
*   函 数 名: App_DataReport_SensorBasic
*   功能说明: 
*   形   参：无
*   返 回 值: 无

SensorBasic
设备基础传感器数据
属性名称            数据类型    长度  是否必选    备注
air_temperature string  6   是 空气温度
air_humidity    int     0 ~ 100 是 空气湿度
light_intensity int     0 ~ 10000   是 光照强度
soil_temperature  string    6   是 土壤温度
soil_moisture   int     0 ~ 100 是 土壤湿度
co2_concentration   int 0 ~ 100 是 co2浓度
longitude   string  10  是 Gps经度
latitude    string  10  是 Gps纬度
electricity int 0 ~ 100 是 电量
fault_number    int 0 ~ 100 是 故障号
version_number  int 1000 ~ 60000    是 版本号
report_time string  14  是 上报时间


*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    json_info_t json_info;

    /*  发送之前，替换掉last 上次的数值    */
    g_wenshi.m_Symbol_last = g_wenshi.m_Symbol;
    g_wenshi.m_T_last = g_wenshi.m_T;
    g_wenshi.m_H_last = g_wenshi.m_H;
    
    g_wenshi.m_Lux_last = g_wenshi.m_Lux;
    
    g_wenshi.m_soil_Symbol_last = g_wenshi.m_soil_Symbol;
    g_wenshi.m_soil_T_last = g_wenshi.m_soil_T;
    g_wenshi.m_soil_H_last = g_wenshi.m_soil_H;
    g_wenshi.m_voltage_level_last = g_wenshi.m_voltage_level;

    g_wenshi.m_soil_LUX_last = g_wenshi.m_soil_LUX;
    g_wenshi.m_soil_N_last = g_wenshi.m_soil_N;
    g_wenshi.m_soil_P_last = g_wenshi.m_soil_P;
    g_wenshi.m_soil_K_last = g_wenshi.m_soil_K;
    g_wenshi.m_soil_Water_last = g_wenshi.m_soil_Water;
    g_wenshi.m_camera_last = g_wenshi.m_camera;

    g_wenshi.m_led_display_last = g_wenshi.m_led_display;
    g_wenshi.m_pt100_last =         g_wenshi.m_pt100;
    g_wenshi.m_pt100_value_last = g_wenshi.m_pt100_value;
    g_wenshi.m_TH_LUX_last =        g_wenshi.m_TH_LUX;
    
    g_wenshi.m_TH_LUX_t_value_last = g_wenshi.m_TH_LUX_t_value;
    g_wenshi.m_TH_LUX_h_value_last = g_wenshi.m_TH_LUX_h_value;
    g_wenshi.m_TH_LUX_lux_value_last = g_wenshi.m_TH_LUX_lux_value;
    
    g_wenshi.m_UV_last =        g_wenshi.m_UV;
    g_wenshi.m_UV_value_last= g_wenshi.m_UV_value;
    
    g_wenshi.airTHvalueH_last= g_wenshi.airTHvalueH;
    g_wenshi.airTH_last= g_wenshi.airTH;
    g_wenshi.airTHvalueT_last= g_wenshi.airTHvalueT;
    g_wenshi.airTHvalueTwet_last= g_wenshi.airTHvalueTwet;
    g_wenshi.airTHvalueLevel_last= g_wenshi.airTHvalueLevel;


    /* 数据组帧*/

    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SensorBasic>>8;//基础传感器  SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SensorBasic;
    lenth= lenth + 1;   
    /*空气温度*/
    if(g_wenshi.m_Symbol >= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_T);
        lenth= lenth + 1;       
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_T);
        lenth= lenth + 1;       
    }       
    /*空气湿度*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*光照强度*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    
    /*土壤温度*/
    #if 0
    if(g_wenshi.m_soil_Symbol>= 0x01)
    {
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400+g_wenshi.m_soil_T);
        lenth= lenth + 1;           
    }
    else
    {
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T)>>8;
        lenth= lenth + 1;       
        json_info.json_buf[lenth] = (400-g_wenshi.m_soil_T);
        lenth= lenth + 1;                   
    }   
    #else
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value()>>8;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = App_fault_analysis_Get_Value();
    lenth= lenth + 1;                   
    #endif
    /*土壤湿度*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*经度*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        /*经度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        /*纬度*/  
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;   
        
    }   
    
    /*电池电量*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*充电状态复用一个字段  1、0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*故障代码 信号强度*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*版本号*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }

    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    /*扩展传感器 PH\  NPK\WATER*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_TH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        /*土壤温度*/
        if(g_wenshi.m_soil_Symbol>= 0x01)
        {
            sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",SENSOR_ID_SOIL_TH,ADDRESS_TURANG_TH,(400+g_wenshi.m_soil_T),g_wenshi.m_soil_H/10); 
        }
        else
        {
            sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",SENSOR_ID_SOIL_TH,ADDRESS_TURANG_TH,(400-g_wenshi.m_soil_T),g_wenshi.m_soil_H/10); 
        }       
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    }
    /*扩展传感器 PH\  NPK\WATER*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_LUX)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_SOIL_PH,ADDRESS_TURANG_PH,g_wenshi.m_soil_LUX); 
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_NPK)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d;",SENSOR_ID_SOIL_NPK,ADDRESS_TURANG_NPK,g_wenshi.m_soil_N,g_wenshi.m_soil_P,g_wenshi.m_soil_K);   
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);          
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_WATER)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_SOIL_WATER,ADDRESS_TURANG_WATER ,g_wenshi.m_soil_Water);    
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_CAMERA)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_CAMERA,ADDRESS_CAMERA ,g_wenshi.m_camera);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_LED_DISPLAY)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_LED_DISPLAY,ADDRESS_LED_DISPLAY ,g_wenshi.m_led_display);   
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }   
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PT100)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_PT100,ADDRESS_PT100,g_wenshi.m_pt100_value);    
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }   
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TH_LUX)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d;",SENSOR_ID_TH_LUX,ADDRESS_TH_LUX,g_wenshi.m_TH_LUX,g_wenshi.m_TH_LUX_h_value,g_wenshi.m_TH_LUX_t_value,g_wenshi.m_TH_LUX_lux_value);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_AIRTH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",SENSOR_ID_AIRTH_SHESHISUO,ADDRESS_AIR_TH,g_wenshi.airTH,g_wenshi.airTHvalueH); 
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_UV)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d;",SENSOR_ID_UV,ADDRESS_UV,g_wenshi.m_UV,g_wenshi.m_UV_value);    
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }       
    /*
    占位  字段名称    备注
    0   PLC标识符    设备标识符
    1   工作模式    状态：1=手动;0=远程
    2   1#电机远程指示    1=接通;0=断开
    3   1#电机正转指示    1=接通;0=断开
    4   1#电机反转指示    1=接通;0=断开
    5   1#电机故障指示    1=接通;0=断开
    6   1#正转限位指示    1=接通;0=断开
    7   1#反转限位指示    1=接通;0=断开
    */  
    if(g_tConfig.PLC_Type == PLC_NULL)
    {

    }
#ifdef ENABLE_TIANJING_PLC  
    else if(g_tConfig.PLC_Type == PLC_TIANJIN)
    {
        if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_PLC_TIANJING)
        {
            memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
            sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_TIANJING,ADDRESS_PLC_201,1);    
            strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
            for(uint8_t i =0;i<NUM_MOTOR;i++)
            {
                memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
                sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,",g_plc.motor[i].Motor_remote,g_plc.motor[i].Motor_positive,g_plc.motor[i].Motor_reversal,
                g_plc.motor[i].Motor_fault,g_plc.motor[i].limit_Positive_turn_limit,g_plc.motor[i].limit_Reverse_turn_limit);   
                strcat(SensorBasic_data_tx,SensorBasic_data_temp);
            }       
        }
        else
        {
            memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
            sprintf(SensorBasic_data_temp,"%d,%d,%d,",SENSOR_ID_PLC_TIANJING,ADDRESS_PLC_201,0);    
            strcat(SensorBasic_data_tx,SensorBasic_data_temp);                      
        }
    }
    #endif
    else if(g_tConfig.PLC_Type == PLC_SHESHISUO)
    {
        App_sheshisuo_PLC_DataReport_SensorBasic(SensorBasic_data_tx,SensorBasic_data_temp);
    }

    
    /*扩展传感器 有数据就上传 PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    if(sensor_data_lenth >0)
    {
        //if(sensor_data_lenth >=1) 
        //  sensor_data_lenth=sensor_data_lenth-1;
        
        json_info.json_buf[lenth] =sensor_data_lenth>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = sensor_data_lenth;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
        lenth= lenth + sensor_data_lenth;   
    }

    json_info.json_len = lenth;
    app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    
}
#endif

/*
*********************************************************************************************************
*   函 数 名: App_DataReport_SensorExtend
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DataReport_SensorExtend(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DataReport_SensorExtend");   
        //return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SensorExtend>>8;//基础传感器 SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SensorExtend;
    lenth= lenth + 1;

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }

    /*sensor_data_lenth*/
    //lenth= lenth + 1;
    //lenth= lenth + 1;

    sprintf(&json_info.json_buf[lenth+2],"%d,%d,%d,%d,",SENSOR_ID_CAMERA,ADDRESS_CAMERA ,g_wenshi.m_camera,g_Camera.rx_datas_count);
    sensor_data_lenth = strlen(&json_info.json_buf[lenth+2]);

    u16 j = 0; 
    for (j= 0; j < g_Camera.data_lenth_each_packages; j++)
    {
        //json_info.json_buf[lenth+2+sensor_data_lenth+j] = g_Camera.data[j] ;
        sprintf(&json_info.json_buf[lenth+2+sensor_data_lenth + (j << 1)],"%02X",(uint8)g_Camera.data[j]);  
    }
    sensor_data_lenth = sensor_data_lenth+g_Camera.data_lenth_each_packages *2;

    json_info.json_buf[lenth] =(sensor_data_lenth)>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth);// 高字节在低位地址；大端模式；
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}

#if 0

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_Report_NBID
*   功能说明: 上报NB卡ID     IMEI     IMSI
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_Report_NBID(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_Report_NBID");   
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_DeviceState>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DeviceState;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
    sprintf(SensorBasic_data_temp,"IMSI:%s;",bc95_getIMSI(&nb_config)); 
    strcpy(SensorBasic_data_tx,SensorBasic_data_temp);
    
    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
    sprintf(SensorBasic_data_temp,"IMEI:%s;",bc95_getIMEI(&nb_config)); 
    strcat(SensorBasic_data_tx,SensorBasic_data_temp);

    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
    sprintf(SensorBasic_data_temp,"ICCID:%s;",bc95_getICCID(&nb_config));   
    strcat(SensorBasic_data_tx,SensorBasic_data_temp);  

    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
    sprintf(SensorBasic_data_temp,"CONFIG:%d,%d,%d;",g_tConfig.report_second,g_tConfig.report_changed,g_tConfig.setAcqTimer);   
    strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；

    lenth= lenth + sensor_data_lenth;
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
        
    json_info.json_len = lenth;
}
#endif



/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_Report_Connectivity
*   功能说明: 上报无线参数
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_Report_Connectivity(void)
{
    u16 lenth = 0;

    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_Report_Connectivity");   
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = CTWING_DATA_UP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 2;   

    /*sensor_data_lenth*/
    json_info.json_buf[lenth] = bc95_getSignalPower();//
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = bc95_getECL();
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = bc95_getSNR();//
    lenth= lenth + 1;   
    
    json_info.json_buf[lenth] = bc95_getCellID()>>24;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = bc95_getCellID()>>16;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = bc95_getCellID()>>8;
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = bc95_getCellID()>>0;
    lenth= lenth + 1;       

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
        
    json_info.json_len = lenth;
}
#if 0

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_gettime
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_gettime(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;

    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_gettime");   
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth; 

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_DeviceState>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DeviceState;
    lenth= lenth + 1;   


    /*sensor_data_lenth*/
    sensor_data_lenth = strlen("gettime");
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;

    strcpy(&json_info.json_buf[lenth],"gettime");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
}

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_gettime_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_gettime_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


//  /*  发送缓存中有数据，不在发送。  */
//  if(json_info.json_len)
//  {
//      p_err("App_DeviceState_gettime_ask");   
//      return;
//  }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;


    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// 指令下发响应
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 结果码：0-成功，不带serviceId和参数；1-成功，带serviceId和参数；2-下行数据处理失败，不带serviceId和参数
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"dtime,%u",(l_long-l_long_s));
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif
#if 0

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_getinfo
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_getinfo(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getinfo");   
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_DeviceState>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DeviceState;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    sensor_data_lenth = strlen("getinfo");
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;

    strcpy(&json_info.json_buf[lenth],"getinfo");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
        
    json_info.json_len = lenth;
}



/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_getreport
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_getreport(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getreport"); 
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;


    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_DeviceState>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DeviceState;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    sensor_data_lenth = strlen("getreport");
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;   
    strcpy(&json_info.json_buf[lenth],"getreport");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
        
    json_info.json_len = lenth;
}
#endif

#if 0
/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_setreport_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_setreport_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


//  /*  发送缓存中有数据，不在发送。  */
//  if(json_info.json_len)
//  {
//      p_err("App_DeviceState_setreport_ask"); 
//      return;
//  }
    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// 指令下发响应
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 结果码：0-成功，不带serviceId和参数；1-成功，带serviceId和参数；2-下行数据处理失败，不带serviceId和参数
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"%u,%d,%d",(l_long-l_long_s),g_tConfig.report_second,g_tConfig.report_changed);
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif
/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_getpic_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_getpic_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getpic_ask");    
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//基础传感器 SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
    lenth= lenth + 1;
    
    //设备执行完命令，命令执行结果上报中的mid要与收到命令中的mid保持一致，这样平台才能刷新对应命令的状态
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   
 
 
    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }

    /*sensor_data_lenth*/
    //lenth= lenth + 1;
    //lenth= lenth + 1;
    
    sprintf(&json_info.json_buf[lenth+2],"%d,%d,%d,%d",ADDRESS_CAMERA,g_Camera.datas_lenth,g_Camera.all_packages,g_Camera.data_lenth_each_packages);
    sensor_data_lenth = strlen(&json_info.json_buf[lenth+2]);

    json_info.json_buf[lenth] =sensor_data_lenth>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;
    json_info.json_buf[lenth] = sensor_data_lenth;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}



/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_poweroffpic_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_poweroffpic_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_poweroffpic_ask");   
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//基础传感器 SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
    lenth= lenth + 1;
    
    //设备执行完命令，命令执行结果上报中的mid要与收到命令中的mid保持一致，这样平台才能刷新对应命令的状态
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   
 
 
    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }

    /*sensor_data_lenth*/
    //lenth= lenth + 1;
    //lenth= lenth + 1;
    
    sprintf(&json_info.json_buf[lenth+2],"%d,1",ADDRESS_CAMERA);
    sensor_data_lenth = strlen(&json_info.json_buf[lenth+2]);

    json_info.json_buf[lenth] =sensor_data_lenth>>8;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;
    json_info.json_buf[lenth] = sensor_data_lenth;// 高字节在低位地址；大端模式；
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_gettime_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_SOCKET_STATE_ask(_End_Device_data_type * pEndDevice,uint8_t work_type)
{
    u16 lenth = 0;
//  u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    u16 i = 0;

    /*  发送缓存中有数据，任要发送。  */  
    if(json_info.json_len)
    {
        bsp_StartTimer(TMR_ID_ZIGBEE_HA_cmd_socke_state_ask,TMR_DELAY_3S);//查询后上报         
        p_err("App_SOCKET_STATE_ask"); 
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SOCKET_STATE_ask>>8;//基础传感器 SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SOCKET_STATE_ask;
    lenth= lenth + 1;   

    //设备执行完命令，命令执行结果上报中的mid要与收到命令中的mid保持一致，这样平台才能刷新对应命令的状态
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*时间device_time int32s  4   是 设备时间,从2018年1月1日0时0分0秒开始的unix时间戳。*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }

    /* 插座网络状态*/ 
    #if 0
    json_info.json_buf[lenth] =1;// pEndDevice->flag_on_network;
    #else
    json_info.json_buf[lenth] =  pEndDevice->flag_on_network;
    #endif
    lenth= lenth + 1;   
    
    /* 插座mac地址*/
    for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
    {
        sprintf(&json_info.json_buf[lenth],"%02X", pEndDevice->macAddr[i]);     
        lenth= lenth + 2;       
    }
    
    /* 插座开关状态*/ 
#if 0
    json_info.json_buf[lenth] = work_type;//pEndDevice->socket_state;   
#else
    json_info.json_buf[lenth] =  pEndDevice->socket_state;  
#endif
    lenth= lenth + 1;   
    
    json_info.json_len = lenth;
}


/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_gettime_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_ZIGBEE_NETWORK_ask(uint8_t network)
{
    u16 lenth = 0;
//  u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  发送缓存中有数据，不在发送。  */
    if(json_info.json_len)
    {
        p_err("App_ZIGBEE_NETWORK_ask"); 
        return;
    }

    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_ZIGBEE_NETWORK_ask>>8;//基础传感器   SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_ZIGBEE_NETWORK_ask;
    lenth= lenth + 1;   

    //设备执行完命令，命令执行结果上报中的mid要与收到命令中的mid保持一致，这样平台才能刷新对应命令的状态
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   


    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;

    /*时间device_time int32s  4   是 设备时间,从2018年1月1日0时0分0秒开始的unix时间戳。*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    
    /* 插座网络状态*/ 
    json_info.json_buf[lenth] = network;    
    lenth= lenth + 1;   
    
    json_info.json_len = lenth;
}

#if 0
/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_setAcqTimer_ask
*   功能说明: 
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_setAcqTimer_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// 指令下发响应
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 结果码：0-成功，不带serviceId和参数；1-成功，带serviceId和参数；2-下行数据处理失败，不带serviceId和参数
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"%u,%d",(l_long-l_long_s),g_tConfig.setAcqTimer);
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


/*
*********************************************************************************************************
*   函 数 名: app_deviceState_updateResult
*   功能说明: 升级结果事件上报		updateResult,0,V101
*   形   参：无  Incident reporting
*   返 回 值: 无
*********************************************************************************************************
*/
void app_deviceState_eventReport(EVENT_REPORT_E eventType)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth; 

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//上行数据帧头0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//上行数据长度 2
    lenth= lenth + 1;   

    msgid++;
    json_info.json_buf[lenth] = msgid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = msgid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = APP4G_SERVICEID_DeviceState>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DeviceState;
    lenth= lenth + 1;   

    memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
    switch (eventType)
    {
        case EVENT_REPORT_UPDATERESULT:
        {
            sprintf(SensorBasic_data_temp,"updateResult,0,%d",g_wenshi.version_number); 
        }
            break;
        case EVENT_REPORT_DownloadStatus:
        {
            sprintf(SensorBasic_data_temp,"updateDownload,%02X,%02X,%02X,%02X,%02X",gFTPrxData.resultCode[0],gFTPrxData.resultCode[1],
                gFTPrxData.resultCode[2],gFTPrxData.resultCode[3],gFTPrxData.resultCode[4]); 
        }
            break;        
        case EVENT_REPORT_VERSION:
        {
            sprintf(SensorBasic_data_temp,"version,%d",g_wenshi.version_number); 
        }
            break;
        case EVENT_REPORT_GETTIME:
        {
            strcpy(SensorBasic_data_temp,"gettime"); 
        }
            break;
        case EVENT_REPORT_GETINFO:
        {
            strcpy(SensorBasic_data_temp,"getinfo"); 
        }
            break;
        case EVENT_REPORT_GETREPORT:
        {
            strcpy(SensorBasic_data_temp,"getreport"); 
        }
            break;            
        case EVENT_REPORT_ReportNBID:
        {
            memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
            sprintf(SensorBasic_data_tx,"IMSI:%s;",bc95_getIMSI(&nb_config)); 
            strcpy(SensorBasic_data_temp,SensorBasic_data_tx);

            memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
            sprintf(SensorBasic_data_tx,"IMEI:%s;",bc95_getIMEI(&nb_config)); 
            strcat(SensorBasic_data_temp,SensorBasic_data_tx);

            memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
            sprintf(SensorBasic_data_tx,"ICCID:%s;",bc95_getICCID(&nb_config));   
            strcat(SensorBasic_data_temp,SensorBasic_data_tx);  

            memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
            sprintf(SensorBasic_data_tx,"CONFIG:%d,%d,%d;",g_tConfig.report_second,g_tConfig.report_changed,g_tConfig.setAcqTimer);   
            strcat(SensorBasic_data_temp,SensorBasic_data_tx);  

        }
            break;
        default:
            break;
        
    }

    /*sensor_data_lenth*/
    sensor_data_lenth = strlen(SensorBasic_data_temp);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;

    strcpy(&json_info.json_buf[lenth],SensorBasic_data_temp);// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}

/*
*********************************************************************************************************
*   函 数 名: App_DeviceState_cmdAsk
*   功能说明: 命令ask
*   形   参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_DeviceState_cmdAsk( CMD_ASK_TYPE_E asktype)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    /* 数据组帧*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// 指令下发响应
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    if((asktype == CMD_ASK_plcwOk)||(asktype == CMD_ASK_plcwError))
    {
        json_info.json_buf[lenth] = g_sheshisuoPLC.tx_mid>>8;
        lenth= lenth + 1;
        json_info.json_buf[lenth] = g_sheshisuoPLC.tx_mid;
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = g_mid>>8;
        lenth= lenth + 1;
        json_info.json_buf[lenth] = g_mid;
        lenth= lenth + 1;
    }

    json_info.json_buf[lenth] = 0X00;// 结果码：0-成功，不带serviceId和参数；1-成功，带serviceId和参数；2-下行数据处理失败，不带serviceId和参数
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*时间*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// 高字节在低位地址；大端模式；
        lenth= lenth + 1;
    }
    if(asktype == CMD_ASK_updateSetParam)
        sprintf(SensorBasic_data_tx,"updateSetParam,%s,%s,%s,%s,%s",gUpdate4G.ipAddress,gUpdate4G.port,
                            gUpdate4G.name,gUpdate4G.passWord,gUpdate4G.root);
    else if(asktype == CMD_ASK_updateGetParam)
        sprintf(SensorBasic_data_tx,"updateGetParam,%s,%s,%s,%s,%s",gUpdate4G.ipAddress,gUpdate4G.port,
                            gUpdate4G.name,gUpdate4G.passWord,gUpdate4G.root);
    else if(asktype == CMD_ASK_updateStart)
        sprintf(SensorBasic_data_tx,"updateStart,%s,%s,%s,%s,%s",gUpdate4G.ipAddress,gUpdate4G.port,
                            gUpdate4G.name,gUpdate4G.passWord,gUpdate4G.root);
    else if(asktype == CMD_ASK_Ok)
        sprintf(SensorBasic_data_tx,"ok");
    else if(asktype == CMD_ASK_setReport)
        sprintf(SensorBasic_data_tx,"%u,%d,%d",(l_long-l_long_s),g_tConfig.report_second,g_tConfig.report_changed);
    else if(asktype == CMD_ASK_setDtime)
        sprintf(SensorBasic_data_tx,"dtime,%u",(l_long-l_long_s));
    else if(asktype == CMD_ASK_setAcqTimer)
        sprintf(SensorBasic_data_tx,"%u,%d",(l_long-l_long_s),g_tConfig.setAcqTimer);
    else if(asktype == CMD_ASK_error)
        sprintf(SensorBasic_data_tx,"error");
    else if(asktype == CMD_ASK_plcwOk)
        sprintf(SensorBasic_data_tx,"%d,0,%d,%d",SENSOR_ID_PLC_SHESHISUO2,g_sheshisuoPLC.rx_occupied,g_sheshisuoPLC.rx_value);//(6,)
    else if(asktype == CMD_ASK_plcwError)
        sprintf(SensorBasic_data_tx,"%d,1,%d,-1",SENSOR_ID_PLC_SHESHISUO2,g_sheshisuoPLC.rx_occupied);//(6,)

    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// 高字节在低位地址；大端模式；
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// 高字节在低位地址；大端模式；
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// 高字节在低位地址；大端模式；
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}

/*
*********************************************************************************************************
*   函 数 名: App_key_process
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_key_process(void)
{
    
    uint8_t ucKeyCode;  

    /* 处理按键事件 */
    ucKeyCode = bsp_GetKey_set();
    if (ucKeyCode > 0)
    {
        switch (ucKeyCode)
        {
            /*确定键按下*/     
            case KEY_DOWN_SURE: 
            {
                /*关机了*/
                if(g_wenshi.power_onoff == POWER_ON)
                {
                    /*zigbee 开启网络*/
                    bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
                    if(g_tZigbeeHACoordinator.start == FALSE)
                    {
                        App_ZIGBEE_HA_Init_Var();// zigbee 初始化
                        g_tZigbeeHACoordinator.start = TRUE;//              
                        g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//开
                    }
                
                    bsp_SetTimer(TMR_ID_caiji_gps_cycle);//手动模式，短按按键一次采集一次、开始采集gps的位置信息       
                    bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//手动模式，短按按键一次采集一次、开始采集温湿度的信息       
                    
                    App_caiji_report_start();//
                    App_nb_Init_start();//nb 开始
                        
                    p_info("短按，开始采集数据");
                }               
            }
                break;      
            /*确定键长按按下*/       
            case KEY_LONG_SUREU: 
            {
                /*关机了*/
                if(g_wenshi.power_onoff == POWER_OFF)
                {
                    g_wenshi.power_onoff = POWER_ON;// 开机哦
                    g_tConfig.power_onoff = g_wenshi.power_onoff;//保存开关机状态
                    App_Config_Save_Into_Flash();//计算累加和，保存到flash
                    App_fault_analysis_Set_Value(REBOOT_FLAG_KEY_POWEROFF_TOON);
                    
                    #if 0
                    g_wenshi.work_state = WENSHI_INIT;
                    
                    /*  显示光照强度*/
                    App_display_illumination(g_wenshi.m_Lux,TRUE);
                    /*  显示温湿度*/
                    App_display_temp(g_wenshi.m_T/10,TRUE);
                    App_display_humidity(g_wenshi.m_H/10,TRUE);
                    /*  显示电池电量*/
                    App_display_battery_value(g_wenshi.m_voltage_level,TRUE);// 2                   
                    p_info("长按，开机");

                    App_caiji_report_start();//
                    App_nb_Init_start();//nb 开始
                    #else
                    HAL_NVIC_SystemReset();//重启
                    #endif
                }
                /*开机*/
                else if(g_wenshi.power_onoff == POWER_ON)
                {
                    #if 1
                    g_wenshi.power_onoff = POWER_OFF;//关机
                    g_tConfig.power_onoff = g_wenshi.power_onoff;//关机
                    App_Config_Save_Into_Flash();//计算累加和，保存到flash
                    
                    App_wenshi_poweroff();
                    p_info("长按，关机");
                    #else
                    HAL_NVIC_SystemReset();

                    #endif
                }
            }
                break;  
            /*确定键抬起*/     
            case KEY_UP_SURE: 
            {
    
            }
                break;  
                                
            default:
                break;
                    
        }
    }

}

/*
*********************************************************************************************************
*   函 数 名: App_wenshi_Init_Var
*   功能说明:  
*   形    参：无
*   返 回 值: 无

*********************************************************************************************************
*/
void App_wenshi_Init_Var(void)
{   
    g_key_press = 0;

    memset(&g_wenshi,0,sizeof(g_wenshi));
    g_wenshi.power_onoff = POWER_ON;// 开机哦
    
    g_wenshi.device_ID[0] = 0;
    g_wenshi.device_ID[1] = VERSION_1;
    g_wenshi.device_ID[2] = VERSION_1;
    g_wenshi.device_ID[3] = VERSION_1;
    g_wenshi.device_ID[4] = 0;
    g_wenshi.device_ID[5] = 2;
    
    p_info("device_ID;%02x %02x %02x %02x %02x %02x",g_wenshi.device_ID[0],g_wenshi.device_ID[1],g_wenshi.device_ID[2],\
        g_wenshi.device_ID[3],g_wenshi.device_ID[4],g_wenshi.device_ID[5]);

    g_wenshi.camera_state = 0;
    g_wenshi.flag_start_camera =0;// 服务器采集摄像头数据

    g_wenshi.work_state = WENSHI_INIT;

    g_wenshi.version_number = VERSION_WENSHI_CODE;


    g_wenshi.m_flag_gps = FALSE;
    App_SocketStatusReport_clear_bak();//清理备份

    g_wenshi.server_ok_count = 0 ;// 服务器计数

    g_wenshi.address_sensor = 0 ;
}



/*
*********************************************************************************************************
*   函 数 名: App_wenshi_poweroff
*   功能说明: 进入关机状态的准备工作
*   形    参：无
*   返 回 值: 无

*********************************************************************************************************
*/
void App_wenshi_poweroff(void)
{
    bsp_HT1621_Show_NULL();// 清除屏幕内容

    Bsp_gps_power_off();
    Bsp_nb_power_off();
    Bsp_sensor_power_off();Bsp_sensor_power_off_power_off();
    Bsp_12V_power_off();
    Bsp_zigbee_power_off();
    Bsp_spiflash_power_off();   


    //HAL_UART_DeInit(hlpuart1);// 调试日志使用
    HAL_UART_DeInit(&huart1);
    HAL_UART_DeInit(&huart3);
    HAL_UART_DeInit(&huart5);

}


/*
*********************************************************************************************************
*   函 数 名: App_wenshi_Loop
*   功能说明:  
*   形    参：无

1\ UI =key  lcd
2\COmMUNICATOR = 
3\sensor = GPS =TH Lumia
4\chaozuo, banding ,action .

*   返 回 值: 无
*********************************************************************************************************
*/
void App_wenshi_Loop(void)
{   
    switch(g_wenshi.work_state)
    {
        case WENSHI_INIT :
        {       
            App_ZIGBEE_HA_Init_Var();// zigbee 初始化
            //g_tZigbeeHACoordinator.start = TRUE;//
        
            g_wenshi.work_state = WENSHI_LOOP;
            bsp_SetTimer(TMR_ID_caiji_gps_cycle);//开始采集gps的位置信息       
            
            bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//开始采集温湿度的信息       
            bsp_SetTimer(TMR_ID_Charge_Voltage_Indicator);//开始采集充电信息

            app_fifo_NB_CoapST_Init();//

            p_info("g_wenshi WENSHI_INIT");
        }
            break;
            
        case WENSHI_LOOP :
        {               
            #if  1
            App_caiji_report_Loop(); /* 采集数据的上报 */
            #endif
            App_caiji_sensor_address();//地址更新
            
            App_display_Loop(); /* 跟新显示内容 */
            
            /* 4个登录响应不成功           超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_server_ok))
            {
                App_fault_analysis_Set_Value(REBOOT_FLAG_NB_INIT);
            
                p_info("服务器通讯，不正常 重启" );
                bsp_DelayMS(10);
                HAL_NVIC_SystemReset();//重启
            }

            /*gps  8小时采集一次、 超时退出的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_gps_cycle))
            {
                bsp_StartTimer(TMR_ID_caiji_gps_cycle, TMR_DELAY_48hour);// 60秒钟倒计时采集gps信息。 
                
                /*开始 gps 采集初始化*/ 
                g_caiji.start_gps = TRUE;
                g_caiji.work_state_gps = CAIJI_GPS_INIT;

                p_info("TMR_ID_caiji_gps_cycle  timer out");                
            }
            
            RXVALUE_TYPE_ST  _CmdCode;                  
            uint32_t CmdCode = 0;
            memset((uint8_t *)&_CmdCode,0,sizeof(_CmdCode));
            if((g_caiji.soil_TH_work_state == CAIJI_PLC_WRITE_COIL)||(g_caiji.soil_TH_work_state == CAIJI_PLC_WRITE_COIL_DELAY))
            {
                // 等待 write 结束  再次查询cmd FIFO
            }
            else
            {
                CmdCode = bsp_GetCmd(& _CmdCode);       
                if(CmdCode != Cmd_NONE)
                {
                    g_caiji.soil_TH_start = TRUE;
                    g_caiji.soil_TH_work_state = CAIJI_PLC_WRITE_COIL;// 进入write 状态
                    MX_UART5_Init();//初始化串口5防止死机； 20211007

                    g_sheshisuoPLC.reg_retry_tx_count = 0;// 
                    g_sheshisuoPLC.rx_valuetype = _CmdCode.valuetype;   
                    g_sheshisuoPLC.rx_occupied  = _CmdCode.occupied;                    
                    g_sheshisuoPLC.rx_value     = _CmdCode.value;
                    g_sheshisuoPLC.tx_mid       = _CmdCode.mid;
                }

            }


            
            App_caiji_Air_T_H_Loop();//采集温度湿度
            
            if(g_Camera.start_state == FALSE)
            {

                if(g_tConfig.PLC_Type == PLC_TIANJIN)
                {
                    #ifdef ENABLE_TIANJING_PLC
                    App_caiji_PLC_Loop();
                    #endif
                }
                else if(g_tConfig.PLC_Type == PLC_SHESHISUO)
                {                   
                    #ifdef ENABLE_SHESHISUO_PLC
                    App_sheshisuo_PLC_Loop();
                    #endif
                }
                else//(g_tConfig.PLC_Type == PLC_NULL)
                {
                    App_caiji_turang_Loop();
                }
            }   
            else
            {
                App_camera_Loop();
            }               
            //App_Config_RS485_Com();/*串口设置时间*/ 
            //App_ZIGBEE_HA_loop();         
            #if 1
            /* NB模块的函数处理*/
            MX_TimerPoll();// NB的专用定时器
            HAL_UART_Poll();// 串口数据通过回调函数进行解析。NB和 GPS
            //NBModule_Main(&nb_config);
            #endif
        
            //App_fifo_NB_CoapST_Loop();// cmd  fifo
            //App_nb_Loop();      //nb 通信
            app_4G_loop();
            //App_Update_Loop();//   升级
            app_update4G_loop();
        }       
            break;  
            
        default:
            break;
    }
}



