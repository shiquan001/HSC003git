/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NRESET_Pin GPIO_PIN_13
#define NRESET_GPIO_Port GPIOC
#define SENSOR_EN_Pin GPIO_PIN_2
#define SENSOR_EN_GPIO_Port GPIOC
#define NB_EN_Pin GPIO_PIN_0
#define NB_EN_GPIO_Port GPIOA
#define HT_WR_Pin GPIO_PIN_3
#define HT_WR_GPIO_Port GPIOA
#define _12V_ENABLE_Pin GPIO_PIN_5
#define _12V_ENABLE_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOB
#define HT_CS_Pin GPIO_PIN_1
#define HT_CS_GPIO_Port GPIOB
#define HT_RD_Pin GPIO_PIN_2
#define HT_RD_GPIO_Port GPIOB
#define HT_DATA_Pin GPIO_PIN_11
#define HT_DATA_GPIO_Port GPIOB
#define ZIGBEE_EN_Pin GPIO_PIN_13
#define ZIGBEE_EN_GPIO_Port GPIOB
#define IIC1_SCL_Pin GPIO_PIN_14
#define IIC1_SCL_GPIO_Port GPIOB
#define IIC1_SDA_Pin GPIO_PIN_15
#define IIC1_SDA_GPIO_Port GPIOB
#define GPS_EN_Pin GPIO_PIN_11
#define GPS_EN_GPIO_Port GPIOA
#define RS485_EN_Pin GPIO_PIN_12
#define RS485_EN_GPIO_Port GPIOA
#define SEN_PWDN_Pin GPIO_PIN_15
#define SEN_PWDN_GPIO_Port GPIOA
#define FLASH_SPI_CS_Pin GPIO_PIN_3
#define FLASH_SPI_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
extern uint8_t g_key_press ;
extern uint8_t * msg ;
extern uint8_t * gps_msg ;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
