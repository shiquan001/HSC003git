#include "app_caiji.h"

#include "bsp.h"

#include "app.h"



MYMODBUS_T gModbus;

struct CAIJI g_caiji ;
char  rx_data_gps[UART3_RX_BUF_SIZE];

/*
*********************************************************************************************************
*   函 数 名: App_caiji_Init_Var
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_report_Init_Var(void)
{   
    memset(&g_caiji,0,sizeof(g_caiji)); 
    
    g_caiji.work_state_report = REPORT_CAIJI_INIT;
    g_caiji.gettime_tx_count = 0;
    
    g_caiji.start_gps = FALSE;
    g_caiji.work_state_gps = CAIJI_GPS_MAX;
    
    g_caiji.soil_TH_start = FALSE;
    g_caiji.soil_TH_work_state = CAIJI_soil_TH_MAX;
    
    g_caiji.rx_data_in_flag =FALSE;
    g_caiji.sensor_basic_server_ask_ok=FALSE;
    g_caiji.timer_server_ask_ok=FALSE;
    g_caiji.report_server_ask_ok=FALSE;
    g_caiji.info_server_ask_ok=FALSE;
    
}


/*
*********************************************************************************************************
*   函 数 名: App_caiji_Init_Var
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_report_gettime_ok(void)
{   

    g_caiji.gettime_tx_count = REPORT_CAIJI_gettime_count;//提前结束count
    
    bsp_SetTimer(TMR_ID_report_refresh);

}

/*
*********************************************************************************************************
*   函 数 名: App_caiji_report_start
*   功能说明: 开始上报数据
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_report_start(void)
{   
    bsp_SetTimer(TMR_ID_report_loop);//开始上报数据
}

/*
*********************************************************************************************************
*   函 数 名: App_caiji__Loop
*   功能说明: get gps signal
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_gps_Loop(void)
{           
    //char ucData = 0;
    //uint16_t pos = 0;
    if(g_caiji.start_gps == TRUE)
    {

    }
    else
    {
        return;
    }

    switch(g_caiji.work_state_gps)
    {
        case CAIJI_GPS_INIT :
        {               
            g_caiji.work_state_gps = CAIJI_GPS_OPEN;

            bsp_StartTimer(TMR_ID_caiji_gps_refresh , TMR_DELAY_10min);
            p_info("g_caiji CAIJI_GPS_INIT");
        }
            break;
        case CAIJI_GPS_OPEN :
        {   
            /*gps5min 超时退出的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_gps_refresh))
            {
                Bsp_gps_power_on();//打开gps电源            
            
                MX_LPUART1_UART_Init();
                bsp_GPS_Init(App_gps_Msg_cb); //初始化串口

                bsp_StartTimer(TMR_ID_caiji_gps_refresh , TMR_DELAY_5min);//TMR_DELAY_2min
                g_caiji.work_state_gps = CAIJI_GPS_LOOP;

                p_info("g_caiji 打开gps电源");              
            }
        }       
            break;          
        case CAIJI_GPS_LOOP :
        {   
            #if 0
            /*解析接收到的串口数据     */ 
            if(bsp_CheckTimer( TMR_ID_GPS_UART_DATAIN))
            {
                memset(rx_data_gps,0,sizeof(rx_data_gps));
    
                while(1)
                {
                    if(comGetChar(GPS_UART1, &ucData))
                    {           
                        rx_data_gps[pos++] = ucData;
                    }
                    else
                        break;
                
                    if(pos >= UART3_RX_BUF_SIZE)
                        break;
                }
                p_info("gps<=%d",pos);

                GPS_handle_msg(rx_data_gps,pos);
            }
            #endif

            /*等待获得有效的gps数据     */
            App_gps_Get();
            
            /*gps5min 超时退出的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_gps_refresh))
            {
                g_caiji.work_state_gps = CAIJI_GPS_CLOSE;

                p_info("g_caiji gps timer out");                
            }
        }       
            break;  
        case CAIJI_GPS_CLOSE :
        {   
            #if 1
            HAL_UART_Close(GPS);// 关闭gps串口
            #endif          
            /*超时关闭gps电源*/
            Bsp_gps_power_off();
            g_caiji.start_gps = FALSE;
            g_caiji.work_state_gps = CAIJI_GPS_MAX;



            p_info("g_caiji 关闭gps电源");              
        }       
            break;              
        default:
            break;
    }
}





/**************************************************************************************                      
* 名    App_485_tx_turang_T_H_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：采集土壤温湿度
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_turang_T_H_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x02;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* 名    App_485_tx_turang_PH_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
[10:25:13.926]发→◇01 03 00 00 00 01 84 0A □
[10:25:13.964]收←◆01 03 02 00 46 39 B6 
PH值：0046 H(十六进制)= 70=> PH = 7.0 
* 出口参数：无
* 说    明：采集土壤PH
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_turang_PH_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
/**************************************************************************************                      
* 名    App_485_tx_turang_NPKmd
* 功    能:
* 入口参数：

    [11:07:01.637]发→◇03 03 00 1E 00 03 64 2F  
    [11:07:01.666]收←◆03 03 06 00 00 00 00 00 00 38 15

* 出口参数：无
* 说    明：采集土壤PH
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_turang_NPK_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x1E;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x03;
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* 名    App_485_tx_turang_water_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
01 03 02 00 0A 38 43 
01 03 02 00 0B F9 83 

* 出口参数：无
* 说    明：采集土壤water
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_turang_water_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
#define LED_NUM_REGISTER 13//12 // led显示屏幕  显示 6个地址
#if 1
/**************************************************************************************                      
* 名    App_485_tx_led_display_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：刷新屏幕  上海宇目智能科技有限公司LED的屏幕。
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
#define NEW_SENSOR 
void App_485_tx_led_display_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
#ifdef NEW_SENSOR
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = LED_NUM_REGISTER;    
    buffer_lenth++; 

    buffer[buffer_lenth] = LED_NUM_REGISTER*2;    
    buffer_lenth++;     

/*
1、温度
2、湿度
3、液位
4、CO2
5、光照
6、光照
7、土壤温度
8、土壤湿度
9、土壤PH
10、N
11、P
12、K
13、湿球温度 0.1
*/
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT/10)>>8; //
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT/10)>>0;    //0.1精度给LED屏幕
    buffer_lenth++;         
    
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH/10)>>8; //
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH/10)>>0;    //0.1精度给LED屏幕
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueLevel)>>8;    //1精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueLevel)>>0;    //1精度给LED屏幕
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_co2_value)>>8; //co2
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_co2_value)>>0; //co2
    buffer_lenth++; 
    
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>0; 
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T)>>8;   //0.1精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T)>>0;   //0.1精度给LED屏幕
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H)>>8;   //0.1精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H)>>0;   //0.1精度给LED屏幕
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_PH)>>8; // 0.1 PH
    buffer_lenth++;        
    buffer[buffer_lenth] = (g_wenshi.m_soil_PH)>>0; // PH
    buffer_lenth++;         
        
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[0]*10)>>8; //N
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[0]*10)>>0; //
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[1]*10)>>8; //P
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[1]*10)>>0; //
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[2]*10)>>8; //
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_soil_N[2]*10)>>0; //K
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.airTHvalueTwet/10)>>8; //
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueTwet/10)>>0;    //0.1精度给LED屏幕
    buffer_lenth++;         
    
#else
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 6;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 6*2;    
    buffer_lenth++;     
    
    /*
    00 1F 环境温度
    00 29 环境湿度
    00 0A 土壤温度
    00 50  土壤湿度
    02 65  光照。
    */

    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>8;    //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>0;    
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>8;    //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>0;    
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>8;   //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>0;   
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>8;   //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>0;   
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>0; 
    buffer_lenth++; 
#endif


    /*crc 16*/
    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
