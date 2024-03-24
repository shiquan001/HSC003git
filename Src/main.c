/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/*
 * @Author: shiquan001
 * @Email: 448332311@qq.com
 * @Date: 2019-08-08 14:10:05
 * @Last Modified by:   shiquan001
 * @Last Modified time: 2019-08-08 14:10:05
 * @Description: Description
 */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
#include "usart.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp.h"
#include "app.h"
#include "app_main.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */




/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//Ҫд�뵽STM32 FLASH���ַ�������
u8 TEXT_Buffer[] = {"STM32L4 FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer)              //���鳤��  
#define SIZE ( TEXT_LENTH/8+((TEXT_LENTH%8)?1:0) )  //������64λ�洢��Ҫд��ĳ���
#define FLASH_SAVE_ADDR  0X08030000     //����FLASH �����ַ(����Ϊ8�ı���������������,Ҫ���ڱ�������ռ�õ�������.
//����,д������ʱ��,���ܻᵼ�²�����������,�Ӷ����𲿷ֳ���ʧ.��������.


/*printf()����λ��Ӧ�Ĵ�ӡ���������*/

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    #if ENABLE_WATCHDOG
    MX_IWDG_Init(); 
    #endif
    /* USER CODE END 1 */
  
    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* ��ʼ��key    ,systick ��Ҫ���ð���ɨ�������*/
    bsp_InitKey_set();

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();


    /* USER CODE BEGIN SysInit */
     //DISABLE_INT();
    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_LPUART1_UART_Init();//GPS
    MX_USART3_UART_Init();//NB
    MX_RTC_Init();
    MX_ADC1_Init();
    MX_UART5_Init();//RS485
    MX_SPI3_Init();
    MX_USART1_UART_Init();//ZIGBEE
    // MX_IWDG_Init();

    /* USER CODE BEGIN 2 */

    /*��ʼ gps �ɼ���ʼ��*/ 
    bsp_GPS_Init(App_gps_Msg_cb); //��ʼ������

    ENABLE_INT();//
  
    #if ENABLE_WATCHDOG
    /* --- ι�� */
    HAL_IWDG_Refresh(&hiwdg) ;
    #endif

    bsp_DelayMS(20);// ���ڴ��ڴ�ӡ����

    bsp_Init();
    App_fault_analysis_Init_Var();
    p_info("App_Ver:%s!",VERSION_WENSHI_cur);
    print_reboot_flag();//��Ҫ�ȳ�ʼ��spi flash�Ĺ���
    clear_reboot_flag();
  
    app_tasks();//����ʹ��
    
    #if 1
    App_caiji_report_Init_Var();
    g_caiji.start_gps = TRUE;
    g_caiji.work_state_gps = CAIJI_GPS_INIT;

    bsp_SetTimer(TMR_ID_caiji_T_H_cycle);//��ʼ�ɼ���ʪ�ȵ���Ϣ       
    bsp_SetTimer(TMR_ID_Charge_Voltage_Indicator);//��ʼ�ɼ������Ϣ
    
    //bsp_SHT30_Init();
    //p_info("bsp_SHT30_Init ������");
    //�������1�����sht30�Ĳ���  
    //if(SHT20_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H))
    //{
    //  bsp_SHT30_Get_TH(&g_wenshi.m_Symbol, &g_wenshi.m_T, &g_wenshi.m_H);//�����¶� ʪ��        
    //}
    #endif
    /*  ��ʾ����ǿ��*/
    App_display_illumination(g_wenshi.m_Lux,TRUE);
    /*  ��ʾ��ʪ��*/
    App_display_temp(g_wenshi.m_T/10,TRUE);
    App_display_humidity(g_wenshi.m_H/10,TRUE);
    
    app_caiji_sensorDataSync();//ͬ������֮�����LED��Ļ����
    App_485_tx_led_display_cmd(ADDRESS_LED_DISPLAY,FUNCTION_CODE_10);// ˢ������

    App_wenshi_Init_Var();
    App_ZIGBEE_HA_Init_Var();// zigbee ��ʼ��

    Bsp_nb_power_off();
    bsp_DelayMS(4000);
    Bsp_nb_power_on();
    bsp_DelayMS(4000);

    app_4G_initVar();//
    app_4G_initStart();//

    NBModule_open(&nb_config);  

    App_nb_Init_Var();// app ��ʼ��

    App_Battery_Init_Var();
    App_display_Init_Var();
    App_caiji_report_Init_Var();

    //App_Update_Init_Var();// update
    app_update4G_initVar();// update

    /*  ��ʾ��ص���*/
    g_wenshi.m_voltage_level = 4;g_tBattery.charge_flag = 0;
    App_display_battery_and_charge(g_wenshi.m_voltage_level,g_tBattery.charge_flag);

    App_caiji_report_start();//
    App_nb_Init_start();//nb ��ʼ

    p_info("while ��ʼ");

    App_Config_Init_Var();//��ȡ������flash�еĿ��ػ����
    g_wenshi.power_onoff = POWER_ON;//ǿ�ƿ���
    
    /*�ػ���*/
    if(g_wenshi.power_onoff == POWER_OFF)
    {
        App_wenshi_poweroff();
        p_info("��ʼ�����ػ�");
    }
    else
    {
        p_info("��ʼ��������");
    }
    //App_server_gettime(0x04D32A8A);
    bsp_ClearKey_set();//
    App_camera_Init_Var();
    #ifdef ENABLE_TIANJING_PLC  
    App_caiji_PLC_Init_Var();
    #endif
    
    #ifdef ENABLE_SHESHISUO_PLC
    App_sheshisuo_PLC_Init_Var();
    #endif  

    bsp_InitCmd();

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        bsp_Idle();
        App_key_process();  /* �������¼� */

        /*�ػ��ˣ����������Ļ���ֱ��������*/
        if(g_wenshi.power_onoff == POWER_OFF)
        {
            /* NBģ��ĺ�������*/
            MX_TimerPoll();// NB��ר�ö�ʱ��
            HAL_UART_Poll();// ��������ͨ���ص��������н�����NB�� GPS
            NBModule_Main(&nb_config);
        }
        else if(g_wenshi.power_onoff == POWER_ON)
        {
            App_wenshi_Loop();//��ʪ����ʾ��
        }   

         /* Refresh IWDG: reload counter */
        #if ENABLE_WATCHDOG
        if (HAL_IWDG_Refresh(&hiwdg) != HAL_OK)
        {
            /* Refresh Error */
            p_info("�㲻��ʱι��....\r\n");  
            Error_Handler();
        }
        #endif
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART5
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI2;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.PLLSAI2.PLLSAI2Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI2.PLLSAI2M = 1;
  PeriphClkInit.PLLSAI2.PLLSAI2N = 8;
  PeriphClkInit.PLLSAI2.PLLSAI2P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI2.PLLSAI2R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_ADC2CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 // printf("HAL error" );

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
