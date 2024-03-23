#include "app_wenshi.h"
#include "debug.h"


#include "bsp.h"

#include "app.h"

#define CTWING_DATA_UP (0X02)// �������ݱ���
#define CTWING_DATALENTH_INDEX1 (1)// �������ݱ��ĳ��ȵ�ַ1
#define CTWING_DATALENTH_INDEX2 (2)// �������ݱ��ĳ��ȵ�ַ2

struct WENSHI g_wenshi ;
uint8_t g_key_press = 0;
uint16_t g_mid = 0;
uint16_t msgid = 0;//����msgid

extern void App_SOCKET_STATE_ask(_End_Device_data_type * pEndDevice,uint8_t work_type);

/*
*********************************************************************************************************
*   �� �� ��: App_server_gettime
*   ����˵��:  ������������ʱ��
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_server_gettime(unsigned long gmt_time_server)
{   
    unsigned long gmt_time = 0;
    unsigned long gmt_time_2018 = 0;
    unsigned int year[6];//int year =0;     int mon =0;     int day =0;     int hour =0;        int min =0;     int sec =0;

    /*ʱ�� ����*/
    gmt_time_2018 = mktime_second(2018,1,1,0,0,0);
    gmt_time =gmt_time_2018+ gmt_time_server; 
    
    GMT_toLocalTime(gmt_time,&year[0],&year[1],&year[2],&year[3],&year[4],&year[5]);
    p_info("1ʱ�����%d-%d-%d, %d:%d:%d",year[0],year[1],year[2],year[3],year[4],year[5]);

    year[0] = year[0] - 2000;//Min_Data = 0 and Max_Data = 99��

    if(MX_RTC_Set(year[0],year[1],year[2],year[3],year[4],year[5])==0)      
    {
        App_caiji_report_gettime_ok();//ֹͣ����Уʱ����

    }
    else
    {
        return ;
    }

}


//******************************************************************************
// fn : App_NB_HexStrToNum
//
// brief : ��ʮ�������ַ���ת������
//
// param : 
//         str -> ʮ�������ַ�������
//
// return : ��ַ
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
*   �� �� ��: ChangeNum
*   ����˵��: ʮ�������ַ��� ת����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/

int ChangeNum(char* str, int length)
{
    char revstr[16] = { 0 };// ?//����ʮ�������ַ����ĳ��ȣ�����ע�����鲻ҪԽ�� ?
//  int num[16] = { 0 };
    int count = 1;
    int result = 0;
    int tmp = 0;
    
    strcpy(revstr, str);

    for (int i = length - 1; i >= 0; i--)
    {
        if ((revstr[i] >= '0') && (revstr[i] <= '9'))
        //num[i] = revstr[i] - 48;//�ַ�0��ASCIIֵΪ48?
            tmp = revstr[i] - 48;//�ַ�0��ASCIIֵΪ48?
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
        count = count * 16;//ʮ������(����ǰ˽��ƾ����������8) ? ? ?
    }
    return result;
}