/**************************************************************************************                      
* 名    app_485TxPlcDisplay_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：刷新屏幕  上海宇目智能科技有限公司LED的屏幕。
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void app_485TxPlcDisplay_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x06;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 12  ;    
    buffer_lenth++;     
/*
数据名称	地址位置	寄存器类型
空气温度	00	寄存器
空气湿度	01	寄存器
土壤温度	02	寄存器
土壤湿度	03	寄存器
光照	    04 05	寄存器

*/
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>8;    //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>0;    
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>8;    //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>0;    
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>8;   //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>0;   
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>8;   //0.01精度给LED屏幕
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>0;   
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //，光照则直接显示原来的数据
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>0; 
    buffer_lenth++; 

    /*crc 16*/
    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

#else
/**************************************************************************************                      
* 名    App_485_tx_led_display_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：刷新屏幕
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_led_display_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x0c;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 0x18;    
    buffer_lenth++;     
/*
00 1F 环境温度
00 29 环境湿度
00 0A 土壤温度
00 50  土壤湿度
02 65  光照
00 02  灌溉状态选择 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中

00 02  环境温度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
00 02  环境湿度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
00 02  土壤温度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
00 02  土壤湿度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
00 02  光照箭头     0-不显示、1-红上、2-红下、3-绿上、4-绿下。
00 01  灌溉状态箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
*/
    buffer[buffer_lenth] = (g_wenshi.m_T/10)>>8;    
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_T/10)>>0;    
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_H/10)>>8;    
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_H/10)>>0;    
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T/10)>>8;   
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T/10)>>0;   
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H/10)>>8;   
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H/10)>>0;   
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; 
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>0; 
    buffer_lenth++; 

    if((g_wenshi.m_sensor_TXflag&SENSOR_TX_SERVER_TURANG_WATER))
    {
        if(g_wenshi.m_soil_Water==1)
        {
            buffer[buffer_lenth] = 0;   // 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中、4-不显示
            buffer_lenth++; 
            buffer[buffer_lenth] = 0;   // 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中、4-不显示
            buffer_lenth++; 
        }
        else
        {
            buffer[buffer_lenth] = 0;   // 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中、4-不显示
            buffer_lenth++; 
            buffer[buffer_lenth] =2;    
            buffer_lenth++; 
        }
    }
    else// 没有传感器
    {
        buffer[buffer_lenth] = 0;   // 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中、4-不显示
        buffer_lenth++; 
        buffer[buffer_lenth] = 4;   // 0-灌溉中、1-开始灌溉、2-灌溉结束、3-灌溉中、4-不显示
        buffer_lenth++; 
    }
    /*
    00 02  环境温度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    00 02  环境湿度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    00 02  土壤温度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    00 02  土壤湿度箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    00 02  光照箭头     0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    00 01  灌溉状态箭头 0-不显示、1-红上、2-红下、3-绿上、4-绿下。
    */
    if((g_wenshi.m_T)> (g_tConfig.T_up*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =3;    
        buffer_lenth++; 
    }
    else if((g_wenshi.m_T)< (g_tConfig.T_down*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =4;    
        buffer_lenth++; 
    }
    else
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =0;    
        buffer_lenth++;     
    }
    
    if((g_wenshi.m_H)> (g_tConfig.H_up*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =3;    
        buffer_lenth++; 
    }
    else if((g_wenshi.m_H)< (g_tConfig.H_down*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =4;    
        buffer_lenth++; 
    }
    else
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =0;    
        buffer_lenth++;     
    }   

    if((g_wenshi.m_soil_T)> (g_tConfig.soil_T_up*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =3;    
        buffer_lenth++; 
    }
    else if((g_wenshi.m_soil_T)< (g_tConfig.soil_T_down*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =4;    
        buffer_lenth++; 
    }
    else
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =0;    
        buffer_lenth++;     
    }
    
    if((g_wenshi.m_soil_H)> (g_tConfig.soil_H_up*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =3;    
        buffer_lenth++; 
    }
    else if((g_wenshi.m_soil_H)< (g_tConfig.soil_H_down*10))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =4;    
        buffer_lenth++; 
    }
    else
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =0;    
        buffer_lenth++;     
    }   

    if((g_wenshi.m_Lux)> (g_tConfig.Lux_up))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =3;    
        buffer_lenth++; 
    }
    else if((g_wenshi.m_Lux)< (g_tConfig.Lux_down))
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =4;    
        buffer_lenth++; 
    }
    else
    {
        buffer[buffer_lenth] =0;    
        buffer_lenth++;         
        buffer[buffer_lenth] =0;    
        buffer_lenth++;     
    }   

    buffer[buffer_lenth] =0;    
    buffer_lenth++;         
    buffer[buffer_lenth] =0;    
    buffer_lenth++; 

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

#endif




/**************************************************************************************                      
* 名    App_485_tx_pt100_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：采集pt100湿度
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_pt100_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}









/**************************************************************************************                      
* 名      App_485_tx_UV_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：采集百叶窗温湿度和光照
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_UV_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

/**************************************************************************************                      
* 名    App_485_tx_TH_LUX_cmd
* 功    能:
* 入口参数：
    address:   地址   
    function:       功能码
* 出口参数：无
* 说    明：采集百叶窗温湿度和光照
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_TH_LUX_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x04;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* 名    App_485_tx_CO2_cmd
* 功    能:
* 入口参数：   
* 出口参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_CO2_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x02;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* 名              App_485_tx_airTH_cmd
* 功    能:
* 入口参数：   
* 出口参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_tx_airTH_cmd(uint8_t address ,uint8_t function,uint8_t inputData)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = inputData;   
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* 名              App_485_tx_airTH_cmd
* 功    能:
* 入口参数：   
* 出口参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_485_txModbusCmd(MYMODBUS_T * pModbus)
{
    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = pModbus->address;//地址码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = pModbus->function;//功能码
    buffer_lenth++; 
    
    buffer[buffer_lenth] = pModbus->dataAddress>>8;//寄存器起始地址 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = pModbus->dataAddress;    
    buffer_lenth++;         

    buffer[buffer_lenth] = pModbus->dataLen>>8;//寄存器长度 2字节
    buffer_lenth++;         
    buffer[buffer_lenth] = pModbus->dataLen;    
    buffer_lenth++;         

    crc1 = CRC16_Modbus(buffer, buffer_lenth);

    buffer[buffer_lenth] = crc1>>8;//    
    buffer_lenth++;     
    buffer[buffer_lenth] = (uint8_t)crc1;//  
    buffer_lenth++; 
    
    //RS485_SendBuf(buffer, buffer_lenth);

    if (g_tUart5.SendBefor != 0)
    {
        g_tUart5.SendBefor();       /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

/*
*********************************************************************************************************
*   函 数 名: App_caiji_sensor_address
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_sensor_address(void)
{
    u8 i = 0;
    u16 flag = 0;
    g_wenshi.address_sensor = 0 ;
    
    if(SENSOR_TX_SERVER_NULL == g_wenshi.m_sensor_TXflag )
    {
        g_wenshi.address_sensor = 0 ;
    }
    else
    {
        for(i=16;i>0;i--)
        {
            flag = (0x0001<<(i-1));
            if(g_wenshi.m_sensor_TXflag&flag)
            {
                g_wenshi.address_sensor++;
                //break;
            }
        }
    }
}
/*
*********************************************************************************************************
*   函 数 名: App_caiji_turang_T_H_Loop
*   功能说明: 采集土壤温度、湿度 ；NPK；PH； 灌溉传感器
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
#define RX_DATA_LENTH 256
static uint8_t rx_data_address[RX_DATA_LENTH];

uint16_t m_sensor_TXflag = SENSOR_TX_SERVER_NULL;
#define  TIMER_SENSOR  500//50//100ms  傳感器相應慢

/**************************************************************************************                      
* 名              App_485_rxModbusCmd
* 功    能:
* 入口参数：   
* 出口参数：1 is ok
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
uint8_t App_485_rxModbusCmd(MYMODBUS_T * pModbus)
{
    uint8_t ucData = 0;
    uint16_t pos = 0;
    uint8_t ret = 0;
    
    uint16_t crc1 = 0;
    uint16_t rx_crc = 0;
    uint16_t i = 0;
    
    /*等待获得有效的soil_TH数据  */    
    if (bsp_CheckTimer(TMR_ID_485_UART))
    {
        memset(rx_data_address,0,sizeof(rx_data_address));
        
        p_info("485 rx_data");
        while(1)
        {   
            if(comGetChar(Reserve_485_uart, &ucData))
            {           
                rx_data_address[pos++] = ucData;
                printf("%02X",ucData);
            }
            else
                break;
    
            if(pos >= (256))
                break;
        }
        /* log */
        if(pos <= 4)
        {
            ret = 4;
            return ret;
        }       
        for(i = 0;i<pos;i++)
        {
            if(rx_data_address[i] == pModbus->address)
            {
                break;
            }
        }
        /* 数据长度太短了 丢弃*/
        if((pos-i) <= 4)
        {
            ret = 4;
            return ret;
        }   
    
        
        /* 计算CRC校验和 */
        crc1 = CRC16_Modbus(&rx_data_address[i], pos-i-2);
        rx_crc = ((uint16_t)rx_data_address[pos-i-2] << 8 | rx_data_address[pos-i-1]);
        
        if (crc1 != rx_crc)
        {
            ret = 5;
            return ret;
        }
        /*
        应答帧（例如读到温度为-10.1℃，湿度为65.8%RH）
        地址码   功能码   字节数   湿度值   温度值   校验码低位 校验码高位
        0x01    0x03    0x04    0x02 0x92   0xFF 0x9B   0x5A    0x3D
        
        温度：当温度低于零度时以补码形式上传
        FF9B H(十六进制)= -101 => 温度= -10.1℃
        湿度：
        292 H (十六进制) = 658 => 湿度 = 65.8%RH
        */
        if((rx_data_address[i+0]==pModbus->address)&&(rx_data_address[i+1]==pModbus->function))
        {
            pModbus->AppRxCount = rx_data_address[i+2];
            memcpy(pModbus->AppRxBuf,&rx_data_address[i+3],pModbus->AppRxCount);            
            ret = 1;                        
        }
    }
    
    return ret;
}

