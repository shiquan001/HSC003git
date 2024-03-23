#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32l4xx_hal.h"


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;


void bsp_DelayUS(uint32_t n);

#define bsp_DelayMS HAL_Delay 

#endif





