/*
*********************************************************************************************************
*   �� �� ��: App_Dinfo_Data_process
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_Dinfo_Data_process(u8 pos,char* msg)
{
    char *p=0;
    char name[32];//��ʱ���� ����16������
    int lenth = 0;
    
    int i,j,n=0,len;
    
    int a[16];//10 ������ 
    
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
    memcpy(&g_tConfig.Name[0],name,12);//ֻ����12���ַ����������
    
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
*   �� �� ��: App_DataRx_process
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
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

    /*֡���͵��ж�*/
    messageId_h = msg[pos];
    pos++;
    messageId = (messageId_h<<8)|msg[pos];
    pos++;  
    /*mid���ж�*/
    g_mid = msg[pos];
    pos++;  
    g_mid = (g_mid<<8)|msg[pos];
    pos++;  

    p_info("messageId=%02X",messageId);

     /*  ����֡ 02*/
    if((messageId == messageId_SOCKET_STATE))/*  ��������   SOCKET_STATE    0xa005 , 0xa006 ����  yess */
    {
        /*  ����ͷ�����е�ʱ�򣬲��ܽ��в����Ŀ���5����*/
        if(g_Camera.start_state == TRUE)
        {       
            return ret;
        }
        _End_Device_data_type * pEndDevice;// �ն�
        u8 rx_macAddr[ZIGBEE_MAC_ADDRESS_SIZE];// ���յ����ն˵�mac
        char addres[3] = {0};//�м����
        int return_value = 0;
        
        App_SocketStatusReport_clear_bak();//������

        /* ʱ��service_time   int32s  4   �� ������ʱ��,��2018��1��1��0ʱ0��0�뿪ʼ��unixʱ���      ��ȡ*/
        gmt_time_server|= msg[pos]<<24;
        pos++;  
        gmt_time_server|= msg[pos]<<16;
        pos++;  
        gmt_time_server|= msg[pos]<<8;
        pos++;  
        gmt_time_server|= msg[pos]<<0;
        pos++;  
        
        //App_server_gettime(gmt_time_server);      
        
        /* mac��ַ��ȡ*/
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
        /*zigbee ��������*/
        bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
        if(g_tZigbeeHACoordinator.start == FALSE)
        {
            App_ZIGBEE_HA_Init_Var();// zigbee ��ʼ��
            g_tZigbeeHACoordinator.start = TRUE;//              
            g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//��
        }       
        /* ��6���ն��� ƥ�䵽��Ӧ������ָ��*/

        for(i=1;i<=g_tZigbeeHACoordinator.End_Device_nums;i++)
        {
            pEndDevice = EndDeviceNums_To_EndDevice(i);
            return_value = (memcmp(pEndDevice->macAddr,rx_macAddr,ZIGBEE_MAC_ADDRESS_SIZE));
            if(return_value == 0)
            {
                g_tZigbeeHAEndDevice_cmd_ASK = pEndDevice;// 
                p_info("rx���Ʋ���=%d",i);

                /* �����Ŀ����͹ر�*/
                g_tZigbeeHACoordinator.work_type_update = TRUE;
                g_tZigbeeHACoordinator.work_type = msg[pos];
                pos++;  
                break;
            }
            else
            {
                p_info("rx���Ʋ���������=%d",i);
            }
        }
        ret = 0;
    }
    else if((messageId == messageId_ZIGBEE_NETWORK))/* �豸����������� ZIGBEE_NETWORK  0xa003 , 0xa004 ����  yes*/
    {
        uint8_t rx_cmd_zigbee_network_onoff = 0;
        uint8_t report_time = 0;

        App_SocketStatusReport_clear_bak();//������
        /* ʱ��service_time   int32s  4   �� ������ʱ��,��2018��1��1��0ʱ0��0�뿪ʼ��unixʱ���      ��ȡ*/
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
                App_ZIGBEE_HA_Init_Var();// zigbee ��ʼ��
                g_tZigbeeHACoordinator.start = TRUE;//              
                g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//��
            }           
            p_info("��ZigBee����");
        }
        else
        {
            g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = FALSE;
            p_info("�ر�ZigBee����");
        }

        App_ZIGBEE_NETWORK_ask(g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff);     
        APP_STATE = NB_CoAP_ST;   //��������
        p_info("App_ZIGBEE_NETWORK_ask ��������");                  

        /*  �ϱ�ʱ����޶�Ϊ4����         */
        report_time =msg[pos];
        pos++;
        if(report_time <= 3)
            report_time = 3;
        
        if(rx_cmd_zigbee_network_onoff == TRUE)
        {
            p_info("��ZigBee����:%dS",g_tZigbeeHACoordinator.report_time );
        }
        else
        {
            p_info("�ر�ZigBee����:%dS",g_tZigbeeHACoordinator.report_time );
        }

        ret = 0;
    }
    else if((messageId == messageId_DEVICE_STATE))/* �豸�����������   ZIGBEE_NETWORK  0xa003 , 0xa004 ����  yes*/
    {
        uint8_t rx_data_lenth = 0;
        char data[16] = {0};//�м����

        App_SocketStatusReport_clear_bak();//������

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
                    p_info("���յ� getpic");                 
                }               
            }
            if(strstr(&msg[pos],"poweroffpic"))
            {
                if(g_Camera.start_state == TRUE)
                {                
                    g_Camera.poweroffpic = 1;
                    p_info("���յ� poweroffpic");                    
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
                    sscanf(p+13,"%d",&getpicpack_count);//����bai����������ְ�du%d��ȡ��i��
                    g_Camera.getpicpack_count = getpicpack_count ;

                    p_info("���յ� getpicpack");                 
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
                App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash

                p_info("���յ� dreport");    
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
                App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash

                p_info("���յ� setreport");      
                App_DeviceState_cmdAsk(CMD_ASK_setReport);
                APP_STATE = NB_CoAP_ST;   //��������
                p_info("App_DeviceState_setreport_ask��������");                    
                
            }           
            if(strstr(&msg[pos],"dinfo,"))
            {
                App_Dinfo_Data_process(pos,msg);

                p_info("���յ� dinfo");                                  
                g_caiji.info_server_ask_ok =TRUE;
            }
            if(strstr(&msg[pos],"plcw,"))
            {
                #ifdef ENABLE_TIANJING_PLC          
                App_plc_Dinfo_Data_process(pos,msg);
                p_info("���յ� plcw");                                   
                g_caiji.info_server_ask_ok =TRUE;
                #endif

                #ifdef ENABLE_SHESHISUO_PLC         
                App_sheshisuo_PLC_Dinfo_Data_process(pos,msg);
                p_info("���յ� plcw");                                   
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
                    App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash
                    p_info("���յ� setplctype:%d,ok",plc_type);      
                }
                else
                {
                    p_err("���յ� setplctype,no ok!");       
                }
                App_caiji_report_start();//�����ϱ�             
            }   
            /* remote reboot                */
            if(strstr(&msg[pos],"rebootmcu"))
            {
                App_fault_analysis_Set_Value(REBOOT_FLAG_REMOTE);                           
                printf("������ remote ����" );
                bsp_DelayMS(10);
                HAL_NVIC_SystemReset();//����
            }       
            /* clearcount                   */
            if(strstr(&msg[pos],"clearcount"))
            {
                App_fault_analysis_Clear_reboot_cout();
                App_caiji_report_start();//�����ϱ�
            }   
            /* reportbasic                  */
            if(strstr(&msg[pos],"reportbasic"))
            {
                App_caiji_report_start();//�����ϱ�
            }               
            return 0;
        }

        /* ʱ��service_time timer��ȡ   dtime,81204459*/
        pos++;  pos++;  pos++;  
        pos++;  pos++;  pos++;  
        
        memset(data,0x00,sizeof(data));
        for(i=0;i<(rx_data_lenth-6);i++)
        {
            data[i] = msg[pos];
            pos++;  
        }
        
        gmt_time_server=atoi(data);//81204459

        /* ʱ��service_time   int32s  4   �� ������ʱ��,��2018��1��1��0ʱ0��0�뿪ʼ��unixʱ���      ��ȡ*/      
        App_server_gettime(gmt_time_server);

        /*�ȵ��ö�ʱ�亯�����ٵ��ö����ں��������ɽ�����⡣*/
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

        APP_STATE = NB_CoAP_ST;   //��������
        p_info("App_DeviceState_gettime_ask��������");                  

        ret = 0;
    }   
    else if((messageId == messageId_DeviceState))/*  0x0001  */
    {
        g_caiji.rx_data_in_flag =TRUE;
        p_info("rx messageId_DeviceState ok");                  
    }
    else if((messageId == messageId_SensorBasic))/*  0x0002  */
    {
        App_fault_analysis_Clear_Value();//ֻ���һ��

        g_caiji.sensor_basic_server_ask_ok=TRUE;
        g_wenshi.server_ok_count=0;//ֻҪ�յ����ݾ���0��
        p_info("messageId_SensorBasic ������0");                     
    }   
    else if((messageId == messageId_SensorExtend))/*  0x0003  */
    {
        g_Camera.server_ask_ok = 1;//  
        p_info("rx server_ask_ok!");                    
    }   
    return ret;
}