/*
*********************************************************************************************************
*   函 数 名: app_caiji_sensorDataSync
*   功能说明: 同步air传感器数据到 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void app_caiji_sensorDataSync(void)
{   
    /* 空气温湿度 */
    g_wenshi.m_Symbol = 0x01;

    g_wenshi.m_H = g_wenshi.airTHvalueH/10;// 数据除以100得到真实的湿度数据
    g_wenshi.m_T = g_wenshi.airTHvalueT/10;// 数据除以100得到真实的湿度数据

    /* 土壤温湿度 */
    g_wenshi.m_soil_H = g_wenshi.m_soil_H;                             
    g_wenshi.m_soil_T = g_wenshi.m_soil_T;

    /* 光照强度 */
    g_wenshi.m_Lux = g_wenshi.m_soil_LUX*10;// 10
    
}
struct WENSHI g_wenshiCopy ;

void App_caiji_sensorExchange(void)
{
    g_wenshi.m_soil_PH = g_wenshiCopy.m_soil_PH;//

    g_wenshi.m_co2 =  g_wenshiCopy.m_co2 ;
    g_wenshi.m_co2_value = g_wenshiCopy.m_co2_value ;

    g_wenshi.m_soil_N[0] = g_wenshiCopy.m_soil_N[0];
    g_wenshi.m_soil_N[1] = g_wenshiCopy.m_soil_N[1];
    g_wenshi.m_soil_N[2] = g_wenshiCopy.m_soil_N[2];


    g_wenshi.weatherStationExisted = g_wenshiCopy.weatherStationExisted ;// 存在过 气象站
    g_wenshi.weatherStationOnline   = g_wenshiCopy.weatherStationOnline;//在线 气象站

    for(uint8_t i = 0;i<16;i++)
        g_wenshi.weatherStation[i] = g_wenshiCopy.weatherStation[i];

    g_wenshi.m_soil_Symbol =g_wenshiCopy.m_soil_Symbol;   
    g_wenshi.m_soil_T  = g_wenshiCopy.m_soil_T ;
    g_wenshi.m_soil_H  = g_wenshiCopy.m_soil_H ;                             
    g_wenshi.m_soil_EC  = g_wenshiCopy.m_soil_EC ;

    g_wenshi.m_soil_LUX = g_wenshiCopy.m_soil_LUX ;
    g_wenshi.m_Lux  = g_wenshiCopy.m_Lux; // 10


    g_wenshi.airTH = g_wenshiCopy.airTH;
    g_wenshi.airTHvalueH = g_wenshiCopy.airTHvalueH ;
    g_wenshi.m_H  = g_wenshiCopy.m_H  ;// 数据除以100得到真实的湿度数据
    g_wenshi.airTHvalueT  = g_wenshiCopy.airTHvalueT ;
    g_wenshi.m_T   = g_wenshiCopy.m_T ;// 数据除以100得到真实的湿度数据
    g_wenshi.airTHvalueTwet = g_wenshiCopy.airTHvalueTwet;
    g_wenshi.airTHvalueLevel = g_wenshiCopy.airTHvalueLevel;
    g_wenshi.airTHvalueHcal = g_wenshiCopy.airTHvalueHcal;

    g_wenshi.m_led_display = g_wenshiCopy.m_led_display;                                   
    g_wenshi.mLedFlag  = g_wenshiCopy.mLedFlag ;// 存在过LED

    g_wenshi.mPlcdisplay = g_wenshiCopy.mPlcdisplay ;
    g_wenshi.mPlcFlag = g_wenshiCopy.mPlcFlag ;// 存在过 PLC
    g_wenshi.mPlcCoil = g_wenshiCopy.mPlcCoil ;
    g_wenshi.mPlcStateInsulation = g_wenshiCopy.mPlcStateInsulation ;
    g_wenshi.mPlcStateRoofVentilation = g_wenshiCopy.mPlcStateRoofVentilation ;

}

/* ===========================    ONLY_8SAME_AIRTHSENSOR  ================================================*/
#ifdef ONLY_8SAME_AIRTHSENSOR
#define  SENSOR_TX_SERVER_NULL				(0X0000)  

#define  SENSOR_TX_SERVER_AIRTH1	 	    (0X0001<<0)
#define  SENSOR_TX_SERVER_AIRTH2	 	    (0X0001<<1)
#define  SENSOR_TX_SERVER_AIRTH3	 	    (0X0001<<2)
#define  SENSOR_TX_SERVER_AIRTH4	 	    (0X0001<<3)

#define  SENSOR_TX_SERVER_AIRTH5	 	    (0X0001<<4)
#define  SENSOR_TX_SERVER_AIRTH6	 	    (0X0001<<5)
#define  SENSOR_TX_SERVER_AIRTH7	 	    (0X0001<<6)
#define  SENSOR_TX_SERVER_AIRTH8	 	    (0X0001<<7)


typedef void(*Ptr_CommExeFuc) (void);


void app_cai_airTHTx()
{


}
void app_cai_airTHRx()
{


}

uint8_t addressAirTh[8] = {11,12,13,14,15,16,20,30};//温湿度传感器modbus地址15
uint8_t curAirTHNum = 0;// 总共8个  0-7

//static const Ptr_CommExeFuc CommExeFuncArray[] =
//{
//    CMDError,
//    HandShake,
//    Telemetry,//閬ユ祴
//    GetDeviceInformation,    //璁惧淇℃伅鑾峰彇鎸囦护
//    
//    GroupInfoGet,
//};

SensorAirTh_T airThSensor[8];// 总共8个  0-7

