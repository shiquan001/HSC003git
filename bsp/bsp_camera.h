/*
*********************************************************************************************************
*
*	模块名称 : 摄像头驱动BSP模块(For OV7670)
*	文件名称 : bsp_camera.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __DCMI_H
#define __DCMI_H

#include "stm32l4xx_hal.h"
#include "main.h"
#include "delay.h"
#include "bsp_i2c_gpio.h"
#include "app_wenshi.h"


/*
SEN_PWDN	PA15	低功耗使能
NRESET	PC13	复位
*/

#define START_RESET_NT99141() 		HAL_GPIO_WritePin(NRESET_GPIO_Port, NRESET_Pin, GPIO_PIN_RESET)
#define END_RESET_NT99141() 		HAL_GPIO_WritePin(NRESET_GPIO_Port, NRESET_Pin, GPIO_PIN_SET)

#define START_POWER_DOWN()			HAL_GPIO_WritePin(SEN_PWDN_GPIO_Port, SEN_PWDN_Pin, GPIO_PIN_SET)
#define END_POWER_DOWN()			HAL_GPIO_WritePin(SEN_PWDN_GPIO_Port, SEN_PWDN_Pin, GPIO_PIN_RESET)

#define NT99141_SLAVE_ADDRESS_WR		0X54 	//写NT99141时的从机地址
#define NT99141_SLAVE_ADDRESS_RD		0X55 	//读NT99141时的从机地址
#define NT99141_JPEG_REG_NUM		(107 - 44 + 1)
#define NT99141_INIT_REG_NUM		22
#define DEGUG_CAMERA	0X01

typedef struct
{
	u8 CaptureOk;		/* 捕获完成 */
	u16 Count_ok;		/* 计数 */
	u16 Dma_Count;		/* 计数 */
	u8 Capture_End;		/* 捕获完成 */
	u32 Capture_End_Cur;		/* 捕获完成位置 */

	u8 VsyncActive_count ;// 帧同步
	
	u8 VsyncActive ;// 帧同步
	u8 VsyncActive_rise_count ;// 帧同步 1
	u8 VsyncActive_fall_count ;// 帧同步 2
	
	u8 HSYNCActive;// 行同步
	
	u32 HSYNCActive_rise_Count;// 行同步
	u32 HSYNCActive_fall_Count;// 行同步

	u32 pclk_count;// 行同步
	
	u8 CamDataEnable;//像素 使能
	
	
	u32 JpegDataCnt;
}CAM_T;

//增加几个NT99141的函数
void NT99141_WriteReg16(uint16_t _ucRegAddr, uint8_t _ucRegValue);
uint8_t NT99141_ReadReg16(uint16_t _ucRegAddr);

uint16_t NT99141_ReadID(void);

uint8_t NT99141_InitReg(void);
uint8_t NT99141_JPEG_Config(void);

void NT99141_Hardware_Init(void);

void bsp_Init_Camera_NT99141(void);

void Camera_NT99141_Start(void);
void Camera_NT99141_Stop(void);


void bsp_camera_ov_ItEnable(void);
void bsp_camera_ov_ItDisable(void);


extern CAM_T g_tCam;
extern u8 JpegBuffer[ PIC_BUFF_LEN];

#endif