json_info_t json_info_bak;// ����

/*
*********************************************************************************************************
*   �� �� ��: App_SocketStatusReport_clear_bak
*   ����˵��:  ���������ϱ��ı���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_SocketStatusReport_clear_bak(void)
{   
    memset(&json_info_bak,0,sizeof(json_info_bak)); 
}

/*
*********************************************************************************************************
*   �� �� ��: App_SocketStatusReport_01
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��

    �ֶ�  ��������    ����  Ĭ��ֵ   ƫ��ֵ
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
    ��Ӧ�ֶ�    AAAA0000            

*********************************************************************************************************
*/
uint8_t App_SocketStatusReport_01(void)
{
    u16 lenth = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t return_value = 0;

    _End_Device_data_type * pEndDevice;// �ն�

    
    /*  ����֮ǰ���滻��last �ϴε���ֵ    */


    /* */
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    /* message id */
    json_info.json_buf[lenth] = messageId_Socket>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_Socket;
    lenth= lenth + 1;

    /*socket_data_length    int16u  2   �� ���� */
    lenth= lenth + 1;
    lenth= lenth + 1;


    /* 6������������ */
    for(j=End_Device1;j<=End_Device6;j++)
    {
        pEndDevice = EndDeviceNums_To_EndDevice(j);

        /* ����mac��ַ*/
        for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
        {
            sprintf(&json_info.json_buf[lenth],"%02X", pEndDevice->macAddr[i]);     
            lenth= lenth + 2;       
        }
        /* ,*/ 
        json_info.json_buf[lenth] = ',';
        lenth= lenth + 1;       
        
        /* ��������״̬*/ 
        sprintf(&json_info.json_buf[lenth],"%X", pEndDevice->flag_on_network);  
        lenth= lenth + 1;   
        /* ,*/ 
        json_info.json_buf[lenth] = ',';
        lenth= lenth + 1;   
        
        /* ��������״̬*/ 
        sprintf(&json_info.json_buf[lenth],"%X", pEndDevice->socket_state);     
        lenth= lenth + 1;   
        /* ,*/ 
        json_info.json_buf[lenth] = ';';
        lenth= lenth + 1;       
    }           

    /* ���������ϱ�*/ 
    sprintf(&json_info.json_buf[lenth],"%03d", g_tZigbeeHACoordinator.End_Device_nums); 
    lenth= lenth + 3;   
    /* ,*/ 
    json_info.json_buf[lenth] = ',';
    lenth= lenth + 1;

    /*socket_data_length    int16u  2   �� ���� */
    json_info.json_buf[2] = (lenth-4)>>8;
    json_info.json_buf[3] = (lenth-4)>>0;

    json_info.json_len = lenth;


    return_value = memcmp(&json_info_bak,&json_info,sizeof(json_info_t));
    if(return_value == 0)
    {
        //p_info("�ϱ��Ĳ���������ͬ�����ϱ�");
        return FALSE;
    }
    else
    {
        memcpy(&json_info_bak,&json_info,sizeof(json_info_t));
        p_info("�ϱ��Ĳ������ݲ���ͬ���ϱ�");
        return TRUE;
    }
                    
    
}


/*
*********************************************************************************************************
*   �� �� ��: App_DataReport_04
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��

�ֶ�  ��������    ����  ƫ��ֵ
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

    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DataReport_04");
        return;
    }
    /*  ����֮ǰ���滻��last �ϴε���ֵ    */
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
    /*�����¶�*/
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
    /*����ʪ��*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*��������*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    
    /*�����¶�*/
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
    /*����ʪ��*/
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       

    /*co2*/
    json_info.json_buf[lenth] = g_wenshi.airTH;
    lenth= lenth + 1;       

    if(g_wenshi.m_flag_gps)
    {
        /*����*/          
        sprintf(&json_info.json_buf[lenth],"%03.06f", g_wenshi.m_longtitude);       
        lenth= lenth + 10;      
        
        /*γ��*/      
        sprintf(&json_info.json_buf[lenth],"%02.06f", g_wenshi.m_latitude);         
        lenth= lenth + 9;               

    }
    else
    {
        /*����*/          
        sprintf(&json_info.json_buf[lenth],"%03.08f", g_wenshi.m_longtitude);       
        lenth= lenth + 10;      
        /*γ��*/      
        sprintf(&json_info.json_buf[lenth],"%02.06f", g_wenshi.m_latitude);         
        lenth= lenth + 9;       
        
    }   
    
    /*��ص���*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    lenth= lenth + 1;   

    /*���ϴ���*/
    json_info.json_buf[lenth] =   g_wenshi.fault_number;    
    lenth= lenth + 1;   
    /*�汾��*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*ʱ��*/
    sprintf(&json_info.json_buf[lenth],"20%02d%02d%02d%02d%02d%02d", g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);      
    lenth= lenth + 14;      
    
    json_info.json_len = lenth;
}

#define LENTH_TEMP (256u)
static char SensorBasic_data_tx[482];
static char SensorBasic_data_temp[LENTH_TEMP];

#ifdef ONLY_8SAME_AIRTHSENSOR // 8������
extern uint8_t addressAirTh[8];
/*
*********************************************************************************************************
*   �� �� ��: App_DataReport_SensorBasic
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��

SensorBasic
�豸��������������
��������            ��������    ����  �Ƿ��ѡ    ��ע
electricity int 0 ~ 100 �� ����
fault_number    int 0 ~ 100 �� ���Ϻ�
version_number  int 1000 ~ 60000    �� �汾��
report_time string  14  �� �ϱ�ʱ��
*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    
    /*  ����֮ǰ���滻��last �ϴε���ֵ    */
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

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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
    /*���ϴ���  fault_analysis 2 */
    /*��λ����  reboot_cout 2*/
    /*��ص���*/    
    /*���״̬����һ���ֶ�  1��0*/
    /*���ϴ��� �ź�ǿ��*/
    /*�汾��*/
    /*�����¶�*/
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
    /*����ʪ��*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*����ǿ��*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    
    /*�����¶�*/
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
    /*����ʪ��*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*����*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        /*����*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;   
        
    }   
    
    /*��ص���*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*���״̬����һ���ֶ�  1��0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*���ϴ��� �ź�ǿ��*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*�汾��*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }   
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    
	/*��չ������ PH\  NPK\co2*/

    /*��չ������ AIRTH 0- 7 */
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


    /*��չ������ �����ݾ��ϴ� PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    //if(sensor_data_lenth >0)
    {
        json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
        lenth= lenth + 1;
    
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + sensor_data_lenth;   
    }
    
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    //app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    if(gUpdate4G.updateStart == FALSE)//���������У����ٷ������ݸ�4gģ��
        app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


/**************************************          ALL_SENSORS     ****************************************************/

