/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bsp.h"


//接收缓存初始化
#define MAX_RECV_LEN 256 
uint8_t msg_buff[MAX_RECV_LEN] = {0};// 第一级接收缓存
uint8_t * msg = msg_buff;

uint8_t gps_msg_buff[MAX_RECV_LEN] = {0};// 第一级接收缓存
uint8_t * gps_msg = gps_msg_buff;


uint8_t msg_debug_buff[MAX_RECV_LEN] = {0};// 第一级接收缓存
uint8_t * msg_debug = msg_debug_buff;


#include "_dma_cfg.h"
typedef void (*uart_common_cb)(char*, uint16_t);

//static bc95_receive_cb  nb_receCb = NULL;

//gps与nb_bc95都是使用串口，所以串口要支持两个模块的回调函数
static uart_common_cb cb_register_table[2];

static uint8_t msgBuf[RECE_BUF_MAX_LEN];

static void uart_dma_send(uint8_t type,uint8_t* buf,uint16_t len);


/* USER CODE END 0 */

UART_HandleTypeDef hlpuart1;//GPS
UART_HandleTypeDef huart5;	//RS485
UART_HandleTypeDef huart1;	//ZIGBEE
UART_HandleTypeDef huart3;	//NB
DMA_HandleTypeDef hdma_lpuart_rx;
DMA_HandleTypeDef hdma_lpuart_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* LPUART1 init function 	GPS	*/

