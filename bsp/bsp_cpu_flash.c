/*
*********************************************************************************************************
*
*	模块名称 : cpu内部falsh操作模块
*	文件名称 : bsp_cpu_flash.c
*	版    本 : V1.0
*	说    明 : 提供读写CPU内部Flash的函数
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-02-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_cpu_flash.h"
#if 0
/*
*********************************************************************************************************
*	函 数 名: bsp_GetSector
*	功能说明: 根据地址计算扇区首地址
*	形    参：无
*	返 回 值: 扇区首地址
*********************************************************************************************************
*/
uint32_t bsp_GetSector(uint32_t _ulWrAddr)
{
	uint32_t sector = 0;

	sector = _ulWrAddr & SECTOR_MASK;

	return sector;
}
#endif
/*
*********************************************************************************************************
*	函 数 名: bsp_ReadCpuFlash
*	功能说明: 读取CPU Flash的内容
*	形    参：_ucpDst : 目标缓冲区
*			 _ulFlashAddr : 起始地址
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0=成功，1=失败
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
	if (_ulSize == 0)
	{
		return 1;
	}

	for (i = 0; i < _ulSize; i++)
	{
		*_ucpDst++ = *(uint8_t *)_ulFlashAddr++;
	}

	return 0;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CmpCpuFlash
*	功能说明: 比较Flash指定地址的数据.
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpBuf : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值:
*			FLASH_IS_EQU		0   Flash内容和待写入的数据相等，不需要擦除和写操作
*			FLASH_REQ_WRITE		1	Flash不需要擦除，直接写
*			FLASH_REQ_ERASE		2	Flash需要先擦除,再写
*			FLASH_PARAM_ERR		3	函数参数错误
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* 相等标志 */
	uint8_t ucByte;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*　函数参数错误　*/
	}

	/* 长度为0时返回正确 */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash内容和待写入的数据相等 */
	}

	ucIsEqu = 1;			/* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	for (i = 0; i < _ulSize; i++)
	{
		ucByte = *(uint8_t *)_ulFlashAddr;

		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFF)
			{
				return FLASH_REQ_ERASE;		/* 需要擦除后再写 */
			}
			else
			{
				ucIsEqu = 0;	/* 不相等，需要写 */
			}
		}

		_ulFlashAddr++;
		_ucpBuf++;
	}

	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash不需要擦除，直接写 */
	}
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;

  if (Addr < (FLASH_BASE_ADDR + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE_ADDR) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE_ADDR + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
	page=page+256;
  }

  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
static uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
//  if(1)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE_ADDR + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE_ADDR + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}
/**
  * @brief  Reads Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be read (in bytes).
  * @retval return FLASH_OK.
  */
flash_status_t Flash_If_Read(uint8_t* buff, uint32_t dest_addr, uint32_t Len)
{

    uint32_t i;
    for(i = 0; i < Len; i++){
            buff[i] = *(__IO uint8_t*)(dest_addr + i);
    }
  /* Return a valid address to avoid HardFault */
  return FLASH_OK;
}

/**
  * @brief  Writes Data into Memory.
  * @param  src: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Len: Number of data to be written (in bytes).
  * @retval 0 if operation is successeful, MAL_FAIL else.
  */
flash_status_t Flash_If_Write(uint8_t *src, uint32_t dest_addr, uint32_t Len)
{
  uint32_t i = 0;

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

  for(i = 0; i < Len; i += 8)
  {
	/* --- 喂狗 */
	HAL_IWDG_Refresh(&hiwdg) ;
	
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by byte */
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (dest_addr+i), *((uint64_t*)(src+i))) == HAL_OK)
    {
     /* Check the written value */
      if(*(uint64_t *)(src + i) != *(uint64_t*)(dest_addr+i))
      {
        /* Flash content doesn't match SRAM content */

        return FLASH_CHECK_ERR;
      }

    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return FLASH_ERR;
    }
  }
  return FLASH_OK;
}

/**
  * @brief  Erases sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successeful, MAL_FAIL else.
  */
flash_status_t Flash_If_Erase(uint32_t Add)
{
  uint32_t PageError = 0;
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

   /* Get the number of sector to erase from 1st sector*/
  eraseinitstruct.Banks = GetBank(Add);
  eraseinitstruct.TypeErase = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.Page = GetPage(Add);
  eraseinitstruct.NbPages = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if (status != HAL_OK)
  {
    return FLASH_ERR;
  }
  return FLASH_OK;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_WriteCpuFlash
*	功能说明: 写数据到CPU 内部Flash。
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpSrc : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0-成功，1-数据长度或地址溢出，2-写Flash出错(估计Flash寿命到)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
	//uint32_t i;
	 uint32_t FlashProtection = 0;
  	uint8_t ucRet;
	//uint16_t usTemp;
	flash_status_t status = FLASH_OK;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0 时不继续操作  */
	if (_ulSize == 0)
	{
		return 0;
	}

	/* 长度为奇数时不继续操作  */
	if ((_ulSize % 2) != 0)
	{
		return 1;
	}	

	ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);

	if (ucRet == FLASH_IS_EQU)
	{
		return 0;
	}
	#if 1

	/* Initialise Flash */
	FLASH_If_Init();
	/* Test if any sector of Flash memory where user application will be loaded is write protected */
	FlashProtection = FLASH_If_GetWriteProtectionStatus();
	if(FlashProtection != FLASHIF_PROTECTION_NONE)
	{
		p_info("不是FLASHIF_PROTECTION_NONE");
	}
	else
	{
		/* FLASH 解锁 */
		HAL_FLASH_Unlock();

		/* erase user application area */
		Flash_If_Erase(_ulFlashAddr);  

		Flash_If_Write(_ucpSrc, _ulFlashAddr,_ulSize);

		/* Flash 加锁，禁止写Flash控制寄存器 */
		HAL_FLASH_Lock();

		p_info("FLASHIF_PROTECTION_NONE");
	}

	#endif

	if (status == FLASH_OK)
	{
		return 0;
	}
	return 2;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
