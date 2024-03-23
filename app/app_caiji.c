#include "app_caiji.h"

#include "bsp.h"

#include "app.h"



MYMODBUS_T gModbus;

struct CAIJI g_caiji ;
char  rx_data_gps[UART3_RX_BUF_SIZE];

/*
*********************************************************************************************************
*   �� �� ��: App_caiji_Init_Var
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
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
*   �� �� ��: App_caiji_Init_Var
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_report_gettime_ok(void)
{   

    g_caiji.gettime_tx_count = REPORT_CAIJI_gettime_count;//��ǰ����count
    
    bsp_SetTimer(TMR_ID_report_refresh);

}

/*
*********************************************************************************************************
*   �� �� ��: App_caiji_report_start
*   ����˵��: ��ʼ�ϱ�����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_report_start(void)
{   
    bsp_SetTimer(TMR_ID_report_loop);//��ʼ�ϱ�����
}

/*
*********************************************************************************************************
*   �� �� ��: App_caiji__Loop
*   ����˵��: get gps signal
*   ��    �Σ���
*   �� �� ֵ: ��
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
            /*gps5min ��ʱ�˳����ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_gps_refresh))
            {
                Bsp_gps_power_on();//��gps��Դ            
            
                MX_LPUART1_UART_Init();
                bsp_GPS_Init(App_gps_Msg_cb); //��ʼ������

                bsp_StartTimer(TMR_ID_caiji_gps_refresh , TMR_DELAY_5min);//TMR_DELAY_2min
                g_caiji.work_state_gps = CAIJI_GPS_LOOP;

                p_info("g_caiji ��gps��Դ");              
            }
        }       
            break;          
        case CAIJI_GPS_LOOP :
        {   
            #if 0
            /*�������յ��Ĵ�������     */ 
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

            /*�ȴ������Ч��gps����     */
            App_gps_Get();
            
            /*gps5min ��ʱ�˳����ж�  */
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
            HAL_UART_Close(GPS);// �ر�gps����
            #endif          
            /*��ʱ�ر�gps��Դ*/
            Bsp_gps_power_off();
            g_caiji.start_gps = FALSE;
            g_caiji.work_state_gps = CAIJI_GPS_MAX;



            p_info("g_caiji �ر�gps��Դ");              
        }       
            break;              
        default:
            break;
    }
}





/**************************************************************************************                      
* ��    App_485_tx_turang_T_H_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    �����ɼ�������ʪ��
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_turang_T_H_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* ��    App_485_tx_turang_PH_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
[10:25:13.926]������01 03 00 00 00 01 84 0A ��
[10:25:13.964]�ա���01 03 02 00 46 39 B6 
PHֵ��0046 H(ʮ������)= 70=> PH = 7.0 
* ���ڲ�������
* ˵    �����ɼ�����PH
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_turang_PH_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
/**************************************************************************************                      
* ��    App_485_tx_turang_NPKmd
* ��    ��:
* ��ڲ�����

    [11:07:01.637]������03 03 00 1E 00 03 64 2F  
    [11:07:01.666]�ա���03 03 06 00 00 00 00 00 00 38 15

* ���ڲ�������
* ˵    �����ɼ�����PH
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_turang_NPK_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x1E;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* ��    App_485_tx_turang_water_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
01 03 02 00 0A 38 43 
01 03 02 00 0B F9 83 

* ���ڲ�������
* ˵    �����ɼ�����water
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_turang_water_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
#define LED_NUM_REGISTER 13//12 // led��ʾ��Ļ  ��ʾ 6����ַ
#if 1
/**************************************************************************************                      
* ��    App_485_tx_led_display_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    ����ˢ����Ļ  �Ϻ���Ŀ���ܿƼ����޹�˾LED����Ļ��
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
#define NEW_SENSOR 
void App_485_tx_led_display_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
#ifdef NEW_SENSOR
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x01;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = LED_NUM_REGISTER;    
    buffer_lenth++; 

    buffer[buffer_lenth] = LED_NUM_REGISTER*2;    
    buffer_lenth++;     

/*
1���¶�
2��ʪ��
3��Һλ
4��CO2
5������
6������
7�������¶�
8������ʪ��
9������PH
10��N
11��P
12��K
13��ʪ���¶� 0.1
*/
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT/10)>>8; //
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT/10)>>0;    //0.1���ȸ�LED��Ļ
    buffer_lenth++;         
    
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH/10)>>8; //
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH/10)>>0;    //0.1���ȸ�LED��Ļ
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueLevel)>>8;    //1���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueLevel)>>0;    //1���ȸ�LED��Ļ
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_co2_value)>>8; //co2
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_co2_value)>>0; //co2
    buffer_lenth++; 
    
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //��������ֱ����ʾԭ��������
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //��������ֱ����ʾԭ��������
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>0; 
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T)>>8;   //0.1���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T)>>0;   //0.1���ȸ�LED��Ļ
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H)>>8;   //0.1���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H)>>0;   //0.1���ȸ�LED��Ļ
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
    buffer[buffer_lenth] = (g_wenshi.airTHvalueTwet/10)>>0;    //0.1���ȸ�LED��Ļ
    buffer_lenth++;         
    
#else
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 6;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 6*2;    
    buffer_lenth++;     
    
    /*
    00 1F �����¶�
    00 29 ����ʪ��
    00 0A �����¶�
    00 50  ����ʪ��
    02 65  ���ա�
    */

    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>8;    //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>0;    
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>8;    //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>0;    
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>8;   //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>0;   
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>8;   //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>0;   
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //��������ֱ����ʾԭ��������
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //��������ֱ����ʾԭ��������
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}
/**************************************************************************************                      
* ��    app_485TxPlcDisplay_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    ����ˢ����Ļ  �Ϻ���Ŀ���ܿƼ����޹�˾LED����Ļ��
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void app_485TxPlcDisplay_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x06;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 12  ;    
    buffer_lenth++;     
/*
��������	��ַλ��	�Ĵ�������
�����¶�	00	�Ĵ���
����ʪ��	01	�Ĵ���
�����¶�	02	�Ĵ���
����ʪ��	03	�Ĵ���
����	    04 05	�Ĵ���

*/
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>8;    //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueT)>>0;    
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>8;    //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.airTHvalueH)>>0;    
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>8;   //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_T*10)>>0;   
    buffer_lenth++;         

    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>8;   //0.01���ȸ�LED��Ļ
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_soil_H*10)>>0;   
    buffer_lenth++; 

    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>24; //��������ֱ����ʾԭ��������
    buffer_lenth++;         
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>16; 
    buffer_lenth++; 
    buffer[buffer_lenth] = (g_wenshi.m_Lux)>>8; //��������ֱ����ʾԭ��������
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