void App_caiji_turang_Loop(void)
{
    
    if((g_caiji.soil_TH_start == TRUE)&&(gUpdate4G.updateStart == FALSE))//升级过程中，不再采集传感器数据
    {

    }
    else
    {
        return;
    }

    switch(g_caiji.soil_TH_work_state)
    {
        case CAIJI_soil_TH_OPEN :
        {   
            Bsp_12V_power_on(); //12 供电。              
            Bsp_sensor_power_on(); //sensor供电。    
            MX_UART5_Init();//初始化串口5防止死机； 20211007
            bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
            g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
            m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//发送给服务器数据传感器的标志

            curAirTHNum = 0;//从0开始
            
            p_info("g_caiji 打开电源");              
        }       
            break;  

        case CAIJI_airTH_TX :
        {   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {           
                /*开始  采集air温湿度数据*/    
                gModbus.address = addressAirTh[curAirTHNum];
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 1;
                gModbus.dataLen = 5;//20秒钟采集一次；一次性读取5个数据；
                App_485_txModbusCmd(&gModbus);  
                comClearRxFifo(COM5);//COM5
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+250));//
                g_caiji.soil_TH_work_state = CAIJI_airTH_RX;
        
                p_info("g_caiji 发送查询命令 airTH:%d",curAirTHNum);                 
            }               
        }       
            break;                  

        case CAIJI_airTH_RX :
        {   
            /*等待获得有效的TH_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {                               
                airThSensor[curAirTHNum].airTH= 1;
                
                airThSensor[curAirTHNum].airTHvalueH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                p_info(" airH:%d;",airThSensor[curAirTHNum].airTHvalueH);                           
                airThSensor[curAirTHNum].m_H = airThSensor[curAirTHNum].airTHvalueH/10;// 数据除以100得到真实的湿度数据

                airThSensor[curAirTHNum].airTHvalueT = ((uint16_t)gModbus.AppRxBuf[2] << 8 | gModbus.AppRxBuf[3]);
                p_info(" airT:%d;",airThSensor[curAirTHNum].airTHvalueT);                       
                airThSensor[curAirTHNum].m_T = airThSensor[curAirTHNum].airTHvalueT/10;// 数据除以100得到真实的湿度数据

                airThSensor[curAirTHNum].airTHvalueTwet= ((uint16_t)gModbus.AppRxBuf[4] << 8 | gModbus.AppRxBuf[5]);
                p_info(" airTwet:%d;",airThSensor[curAirTHNum].airTHvalueTwet);                     

                airThSensor[curAirTHNum].airTHvalueLevel= ((uint16_t)gModbus.AppRxBuf[6] << 8 | gModbus.AppRxBuf[7]);
                if(airThSensor[curAirTHNum].airTHvalueLevel == 0xffff)//-1有水
                {
                    airThSensor[curAirTHNum].airTHvalueLevel = 1;// 1不缺水；
                }
                else//0无水
                {
                    airThSensor[curAirTHNum].airTHvalueLevel = 0;//0 缺水；
                }
                p_info(" airLevel:%d;",airThSensor[curAirTHNum].airTHvalueLevel);                       

                airThSensor[curAirTHNum].airTHvalueHcal= ((uint16_t)gModbus.AppRxBuf[8] << 8 | gModbus.AppRxBuf[9]);
                p_info(" airTHvalueHcal:%d;",airThSensor[curAirTHNum].airTHvalueHcal);                     

                m_sensor_TXflag |= (0X0001<<curAirTHNum);//发送给服务器数据传感器的标志
                p_info("g_caiji airTH 2 ok:%d",curAirTHNum);               
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
                g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
                curAirTHNum++;
                if(curAirTHNum >= 8)// 超过8退出
                {
                    g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                }
            }       
            else
            {
                airThSensor[curAirTHNum].airTH = 0;             
            }
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji airTH 2timer out:%d",curAirTHNum);         
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
                g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
                curAirTHNum++;
                if(curAirTHNum >= 8)// 超过8退出
                {
                    g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                }
            }
        }       
            break;          

        
        case CAIJI_CLOSE :
        {   
            g_wenshi.m_sensor_TXflag = m_sensor_TXflag;// 同步数据
            App_caiji_sensorExchange();//  同步数据
            
            bsp_StopTimer(TMR_ID_caiji_soil_TH_refresh);//
            HAL_UART_DeInit(&huart5);//
                
            /*超时关闭soil_TH电源*/
            Bsp_12V_power_off();
            Bsp_sensor_power_off(); //sensor供电。   
            g_caiji.soil_TH_start = FALSE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_MAX;
            
           /* :  数据变化上报 */
            if(App_data_report_process()&&(g_tConfig.report_changed))
            {
                App_caiji_report_start();//
                p_info("g_caiji 数据变化上报");               
            }
            
            p_info("g_caiji 关闭soil_TH电源");      

            //App_caiji_sensor_address();//地址更新
        }       
            break;              
        default:
            break;
    }
}

#endif


