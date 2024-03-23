/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
//**************************************
// fn : Bsp_gps_en
//
// brief : ʹ��GPS
//
// param : none
//
// return : none
extern void Bsp_gps_power_on(void);
extern void Bsp_gps_power_off(void);
extern void Bsp_nb_power_on(void);
extern void Bsp_nb_power_off(void);
extern void Bsp_sensor_power_on(void);
extern void Bsp_sensor_power_off(void);
extern void Bsp_12V_power_on(void);
extern void Bsp_12V_power_off(void);
extern void Bsp_zigbee_power_off(void);
extern void Bsp_zigbee_power_on(void);


extern void Bsp_spiflash_power_on(void);
extern void Bsp_spiflash_power_off(void);
extern void Bsp_sensor_power_off_power_off(void);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