#ifdef ALL_SENSORS // ԭ���Ĵ�����
/*
*********************************************************************************************************
*   �� �� ��: App_DataReport_SensorBasic
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��

SensorBasic
�豸��������������
��������            ��������    ����  �Ƿ��ѡ    ��ע
electricity int 0 ~ 100 �� ����
fault_number    int 0 ~ 100 �� ���Ϻ�
version_number  int 1000 ~ 60000    �� �汾��
report_time string  14  �� �ϱ�ʱ��
*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    
    /*  ����֮ǰ���滻��last �ϴε���ֵ    */
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

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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
    /*���ϴ���  fault_analysis 2 */
    /*��λ����  reboot_cout 2*/
    /*��ص���*/    
    /*���״̬����һ���ֶ�  1��0*/
    /*���ϴ��� �ź�ǿ��*/
    /*�汾��*/
    /*�����¶�*/
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
    /*����ʪ��*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*����ǿ��*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    
    /*�����¶�*/
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
    /*����ʪ��*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*����*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        /*����*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;   
        
    }   
    
    /*��ص���*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*���״̬����һ���ֶ�  1��0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*���ϴ��� �ź�ǿ��*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*�汾��*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }   
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    
//    /*�������豸4G�汾*/
//    /*ʱ��*/
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
    /*��չ������ ����վ*/
    if(g_wenshi.weatherStationExisted == 1)// ���ڹ� ����վ����Ҫ�ϱ� ���� ���� ���ߣ�
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
	/*��չ������ PH\  NPK\co2*/
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

    /*��չ������ ����*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_TH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        /*�����¶�*/
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
    /*��չ������ LUX*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_LUX)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_SOIL_LUX,ADDRESS_TURANG_LUX,g_wenshi.m_soil_LUX);   
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);
    }
    /*��չ������ AIRTH*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_AIRTH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,%d,%d,%d,%d;",SENSOR_ID_AIRTH_SHESHISUO,ADDRESS_AIR_TH,g_wenshi.airTHvalueH,g_wenshi.airTHvalueT,
        g_wenshi.airTHvalueTwet,g_wenshi.airTHvalueLevel,g_wenshi.airTHvalueHcal);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }   
    /*   LED��Ļ */
    if((g_wenshi.mLedFlag == 1))// ���ڹ�LED ���� ���� null 
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d;",SENSOR_ID_LED_DISPLAY,ADDRESS_LED_DISPLAY,g_wenshi.m_led_display);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }
    else // ������ ������null
    {

    }

    /*   PLC ��Ļ */
    if(g_wenshi.mPlcFlag == 1)// ���ڹ� PLC ���� ���� null 
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        sprintf(SensorBasic_data_temp,"%d,%d,%d,1,%d,2,%d;",SENSOR_ID_PLC_SHESHISUO2,ADDRESS_PLC_DISPLAY,
            g_wenshi.mPlcdisplay,g_wenshi.mPlcStateInsulation,g_wenshi.mPlcStateRoofVentilation);  
        strcat(SensorBasic_data_tx,SensorBasic_data_temp);  
    }       
    else
    {

    }
    /*��չ������ �����ݾ��ϴ� PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    //if(sensor_data_lenth >0)
    {
        json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
        lenth= lenth + 1;
    
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + sensor_data_lenth;   
    }
    
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    //app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    if(gUpdate4G.updateStart == FALSE)//���������У����ٷ������ݸ�4gģ��
        app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


#if 0
/*
*********************************************************************************************************
*   �� �� ��: App_DataReport_SensorBasic
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��

SensorBasic
�豸��������������
��������            ��������    ����  �Ƿ��ѡ    ��ע
air_temperature string  6   �� �����¶�
air_humidity    int     0 ~ 100 �� ����ʪ��
light_intensity int     0 ~ 10000   �� ����ǿ��
soil_temperature  string    6   �� �����¶�
soil_moisture   int     0 ~ 100 �� ����ʪ��
co2_concentration   int 0 ~ 100 �� co2Ũ��
longitude   string  10  �� Gps����
latitude    string  10  �� Gpsγ��
electricity int 0 ~ 100 �� ����
fault_number    int 0 ~ 100 �� ���Ϻ�
version_number  int 1000 ~ 60000    �� �汾��
report_time string  14  �� �ϱ�ʱ��


*********************************************************************************************************
*/
void App_DataReport_SensorBasic(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    json_info_t json_info;

    /*  ����֮ǰ���滻��last �ϴε���ֵ    */
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


    /* ������֡*/

    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SensorBasic>>8;//����������  SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SensorBasic;
    lenth= lenth + 1;   
    /*�����¶�*/
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
    /*����ʪ��*/
    json_info.json_buf[lenth] = g_wenshi.m_H/10;
    lenth= lenth + 1;       
    
    /*����ǿ��*/
    json_info.json_buf[lenth] = g_wenshi.m_Lux>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.m_Lux;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    
    /*�����¶�*/
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
    /*����ʪ��*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.m_soil_H/10;
    lenth= lenth + 1;       
    #else
    json_info.json_buf[lenth] = g_tConfig.PLC_Type;
    lenth= lenth + 1;   
    #endif
    /*co2*/
    #if 0
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_wenshi.airTHvalueH;
    lenth= lenth + 1;       
#else
    json_info.json_buf[lenth] = g_Indicat.reboot_cout>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;       
    json_info.json_buf[lenth] = g_Indicat.reboot_cout;
    lenth= lenth + 1;
#endif
    if(g_wenshi.m_flag_gps)
    {       
        /*����*/  
        l_long = (g_wenshi.m_longtitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        l_long = (g_wenshi.m_latitude*10000000);
        json_info.json_buf[lenth] = l_long>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = l_long>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        /*����*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        /*γ��*/  
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;   
        
    }   
    
    /*��ص���*/
    json_info.json_buf[lenth] =   g_wenshi.m_voltage_level;     
    /*���״̬����һ���ֶ�  1��0*/
    if(g_tBattery.charge_flag == CHARGE_YES)
        json_info.json_buf[lenth] |=  (1<<4);   
    else
        json_info.json_buf[lenth] &=  ~(1<<4);  
    lenth= lenth + 1;   


    /*���ϴ��� �ź�ǿ��*/
    json_info.json_buf[lenth] =   g_wenshi.m_nb_signal_level;//g_wenshi.fault_number;   
    lenth= lenth + 1;
    
    /*�汾��*/
    json_info.json_buf[lenth] =   g_wenshi.version_number>>8;   
    lenth= lenth + 1;       
    json_info.json_buf[lenth] =   g_wenshi.version_number;  
    lenth= lenth + 1;   

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }

    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));
    /*��չ������ PH\  NPK\WATER*/
    if(g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_TH)
    {
        memset(SensorBasic_data_temp,0,sizeof(SensorBasic_data_temp));
        /*�����¶�*/
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
    /*��չ������ PH\  NPK\WATER*/
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
    ռλ  �ֶ�����    ��ע
    0   PLC��ʶ��    �豸��ʶ��
    1   ����ģʽ    ״̬��1=�ֶ�;0=Զ��
    2   1#���Զ��ָʾ    1=��ͨ;0=�Ͽ�
    3   1#�����תָʾ    1=��ͨ;0=�Ͽ�
    4   1#�����תָʾ    1=��ͨ;0=�Ͽ�
    5   1#�������ָʾ    1=��ͨ;0=�Ͽ�
    6   1#��ת��λָʾ    1=��ͨ;0=�Ͽ�
    7   1#��ת��λָʾ    1=��ͨ;0=�Ͽ�
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

    
    /*��չ������ �����ݾ��ϴ� PH\  NPK\WATER*/
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    if(sensor_data_lenth >0)
    {
        //if(sensor_data_lenth >=1) 
        //  sensor_data_lenth=sensor_data_lenth-1;
        
        json_info.json_buf[lenth] =sensor_data_lenth>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = sensor_data_lenth;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        
        strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + sensor_data_lenth;   
    }

    json_info.json_len = lenth;
    app_fifo_NB_CoapST_Put(json_info.json_buf,&json_info.json_len);
    
}
#endif