#ifdef ALL_SENSORS// 原来的传感器
void App_caiji_turang_Loop(void)
{
    int16_t m_soil_T = 0;
    
    if((g_caiji.soil_TH_start == TRUE)&&(gUpdate4G.updateStart == FALSE))//升级过程中，不再采集传感器数据
    {

    }
    else
    {
        return;
    }

    switch(g_caiji.soil_TH_work_state)
    {
        case CAIJI_soil_TH_OPEN :
        {   
            Bsp_12V_power_on(); //12 供电。              
            Bsp_sensor_power_on(); //sensor供电。    
            MX_UART5_Init();//初始化串口5防止死机； 20211007
            bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
            g_caiji.soil_TH_work_state = CAIJI_SOIL_PH_TX;
            m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//发送给服务器数据传感器的标志
            
            p_info("g_caiji 打开soil_TH电源");              
        }       
            break;  
        
        case CAIJI_SOIL_PH_TX :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_TURANG_PH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  气象站应答时间需要500ms
                g_caiji.soil_TH_work_state = CAIJI_SOIL_PH_RX_LOOP;

                p_info("g_caiji 发送查询命令PH");                 
            }                   
        }       
            break;  
            
        case CAIJI_SOIL_PH_RX_LOOP :
        {   
            /*等待获得有效的soil_TH数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_soil_PH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);              
                g_wenshiCopy.m_soil_PH = g_wenshiCopy.m_soil_PH/10;// 原始数据是放大100倍的，上传是10倍的。
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_PH;//发送给服务器数据传感器的标志
                p_info("PH:%d;",g_wenshiCopy.m_soil_PH);
                
                g_caiji.soil_TH_work_state = CAIJI_CO2_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji PH ok");   
            }        

            /*soil_TH 3s 超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_CO2_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//

                p_err("g_caiji PH timer out");             
            }
        }       
            break;              
        
        case CAIJI_CO2_TX :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_CO2;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  气象站应答时间需要500ms
                g_caiji.soil_TH_work_state = CAIJI_CO2_RX_LOOP;

                p_info("g_caiji 发送查询命令 CO2");                 
            }                   
        }       
            break;  
            
        case CAIJI_CO2_RX_LOOP :
        {   
            /*等待获得有效的soil_TH数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_co2= 1;
                g_wenshiCopy.m_co2_value = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);              
                
                m_sensor_TXflag |= SENSOR_TX_SERVER_CO2;//发送给服务器数据传感器的标志
                p_info("g_caiji CO2 1 ok:%d",g_wenshiCopy.m_co2_value);             
                
                g_caiji.soil_TH_work_state = CAIJI_SOIL_NPK_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
            }        

            /*soil_TH 3s 超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_SOIL_NPK_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//
                g_wenshiCopy.m_co2 = 0;
                
                p_err("g_caiji CO2 1 no ok");
            }
        }       
            break;              
        case CAIJI_SOIL_NPK_TX :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_TURANG_NPK;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 3;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  气象站应答时间需要500ms
                g_caiji.soil_TH_work_state = CAIJI_SOIL_NPK_RX_LOOP;

                p_info("g_caiji 发送查询命令NPK");                 
            }                   
        }       
            break;  
            
        case CAIJI_SOIL_NPK_RX_LOOP :
        {   
            /*等待获得有效的soil_TH数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                for(uint8_t num=0;num<NPKsensor_NUM;num++)
                {
                    g_wenshiCopy.m_soil_N[num] = ((uint16_t)gModbus.AppRxBuf[num*2+0] << 8 | gModbus.AppRxBuf[num*2+1]);              
                }
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_NPK;//发送给服务器数据传感器的标志
                p_info("NPK:%d;N:%d;K:%d;",g_wenshiCopy.m_soil_N[0],g_wenshiCopy.m_soil_N[1],g_wenshiCopy.m_soil_N[2]);
                
                g_caiji.soil_TH_work_state = CAIJI_WEATHER_STATION_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji NPK ok");   
            }

                
            /*soil_TH 3s 超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_WEATHER_STATION_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//

                p_err("g_caiji NPK timer out");             
            }
        }       
            break;  
                
        case CAIJI_WEATHER_STATION_TX :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_WEATHER_STATION;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 16;
                App_485_txModbusCmd(&gModbus);
                //App_485_tx_turang_T_H_cmd(ADDRESS_TURANG_TH,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  气象站应答时间需要500ms
                g_caiji.soil_TH_work_state = CAIJI_WEATHER_STATION_RX_LOOP;

                p_info("g_caiji 发送查询命令TH");                 
            }                   
        }       
            break;  
            
        case CAIJI_WEATHER_STATION_RX_LOOP :
        {   
            /*等待获得有效的soil_TH数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.weatherStationExisted = 1;// 存在过 气象站
                g_wenshiCopy.weatherStationOnline  = 1;//在线 气象站
                //memcpy((char *)&g_wenshiCopy.weatherStation[0], (char *)&gModbus.AppRxBuf[0],32); // copy  信息             
                for(uint8_t num=0;num<16;num++)
                {
                    g_wenshiCopy.weatherStation[num] = ((uint16_t)gModbus.AppRxBuf[num*2+0] << 8 | gModbus.AppRxBuf[num*2+1]);              
                }
                m_sensor_TXflag |= SENSOR_TX_SERVER_WEATHER_STATION;//发送给服务器数据传感器的标志
                p_info("气象站,风速:%d;雨量:%d;大气温度:%d;",g_wenshiCopy.weatherStation[0],g_wenshiCopy.weatherStation[1],g_wenshiCopy.weatherStation[2]);
                
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji WEATHER_STATION ok");   
            }

                
            /*soil_TH 3s 超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//
                g_wenshiCopy.weatherStationOnline  = 0;//在线 气象站

                p_err("g_caiji WEATHER_STATION timer out");             
            }
        }       
            break;  
        
        case CAIJI_soil_TH_DELAY :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_TURANG_TH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 3;
                App_485_txModbusCmd(&gModbus);
                //App_485_tx_turang_T_H_cmd(ADDRESS_TURANG_TH,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_LOOP;

                p_info("g_caiji 发送查询命令TH");                 
            }                   
        }       
            break;  
            
        case CAIJI_soil_TH_LOOP :
        {   
            /*等待获得有效的soil_TH数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                m_soil_T = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);              
                if(m_soil_T >=0.0)
                {
                    g_wenshiCopy.m_soil_Symbol=1;   
                    g_wenshiCopy.m_soil_T = m_soil_T;
                }
                else
                {
                    g_wenshiCopy.m_soil_Symbol=0;
                    g_wenshiCopy.m_soil_T = -m_soil_T;
                }
                g_wenshiCopy.m_soil_H = ((uint16_t)gModbus.AppRxBuf[2] << 8 | gModbus.AppRxBuf[3]);                             
                g_wenshiCopy.m_soil_EC = ((uint16_t)gModbus.AppRxBuf[4] << 8 | gModbus.AppRxBuf[5]);
                
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_TH;//发送给服务器数据传感器的标志
                p_info("土壤, Symbol:%c;温度:%d;湿度:%d;EC:%d;",(char)g_wenshiCopy.m_soil_Symbol,g_wenshiCopy.m_soil_T,g_wenshiCopy.m_soil_H,g_wenshiCopy.m_soil_EC);
                
                g_caiji.soil_TH_work_state = CAIJI_soil_LUX_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji soil_TH ok");   
            }

                
            /*soil_TH 3s 超时退出的判断   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_soil_LUX_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//

                p_err("g_caiji soil_TH timer out");             
            }
        }       
            break;  
        case CAIJI_soil_LUX_DELAY :
        {                   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*开始  采集土壤温湿度数据*/     
                gModbus.address = ADDRESS_TURANG_LUX;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);              
                //App_485_tx_turang_PH_cmd(ADDRESS_TURANG_LUX,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                g_caiji.soil_TH_work_state = CAIJI_soil_LUX_LOOP;

                p_info("g_caiji 发送查询命令LUX");                 
            }                   
        }       
            break;  
            
        case CAIJI_soil_LUX_LOOP :
        {   
            /*等待获得有效的m_soil_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_soil_LUX = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_LUX;//发送给服务器数据传感器的标志                                 
                p_info("土壤, m_soil_LUX:%d;",g_wenshiCopy.m_soil_LUX);
                g_wenshiCopy.m_Lux = g_wenshiCopy.m_soil_LUX*10;// 10
                
                g_caiji.soil_TH_work_state = CAIJI_AIRTH_DELAY2;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 50);//
                
                p_info("g_caiji m_soil_LUX ok");                
            }           
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_AIRTH_DELAY2;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 50);//

                p_err("g_caiji m_soil_LUX timer out");              
            }
        }       
            break;  


        case CAIJI_AIRTH_DELAY2 :
        {   
            /*soil_TH 的判断  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {           
                /*开始  采集air温湿度数据*/    
                gModbus.address = ADDRESS_AIR_TH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 1;
                gModbus.dataLen = 5;//20秒钟采集一次；一次性读取5个数据；
                App_485_txModbusCmd(&gModbus);  
                comClearRxFifo(COM5);//COM5
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+250));//
                g_caiji.soil_TH_work_state = CAIJI_AIRTH_LOOP2;
        
                p_info("g_caiji 发送查询命令 airTH");                 
            }               
        }       
            break;      
        case CAIJI_AIRTH_LOOP2 :
        {   
            /*等待获得有效的TH_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {                               
                g_wenshiCopy.airTH= 1;
                
                g_wenshiCopy.airTHvalueH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                p_info(" airH:%d;",g_wenshiCopy.airTHvalueH);                           
                g_wenshiCopy.m_H = g_wenshiCopy.airTHvalueH/10;// 数据除以100得到真实的湿度数据

                g_wenshiCopy.airTHvalueT = ((uint16_t)gModbus.AppRxBuf[2] << 8 | gModbus.AppRxBuf[3]);
                p_info(" airT:%d;",g_wenshiCopy.airTHvalueT);                       
                g_wenshiCopy.m_T = g_wenshiCopy.airTHvalueT/10;// 数据除以100得到真实的湿度数据

                g_wenshiCopy.airTHvalueTwet= ((uint16_t)gModbus.AppRxBuf[4] << 8 | gModbus.AppRxBuf[5]);
                p_info(" airTwet:%d;",g_wenshiCopy.airTHvalueTwet);                     

                g_wenshiCopy.airTHvalueLevel= ((uint16_t)gModbus.AppRxBuf[6] << 8 | gModbus.AppRxBuf[7]);
                if(g_wenshiCopy.airTHvalueLevel == 0xffff)//-1有水
                {
                    g_wenshiCopy.airTHvalueLevel = 1;// 1不缺水；
                }
                else//0无水
                {
                    g_wenshiCopy.airTHvalueLevel = 0;//0 缺水；
                }
                p_info(" airLevel:%d;",g_wenshiCopy.airTHvalueLevel);                       

                g_wenshiCopy.airTHvalueHcal= ((uint16_t)gModbus.AppRxBuf[8] << 8 | gModbus.AppRxBuf[9]);
                p_info(" airTHvalueHcal:%d;",g_wenshiCopy.airTHvalueHcal);                     

                m_sensor_TXflag |= SENSOR_TX_SERVER_AIRTH;//发送给服务器数据传感器的标志
                p_info("g_caiji airTH 2 ok");               
                
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_LED_DISPLAY;
                gModbus.address = ADDRESS_LED_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = LED_NUM_REGISTER;
                app_caiji_sensorDataSync();//同步数据之后更新LED屏幕内容
                App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//5s 给ph开机时间
                p_info("g_caiji tx LED");               
            }       
            else
            {
                g_wenshiCopy.airTH = 0;             
            }
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji airTH 2timer out");          
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_LED_DISPLAY;
                gModbus.address = ADDRESS_LED_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = LED_NUM_REGISTER;
                app_caiji_sensorDataSync();//同步数据之后更新LED屏幕内容
                App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                p_info("g_caiji tx LED");               
            }
        }       
            break;          
        case CAIJI_LED_DISPLAY :
        {   
            /*等待获得有效的m_soil_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.mLedFlag = 1;// 存在过LED
                g_wenshiCopy.m_led_display=1;                                   
                m_sensor_TXflag |= SENSOR_TX_SERVER_LED_DISPLAY;//发送给服务器数据传感器的标志
                p_info(" m_led_display:%d;;",g_wenshiCopy.m_led_display);                       
                
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_PLC_DISPLAY;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                app_485TxPlcDisplay_cmd(ADDRESS_PLC_DISPLAY,FUNCTION_CODE_10);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC");               
            }           
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_wenshiCopy.m_led_display = 0;
                p_err("g_caiji CAIJI_LED_DISPLAY timer out");               
                
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_PLC_DISPLAY;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                app_485TxPlcDisplay_cmd(ADDRESS_PLC_DISPLAY,FUNCTION_CODE_10);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC");               
            }
        }       
            break;          
        case CAIJI_PLC_DISPLAY :
        {   
            /*等待获得有效的m_soil_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_SHESHISUO;//发送给服务器数据传感器的标志
                g_wenshiCopy.mPlcFlag = 1;// 存在过 PLC
                g_wenshiCopy.mPlcdisplay = 1;
                p_info(" mPlcdisplay:%d;;",g_wenshiCopy.mPlcdisplay);                       
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_PLC_READCOIL;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_01;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                App_485_txModbusCmd(&gModbus);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC COIL");               
            }           
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji CAIJI_PLC_DISPLAY timer out");               
                g_wenshiCopy.mPlcdisplay = 0;
                
                bsp_DelayMS(50);//延时10ms 再发送
                g_caiji.soil_TH_work_state = CAIJI_PLC_READCOIL;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_01;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                App_485_txModbusCmd(&gModbus);// 刷新数据
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                
                p_info("g_caiji tx PLC COIL");               
            }
        }       
            break;                  
        case CAIJI_PLC_READCOIL://读取PLC的线圈值
        {
            /*等待获得有效的m_soil_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_SHESHISUO;//发送给服务器数据传感器的标志
                g_caiji.soil_TH_work_state = CAIJI_CLOSE;

                g_wenshiCopy.mPlcCoil = gModbus.AppRxBuf[0];
                /* 保温被 设备状态 分类 */
                if((g_wenshiCopy.mPlcCoil>>0)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Opened_STATE;
                else if((g_wenshiCopy.mPlcCoil>>1)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Stopped_STATE ;
                else if((g_wenshiCopy.mPlcCoil>>2)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Closed_STATE;
                
                /* 顶通风 设备状态 分类 */
                if((g_wenshiCopy.mPlcCoil>>3)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Opened_STATE;
                else if((g_wenshiCopy.mPlcCoil>>4)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Stopped_STATE ;
                else if((g_wenshiCopy.mPlcCoil>>5)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Closed_STATE;
            }           
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                p_err("g_caiji CAIJI_PLC_READCOIL timer out");               
            }

        }
            break;
        case CAIJI_PLC_WRITE_COIL://write PLC的线圈值
        {
			/* 大于3次，退出*/
			if(	g_sheshisuoPLC.reg_retry_tx_count> 3)
			{
				p_err("CAIJI_PLC_WRITE_COIL 大于3次，退出");	
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;// 成功了 ，则全部重新查询之后
                App_DeviceState_cmdAsk(CMD_ASK_plcwError);
			}
			else
			{
				comClearRxFifo(COM5);//COM5		
				PLC_TypeOccupiedValue_convertToAddress(6,g_sheshisuoPLC.rx_occupied,g_sheshisuoPLC.rx_value);

				bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//5秒钟采集，不能太快会导致重启
				g_caiji.soil_TH_work_state = CAIJI_PLC_WRITE_COIL_DELAY;				
				p_info("g_sheshisuoPLC 发送 CAIJI_PLC_WRITE_COIL  ");	

			}	        

        }
            break;        
        case CAIJI_PLC_WRITE_COIL_DELAY://write PLC的线圈值 delay
        {
            /*等待获得有效的m_soil_LUX 数据     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;// 成功了 ，则全部重新查询之后
                App_DeviceState_cmdAsk(CMD_ASK_plcwOk);
            }           
                
            /*0.5s 超时退出的判断     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_PLC_WRITE_COIL;
				g_sheshisuoPLC.reg_retry_tx_count++;// 
				p_err("CAIJI_PLC_WRITE_COIL_DELAY PLC_TimerOutError:%d",g_sheshisuoPLC.reg_retry_tx_count);		
            }
        }
            break;        
        
        case CAIJI_CLOSE :
        {   
            g_wenshi.m_sensor_TXflag = m_sensor_TXflag;// 同步数据
            App_caiji_sensorExchange();//  同步数据
            
            bsp_StopTimer(TMR_ID_caiji_soil_TH_refresh);//
            HAL_UART_DeInit(&huart5);//
                
            /*超时关闭soil_TH电源*/
            Bsp_12V_power_off();
            Bsp_sensor_power_off(); //sensor供电。   
            g_caiji.soil_TH_start = FALSE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_MAX;
            
           /* :  数据变化上报 */
            if(App_data_report_process()&&(g_tConfig.report_changed))
            {
                App_caiji_report_start();//
                p_info("g_caiji 数据变化上报");               
            }
            
            p_info("g_caiji 关闭soil_TH电源");      

            //App_caiji_sensor_address();//地址更新
        }       
            break;              
        default:
            break;
    }
}
#endif
/*
*********************************************************************************************************
*   函 数 名: filter
*   功能说明: 

滑动平均滤波算法只采样一次，将一次采样值和过去的若干次采样值一起求平均，得到的有效采样值即可投入使用。
如果取N个采样值求平均，存储区中必须开辟N个数据的暂存区。每新采集一个数据便存入暂存区中，
同时去掉一个最老数据，保存这N个数据始终是最新更新的数据。采用环型队列结构可以方便地实现这种数据存放方式。

*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
#define N 6// 6 point 
uint32_t filter(uint32_t data_in)
{
    static uint32_t value_buff[N];                  //N相当于选定一个窗口大小，对窗口数据做平均！
    static char flag_power_on = 0;// 0 开机之前的数据，1开机采集N次之后的数据。
    uint32_t i=0;

    char count;
    uint32_t sum=0;
    
    value_buff[i++]= data_in;
    
    if(i==N)
    {
        flag_power_on = 1;
        i=0;              //当数据大于数组长度，替换数据组的一个数据  相当于环形队列更新，先进先出！
    }
    
    for(count=0;count<N;count++)
        sum+=value_buff[count];

    if(flag_power_on == 1)
    {
        return (uint32_t)(sum/N);//1开机采集N次之后的数据。
    }
    else
    {
        return (uint32_t)(sum/i);// 0 开机之前的数据，
    }
}

/*
*********************************************************************************************************
*   函 数 名: App_caiji_Lux_Loop
*   功能说明: 
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_Lux_Loop(void)
{
    uint32_t l_lux = 0;
    /*  采集目前温室的参数 */
    if(bsp_LUX_GetLux(&l_lux) == 0)
    {       
        g_wenshi.m_Lux = filter(l_lux );
        p_info("g_caiji 光照强度:%d",g_wenshi.m_Lux);           
    }
    else
    {
        p_err("g_caiji 光照强度 ack校验失败");
    }
}


/*
*********************************************************************************************************
*   函 数 名: App_caiji_Air_T_H_Loop
*   功能说明: get  空气的温度和湿度 \光照
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_Air_T_H_Loop(void)
{       
    /*充电监测 30S  超时退出的判断  */
    if(bsp_CheckTimer(TMR_ID_Charge_Voltage_Indicator))
    {   
        bsp_StartTimer(TMR_ID_Charge_Voltage_Indicator, TMR_DELAY_30S);// 30秒钟  
        App_Charge_Voltage_Indicator(); //充电监测20210627
    }

    /*温湿度 1min 超时退出的判断  */
    if(bsp_CheckTimer(TMR_ID_caiji_T_H_cycle))
    {   
        bsp_StartTimer(TMR_ID_caiji_T_H_cycle, g_tConfig.setAcqTimer*TMR_DELAY_1S);// 倒计时采集  温湿度信息。          
        p_info("TMR_ID_caiji_T_H_cycle start");
    
        Bsp_sensor_power_on(); //sensor供电。    
        
        /*开始 soil 土壤  采集初始化*/ 
        if(g_caiji.soil_TH_work_state == CAIJI_soil_TH_MAX)// MAX 的状态下 才能进行采集
        {
            g_caiji.soil_TH_start = TRUE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;
        }
        
        App_Battery_Voltage_Display_Data(); //电池电量  
        g_wenshi.m_voltage_level = 4;g_tBattery.charge_flag = 0;
        //g_wenshi.m_voltage_level = g_tBattery.voltage_level_g;                
        p_info("g_caiji 电池电量:%d",g_tBattery.voltage_level_g);   
        
        if(g_caiji.soil_TH_work_state == CAIJI_soil_TH_MAX)
            Bsp_sensor_power_off(); //sensor供电。   

        /* :  数据变化上报 */
        if(App_data_report_process()&&(g_tConfig.report_changed))
        {
            App_caiji_report_start();//
            p_info("g_caiji 数据变化上报");               
        }       
    }

}