#else
/**************************************************************************************                      
* ��    App_485_tx_led_display_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    ����ˢ����Ļ
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_led_display_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[64];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x0c;    
    buffer_lenth++; 

    buffer[buffer_lenth] = 0x18;    
    buffer_lenth++;     
/*
00 1F �����¶�
00 29 ����ʪ��
00 0A �����¶�
00 50  ����ʪ��
02 65  ����
00 02  ���״̬ѡ�� 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-�����

00 02  �����¶ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
00 02  ����ʪ�ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
00 02  �����¶ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
00 02  ����ʪ�ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
00 02  ���ռ�ͷ     0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
00 01  ���״̬��ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
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
            buffer[buffer_lenth] = 0;   // 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-����С�4-����ʾ
            buffer_lenth++; 
            buffer[buffer_lenth] = 0;   // 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-����С�4-����ʾ
            buffer_lenth++; 
        }
        else
        {
            buffer[buffer_lenth] = 0;   // 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-����С�4-����ʾ
            buffer_lenth++; 
            buffer[buffer_lenth] =2;    
            buffer_lenth++; 
        }
    }
    else// û�д�����
    {
        buffer[buffer_lenth] = 0;   // 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-����С�4-����ʾ
        buffer_lenth++; 
        buffer[buffer_lenth] = 4;   // 0-����С�1-��ʼ��ȡ�2-��Ƚ�����3-����С�4-����ʾ
        buffer_lenth++; 
    }
    /*
    00 02  �����¶ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
    00 02  ����ʪ�ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
    00 02  �����¶ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
    00 02  ����ʪ�ȼ�ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
    00 02  ���ռ�ͷ     0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
    00 01  ���״̬��ͷ 0-����ʾ��1-���ϡ�2-���¡�3-���ϡ�4-���¡�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

#endif




/**************************************************************************************                      
* ��    App_485_tx_pt100_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    �����ɼ�pt100ʪ��
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_pt100_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}









/**************************************************************************************                      
* ��      App_485_tx_UV_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    �����ɼ���Ҷ����ʪ�Ⱥ͹���
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_UV_cmd(uint8_t address ,uint8_t function)
{
    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

/**************************************************************************************                      
* ��    App_485_tx_TH_LUX_cmd
* ��    ��:
* ��ڲ�����
    address:   ��ַ   
    function:       ������
* ���ڲ�������
* ˵    �����ɼ���Ҷ����ʪ�Ⱥ͹���
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_TH_LUX_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x00;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* ��    App_485_tx_CO2_cmd
* ��    ��:
* ��ڲ�����   
* ���ڲ�������
* ˵    ���� 
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_CO2_cmd(uint8_t address ,uint8_t function)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = 0x02;    
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* ��              App_485_tx_airTH_cmd
* ��    ��:
* ��ڲ�����   
* ���ڲ�������
* ˵    ���� 
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_tx_airTH_cmd(uint8_t address ,uint8_t function,uint8_t inputData)
{

    //u8 l_sum = 0;
    //u8 i = 0;

    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = 0x00;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = inputData;   
    buffer_lenth++;         

    buffer[buffer_lenth] = 0x00;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}


/**************************************************************************************                      
* ��              App_485_tx_airTH_cmd
* ��    ��:
* ��ڲ�����   
* ���ڲ�������
* ˵    ���� 
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
void App_485_txModbusCmd(MYMODBUS_T * pModbus)
{
    uint8_t buffer[16];
    u8 buffer_lenth=0;
    u16 crc1 = 0;
    
    memset(buffer,0x00,sizeof(buffer));
    
    buffer[buffer_lenth] = pModbus->address;//��ַ��
    buffer_lenth++; 
    
    buffer[buffer_lenth] = pModbus->function;//������
    buffer_lenth++; 
    
    buffer[buffer_lenth] = pModbus->dataAddress>>8;//�Ĵ�����ʼ��ַ 2�ֽ�
    buffer_lenth++;         
    buffer[buffer_lenth] = pModbus->dataAddress;    
    buffer_lenth++;         

    buffer[buffer_lenth] = pModbus->dataLen>>8;//�Ĵ������� 2�ֽ�
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
        g_tUart5.SendBefor();       /* �����RS485ͨ�ţ���������������н�RS485����Ϊ����ģʽ */
    }
    HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
    RS485_SendOver();

}

