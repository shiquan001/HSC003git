/*
*********************************************************************************************************
*
*	ģ������ : cpu�ڲ�falsh����ģ��
*	�ļ����� : bsp_cpu_flash.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_CPU_FLASH_H_
#define _BSP_CPU_FLASH_H_


#include "stm32l4xx_hal.h"

#define FLASH_BASE_ADDR	0x08000000			/* Flash����ַ */
#if 0
#define	FLASH_SIZE		(256*1024)		/* Flash ���� */
#else
//#define	FLASH_SIZE		(1024*1024)		/* Flash ���� ʵ���ϵ�������256kb*/
#endif

/* ����F103��  512K FLASH , ÿ��PAGE = 2K �ֽڣ��ܹ� 256�� PAGE  */
//#define SECTOR_MASK			0xFFFFF800

#define FLASH_IS_EQU		0   /* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
#define FLASH_REQ_WRITE		1	/* Flash����Ҫ������ֱ��д */
#define FLASH_REQ_ERASE		2	/* Flash��Ҫ�Ȳ���,��д */
#define FLASH_PARAM_ERR		3	/* ������������ */


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


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