/*
*********************************************************************************************************
*   �� �� ��: App_DataReport_SensorExtend
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DataReport_SensorExtend(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DataReport_SensorExtend");   
        //return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SensorExtend>>8;//���������� SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SensorExtend;
    lenth= lenth + 1;

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
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

    json_info.json_buf[lenth] =(sensor_data_lenth)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}

#if 0

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_Report_NBID
*   ����˵��: �ϱ�NB��ID     IMEI     IMSI
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_Report_NBID(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_Report_NBID");   
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��

    lenth= lenth + sensor_data_lenth;
    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        
    json_info.json_len = lenth;
}
#endif



/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_Report_Connectivity
*   ����˵��: �ϱ����߲���
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_Report_Connectivity(void)
{
    u16 lenth = 0;

    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_Report_Connectivity");   
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = CTWING_DATA_UP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        
    json_info.json_len = lenth;
}
#if 0

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_gettime
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_gettime(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;

    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_gettime");   
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth; 

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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

    strcpy(&json_info.json_buf[lenth],"gettime");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
}

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_gettime_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_gettime_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


//  /*  ���ͻ����������ݣ����ڷ��͡�  */
//  if(json_info.json_len)
//  {
//      p_err("App_DeviceState_gettime_ask");   
//      return;
//  }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;


    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// ָ���·���Ӧ
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// ����룺0-�ɹ�������serviceId�Ͳ�����1-�ɹ�����serviceId�Ͳ�����2-�������ݴ���ʧ�ܣ�����serviceId�Ͳ���
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"dtime,%u",(l_long-l_long_s));
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif
#if 0

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_getinfo
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_getinfo(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getinfo");   
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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

    strcpy(&json_info.json_buf[lenth],"getinfo");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        
    json_info.json_len = lenth;
}



/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_getreport
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_getreport(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getreport"); 
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;


    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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
    strcpy(&json_info.json_buf[lenth],"getreport");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
        
    json_info.json_len = lenth;
}
#endif

#if 0
/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_setreport_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_setreport_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


//  /*  ���ͻ����������ݣ����ڷ��͡�  */
//  if(json_info.json_len)
//  {
//      p_err("App_DeviceState_setreport_ask"); 
//      return;
//  }
    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// ָ���·���Ӧ
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// ����룺0-�ɹ�������serviceId�Ͳ�����1-�ɹ�����serviceId�Ͳ�����2-�������ݴ���ʧ�ܣ�����serviceId�Ͳ���
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"%u,%d,%d",(l_long-l_long_s),g_tConfig.report_second,g_tConfig.report_changed);
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif
/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_getpic_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_getpic_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_getpic_ask");    
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//���������� SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
    lenth= lenth + 1;
    
    //�豸ִ�����������ִ�н���ϱ��е�midҪ���յ������е�mid����һ�£�����ƽ̨����ˢ�¶�Ӧ�����״̬
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   
 
 
    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }

    /*sensor_data_lenth*/
    //lenth= lenth + 1;
    //lenth= lenth + 1;
    
    sprintf(&json_info.json_buf[lenth+2],"%d,%d,%d,%d",ADDRESS_CAMERA,g_Camera.datas_lenth,g_Camera.all_packages,g_Camera.data_lenth_each_packages);
    sensor_data_lenth = strlen(&json_info.json_buf[lenth+2]);

    json_info.json_buf[lenth] =sensor_data_lenth>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;
    json_info.json_buf[lenth] = sensor_data_lenth;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}



