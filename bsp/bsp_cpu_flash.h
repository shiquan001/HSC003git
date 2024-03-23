/*
*********************************************************************************************************
*
*	模块名称 : cpu内部falsh操作模块
*	文件名称 : bsp_cpu_flash.h
*	版    本 : V1.0
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_CPU_FLASH_H_
#define _BSP_CPU_FLASH_H_


#include "stm32l4xx_hal.h"

#define FLASH_BASE_ADDR	0x08000000			/* Flash基地址 */
#if 0
#define	FLASH_SIZE		(256*1024)		/* Flash 容量 */
#else
//#define	FLASH_SIZE		(1024*1024)		/* Flash 容量 实际上的容量是256kb*/
#endif

/* 对于F103，  512K FLASH , 每个PAGE = 2K 字节，总共 256个 PAGE  */
//#define SECTOR_MASK			0xFFFFF800

#define FLASH_IS_EQU		0   /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
#define FLASH_REQ_WRITE		1	/* Flash不需要擦除，直接写 */
#define FLASH_REQ_ERASE		2	/* Flash需要先擦除,再写 */
#define FLASH_PARAM_ERR		3	/* 函数参数错误 */


/**
  * @brief	HAL Status structures definition
  flash_status_t
  */
typedef enum
{
  FLASH_OK	   = 0x00,
  FLASH_ERR ,
  FLASH_CHECK_ERR,
} flash_status_t;

uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize);
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize);
flash_status_t Flash_If_Write(uint8_t *src, uint32_t dest_addr, uint32_t Len);
flash_status_t Flash_If_Erase(uint32_t Add);
flash_status_t Flash_If_Read(uint8_t* buff, uint32_t dest_addr, uint32_t Len);

#endif


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

