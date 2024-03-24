/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
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
#define  STM32_FLASH_BASE_FLAG_SET   0x0803ff00	  //保存设置的参数
#else
#define  STM32_FLASH_BASE_FLAG_SET   0X08013000	  //保存设置的参数 128k
#endif

#define  STM32_FLASH_BASE_FLAG_UPDATE   0X0807FE00	  //升级设置的参数

#include "stm32l4xx_hal.h"


/* 这个宏仅用于调试阶段排错 */
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


#define debug_handle huart5  //  调试串口

// #include "delay.h"
#include "debug.h"

#include "main.h"
#include "app_wenshi.h"

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);
void bsp_Init_bootloard(void);

#endif

/*****************************    (END OF FILE) *********************************/