/*
*********************************************************************************************************
*   �� �� ��: App_caiji_sensor_address
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
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
*   �� �� ��: App_caiji_turang_T_H_Loop
*   ����˵��: �ɼ������¶ȡ�ʪ�� ��NPK��PH�� ��ȴ�����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
#define RX_DATA_LENTH 256
static uint8_t rx_data_address[RX_DATA_LENTH];

uint16_t m_sensor_TXflag = SENSOR_TX_SERVER_NULL;
#define  TIMER_SENSOR  500//50//100ms  ������������

/**************************************************************************************                      
* ��              App_485_rxModbusCmd
* ��    ��:
* ��ڲ�����   
* ���ڲ�����1 is ok
* ˵    ���� 
* ʱ    �䣺
* ��    ��: ��ʯȪ
* ���÷�������
****************************************************************************************/
uint8_t App_485_rxModbusCmd(MYMODBUS_T * pModbus)
{
    uint8_t ucData = 0;
    uint16_t pos = 0;
    uint8_t ret = 0;
    
    uint16_t crc1 = 0;
    uint16_t rx_crc = 0;
    uint16_t i = 0;
    
    /*�ȴ������Ч��soil_TH����  */    
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
        /* ���ݳ���̫���� ����*/
        if((pos-i) <= 4)
        {
            ret = 4;
            return ret;
        }   
    
        
        /* ����CRCУ��� */
        crc1 = CRC16_Modbus(&rx_data_address[i], pos-i-2);
        rx_crc = ((uint16_t)rx_data_address[pos-i-2] << 8 | rx_data_address[pos-i-1]);
        
        if (crc1 != rx_crc)
        {
            ret = 5;
            return ret;
        }
        /*
        Ӧ��֡����������¶�Ϊ-10.1�棬ʪ��Ϊ65.8%RH��
        ��ַ��   ������   �ֽ���   ʪ��ֵ   �¶�ֵ   У�����λ У�����λ
        0x01    0x03    0x04    0x02 0x92   0xFF 0x9B   0x5A    0x3D
        
        �¶ȣ����¶ȵ������ʱ�Բ�����ʽ�ϴ�
        FF9B H(ʮ������)= -101 => �¶�= -10.1��
        ʪ�ȣ�
        292 H (ʮ������) = 658 => ʪ�� = 65.8%RH
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
*   �� �� ��: app_caiji_sensorDataSync
*   ����˵��: ͬ��air���������ݵ� 
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_caiji_sensorDataSync(void)
{   
    /* ������ʪ�� */
    g_wenshi.m_Symbol = 0x01;

    g_wenshi.m_H = g_wenshi.airTHvalueH/10;// ���ݳ���100�õ���ʵ��ʪ������
    g_wenshi.m_T = g_wenshi.airTHvalueT/10;// ���ݳ���100�õ���ʵ��ʪ������

    /* ������ʪ�� */
    g_wenshi.m_soil_H = g_wenshi.m_soil_H;                             
    g_wenshi.m_soil_T = g_wenshi.m_soil_T;

    /* ����ǿ�� */
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


    g_wenshi.weatherStationExisted = g_wenshiCopy.weatherStationExisted ;// ���ڹ� ����վ
    g_wenshi.weatherStationOnline   = g_wenshiCopy.weatherStationOnline;//���� ����վ

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
    g_wenshi.m_H  = g_wenshiCopy.m_H  ;// ���ݳ���100�õ���ʵ��ʪ������
    g_wenshi.airTHvalueT  = g_wenshiCopy.airTHvalueT ;
    g_wenshi.m_T   = g_wenshiCopy.m_T ;// ���ݳ���100�õ���ʵ��ʪ������
    g_wenshi.airTHvalueTwet = g_wenshiCopy.airTHvalueTwet;
    g_wenshi.airTHvalueLevel = g_wenshiCopy.airTHvalueLevel;
    g_wenshi.airTHvalueHcal = g_wenshiCopy.airTHvalueHcal;

    g_wenshi.m_led_display = g_wenshiCopy.m_led_display;                                   
    g_wenshi.mLedFlag  = g_wenshiCopy.mLedFlag ;// ���ڹ�LED

    g_wenshi.mPlcdisplay = g_wenshiCopy.mPlcdisplay ;
    g_wenshi.mPlcFlag = g_wenshiCopy.mPlcFlag ;// ���ڹ� PLC
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

uint8_t addressAirTh[8] = {11,12,13,14,15,16,20,30};//��ʪ�ȴ�����modbus��ַ15
uint8_t curAirTHNum = 0;// �ܹ�8��  0-7

//static const Ptr_CommExeFuc CommExeFuncArray[] =
//{
//    CMDError,
//    HandShake,
//    Telemetry,//遥测
//    GetDeviceInformation,    //设备信息获取指令
//    
//    GroupInfoGet,
//};

SensorAirTh_T airThSensor[8];// �ܹ�8��  0-7

void App_caiji_turang_Loop(void)
{
    
    if((g_caiji.soil_TH_start == TRUE)&&(gUpdate4G.updateStart == FALSE))//���������У����ٲɼ�����������
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
            Bsp_12V_power_on(); //12 ���硣              
            Bsp_sensor_power_on(); //sensor���硣    
            MX_UART5_Init();//��ʼ������5��ֹ������ 20211007
            bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
            g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
            m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//���͸����������ݴ������ı�־

            curAirTHNum = 0;//��0��ʼ
            
            p_info("g_caiji �򿪵�Դ");              
        }       
            break;  

        case CAIJI_airTH_TX :
        {   
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {           
                /*��ʼ  �ɼ�air��ʪ������*/    
                gModbus.address = addressAirTh[curAirTHNum];
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 1;
                gModbus.dataLen = 5;//20���Ӳɼ�һ�Σ�һ���Զ�ȡ5�����ݣ�
                App_485_txModbusCmd(&gModbus);  
                comClearRxFifo(COM5);//COM5
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+250));//
                g_caiji.soil_TH_work_state = CAIJI_airTH_RX;
        
                p_info("g_caiji ���Ͳ�ѯ���� airTH:%d",curAirTHNum);                 
            }               
        }       
            break;                  

        case CAIJI_airTH_RX :
        {   
            /*�ȴ������Ч��TH_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {                               
                airThSensor[curAirTHNum].airTH= 1;
                
                airThSensor[curAirTHNum].airTHvalueH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                p_info(" airH:%d;",airThSensor[curAirTHNum].airTHvalueH);                           
                airThSensor[curAirTHNum].m_H = airThSensor[curAirTHNum].airTHvalueH/10;// ���ݳ���100�õ���ʵ��ʪ������

                airThSensor[curAirTHNum].airTHvalueT = ((uint16_t)gModbus.AppRxBuf[2] << 8 | gModbus.AppRxBuf[3]);
                p_info(" airT:%d;",airThSensor[curAirTHNum].airTHvalueT);                       
                airThSensor[curAirTHNum].m_T = airThSensor[curAirTHNum].airTHvalueT/10;// ���ݳ���100�õ���ʵ��ʪ������

                airThSensor[curAirTHNum].airTHvalueTwet= ((uint16_t)gModbus.AppRxBuf[4] << 8 | gModbus.AppRxBuf[5]);
                p_info(" airTwet:%d;",airThSensor[curAirTHNum].airTHvalueTwet);                     

                airThSensor[curAirTHNum].airTHvalueLevel= ((uint16_t)gModbus.AppRxBuf[6] << 8 | gModbus.AppRxBuf[7]);
                if(airThSensor[curAirTHNum].airTHvalueLevel == 0xffff)//-1��ˮ
                {
                    airThSensor[curAirTHNum].airTHvalueLevel = 1;// 1��ȱˮ��
                }
                else//0��ˮ
                {
                    airThSensor[curAirTHNum].airTHvalueLevel = 0;//0 ȱˮ��
                }
                p_info(" airLevel:%d;",airThSensor[curAirTHNum].airTHvalueLevel);                       

                airThSensor[curAirTHNum].airTHvalueHcal= ((uint16_t)gModbus.AppRxBuf[8] << 8 | gModbus.AppRxBuf[9]);
                p_info(" airTHvalueHcal:%d;",airThSensor[curAirTHNum].airTHvalueHcal);                     

                m_sensor_TXflag |= (0X0001<<curAirTHNum);//���͸����������ݴ������ı�־
                p_info("g_caiji airTH 2 ok:%d",curAirTHNum);               
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
                g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
                curAirTHNum++;
                if(curAirTHNum >= 8)// ����8�˳�
                {
                    g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                }
            }       
            else
            {
                airThSensor[curAirTHNum].airTH = 0;             
            }
                
            /*0.5s ��ʱ�˳����ж�     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji airTH 2timer out:%d",curAirTHNum);         
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
                g_caiji.soil_TH_work_state = CAIJI_airTH_TX;
                curAirTHNum++;
                if(curAirTHNum >= 8)// ����8�˳�
                {
                    g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                }
            }
        }       
            break;          

        
        case CAIJI_CLOSE :
        {   
            g_wenshi.m_sensor_TXflag = m_sensor_TXflag;// ͬ������
            App_caiji_sensorExchange();//  ͬ������
            
            bsp_StopTimer(TMR_ID_caiji_soil_TH_refresh);//
            HAL_UART_DeInit(&huart5);//
                
            /*��ʱ�ر�soil_TH��Դ*/
            Bsp_12V_power_off();
            Bsp_sensor_power_off(); //sensor���硣   
            g_caiji.soil_TH_start = FALSE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_MAX;
            
           /* :  ���ݱ仯�ϱ� */
            if(App_data_report_process()&&(g_tConfig.report_changed))
            {
                App_caiji_report_start();//
                p_info("g_caiji ���ݱ仯�ϱ�");               
            }
            
            p_info("g_caiji �ر�soil_TH��Դ");      

            //App_caiji_sensor_address();//��ַ����
        }       
            break;              
        default:
            break;
    }
}

#endif