/*
*********************************************************************************************************
*   函 数 名: App_data_report_process
*   功能说明:  数据变化上报
*   形    参：无
*   返 回 值: 无
上次上报的数据，对比现在采集的数据，
温度0.5度，湿度5%；光照强度100lux；

*********************************************************************************************************
*/
#define  TEMP_CHA 5 // 0.5℃
#define  Humi_CHA 50 // 5%
#define  Lux_CHA 100//100lux
#define  CO2_CHA 60//30ppm

uint8_t App_data_report_process(void)
{
    /*  温度0.5度  */
    if(abs(g_wenshi.m_T-g_wenshi.m_T_last) >TEMP_CHA)
    {
        p_info("温度 数据变化上报%d",abs(g_wenshi.m_T-g_wenshi.m_T_last));                  
        return TRUE;
    }
    /*  湿度5%    */  
    if(abs(g_wenshi.m_H-g_wenshi.m_H_last) >Humi_CHA)
    {
        p_info("湿度 数据变化上报");                
        return TRUE;
    }

    /*  光照强度5%  */  
    float lux = 0;
    //float value = 0;      
    lux = abs(g_wenshi.m_Lux-g_wenshi.m_Lux_last) ;
    //value = lux/(float)g_wenshi.m_Lux;
    #if 0
    if((value>Lux_CHA)&&(lux >=8))//光照强度5%；10
    #else
    if((lux>Lux_CHA))//光照强度100差值
    #endif
    {
        p_info("光照强度 数据变化上报");              
        return TRUE;
    }

    /*  土壤温度0.5度    */
    if(abs(g_wenshi.m_soil_T-g_wenshi.m_soil_T_last) >TEMP_CHA)
    {
        p_info("土壤温度1度 数据变化上报%d",abs(g_wenshi.m_soil_T-g_wenshi.m_soil_T_last));              
        return TRUE;
    }
    /*  土壤湿度5%  */  
    if(abs(g_wenshi.m_soil_H-g_wenshi.m_soil_H_last) >Humi_CHA)
    {
        p_info("土壤湿度5% 数据变化上报");                
        return TRUE;
    }

    /*  土壤灌溉    */  
    if( g_wenshi.m_soil_Water!=g_wenshi.m_soil_Water_last )
    {
        p_info("土壤灌溉 数据变化上报");              
        return TRUE;
    }

    /*  led_display */  
    if( g_wenshi.m_led_display!=g_wenshi.m_led_display_last )
    {
        p_info("led_display 数据变化上报");               
        return TRUE;
    }   
    /*  m_pt100 */  
    if( g_wenshi.m_pt100!=g_wenshi.m_pt100_last )
    {
        p_info("m_pt100 数据变化上报");               
        return TRUE;
    }   
    /*  pt100_value */  
    if(abs(g_wenshi.m_pt100_value-g_wenshi.m_pt100_value_last) >TEMP_CHA)
    //if( g_wenshi.m_pt100_value!=g_wenshi.m_pt100_value_last )
    {
        p_info("pt100_value 数据变化上报");               
        return TRUE;
    }   

    
    /*  TH_LUX  */  
    if( g_wenshi.m_TH_LUX!=g_wenshi.m_TH_LUX_last )
    {
        p_info("TH_LUX 数据变化上报");                
        return TRUE;
    }
    /*  温度1度  */
    if(abs(g_wenshi.m_TH_LUX_t_value-g_wenshi.m_TH_LUX_t_value_last) >TEMP_CHA)
    {
        p_info("TH_LUX_t 数据变化上报%d",abs(g_wenshi.m_TH_LUX_t_value-g_wenshi.m_TH_LUX_t_value_last));                  
        return TRUE;
    }
    /*  湿度5%    */  
    if(abs(g_wenshi.m_TH_LUX_h_value-g_wenshi.m_TH_LUX_h_value_last) >Humi_CHA)
    {
        p_info("TH_LUX_h 数据变化上报");              
        return TRUE;
    }

    /*  m_TH_LUX_lux_value  */      
    /*  光照强度5%  */  
    //float lux = 0;
    //float value = 0;      
    lux = abs(g_wenshi.m_TH_LUX_lux_value-g_wenshi.m_TH_LUX_lux_value_last) ;
    //value = lux/(float)g_wenshi.m_TH_LUX_lux_value;
    #if 0
    if((value>Lux_CHA)&&(lux >=8))//光照强度5%；10
    #else
    if((lux >= 100))//光照强度
    #endif
    {
        p_info("TH_LUX_lux 数据变化上报");                
        return TRUE;
    }
    /*  co2 40ppm*/     
//    if(abs(g_wenshi.airTHvalueH_last-g_wenshi.airTHvalueH)>CO2_CHA)
//    {
//        p_info("CO2 数据变化上报");               
//        return TRUE;
//    }
//    if( g_wenshi.airTH!=g_wenshi.airTH_last)
//    {
//        p_info("CO2 数据变化上报");               
//        return TRUE;
//    }

    /*  0~15 mW/ cm2\   0-1500\ 放大了100倍紫外线指数0-15；变化了1个则上报*/       
    if(abs(g_wenshi.m_UV_value_last-g_wenshi.m_UV_value)>100)
    {
        p_info("UV 数据变化上报");                
        return TRUE;
    }
    if( g_wenshi.m_UV!=g_wenshi.m_UV_last)
    {
        p_info("UV 数据变化上报");                
        return TRUE;
    }       
    return FALSE;

}