/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_poweroffpic_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_poweroffpic_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_DeviceState_poweroffpic_ask");   
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_DEVICE_STATE_ask>>8;//���������� SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_DEVICE_STATE_ask;
    lenth= lenth + 1;
    
    //�豸ִ�����������ִ�н���ϱ��е�midҪ���յ������е�mid����һ�£�����ƽ̨����ˢ�¶�Ӧ�����״̬
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   
 
 
    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }

    /*sensor_data_lenth*/
    //lenth= lenth + 1;
    //lenth= lenth + 1;
    
    sprintf(&json_info.json_buf[lenth+2],"%d,1",ADDRESS_CAMERA);
    sensor_data_lenth = strlen(&json_info.json_buf[lenth+2]);

    json_info.json_buf[lenth] =sensor_data_lenth>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;
    json_info.json_buf[lenth] = sensor_data_lenth;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + 1;

//  strcpy(&json_info.json_buf[lenth],"dtime");// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;
    
    json_info.json_len = lenth;
}

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_gettime_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_SOCKET_STATE_ask(_End_Device_data_type * pEndDevice,uint8_t work_type)
{
    u16 lenth = 0;
//  u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    u16 i = 0;

    /*  ���ͻ����������ݣ���Ҫ���͡�  */  
    if(json_info.json_len)
    {
        bsp_StartTimer(TMR_ID_ZIGBEE_HA_cmd_socke_state_ask,TMR_DELAY_3S);//��ѯ���ϱ�         
        p_err("App_SOCKET_STATE_ask"); 
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_SOCKET_STATE_ask>>8;//���������� SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_SOCKET_STATE_ask;
    lenth= lenth + 1;   

    //�豸ִ�����������ִ�н���ϱ��е�midҪ���յ������е�mid����һ�£�����ƽ̨����ˢ�¶�Ӧ�����״̬
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;
    

    /*ʱ��device_time int32s  4   �� �豸ʱ��,��2018��1��1��0ʱ0��0�뿪ʼ��unixʱ�����*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }

    /* ��������״̬*/ 
    #if 0
    json_info.json_buf[lenth] =1;// pEndDevice->flag_on_network;
    #else
    json_info.json_buf[lenth] =  pEndDevice->flag_on_network;
    #endif
    lenth= lenth + 1;   
    
    /* ����mac��ַ*/
    for(i=0;i<ZIGBEE_MAC_ADDRESS_SIZE;i++)
    {
        sprintf(&json_info.json_buf[lenth],"%02X", pEndDevice->macAddr[i]);     
        lenth= lenth + 2;       
    }
    
    /* ��������״̬*/ 
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
*   �� �� ��: App_DeviceState_gettime_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_ZIGBEE_NETWORK_ask(uint8_t network)
{
    u16 lenth = 0;
//  u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;


    /*  ���ͻ����������ݣ����ڷ��͡�  */
    if(json_info.json_len)
    {
        p_err("App_ZIGBEE_NETWORK_ask"); 
        return;
    }

    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[lenth] = messageId_ZIGBEE_NETWORK_ask>>8;//����������   SensorBasic 0x0002
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)messageId_ZIGBEE_NETWORK_ask;
    lenth= lenth + 1;   

    //�豸ִ�����������ִ�н���ϱ��е�midҪ���յ������е�mid����һ�£�����ƽ̨����ˢ�¶�Ӧ�����״̬
    json_info.json_buf[lenth] = g_mid>>8;//mid  2
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (u8)g_mid;
    lenth= lenth + 1;   


    json_info.json_buf[lenth] = 0x00;//errcode  1
    lenth= lenth + 1;

    /*ʱ��device_time int32s  4   �� �豸ʱ��,��2018��1��1��0ʱ0��0�뿪ʼ��unixʱ�����*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    
    /* ��������״̬*/ 
    json_info.json_buf[lenth] = network;    
    lenth= lenth + 1;   
    
    json_info.json_len = lenth;
}

#if 0
/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_setAcqTimer_ask
*   ����˵��: 
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_setAcqTimer_ask(void)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// ָ���·���Ӧ
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = g_mid>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = g_mid;
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;// ����룺0-�ɹ�������serviceId�Ͳ�����1-�ɹ�����serviceId�Ͳ�����2-�������ݴ���ʧ�ܣ�����serviceId�Ͳ���
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    sprintf(SensorBasic_data_tx,"%u,%d",(l_long-l_long_s),g_tConfig.setAcqTimer);
    
    sensor_data_lenth = strlen(SensorBasic_data_tx);
    json_info.json_buf[lenth] = (sensor_data_lenth)>>8;//  
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (sensor_data_lenth)>>0;//  
    lenth= lenth + 1;
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}
#endif