void MX_LPUART1_UART_Init(void)
{

  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 9600;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* UART5 init function 		RS485	*/
void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;//9600
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;

  //20220303 解决串口接收停止工作
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;//UART_ADVFEATURE_NO_INIT;
  huart5.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart5.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;

  
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART1 init function 		ZIGBEE	*/

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function 	NB	*/

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  #if 0
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  #endif
  //20220303 解决串口接收停止工作
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT|UART_ADVFEATURE_DMADISABLEONERROR_INIT;//UART_ADVFEATURE_NO_INIT;
  huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  huart3.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspInit 0 */

  /* USER CODE END LPUART1_MspInit 0 */
    /* LPUART1 clock enable */
    __HAL_RCC_LPUART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**LPUART1 GPIO Configuration    
    PC1     ------> LPUART1_TX
    PB10     ------> LPUART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* LPUART1 DMA Init */
    /* LPUART_RX Init */
    hdma_lpuart_rx.Instance = DMA2_Channel7;
    hdma_lpuart_rx.Init.Request = DMA_REQUEST_4;
    hdma_lpuart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_lpuart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_lpuart_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_lpuart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_lpuart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_lpuart_rx.Init.Mode = DMA_CIRCULAR;
    hdma_lpuart_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_lpuart_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_lpuart_rx);

    /* LPUART_TX Init */
    hdma_lpuart_tx.Instance = DMA2_Channel6;
    hdma_lpuart_tx.Init.Request = DMA_REQUEST_4;
    hdma_lpuart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_lpuart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_lpuart_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_lpuart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_lpuart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_lpuart_tx.Init.Mode = DMA_NORMAL;
    hdma_lpuart_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_lpuart_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_lpuart_tx);

    /* LPUART1 interrupt Init */
    HAL_NVIC_SetPriority(LPUART1_IRQn, 8, 0);
    HAL_NVIC_EnableIRQ(LPUART1_IRQn);
  /* USER CODE BEGIN LPUART1_MspInit 1 */

  /* USER CODE END LPUART1_MspInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 7, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE); //接收非空中断

	if(HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1) != HAL_OK)
	{
		Error_Handler();
	}

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE); //接收非空中断

  if(HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1) != HAL_OK)
  {
	  Error_Handler();
  }

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Request = DMA_REQUEST_2;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 9, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

#if 0
	__HAL_UART_CLEAR_FLAG(&huart3,UART_CLEAR_TCF);
	
	  __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE); //接收非空中断
	
	if(HAL_UART_Receive_IT(&huart3,(uint8_t*)gps_msg,1) != HAL_OK)
	{
		Error_Handler();
	}
#endif
  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==LPUART1)
  {
  /* USER CODE BEGIN LPUART1_MspDeInit 0 */

  /* USER CODE END LPUART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPUART1_CLK_DISABLE();
  
    /**LPUART1 GPIO Configuration    
    PC1     ------> LPUART1_TX
    PB10     ------> LPUART1_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    /* LPUART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* LPUART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(LPUART1_IRQn);
  /* USER CODE BEGIN LPUART1_MspDeInit 1 */

  /* USER CODE END LPUART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();
  
    /**UART5 GPIO Configuration    
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    /* UART5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PC4     ------> USART3_TX
    PC5     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	uint8_t ret = HAL_OK;
	msg++;
	if( msg == msg_buff + MAX_RECV_LEN)
	{
		msg = msg_buff;
	}
	do
	{
		ret = HAL_UART_Receive_IT(UartHandle,(uint8_t *)msg,1); 
	}while(ret != HAL_OK);
	 
	if(*(msg-1) == '\n')   //接收以\n为结尾字符，则表示接收完成
	{
		flag  = 1;
	}
}
#endif
 u32	l_count  = 0;

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->ErrorCode&HAL_UART_ERROR_ORE)
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
	}


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{ 
	
	if(huart == &huart1)
	{
		g_tUart1.pRxBuf[g_tUart1.usRxWrite] = msg_buff[0];
		if (++g_tUart1.usRxWrite >= g_tUart1.usRxBufSize)
		{
			g_tUart1.usRxWrite = 0;
		}
		if (g_tUart1.usRxCount < g_tUart1.usRxBufSize)
		{
			g_tUart1.usRxCount++;
		}
		//HAL_UART_Receive_IT(&huart1,(uint8_t*)msg,1);// 
		bsp_StartTimer(TMR_ID_ZIGBEE_HA_UART_DATAIN, 2);		/* 设置5ms	 4ms 超时 (115200) */		
		//bsp_StartTimer(TMR_ID_485_UART, 5);						/* 设置5ms	 4ms 超时 (115200) */		
	}
	if(huart == &huart3)
	{
		g_tUart3.pRxBuf[g_tUart3.usRxWrite] = gps_msg_buff[0];
		if (++g_tUart3.usRxWrite >= g_tUart3.usRxBufSize)
		{
			g_tUart3.usRxWrite = 0;
		}
		if (g_tUart3.usRxCount < g_tUart3.usRxBufSize)
		{
			g_tUart3.usRxCount++;
		}
		bsp_StartTimer(TMR_ID_GPS_UART_DATAIN, 5);		/* 设置5ms	 超时 (9600) */		
	}
		
	if(huart == &huart5)
	{
		g_tUart5.pRxBuf[g_tUart5.usRxWrite] = msg_debug_buff[0];
		if (++g_tUart5.usRxWrite >= g_tUart5.usRxBufSize)
		{
			g_tUart5.usRxWrite = 0;
		}
		if (g_tUart5.usRxCount < g_tUart5.usRxBufSize)
		{
			g_tUart5.usRxCount++;
		}
		//HAL_UART_Receive_IT(&huart5,(uint8_t*)msg_debug,1);// 	
		bsp_StartTimer(TMR_ID_485_UART, 30);						/* 设置5ms 超时 (9600) */		
	}	
	if(huart == &hlpuart1)
	{
		l_count++;
	}


	
} 


void  HAL_UARTDMA_Init(UART_TYPE type,void* cb,uint32_t baud)
{
  baud = baud;
  UART_HandleTypeDef *handle = NULL;
  USART_TypeDef* hUart = NULL;
  cb_register_table[type] = (uart_common_cb)cb;
  
  if(type == NB)
  {
    handle = &huart3;
    hUart = USART3;
  }
  else
  {
    handle = &hlpuart1;
    hUart = LPUART1;
  }
  
  HAL_UART_Receive_DMA(handle,UartDma_Init((uint8_t)type,uart_dma_send,hUart),RECE_BUF_MAX_LEN);
  
}
//******************************************************************************
// fn : USART1_UART_Write
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
void HAL_UART_Write(UART_TYPE type,uint8_t*buf,uint16_t len)
{
  buf[len] = 0;
  printf("\r\n->:%s",buf);
  
  UartDma_Write(type,buf,len);
}