#ifdef ALL_SENSORS// ԭ���Ĵ�����
void App_caiji_turang_Loop(void)
{
    int16_t m_soil_T = 0;
    
    if((g_caiji.soil_TH_start == TRUE)&&(gUpdate4G.updateStart == FALSE))//���������У����ٲɼ�����������
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
            Bsp_12V_power_on(); //12 ���硣              
            Bsp_sensor_power_on(); //sensor���硣    
            MX_UART5_Init();//��ʼ������5��ֹ������ 20211007
            bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 10);//10MS
            g_caiji.soil_TH_work_state = CAIJI_SOIL_PH_TX;
            m_sensor_TXflag = SENSOR_TX_SERVER_NULL;//���͸����������ݴ������ı�־
            
            p_info("g_caiji ��soil_TH��Դ");              
        }       
            break;  
        
        case CAIJI_SOIL_PH_TX :
        {                   
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_TURANG_PH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  ����վӦ��ʱ����Ҫ500ms
                g_caiji.soil_TH_work_state = CAIJI_SOIL_PH_RX_LOOP;

                p_info("g_caiji ���Ͳ�ѯ����PH");                 
            }                   
        }       
            break;  
            
        case CAIJI_SOIL_PH_RX_LOOP :
        {   
            /*�ȴ������Ч��soil_TH����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_soil_PH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);              
                g_wenshiCopy.m_soil_PH = g_wenshiCopy.m_soil_PH/10;// ԭʼ�����ǷŴ�100���ģ��ϴ���10���ġ�
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_PH;//���͸����������ݴ������ı�־
                p_info("PH:%d;",g_wenshiCopy.m_soil_PH);
                
                g_caiji.soil_TH_work_state = CAIJI_CO2_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji PH ok");   
            }        

            /*soil_TH 3s ��ʱ�˳����ж�   */
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
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_CO2;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  ����վӦ��ʱ����Ҫ500ms
                g_caiji.soil_TH_work_state = CAIJI_CO2_RX_LOOP;

                p_info("g_caiji ���Ͳ�ѯ���� CO2");                 
            }                   
        }       
            break;  
            
        case CAIJI_CO2_RX_LOOP :
        {   
            /*�ȴ������Ч��soil_TH����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_co2= 1;
                g_wenshiCopy.m_co2_value = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);              
                
                m_sensor_TXflag |= SENSOR_TX_SERVER_CO2;//���͸����������ݴ������ı�־
                p_info("g_caiji CO2 1 ok:%d",g_wenshiCopy.m_co2_value);             
                
                g_caiji.soil_TH_work_state = CAIJI_SOIL_NPK_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
            }        

            /*soil_TH 3s ��ʱ�˳����ж�   */
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
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_TURANG_NPK;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 3;
                App_485_txModbusCmd(&gModbus);
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  ����վӦ��ʱ����Ҫ500ms
                g_caiji.soil_TH_work_state = CAIJI_SOIL_NPK_RX_LOOP;

                p_info("g_caiji ���Ͳ�ѯ����NPK");                 
            }                   
        }       
            break;  
            
        case CAIJI_SOIL_NPK_RX_LOOP :
        {   
            /*�ȴ������Ч��soil_TH����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                for(uint8_t num=0;num<NPKsensor_NUM;num++)
                {
                    g_wenshiCopy.m_soil_N[num] = ((uint16_t)gModbus.AppRxBuf[num*2+0] << 8 | gModbus.AppRxBuf[num*2+1]);              
                }
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_NPK;//���͸����������ݴ������ı�־
                p_info("NPK:%d;N:%d;K:%d;",g_wenshiCopy.m_soil_N[0],g_wenshiCopy.m_soil_N[1],g_wenshiCopy.m_soil_N[2]);
                
                g_caiji.soil_TH_work_state = CAIJI_WEATHER_STATION_TX;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji NPK ok");   
            }

                
            /*soil_TH 3s ��ʱ�˳����ж�   */
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
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_WEATHER_STATION;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 16;
                App_485_txModbusCmd(&gModbus);
                //App_485_tx_turang_T_H_cmd(ADDRESS_TURANG_TH,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+600));//750ms  ����վӦ��ʱ����Ҫ500ms
                g_caiji.soil_TH_work_state = CAIJI_WEATHER_STATION_RX_LOOP;

                p_info("g_caiji ���Ͳ�ѯ����TH");                 
            }                   
        }       
            break;  
            
        case CAIJI_WEATHER_STATION_RX_LOOP :
        {   
            /*�ȴ������Ч��soil_TH����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.weatherStationExisted = 1;// ���ڹ� ����վ
                g_wenshiCopy.weatherStationOnline  = 1;//���� ����վ
                //memcpy((char *)&g_wenshiCopy.weatherStation[0], (char *)&gModbus.AppRxBuf[0],32); // copy  ��Ϣ             
                for(uint8_t num=0;num<16;num++)
                {
                    g_wenshiCopy.weatherStation[num] = ((uint16_t)gModbus.AppRxBuf[num*2+0] << 8 | gModbus.AppRxBuf[num*2+1]);              
                }
                m_sensor_TXflag |= SENSOR_TX_SERVER_WEATHER_STATION;//���͸����������ݴ������ı�־
                p_info("����վ,����:%d;����:%d;�����¶�:%d;",g_wenshiCopy.weatherStation[0],g_wenshiCopy.weatherStation[1],g_wenshiCopy.weatherStation[2]);
                
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji WEATHER_STATION ok");   
            }

                
            /*soil_TH 3s ��ʱ�˳����ж�   */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//
                g_wenshiCopy.weatherStationOnline  = 0;//���� ����վ

                p_err("g_caiji WEATHER_STATION timer out");             
            }
        }       
            break;  
        
        case CAIJI_soil_TH_DELAY :
        {                   
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_TURANG_TH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 3;
                App_485_txModbusCmd(&gModbus);
                //App_485_tx_turang_T_H_cmd(ADDRESS_TURANG_TH,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_LOOP;

                p_info("g_caiji ���Ͳ�ѯ����TH");                 
            }                   
        }       
            break;  
            
        case CAIJI_soil_TH_LOOP :
        {   
            /*�ȴ������Ч��soil_TH����     */         
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
                
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_TH;//���͸����������ݴ������ı�־
                p_info("����, Symbol:%c;�¶�:%d;ʪ��:%d;EC:%d;",(char)g_wenshiCopy.m_soil_Symbol,g_wenshiCopy.m_soil_T,g_wenshiCopy.m_soil_H,g_wenshiCopy.m_soil_EC);
                
                g_caiji.soil_TH_work_state = CAIJI_soil_LUX_DELAY;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 500);//50Ms
                
                p_info("g_caiji soil_TH ok");   
            }

                
            /*soil_TH 3s ��ʱ�˳����ж�   */
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
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                /*��ʼ  �ɼ�������ʪ������*/     
                gModbus.address = ADDRESS_TURANG_LUX;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 1;
                App_485_txModbusCmd(&gModbus);              
                //App_485_tx_turang_PH_cmd(ADDRESS_TURANG_LUX,FUNCTION_CODE_03);// 
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                g_caiji.soil_TH_work_state = CAIJI_soil_LUX_LOOP;

                p_info("g_caiji ���Ͳ�ѯ����LUX");                 
            }                   
        }       
            break;  
            
        case CAIJI_soil_LUX_LOOP :
        {   
            /*�ȴ������Ч��m_soil_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.m_soil_LUX = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                m_sensor_TXflag |= SENSOR_TX_SERVER_TURANG_LUX;//���͸����������ݴ������ı�־                                 
                p_info("����, m_soil_LUX:%d;",g_wenshiCopy.m_soil_LUX);
                g_wenshiCopy.m_Lux = g_wenshiCopy.m_soil_LUX*10;// 10
                
                g_caiji.soil_TH_work_state = CAIJI_AIRTH_DELAY2;
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , 50);//
                
                p_info("g_caiji m_soil_LUX ok");                
            }           
                
            /*0.5s ��ʱ�˳����ж�     */
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
            /*soil_TH ���ж�  */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {           
                /*��ʼ  �ɼ�air��ʪ������*/    
                gModbus.address = ADDRESS_AIR_TH;
                gModbus.function = FUNCTION_CODE_03;
                gModbus.dataAddress= 1;
                gModbus.dataLen = 5;//20���Ӳɼ�һ�Σ�һ���Զ�ȡ5�����ݣ�
                App_485_txModbusCmd(&gModbus);  
                comClearRxFifo(COM5);//COM5
                
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , (TIMER_SENSOR+250));//
                g_caiji.soil_TH_work_state = CAIJI_AIRTH_LOOP2;
        
                p_info("g_caiji ���Ͳ�ѯ���� airTH");                 
            }               
        }       
            break;      
        case CAIJI_AIRTH_LOOP2 :
        {   
            /*�ȴ������Ч��TH_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {                               
                g_wenshiCopy.airTH= 1;
                
                g_wenshiCopy.airTHvalueH = ((uint16_t)gModbus.AppRxBuf[0] << 8 | gModbus.AppRxBuf[1]);
                p_info(" airH:%d;",g_wenshiCopy.airTHvalueH);                           
                g_wenshiCopy.m_H = g_wenshiCopy.airTHvalueH/10;// ���ݳ���100�õ���ʵ��ʪ������

                g_wenshiCopy.airTHvalueT = ((uint16_t)gModbus.AppRxBuf[2] << 8 | gModbus.AppRxBuf[3]);
                p_info(" airT:%d;",g_wenshiCopy.airTHvalueT);                       
                g_wenshiCopy.m_T = g_wenshiCopy.airTHvalueT/10;// ���ݳ���100�õ���ʵ��ʪ������

                g_wenshiCopy.airTHvalueTwet= ((uint16_t)gModbus.AppRxBuf[4] << 8 | gModbus.AppRxBuf[5]);
                p_info(" airTwet:%d;",g_wenshiCopy.airTHvalueTwet);                     

                g_wenshiCopy.airTHvalueLevel= ((uint16_t)gModbus.AppRxBuf[6] << 8 | gModbus.AppRxBuf[7]);
                if(g_wenshiCopy.airTHvalueLevel == 0xffff)//-1��ˮ
                {
                    g_wenshiCopy.airTHvalueLevel = 1;// 1��ȱˮ��
                }
                else//0��ˮ
                {
                    g_wenshiCopy.airTHvalueLevel = 0;//0 ȱˮ��
                }
                p_info(" airLevel:%d;",g_wenshiCopy.airTHvalueLevel);                       

                g_wenshiCopy.airTHvalueHcal= ((uint16_t)gModbus.AppRxBuf[8] << 8 | gModbus.AppRxBuf[9]);
                p_info(" airTHvalueHcal:%d;",g_wenshiCopy.airTHvalueHcal);                     

                m_sensor_TXflag |= SENSOR_TX_SERVER_AIRTH;//���͸����������ݴ������ı�־
                p_info("g_caiji airTH 2 ok");               
                
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_LED_DISPLAY;
                gModbus.address = ADDRESS_LED_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = LED_NUM_REGISTER;
                app_caiji_sensorDataSync();//ͬ������֮�����LED��Ļ����
                App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//5s ��ph����ʱ��
                p_info("g_caiji tx LED");               
            }       
            else
            {
                g_wenshiCopy.airTH = 0;             
            }
                
            /*0.5s ��ʱ�˳����ж�     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji airTH 2timer out");          
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_LED_DISPLAY;
                gModbus.address = ADDRESS_LED_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = LED_NUM_REGISTER;
                app_caiji_sensorDataSync();//ͬ������֮�����LED��Ļ����
                App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//
                p_info("g_caiji tx LED");               
            }
        }       
            break;          
        case CAIJI_LED_DISPLAY :
        {   
            /*�ȴ������Ч��m_soil_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_wenshiCopy.mLedFlag = 1;// ���ڹ�LED
                g_wenshiCopy.m_led_display=1;                                   
                m_sensor_TXflag |= SENSOR_TX_SERVER_LED_DISPLAY;//���͸����������ݴ������ı�־
                p_info(" m_led_display:%d;;",g_wenshiCopy.m_led_display);                       
                
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_PLC_DISPLAY;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                app_485TxPlcDisplay_cmd(ADDRESS_PLC_DISPLAY,FUNCTION_CODE_10);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC");               
            }           
                
            /*0.5s ��ʱ�˳����ж�     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_wenshiCopy.m_led_display = 0;
                p_err("g_caiji CAIJI_LED_DISPLAY timer out");               
                
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_PLC_DISPLAY;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_10;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                app_485TxPlcDisplay_cmd(ADDRESS_PLC_DISPLAY,FUNCTION_CODE_10);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC");               
            }
        }       
            break;          
        case CAIJI_PLC_DISPLAY :
        {   
            /*�ȴ������Ч��m_soil_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_SHESHISUO;//���͸����������ݴ������ı�־
                g_wenshiCopy.mPlcFlag = 1;// ���ڹ� PLC
                g_wenshiCopy.mPlcdisplay = 1;
                p_info(" mPlcdisplay:%d;;",g_wenshiCopy.mPlcdisplay);                       
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_PLC_READCOIL;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_01;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                App_485_txModbusCmd(&gModbus);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                p_info("g_caiji tx PLC COIL");               
            }           
                
            /*0.5s ��ʱ�˳����ж�     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                p_err("g_caiji CAIJI_PLC_DISPLAY timer out");               
                g_wenshiCopy.mPlcdisplay = 0;
                
                bsp_DelayMS(50);//��ʱ10ms �ٷ���
                g_caiji.soil_TH_work_state = CAIJI_PLC_READCOIL;
                gModbus.address = ADDRESS_PLC_DISPLAY;
                gModbus.function = FUNCTION_CODE_01;
                gModbus.dataAddress= 0;
                gModbus.dataLen = 0x06;
                App_485_txModbusCmd(&gModbus);// ˢ������
                comClearRxFifo(COM5);//COM5
                bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//                
                
                p_info("g_caiji tx PLC COIL");               
            }
        }       
            break;                  
        case CAIJI_PLC_READCOIL://��ȡPLC����Ȧֵ
        {
            /*�ȴ������Ч��m_soil_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                m_sensor_TXflag |= SENSOR_TX_SERVER_PLC_SHESHISUO;//���͸����������ݴ������ı�־
                g_caiji.soil_TH_work_state = CAIJI_CLOSE;

                g_wenshiCopy.mPlcCoil = gModbus.AppRxBuf[0];
                /* ���±� �豸״̬ ���� */
                if((g_wenshiCopy.mPlcCoil>>0)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Opened_STATE;
                else if((g_wenshiCopy.mPlcCoil>>1)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Stopped_STATE ;
                else if((g_wenshiCopy.mPlcCoil>>2)&0x01)
                    g_wenshiCopy.mPlcStateInsulation = Equipment_Closed_STATE;
                
                /* ��ͨ�� �豸״̬ ���� */
                if((g_wenshiCopy.mPlcCoil>>3)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Opened_STATE;
                else if((g_wenshiCopy.mPlcCoil>>4)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Stopped_STATE ;
                else if((g_wenshiCopy.mPlcCoil>>5)&0x01)
                    g_wenshiCopy.mPlcStateRoofVentilation = Equipment_Closed_STATE;
            }           
                
            /*0.5s ��ʱ�˳����ж�     */
            if(bsp_CheckTimer(TMR_ID_caiji_soil_TH_refresh))
            {
                g_caiji.soil_TH_work_state = CAIJI_CLOSE;
                p_err("g_caiji CAIJI_PLC_READCOIL timer out");               
            }

        }
            break;
        case CAIJI_PLC_WRITE_COIL://write PLC����Ȧֵ
        {
			/* ����3�Σ��˳�*/
			if(	g_sheshisuoPLC.reg_retry_tx_count> 3)
			{
				p_err("CAIJI_PLC_WRITE_COIL ����3�Σ��˳�");	
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;// �ɹ��� ����ȫ�����²�ѯ֮��
                App_DeviceState_cmdAsk(CMD_ASK_plcwError);
			}
			else
			{
				comClearRxFifo(COM5);//COM5		
				PLC_TypeOccupiedValue_convertToAddress(6,g_sheshisuoPLC.rx_occupied,g_sheshisuoPLC.rx_value);

				bsp_StartTimer(TMR_ID_caiji_soil_TH_refresh , TIMER_SENSOR);//5���Ӳɼ�������̫��ᵼ������
				g_caiji.soil_TH_work_state = CAIJI_PLC_WRITE_COIL_DELAY;				
				p_info("g_sheshisuoPLC ���� CAIJI_PLC_WRITE_COIL  ");	

			}	        

        }
            break;        
        case CAIJI_PLC_WRITE_COIL_DELAY://write PLC����Ȧֵ delay
        {
            /*�ȴ������Ч��m_soil_LUX ����     */         
            if (App_485_rxModbusCmd(&gModbus)==1)
            {               
                g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;// �ɹ��� ����ȫ�����²�ѯ֮��
                App_DeviceState_cmdAsk(CMD_ASK_plcwOk);
            }           
                
            /*0.5s ��ʱ�˳����ж�     */
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
            g_wenshi.m_sensor_TXflag = m_sensor_TXflag;// ͬ������
            App_caiji_sensorExchange();//  ͬ������
            
            bsp_StopTimer(TMR_ID_caiji_soil_TH_refresh);//
            HAL_UART_DeInit(&huart5);//
                
            /*��ʱ�ر�soil_TH��Դ*/
            Bsp_12V_power_off();
            Bsp_sensor_power_off(); //sensor���硣   
            g_caiji.soil_TH_start = FALSE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_MAX;
            
           /* :  ���ݱ仯�ϱ� */
            if(App_data_report_process()&&(g_tConfig.report_changed))
            {
                App_caiji_report_start();//
                p_info("g_caiji ���ݱ仯�ϱ�");               
            }
            
            p_info("g_caiji �ر�soil_TH��Դ");      

            //App_caiji_sensor_address();//��ַ����
        }       
            break;              
        default:
            break;
    }
}
#endif
/*
*********************************************************************************************************
*   �� �� ��: filter
*   ����˵��: 

����ƽ���˲��㷨ֻ����һ�Σ���һ�β���ֵ�͹�ȥ�����ɴβ���ֵһ����ƽ�����õ�����Ч����ֵ����Ͷ��ʹ�á�
���ȡN������ֵ��ƽ�����洢���б��뿪��N�����ݵ��ݴ�����ÿ�²ɼ�һ�����ݱ�����ݴ����У�
ͬʱȥ��һ���������ݣ�������N������ʼ�������¸��µ����ݡ����û��Ͷ��нṹ���Է����ʵ���������ݴ�ŷ�ʽ��

*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
#define N 6// 6 point 
uint32_t filter(uint32_t data_in)
{
    static uint32_t value_buff[N];                  //N�൱��ѡ��һ�����ڴ�С���Դ���������ƽ����
    static char flag_power_on = 0;// 0 ����֮ǰ�����ݣ�1�����ɼ�N��֮������ݡ�
    uint32_t i=0;

    char count;
    uint32_t sum=0;
    
    value_buff[i++]= data_in;
    
    if(i==N)
    {
        flag_power_on = 1;
        i=0;              //�����ݴ������鳤�ȣ��滻�������һ������  �൱�ڻ��ζ��и��£��Ƚ��ȳ���
    }
    
    for(count=0;count<N;count++)
        sum+=value_buff[count];

    if(flag_power_on == 1)
    {
        return (uint32_t)(sum/N);//1�����ɼ�N��֮������ݡ�
    }
    else
    {
        return (uint32_t)(sum/i);// 0 ����֮ǰ�����ݣ�
    }
}

/*
*********************************************************************************************************
*   �� �� ��: App_caiji_Lux_Loop
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_Lux_Loop(void)
{
    uint32_t l_lux = 0;
    /*  �ɼ�Ŀǰ���ҵĲ��� */
    if(bsp_LUX_GetLux(&l_lux) == 0)
    {       
        g_wenshi.m_Lux = filter(l_lux );
        p_info("g_caiji ����ǿ��:%d",g_wenshi.m_Lux);           
    }
    else
    {
        p_err("g_caiji ����ǿ�� ackУ��ʧ��");
    }
}