/*
*********************************************************************************************************
*   函 数 名: App_caiji_report_Loop
*   功能说明:  
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void App_caiji_report_Loop(void)
{       
    if(seq_record & NB_SEQ_INIT)
    {
        //p_info("REPORT_CAIJI_LOOP 开始发送数据");   
    }
    else
    {
        //p_err("App_caiji_report_Loop,无连接");
        return;
    }   

    switch(g_caiji.work_state_report)
    {
        case REPORT_CAIJI_INIT :
        {               
            g_caiji.work_state_report = REPORT_CAIJI_gettime;

            bsp_StartTimer(TMR_ID_report_poweron, TMR_DELAY_30S);// 2分钟倒计时，超时判断
            
            bsp_SetTimer(TMR_ID_report_refresh);// 与升级命令错开时间；开机1秒钟进行校时命令的交互
            g_caiji.gettime_tx_count = 0;

            p_info("g_caiji CAIJI_INIT");
        }
            break;
        case REPORT_CAIJI_gettime :
        {       
            /*  20s 采集一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_POWER_ON;
                    g_caiji.gettime_tx_count = 0;
                    bsp_StopTimer(TMR_ID_report_refresh);//先停止，再重新计时
                    bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_1S);
                    
                    p_info("g_caiji REPORT_CAIJI_gettime_count");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETTIME);//                              
                    p_info("REPORT_CAIJI_gettime发送数据");                     
                }              
            }
            if(g_caiji.timer_server_ask_ok==TRUE)
            {
                g_caiji.timer_server_ask_ok=FALSE;
        
                g_caiji.work_state_report = REPORT_CAIJI_POWER_ON;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                p_info("g_caiji timer_server_ask_ok");
            }
        }
            break;

        case REPORT_CAIJI_POWER_ON :
        {               
            /*  20s 采集一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);                

                App_DataReport_SensorBasic();
                
                #ifdef ENABLE_SHESHISUO_PLC 
                App_sheshisuo_PLC_DataReport_SensorExtend();
                #endif                            
                //APP_STATE = NB_CoAP_ST;   //发送数据
                p_info("POWER_ON开始发送数据");                   
            }
            /*  开机上报，2分钟倒计时，超时判断 */
            if(bsp_CheckTimer(TMR_ID_report_poweron))
            {               
                g_caiji.work_state_report = REPORT_CAIJI_getinfo;

                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                
                p_info("开机上报，2分钟倒计时到了");                
            }   
            if(g_caiji.sensor_basic_server_ask_ok==TRUE)
            {
                g_caiji.sensor_basic_server_ask_ok=FALSE;
                g_caiji.work_state_report = REPORT_CAIJI_getinfo;
                
                p_info("开机上报 sensor_basic_server_ask_ok");              
            }
        }       
            break;

            

        case REPORT_CAIJI_getinfo :
        {       
            /*  20s 采集一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_getreport;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                    p_info("g_caiji REPORT_CAIJI_getinfo");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETINFO);//                              
                    p_info("REPORT_CAIJI_getinfo 发送数据");                        
                }              
            }
            if(g_caiji.info_server_ask_ok==TRUE)
            {
                g_caiji.info_server_ask_ok=FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_getreport;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                p_info("g_caiji info_server_ask_ok");
            }
        }
            break;  

        case REPORT_CAIJI_getreport:
        {       
            /*  20s 采集一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_report_imei_imsi;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                    p_info("g_caiji REPORT_CAIJI_getreport");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETREPORT);//                            
                    p_info("REPORT_CAIJI_getreport 发送数据");                      
                }              
            }
            if(g_caiji.report_server_ask_ok==TRUE)
            {
                g_caiji.report_server_ask_ok=FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_report_imei_imsi;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                p_info("g_caiji report_server_ask_ok");
            }
        }
            break;              
        case REPORT_CAIJI_report_imei_imsi :
        {   
            if(g_caiji.rx_data_in_flag == TRUE)
            {
                g_caiji.rx_data_in_flag =FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_report_Connectivity;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                p_info("g_caiji REPORT_CAIJI_report_imei_imsi");
            }
        
            /*  20s 采集一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >=2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_report_Connectivity;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// 开机30秒钟初始化，
                    p_info("g_caiji REPORT_CAIJI_report_imei_imsi");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_ReportNBID);//                            
                    p_info("REPORT_CAIJI_report_imei_imsi 发送数据");                       
                }               
            }           
        }
            break;
        case REPORT_CAIJI_report_Connectivity :
        {   
            #if 0
            if(g_caiji.rx_data_in_flag == TRUE)
            {
                g_caiji.rx_data_in_flag =FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_LOOP;
                bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//数据变化持续2小时不变化则上报一次
                p_info("g_caiji REPORT_CAIJI_report_Connectivity");
            }
            #endif
        
            /*  10s 发送一次*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >=3)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_LOOP;
                    bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//数据变化持续2小时不变化则上报一次
                    p_info("g_caiji REPORT_CAIJI_report_Connectivity");
                }
                else
                {
                    //App_DeviceState_Report_Connectivity();//                            
                    APP_STATE = NB_CoAP_ST;   //发送数据
                    p_info("REPORT_CAIJI_report_Connectivity 发送数据");                        
                }               
            }           
        }
            break;
                        
            
        case REPORT_CAIJI_LOOP :
        {               
            /* 数据变化持续2小时不变化则上报一次 */
            if(bsp_CheckTimer(TMR_ID_report_loop))
            {
                bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//数据变化持续2小时不变化则上报一次              
                g_wenshi.server_ok_count++;
                if(g_wenshi.server_ok_count >= 5)//连续3次不正常，重启
                {
                    /*服务器通讯，不正常、 超时退出的判断   */
                    //if(bsp_CheckTimer(TMR_ID_server_ok))
                    {
                        p_info("server_ok_count %d次数，重启",g_wenshi.server_ok_count);
                        App_fault_analysis_Set_Value(REBOOT_FLAG_SERVER);
                        bsp_DelayMS(10);
                        HAL_NVIC_SystemReset();//重启
                    }

                }

                    #if 0
                    App_DataReport_04();//
                    #else
                    App_DataReport_SensorBasic();
                    #if ENABLE_WATCHDOG 
                    HAL_IWDG_Refresh(&hiwdg) ;/* --- 喂狗 */
                    #endif                  
                    #ifdef ENABLE_SHESHISUO_PLC 
                    App_sheshisuo_PLC_DataReport_SensorExtend();
                    #endif                      
                    #endif 
                    //APP_STATE = NB_CoAP_ST;   //发送数据
                    p_info("REPORT_CAIJI_LOOP 开始发送数据");                         
            }
            
        }       
            break;  
            
        default:
            break;
    }
}