//******************************************************************************
// fn : uart_dma_send
//
// brief : 通过串口向外发送指定数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
static void uart_dma_send(uint8_t type,uint8_t* buf,uint16_t len)
{
  UART_HandleTypeDef* handle = NULL;
  
  if(type == NB)
  {
    handle = &huart3;
  }
  else
  {
    handle = &hlpuart1;
  }
  HAL_UART_Transmit_DMA(handle,buf,len); 
}

//******************************************************************************
// fn : HAL_UART_Read
//
// brief : 通过串口读取数据
//
// param : buf -> 数据缓存地址
//         len -> 数据长度
//
// return : none
uint16_t HAL_UART_Read(UART_TYPE type,uint8_t*buf,uint16_t len)
{
  return UartDma_Read(type,buf,len);
}
//******************************************************************************
// fn : HAL_UART_Poll
//
// brief : 轮询串口事件
//
// param : none
//
// return : 事件。见dma_rece_cfg.h中定义
uint8_t HAL_UART_Poll(void)
{
  uint8_t evt =  UartDma_Poll();   //查询串口是否有数据
  uint16_t len = 0;
  
  UART_TYPE  type_uart[2];
  uint8_t    type_count = 0;
  
  
  if(evt)
  {

 	 //printf("evt<-:%d\r\n",evt);	  
    //数据模型是栈的方法
    if(evt & (HAL_LPUART_RX_TIMEOUT|HAL_LPUART_RX_FULL))
    {
      type_uart[type_count++] = NB;
    }
    
    if(evt & (HAL_UART2_RX_TIMEOUT|HAL_UART2_RX_FULL))
    {
      type_uart[type_count++] = GPS;
    }
    
    for(uint8_t i = 0 ; i < type_count ; i++)
    {
    
      len = HAL_UART_RxBufLen(type_uart[i]);
	  if(len>=256)
	  	{
	  	__nop();

	  	}
	  
	  if(type_uart[i] == NB)
  	 {
   	 	  printf("NB<-:%d\r\n",len);	  
	      if(len)
	      {
	        len = UartDma_Read(type_uart[i],msgBuf,len);
			//printf("\r\n<-:%d",len);
			
			msgBuf[len] = 0;
			if(type_uart[i] == NB)
            {         
	            //printf("%s\r\n",msgBuf);
	            if(len <= 32)
                {   
    	            for(uint16_t i = 0;i<len;i++)
                        printf("%02X ",msgBuf[i]);
                    printf("\r\n");
                }
                else
                {
                    for(uint16_t i = 0;i<32;i++)
                        printf("%02X ",msgBuf[i]);
                    printf("\r\n");
                }
            }
	        (cb_register_table[type_uart[i]])((char*)msgBuf,len);
	      }	 
  	 }
	  if(type_uart[i] == GPS)
	  {
       	 printf("GPS<-:%d\r\n\r",len);
		 if(len)
		 {
		   len = UartDma_Read(type_uart[i],msgBuf,len);		   		   
		   msgBuf[len] = 0;
	       	// printf("GPS<-:%s\r\n",msgBuf);
			(cb_register_table[type_uart[i]])((char*)msgBuf,len);
		 }	

		 
	  }

    }
  }
  return evt;
}
//******************************************************************************
// fn : HAL_UART_RxBufLen
//
// brief : 返回当前接收缓存区中有效的数据长度
//
// param : none
//
// return : none
uint16_t HAL_UART_RxBufLen(UART_TYPE type)
{
  return UartDma_Avail(type);
}
//******************************************************************************
// fn : HAL_UART_Close
//
// brief : 关闭使用
//
// param : none
//
// return : none
void HAL_UART_Close(UART_TYPE type)
{
  if(type == NB)
  {
    HAL_UART_DeInit(&huart3);
  }
  else
  {
    HAL_UART_DeInit(&hlpuart1);
  }
  //HAL_UART_MspDeInit();
}


//******************************************************************************
// 	串口调试采用 lpuart1
//******************************************************************************

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */

  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  	#if 0
	 HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	#else
	 UartDma_Write(GPS,(uint8_t *)&ch, 1);
	#endif
 	 //HAL_UART_Transmit(&huart5, (uint8_t *)&ch, 1, 0xFFFF);
	 //RS485_SendBuf((uint8_t *)&ch, 1);
	 
  return ch;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