/*
*********************************************************************************************************
*   �� �� ��: App_caiji_Air_T_H_Loop
*   ����˵��: get  �������¶Ⱥ�ʪ�� \����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_Air_T_H_Loop(void)
{       
    /*����� 30S  ��ʱ�˳����ж�  */
    if(bsp_CheckTimer(TMR_ID_Charge_Voltage_Indicator))
    {   
        bsp_StartTimer(TMR_ID_Charge_Voltage_Indicator, TMR_DELAY_30S);// 30����  
        App_Charge_Voltage_Indicator(); //�����20210627
    }

    /*��ʪ�� 1min ��ʱ�˳����ж�  */
    if(bsp_CheckTimer(TMR_ID_caiji_T_H_cycle))
    {   
        bsp_StartTimer(TMR_ID_caiji_T_H_cycle, g_tConfig.setAcqTimer*TMR_DELAY_1S);// ����ʱ�ɼ�  ��ʪ����Ϣ��          
        p_info("TMR_ID_caiji_T_H_cycle start");
    
        Bsp_sensor_power_on(); //sensor���硣    
        
        /*��ʼ soil ����  �ɼ���ʼ��*/ 
        if(g_caiji.soil_TH_work_state == CAIJI_soil_TH_MAX)// MAX ��״̬�� ���ܽ��вɼ�
        {
            g_caiji.soil_TH_start = TRUE;
            g_caiji.soil_TH_work_state = CAIJI_soil_TH_OPEN;
        }
        
        App_Battery_Voltage_Display_Data(); //��ص���  
        g_wenshi.m_voltage_level = 4;g_tBattery.charge_flag = 0;
        //g_wenshi.m_voltage_level = g_tBattery.voltage_level_g;                
        p_info("g_caiji ��ص���:%d",g_tBattery.voltage_level_g);   
        
        if(g_caiji.soil_TH_work_state == CAIJI_soil_TH_MAX)
            Bsp_sensor_power_off(); //sensor���硣   

        /* :  ���ݱ仯�ϱ� */
        if(App_data_report_process()&&(g_tConfig.report_changed))
        {
            App_caiji_report_start();//
            p_info("g_caiji ���ݱ仯�ϱ�");               
        }       
    }

}