/*
*********************************************************************************************************
*   �� �� ��: app_deviceState_updateResult
*   ����˵��: ��������¼��ϱ�		updateResult,0,V101
*   ��   �Σ���  Incident reporting
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_deviceState_eventReport(EVENT_REPORT_E eventType)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth; 

    json_info.json_buf[0] = APP4G_MSG_DATAUP;//��������֡ͷ0x02
    lenth= lenth + 1;

    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X00;//�������ݳ��� 2
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

    strcpy(&json_info.json_buf[lenth],SensorBasic_data_temp);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}

/*
*********************************************************************************************************
*   �� �� ��: App_DeviceState_cmdAsk
*   ����˵��: ����ask
*   ��   �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_DeviceState_cmdAsk( CMD_ASK_TYPE_E asktype)
{
    u16 lenth = 0;
    u16 sensor_data_lenth = 0;
    
    unsigned long l_long = 0;
    unsigned long l_long_s = 0;
    /* ������֡*/
    memset(&json_info,'0',sizeof(json_info));
    json_info.json_len = lenth;
    
    json_info.json_buf[0] = APP4G_MSG_DATADOWN_ASK;// ָ���·���Ӧ
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

    json_info.json_buf[lenth] = 0X00;// ����룺0-�ɹ�������serviceId�Ͳ�����1-�ɹ�����serviceId�Ͳ�����2-�������ݴ���ʧ�ܣ�����serviceId�Ͳ���
    lenth= lenth + 1;   
    json_info.json_buf[lenth] = 0X01;// 
    lenth= lenth + 1;   

    json_info.json_buf[lenth] = APP4G_SERVICEID_DEVICESTATEask>>8;
    lenth= lenth + 1;
    json_info.json_buf[lenth] = (char)APP4G_SERVICEID_DEVICESTATEask;
    lenth= lenth + 1;   

    /*sensor_data_lenth*/
    memset(SensorBasic_data_tx,0,sizeof(SensorBasic_data_tx));

    /*ʱ��*/
    l_long = mktime_second(g_Date.Year,g_Date.Month,g_Date.Date,g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
    l_long_s = mktime_second(18,1,1,0,0,0);
    if(l_long>=l_long_s)
    {
        json_info.json_buf[lenth] = (l_long-l_long_s)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (l_long-l_long_s)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
    }
    else
    {
        json_info.json_buf[lenth] = (0)>>24;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>16;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
        lenth= lenth + 1;
        json_info.json_buf[lenth] = (0)>>0;// ���ֽ��ڵ�λ��ַ�����ģʽ��
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
    
    strcpy(&json_info.json_buf[lenth],SensorBasic_data_tx);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    lenth= lenth + sensor_data_lenth;

    json_info.json_buf[CTWING_DATALENTH_INDEX1] =(lenth-3)>>8;// ���ֽ��ڵ�λ��ַ�����ģʽ��
    json_info.json_buf[CTWING_DATALENTH_INDEX2] = (lenth-3);// ���ֽ��ڵ�λ��ַ�����ģʽ��
    
    json_info.json_len = lenth;
    app_4G_sendData(json_info.json_buf,&json_info.json_len);
}

/*
*********************************************************************************************************
*   �� �� ��: App_key_process
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_key_process(void)
{
    
    uint8_t ucKeyCode;  

    /* �������¼� */
    ucKeyCode = bsp_GetKey_set();
    if (ucKeyCode > 0)
    {
        switch (ucKeyCode)
        {
            /*ȷ��������*/     
            case KEY_DOWN_SURE: 
            {
                /*�ػ���*/
                if(g_wenshi.power_onoff == POWER_ON)
                {
                    /*zigbee ��������*/
                    bsp_StartTimer(TMR_ID_ZIGBEE_HA_OFF_TIMER,TMR_DELAY_2min);
                    if(g_tZigbeeHACoordinator.start == FALSE)
                    {
                        App_ZIGBEE_HA_Init_Var();// zigbee ��ʼ��
                        g_tZigbeeHACoordinator.start = TRUE;//              
                        g_tZigbeeHACoordinator.rx_cmd_zigbee_network_onoff = TRUE;//��
                    }
                
                    bsp_SetTimer(TMR_ID_caiji_gps_cycle);//�ֶ�ģʽ���̰�����һ�βɼ�һ�Ρ���ʼ�ɼ�gps��λ����Ϣ       
                    bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//�ֶ�ģʽ���̰�����һ�βɼ�һ�Ρ���ʼ�ɼ���ʪ�ȵ���Ϣ       
                    
                    App_caiji_report_start();//
                    App_nb_Init_start();//nb ��ʼ
                        
                    p_info("�̰�����ʼ�ɼ�����");
                }               
            }
                break;      
            /*ȷ������������*/       
            case KEY_LONG_SUREU: 
            {
                /*�ػ���*/
                if(g_wenshi.power_onoff == POWER_OFF)
                {
                    g_wenshi.power_onoff = POWER_ON;// ����Ŷ
                    g_tConfig.power_onoff = g_wenshi.power_onoff;//���濪�ػ�״̬
                    App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash
                    App_fault_analysis_Set_Value(REBOOT_FLAG_KEY_POWEROFF_TOON);
                    
                    #if 0
                    g_wenshi.work_state = WENSHI_INIT;
                    
                    /*  ��ʾ����ǿ��*/
                    App_display_illumination(g_wenshi.m_Lux,TRUE);
                    /*  ��ʾ��ʪ��*/
                    App_display_temp(g_wenshi.m_T/10,TRUE);
                    App_display_humidity(g_wenshi.m_H/10,TRUE);
                    /*  ��ʾ��ص���*/
                    App_display_battery_value(g_wenshi.m_voltage_level,TRUE);// 2                   
                    p_info("����������");

                    App_caiji_report_start();//
                    App_nb_Init_start();//nb ��ʼ
                    #else
                    HAL_NVIC_SystemReset();//����
                    #endif
                }
                /*����*/
                else if(g_wenshi.power_onoff == POWER_ON)
                {
                    #if 1
                    g_wenshi.power_onoff = POWER_OFF;//�ػ�
                    g_tConfig.power_onoff = g_wenshi.power_onoff;//�ػ�
                    App_Config_Save_Into_Flash();//�����ۼӺͣ����浽flash
                    
                    App_wenshi_poweroff();
                    p_info("�������ػ�");
                    #else
                    HAL_NVIC_SystemReset();

                    #endif
                }
            }
                break;  
            /*ȷ����̧��*/     
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
*   �� �� ��: App_wenshi_Init_Var
*   ����˵��:  
*   ��    �Σ���
*   �� �� ֵ: ��

*********************************************************************************************************
*/
void App_wenshi_Init_Var(void)
{   
    g_key_press = 0;

    memset(&g_wenshi,0,sizeof(g_wenshi));
    g_wenshi.power_onoff = POWER_ON;// ����Ŷ
    
    g_wenshi.device_ID[0] = 0;
    g_wenshi.device_ID[1] = VERSION_1;
    g_wenshi.device_ID[2] = VERSION_1;
    g_wenshi.device_ID[3] = VERSION_1;
    g_wenshi.device_ID[4] = 0;
    g_wenshi.device_ID[5] = 2;
    
    p_info("device_ID;%02x %02x %02x %02x %02x %02x",g_wenshi.device_ID[0],g_wenshi.device_ID[1],g_wenshi.device_ID[2],\
        g_wenshi.device_ID[3],g_wenshi.device_ID[4],g_wenshi.device_ID[5]);

    g_wenshi.camera_state = 0;
    g_wenshi.flag_start_camera =0;// �������ɼ�����ͷ����

    g_wenshi.work_state = WENSHI_INIT;

    g_wenshi.version_number = VERSION_WENSHI_CODE;


    g_wenshi.m_flag_gps = FALSE;
    App_SocketStatusReport_clear_bak();//������

    g_wenshi.server_ok_count = 0 ;// ����������

    g_wenshi.address_sensor = 0 ;
}



/*
*********************************************************************************************************
*   �� �� ��: App_wenshi_poweroff
*   ����˵��: ����ػ�״̬��׼������
*   ��    �Σ���
*   �� �� ֵ: ��

*********************************************************************************************************
*/
void App_wenshi_poweroff(void)
{
    bsp_HT1621_Show_NULL();// �����Ļ����

    Bsp_gps_power_off();
    Bsp_nb_power_off();
    Bsp_sensor_power_off();Bsp_sensor_power_off_power_off();
    Bsp_12V_power_off();
    Bsp_zigbee_power_off();
    Bsp_spiflash_power_off();   


    //HAL_UART_DeInit(hlpuart1);// ������־ʹ��
    HAL_UART_DeInit(&huart1);
    HAL_UART_DeInit(&huart3);
    HAL_UART_DeInit(&huart5);

}


/*
*********************************************************************************************************
*   �� �� ��: App_wenshi_Loop
*   ����˵��:  
*   ��    �Σ���

1\ UI =key  lcd
2\COmMUNICATOR = 
3\sensor = GPS =TH Lumia
4\chaozuo, banding ,action .

*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_wenshi_Loop(void)
{   
    switch(g_wenshi.work_state)
    {
        case WENSHI_INIT :
        {       
            App_ZIGBEE_HA_Init_Var();// zigbee ��ʼ��
            //g_tZigbeeHACoordinator.start = TRUE;//
        
            g_wenshi.work_state = WENSHI_LOOP;
            bsp_SetTimer(TMR_ID_caiji_gps_cycle);//��ʼ�ɼ�gps��λ����Ϣ       
            
            bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//��ʼ�ɼ���ʪ�ȵ���Ϣ       
            bsp_SetTimer(TMR_ID_Charge_Voltage_Indicator);//��ʼ�ɼ������Ϣ

            app_fifo_NB_CoapST_Init();//

            p_info("g_wenshi WENSHI_INIT");
        }
            break;
            
        case WENSHI_LOOP :
        {               
            #if  1
            App_caiji_report_Loop(); /* �ɼ����ݵ��ϱ� */
            #endif
            App_caiji_sensor_address();//��ַ����
            
            App_display_Loop(); /* ������ʾ���� */
            
            /* 4����¼��Ӧ���ɹ�           ��ʱ�˳����ж�   */
            if(bsp_CheckTimer(TMR_ID_server_ok))
            {
                App_fault_analysis_Set_Value(REBOOT_FLAG_NB_INIT);
            
                p_info("������ͨѶ�������� ����" );
                bsp_DelayMS(10);
                HAL_NVIC_SystemReset();//����
            }

            /*gps  8Сʱ�ɼ�һ�Ρ� ��ʱ�˳����ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_gps_cycle))
            {
                bsp_StartTimer(TMR_ID_caiji_gps_cycle, TMR_DELAY_48hour);// 60���ӵ���ʱ�ɼ�gps��Ϣ�� 
                
                /*��ʼ gps �ɼ���ʼ��*/ 
                g_caiji.start_gps = TRUE;
                g_caiji.work_state_gps = CAIJI_GPS_INIT;

                p_info("TMR_ID_caiji_gps_cycle  timer out");                
            }
            
            RXVALUE_TYPE_ST  _CmdCode;                  
            uint32_t CmdCode = 0;
            memset((uint8_t *)&_CmdCode,0,sizeof(_CmdCode));
            if((g_caiji.soil_TH_work_state == CAIJI_PLC_WRITE_COIL)||(g_caiji.soil_TH_work_state == CAIJI_PLC_WRITE_COIL_DELAY))
            {
                // �ȴ� write ����  �ٴβ�ѯcmd FIFO
            }
            else
            {
                CmdCode = bsp_GetCmd(& _CmdCode);       
                if(CmdCode != Cmd_NONE)
                {
                    g_caiji.soil_TH_start = TRUE;
                    g_caiji.soil_TH_work_state = CAIJI_PLC_WRITE_COIL;// ����write ״̬
                    MX_UART5_Init();//��ʼ������5��ֹ������ 20211007

                    g_sheshisuoPLC.reg_retry_tx_count = 0;// 
                    g_sheshisuoPLC.rx_valuetype = _CmdCode.valuetype;   
                    g_sheshisuoPLC.rx_occupied  = _CmdCode.occupied;                    
                    g_sheshisuoPLC.rx_value     = _CmdCode.value;
                    g_sheshisuoPLC.tx_mid       = _CmdCode.mid;
                }

            }


            
            App_caiji_Air_T_H_Loop();//�ɼ��¶�ʪ��
            
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
            //App_Config_RS485_Com();/*��������ʱ��*/ 
            //App_ZIGBEE_HA_loop();         
            #if 1
            /* NBģ��ĺ�������*/
            MX_TimerPoll();// NB��ר�ö�ʱ��
            HAL_UART_Poll();// ��������ͨ���ص��������н�����NB�� GPS
            //NBModule_Main(&nb_config);
            #endif
        
            //App_fifo_NB_CoapST_Loop();// cmd  fifo
            //App_nb_Loop();      //nb ͨ��
            app_4G_loop();
            //App_Update_Loop();//   ����
            app_update4G_loop();
        }       
            break;  
            
        default:
            break;
    }
}