/*
*********************************************************************************************************
*   函 数 名:  Modbus_Protocolverification
*   功能说明:   modbus
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static uint8_t Modbus_Protocolverification(uint8_t * start_index,uint8_t address)
{
    uint8_t ucData = 0;
    uint16_t pos = 0;
    uint8_t ret = 0;
    
    uint16_t crc1 = 0;
    uint16_t rx_crc = 0;

    uint16_t i = 0;

    /*等待获得有效的PLC_TH数据  */         
    if (bsp_CheckTimer(TMR_ID_485_UART))
    {
        memset(rx_data_address,0,sizeof(rx_data_address));
        
        p_info("plc 485 rx!");
        while(1)
        {               
            if(comGetChar(Reserve_485_uart, &ucData))
            {           
                rx_data_address[pos++] = ucData;
                printf("%02X",ucData);
            }
            else
                break;
    
            if(pos >= (RX_DATA_LENTH))
                break;
        }               
        printf("\r\n");
        /* log */
        if(pos <= 4)
        {
            ret = VERIFICATION_SHORT_LENTH;
            return ret;
        }       
        for(i = 0;i<pos;i++)
        {
            if(rx_data_address[i] == address)
            {
                break;
            }
        }
        /* 数据长度太短了 丢弃*/
        if((pos-i) <= 4)
        {
            ret = VERIFICATION_ADDRESS;
            return ret;
        }   
        
        /* 计算CRC校验和 */
        crc1 = CRC16_Modbus(&rx_data_address[i], pos-i-2);
        rx_crc = ((uint16_t)rx_data_address[pos-i-2] << 8 | rx_data_address[pos-i-1]);
        
        if (crc1 != rx_crc)
        {
            ret = VERIFICATION_CRC;
            return ret;
        }

        * start_index = i;
        ret = VERIFICATION_OK;
        return ret;     
    }
    
    ret = VERIFICATION_NO_DATA;
    return ret;     

}


