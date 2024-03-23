/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */
#include "bsp.h"
#include "app.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NRESET_GPIO_Port, NRESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SENSOR_EN_GPIO_Port, SENSOR_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, NB_EN_Pin|HT_WR_Pin|_12V_ENABLE_Pin|RS485_EN_Pin 
                          |SEN_PWDN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HT_CS_Pin|HT_RD_Pin|HT_DATA_Pin|IIC1_SCL_Pin 
                          |IIC1_SDA_Pin|FLASH_SPI_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ZIGBEE_EN_GPIO_Port, ZIGBEE_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPS_EN_GPIO_Port, GPS_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = NRESET_Pin|SENSOR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin 
                           PAPin PAPin */
  GPIO_InitStruct.Pin = NB_EN_Pin|HT_WR_Pin|_12V_ENABLE_Pin|GPS_EN_Pin 
                          |RS485_EN_Pin|SEN_PWDN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin 
                           PBPin PBPin */
  GPIO_InitStruct.Pin = HT_CS_Pin|HT_RD_Pin|HT_DATA_Pin|IIC1_SCL_Pin 
                          |IIC1_SDA_Pin|FLASH_SPI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ZIGBEE_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ZIGBEE_EN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/*
*********************************************************************************************************
*	函 数 名: Bsp_gps_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_gps_power_on(void)
{
//  HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_RESET);
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_gps_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_gps_power_off(void)
{
//  HAL_GPIO_WritePin(GPS_EN_GPIO_Port,GPS_EN_Pin,GPIO_PIN_SET);
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_nb_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
#define HIGH_LEVEL 
void Bsp_nb_power_on(void)
{
	#ifdef HIGH_LEVEL
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_SET);	// 1    ON
	#else
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_RESET);//0 ON	
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_nb_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_nb_power_off(void)
{
	#ifdef HIGH_LEVEL
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_RESET);	
	#else
	HAL_GPIO_WritePin(NB_EN_GPIO_Port,NB_EN_Pin,GPIO_PIN_SET);	
	#endif
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_sensor_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_sensor_power_on(void)
{
//  HAL_GPIO_WritePin(SENSOR_EN_GPIO_Port,SENSOR_EN_Pin,GPIO_PIN_RESET);
}
/*
*********************************************************************************************************
*	函 数 名: Bsp_sensor_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_sensor_power_off(void)
{
#if 0
  HAL_GPIO_WritePin(SENSOR_EN_GPIO_Port,SENSOR_EN_Pin,GPIO_PIN_SET);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_sensor_power_off_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_sensor_power_off_power_off(void)
{
#if 1
//  HAL_GPIO_WritePin(SENSOR_EN_GPIO_Port,SENSOR_EN_Pin,GPIO_PIN_SET);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_spiflash_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_spiflash_power_on(void)
{
//  Bsp_sensor_power_on();
}
/*
*********************************************************************************************************
*	函 数 名: Bsp_spiflash_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_spiflash_power_off(void)
{
//  SF_CS_0();
//  Bsp_sensor_power_off();
  
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_zigbee_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_zigbee_power_on(void)
{
//  HAL_GPIO_WritePin(ZIGBEE_EN_GPIO_Port,ZIGBEE_EN_Pin,GPIO_PIN_RESET);
}


/*
*********************************************************************************************************
*	函 数 名: Bsp_zigbee_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_zigbee_power_off(void)
{
//  HAL_GPIO_WritePin(ZIGBEE_EN_GPIO_Port,ZIGBEE_EN_Pin,GPIO_PIN_SET);
}
/*
*********************************************************************************************************
*	函 数 名: Bsp_12V_power_on
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_12V_power_on(void)
{
//  HAL_GPIO_WritePin(_12V_ENABLE_GPIO_Port,_12V_ENABLE_Pin,GPIO_PIN_SET);
}
/*
*********************************************************************************************************
*	函 数 名: Bsp_12V_power_off
*	功能说明:   
*			  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_12V_power_off(void)
{
//  HAL_GPIO_WritePin(_12V_ENABLE_GPIO_Port,_12V_ENABLE_Pin,GPIO_PIN_RESET);
}


/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