/*
*********************************************************************************************************
*   �� �� ��: App_data_report_process
*   ����˵��:  ���ݱ仯�ϱ�
*   ��    �Σ���
*   �� �� ֵ: ��
�ϴ��ϱ������ݣ��Ա����ڲɼ������ݣ�
�¶�0.5�ȣ�ʪ��5%������ǿ��100lux��

*********************************************************************************************************
*/
#define  TEMP_CHA 5 // 0.5��
#define  Humi_CHA 50 // 5%
#define  Lux_CHA 100//100lux
#define  CO2_CHA 60//30ppm

uint8_t App_data_report_process(void)
{
    /*  �¶�0.5��  */
    if(abs(g_wenshi.m_T-g_wenshi.m_T_last) >TEMP_CHA)
    {
        p_info("�¶� ���ݱ仯�ϱ�%d",abs(g_wenshi.m_T-g_wenshi.m_T_last));                  
        return TRUE;
    }
    /*  ʪ��5%    */  
    if(abs(g_wenshi.m_H-g_wenshi.m_H_last) >Humi_CHA)
    {
        p_info("ʪ�� ���ݱ仯�ϱ�");                
        return TRUE;
    }

    /*  ����ǿ��5%  */  
    float lux = 0;
    //float value = 0;      
    lux = abs(g_wenshi.m_Lux-g_wenshi.m_Lux_last) ;
    //value = lux/(float)g_wenshi.m_Lux;
    #if 0
    if((value>Lux_CHA)&&(lux >=8))//����ǿ��5%��10
    #else
    if((lux>Lux_CHA))//����ǿ��100��ֵ
    #endif
    {
        p_info("����ǿ�� ���ݱ仯�ϱ�");              
        return TRUE;
    }

    /*  �����¶�0.5��    */
    if(abs(g_wenshi.m_soil_T-g_wenshi.m_soil_T_last) >TEMP_CHA)
    {
        p_info("�����¶�1�� ���ݱ仯�ϱ�%d",abs(g_wenshi.m_soil_T-g_wenshi.m_soil_T_last));              
        return TRUE;
    }
    /*  ����ʪ��5%  */  
    if(abs(g_wenshi.m_soil_H-g_wenshi.m_soil_H_last) >Humi_CHA)
    {
        p_info("����ʪ��5% ���ݱ仯�ϱ�");                
        return TRUE;
    }

    /*  �������    */  
    if( g_wenshi.m_soil_Water!=g_wenshi.m_soil_Water_last )
    {
        p_info("������� ���ݱ仯�ϱ�");              
        return TRUE;
    }

    /*  led_display */  
    if( g_wenshi.m_led_display!=g_wenshi.m_led_display_last )
    {
        p_info("led_display ���ݱ仯�ϱ�");               
        return TRUE;
    }   
    /*  m_pt100 */  
    if( g_wenshi.m_pt100!=g_wenshi.m_pt100_last )
    {
        p_info("m_pt100 ���ݱ仯�ϱ�");               
        return TRUE;
    }   
    /*  pt100_value */  
    if(abs(g_wenshi.m_pt100_value-g_wenshi.m_pt100_value_last) >TEMP_CHA)
    //if( g_wenshi.m_pt100_value!=g_wenshi.m_pt100_value_last )
    {
        p_info("pt100_value ���ݱ仯�ϱ�");               
        return TRUE;
    }   

    
    /*  TH_LUX  */  
    if( g_wenshi.m_TH_LUX!=g_wenshi.m_TH_LUX_last )
    {
        p_info("TH_LUX ���ݱ仯�ϱ�");                
        return TRUE;
    }
    /*  �¶�1��  */
    if(abs(g_wenshi.m_TH_LUX_t_value-g_wenshi.m_TH_LUX_t_value_last) >TEMP_CHA)
    {
        p_info("TH_LUX_t ���ݱ仯�ϱ�%d",abs(g_wenshi.m_TH_LUX_t_value-g_wenshi.m_TH_LUX_t_value_last));                  
        return TRUE;
    }
    /*  ʪ��5%    */  
    if(abs(g_wenshi.m_TH_LUX_h_value-g_wenshi.m_TH_LUX_h_value_last) >Humi_CHA)
    {
        p_info("TH_LUX_h ���ݱ仯�ϱ�");              
        return TRUE;
    }

    /*  m_TH_LUX_lux_value  */      
    /*  ����ǿ��5%  */  
    //float lux = 0;
    //float value = 0;      
    lux = abs(g_wenshi.m_TH_LUX_lux_value-g_wenshi.m_TH_LUX_lux_value_last) ;
    //value = lux/(float)g_wenshi.m_TH_LUX_lux_value;
    #if 0
    if((value>Lux_CHA)&&(lux >=8))//����ǿ��5%��10
    #else
    if((lux >= 100))//����ǿ��
    #endif
    {
        p_info("TH_LUX_lux ���ݱ仯�ϱ�");                
        return TRUE;
    }
    /*  co2 40ppm*/     
//    if(abs(g_wenshi.airTHvalueH_last-g_wenshi.airTHvalueH)>CO2_CHA)
//    {
//        p_info("CO2 ���ݱ仯�ϱ�");               
//        return TRUE;
//    }
//    if( g_wenshi.airTH!=g_wenshi.airTH_last)
//    {
//        p_info("CO2 ���ݱ仯�ϱ�");               
//        return TRUE;
//    }

    /*  0~15 mW/ cm2\   0-1500\ �Ŵ���100��������ָ��0-15���仯��1�����ϱ�*/       
    if(abs(g_wenshi.m_UV_value_last-g_wenshi.m_UV_value)>100)
    {
        p_info("UV ���ݱ仯�ϱ�");                
        return TRUE;
    }
    if( g_wenshi.m_UV!=g_wenshi.m_UV_last)
    {
        p_info("UV ���ݱ仯�ϱ�");                
        return TRUE;
    }       
    return FALSE;

}

