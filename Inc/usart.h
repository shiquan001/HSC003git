/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "NB_BC95.h"
#include "bsp_gps_l70_r.h"
#define ZIGBEE_HANDLE_UART  huart1

/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */
typedef enum 
{
  NB,
  GPS
}UART_TYPE;

/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
extern uint8_t aRxBuffer1[80];      //接收缓存

extern uint8_t * msg_debug;
extern uint8_t * msg;
extern uint8_t * gps_msg;

void HAL_UARTDMA_Init( UART_TYPE type,void* cb,uint32_t baud);
//******************************************************************************
// fn : USART1_UART_Write
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
void HAL_UART_Write(UART_TYPE type,uint8_t*buf,uint16_t len);
//******************************************************************************
// fn : USART1_UART_Read
//
// brief : 通过串口读取数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
uint16_t HAL_UART_Read(UART_TYPE type,uint8_t*buf,uint16_t len);
//******************************************************************************
// fn : USART1_UART_Poll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件。见dma_rece_cfg.h中定义
uint8_t HAL_UART_Poll(void);
//******************************************************************************
// fn : USART1_UART_RxBufLen
//
// brief : 返回当前接收缓存区中有效的数据长度
//
// param : none
//
// return : none
uint16_t HAL_UART_RxBufLen(UART_TYPE type);

//******************************************************************************
// fn : HAL_UART_Close
//
// brief : 关闭使用
//
// param : none
//
// return : none
void HAL_UART_Close(UART_TYPE type);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
