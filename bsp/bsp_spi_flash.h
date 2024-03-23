/*
*********************************************************************************************************
*
*	ģ������ : SPI�ӿڴ���FLASH ��дģ��
*	�ļ����� : bsp_spi_flash.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_SPI_FLASH_H
#define _BSP_SPI_FLASH_H

#include "main.h"
#if 0
/* ����Flsh��ƬѡGPIO�˿�  */
#define SF_RCC_CS 			RCC_AHB1Periph_GPIOB
#define SF_PORT_CS			GPIOB
#define SF_PIN_CS			GPIO_Pin_11
#endif
#define SF_CS_0()			HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO_Port, FLASH_SPI_CS_Pin, GPIO_PIN_RESET);

#define SF_CS_1()			HAL_GPIO_WritePin(FLASH_SPI_CS_GPIO_Port, FLASH_SPI_CS_Pin, GPIO_PIN_SET);


#define SF_MAX_PAGE_SIZE	(4 * 1024)

/* ���崮��Flash ID */
//ID = 00EF4016,
enum
{
	SST25VF016B_ID = 0xBF2541,
	MX25L1606E_ID  = 0xC22015,
	W25Q16BV_ID    = 0xEF4015,
	W25Q32BV_ID    = 0xEF4016,
	W25Q64BV_ID    = 0xEF4017,
	W25Q128_ID	   = 0x00EF4018
};

typedef struct
{
	uint32_t ChipID;		/* оƬID */
	char ChipName[16];		/* оƬ�ͺ��ַ�������Ҫ������ʾ */
	uint32_t TotalSize;		/* ������ */
	uint16_t PageSize;		/* ҳ���С */
}SFLASH_T;

void bsp_InitSFlash(void);
uint32_t sf_ReadID(void);
void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_PageWrite(uint8_t * _pBuf, uint32_t _uiWriteAddr, uint16_t _usSize);
uint8_t sf_WriteBuffer(uint8_t* _pBuf, uint32_t _uiWriteAddr, uint16_t _usWriteSize);
void sf_ReadBuffer(uint8_t * _pBuf, uint32_t _uiReadAddr, uint32_t _uiSize);
void sf_ReadInfo(void);
void sf_EraseChip(void);

extern SFLASH_T g_tSF;

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