/*
*********************************************************************************************************
*   �� �� ��: App_caiji_report_Loop
*   ����˵��:  
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void App_caiji_report_Loop(void)
{       
    if(seq_record & NB_SEQ_INIT)
    {
        //p_info("REPORT_CAIJI_LOOP ��ʼ��������");   
    }
    else
    {
        //p_err("App_caiji_report_Loop,������");
        return;
    }   

    switch(g_caiji.work_state_report)
    {
        case REPORT_CAIJI_INIT :
        {               
            g_caiji.work_state_report = REPORT_CAIJI_gettime;

            bsp_StartTimer(TMR_ID_report_poweron, TMR_DELAY_30S);// 2���ӵ���ʱ����ʱ�ж�
            
            bsp_SetTimer(TMR_ID_report_refresh);// �����������ʱ�䣻����1���ӽ���Уʱ����Ľ���
            g_caiji.gettime_tx_count = 0;

            p_info("g_caiji CAIJI_INIT");
        }
            break;
        case REPORT_CAIJI_gettime :
        {       
            /*  20s �ɼ�һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_POWER_ON;
                    g_caiji.gettime_tx_count = 0;
                    bsp_StopTimer(TMR_ID_report_refresh);//��ֹͣ�������¼�ʱ
                    bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_1S);
                    
                    p_info("g_caiji REPORT_CAIJI_gettime_count");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETTIME);//                              
                    p_info("REPORT_CAIJI_gettime��������");                     
                }              
            }
            if(g_caiji.timer_server_ask_ok==TRUE)
            {
                g_caiji.timer_server_ask_ok=FALSE;
        
                g_caiji.work_state_report = REPORT_CAIJI_POWER_ON;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                p_info("g_caiji timer_server_ask_ok");
            }
        }
            break;

        case REPORT_CAIJI_POWER_ON :
        {               
            /*  20s �ɼ�һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);                

                App_DataReport_SensorBasic();
                
                #ifdef ENABLE_SHESHISUO_PLC 
                App_sheshisuo_PLC_DataReport_SensorExtend();
                #endif                            
                //APP_STATE = NB_CoAP_ST;   //��������
                p_info("POWER_ON��ʼ��������");                   
            }
            /*  �����ϱ���2���ӵ���ʱ����ʱ�ж� */
            if(bsp_CheckTimer(TMR_ID_report_poweron))
            {               
                g_caiji.work_state_report = REPORT_CAIJI_getinfo;

                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                
                p_info("�����ϱ���2���ӵ���ʱ����");                
            }   
            if(g_caiji.sensor_basic_server_ask_ok==TRUE)
            {
                g_caiji.sensor_basic_server_ask_ok=FALSE;
                g_caiji.work_state_report = REPORT_CAIJI_getinfo;
                
                p_info("�����ϱ� sensor_basic_server_ask_ok");              
            }
        }       
            break;

            

        case REPORT_CAIJI_getinfo :
        {       
            /*  20s �ɼ�һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_getreport;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                    p_info("g_caiji REPORT_CAIJI_getinfo");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETINFO);//                              
                    p_info("REPORT_CAIJI_getinfo ��������");                        
                }              
            }
            if(g_caiji.info_server_ask_ok==TRUE)
            {
                g_caiji.info_server_ask_ok=FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_getreport;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                p_info("g_caiji info_server_ask_ok");
            }
        }
            break;  

        case REPORT_CAIJI_getreport:
        {       
            /*  20s �ɼ�һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_report_imei_imsi;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                    p_info("g_caiji REPORT_CAIJI_getreport");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_GETREPORT);//                            
                    p_info("REPORT_CAIJI_getreport ��������");                      
                }              
            }
            if(g_caiji.report_server_ask_ok==TRUE)
            {
                g_caiji.report_server_ask_ok=FALSE;

                g_caiji.work_state_report = REPORT_CAIJI_report_imei_imsi;
                g_caiji.gettime_tx_count = 0;
                bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
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
                bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                p_info("g_caiji REPORT_CAIJI_report_imei_imsi");
            }
        
            /*  20s �ɼ�һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >=2)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_report_Connectivity;
                    g_caiji.gettime_tx_count = 0;
                    bsp_SetTimer(TMR_ID_report_refresh);// ����30���ӳ�ʼ����
                    p_info("g_caiji REPORT_CAIJI_report_imei_imsi");
                }
                else
                {
                    app_deviceState_eventReport(EVENT_REPORT_ReportNBID);//                            
                    p_info("REPORT_CAIJI_report_imei_imsi ��������");                       
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
                bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//���ݱ仯����2Сʱ���仯���ϱ�һ��
                p_info("g_caiji REPORT_CAIJI_report_Connectivity");
            }
            #endif
        
            /*  10s ����һ��*/
            if(bsp_CheckTimer(TMR_ID_report_refresh))
            {
                bsp_StartTimer(TMR_ID_report_refresh,TMR_DELAY_10S);
                
                g_caiji.gettime_tx_count++;
                if(g_caiji.gettime_tx_count >=3)
                {
                    g_caiji.work_state_report = REPORT_CAIJI_LOOP;
                    bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//���ݱ仯����2Сʱ���仯���ϱ�һ��
                    p_info("g_caiji REPORT_CAIJI_report_Connectivity");
                }
                else
                {
                    //App_DeviceState_Report_Connectivity();//                            
                    APP_STATE = NB_CoAP_ST;   //��������
                    p_info("REPORT_CAIJI_report_Connectivity ��������");                        
                }               
            }           
        }
            break;
                        
            
        case REPORT_CAIJI_LOOP :
        {               
            /* ���ݱ仯����2Сʱ���仯���ϱ�һ�� */
            if(bsp_CheckTimer(TMR_ID_report_loop))
            {
                bsp_StartTimer(TMR_ID_report_loop, (g_tConfig.report_second*TMR_DELAY_1S));//���ݱ仯����2Сʱ���仯���ϱ�һ��              
                g_wenshi.server_ok_count++;
                if(g_wenshi.server_ok_count >= 5)//����3�β�����������
                {
                    /*������ͨѶ���������� ��ʱ�˳����ж�   */
                    //if(bsp_CheckTimer(TMR_ID_server_ok))
                    {
                        p_info("server_ok_count %d����������",g_wenshi.server_ok_count);
                        App_fault_analysis_Set_Value(REBOOT_FLAG_SERVER);
                        bsp_DelayMS(10);
                        HAL_NVIC_SystemReset();//����
                    }

                }

                    #if 0
                    App_DataReport_04();//
                    #else
                    App_DataReport_SensorBasic();
                    #if ENABLE_WATCHDOG 
                    HAL_IWDG_Refresh(&hiwdg) ;/* --- ι�� */
                    #endif                  
                    #ifdef ENABLE_SHESHISUO_PLC 
                    App_sheshisuo_PLC_DataReport_SensorExtend();
                    #endif                      
                    #endif 
                    //APP_STATE = NB_CoAP_ST;   //��������
                    p_info("REPORT_CAIJI_LOOP ��ʼ��������");                         
            }
            
        }       
            break;  
            
        default:
            break;
    }
}


/*
*********************************************************************************************************
*   �� �� ��:  Modbus_Protocolverification
*   ����˵��:   modbus
*   ��    �Σ���
*   �� �� ֵ: ��
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

    /*�ȴ������Ч��PLC_TH����  */         
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
        /* ���ݳ���̫���� ����*/
        if((pos-i) <= 4)
        {
            ret = VERIFICATION_ADDRESS;
            return ret;
        }   
        
        /* ����CRCУ��� */
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


