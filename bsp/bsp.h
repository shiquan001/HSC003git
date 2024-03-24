/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��
*	�ļ����� : bsp.h
*	˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*			  ����Ҫ#include ÿ��ģ��� h �ļ�
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

/*  spi flash address*/
#define SPI_FLASH_POWER_ON_DELAY 100//40//POWER ON 40MS


#if 1
#define  STM32_FLASH_BASE_FLAG_SET   0x0803ff00	  //�������õĲ���
#else
#define  STM32_FLASH_BASE_FLAG_SET   0X08013000	  //�������õĲ��� 128k
#endif

#define  STM32_FLASH_BASE_FLAG_UPDATE   0X0807FE00	  //�������õĲ���

#include "stm32l4xx_hal.h"


/* ���������ڵ��Խ׶��Ŵ� */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_Ht1621.h"

#include "gpio.h"
#include "rtc.h"
#include "usart.h"
#include "spi.h"
#include "iwdg.h"


#include "bsp_uart_fifo.h"
#include "bsp_timer.h"
#include "adc.h"

#include "bsp_i2c_gpio.h"
#include "bsp_gps_l70_r.h"
#include "bsp_LUX_V20.h"
#include "bsp_SHT10.h"
#include "bsp_key_set.h"

#include "bsp_camera.h"
#include "bsp_cpu_flash.h"
#include "flash_if.h"

#include "stmflash.h"
#include "bsp_spi_bus.h"
#include "bsp_spi_flash.h"
#include "bsp_SHT20A.h"

#include "bsp_SHT30A.h"

#include "bsp_user_lib.h"
#include "bsp_modbus.h"
#include "bsp_msg.h"
#include "bsp_fifo_cmd.h"

#include "NB_Board_Cfg.h"

#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif


#define debug_handle huart5  //  ���Դ���

// #include "delay.h"
#include "debug.h"

#include "main.h"
#include "app_wenshi.h"

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);
void bsp_Init_bootloard(void);

#endif

/*****************************    (END OF FILE) *********************************/
