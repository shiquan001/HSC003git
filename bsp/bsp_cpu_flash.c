/*
*********************************************************************************************************
*
*	ģ������ : cpu�ڲ�falsh����ģ��
*	�ļ����� : bsp_cpu_flash.c
*	��    �� : V1.0
*	˵    �� : �ṩ��дCPU�ڲ�Flash�ĺ���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "bsp_cpu_flash.h"
#if 0
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetSector
*	����˵��: ���ݵ�ַ���������׵�ַ
*	��    �Σ���
*	�� �� ֵ: �����׵�ַ
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
*	�� �� ��: bsp_ReadCpuFlash
*	����˵��: ��ȡCPU Flash������
*	��    �Σ�_ucpDst : Ŀ�껺����
*			 _ulFlashAddr : ��ʼ��ַ
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 0=�ɹ���1=ʧ��
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* ����Ϊ0ʱ����������,������ʼ��ַΪ���ַ����� */
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
*	�� �� ��: bsp_CmpCpuFlash
*	����˵��: �Ƚ�Flashָ����ַ������.
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpBuf : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ:
*			FLASH_IS_EQU		0   Flash���ݺʹ�д���������ȣ�����Ҫ������д����
*			FLASH_REQ_WRITE		1	Flash����Ҫ������ֱ��д
*			FLASH_REQ_ERASE		2	Flash��Ҫ�Ȳ���,��д
*			FLASH_PARAM_ERR		3	������������
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* ��ȱ�־ */
	uint8_t ucByte;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*��������������*/
	}

	/* ����Ϊ0ʱ������ȷ */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash���ݺʹ�д���������� */
	}

	ucIsEqu = 1;			/* �ȼ��������ֽںʹ�д���������ȣ���������κ�һ������ȣ�������Ϊ 0 */
	for (i = 0; i < _ulSize; i++)
	{
		ucByte = *(uint8_t *)_ulFlashAddr;

		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFF)
			{
				return FLASH_REQ_ERASE;		/* ��Ҫ��������д */
			}
			else
			{
				ucIsEqu = 0;	/* ����ȣ���Ҫд */
			}
		}

		_ulFlashAddr++;
		_ucpBuf++;
	}

	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash����Ҫ������ֱ��д */
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
	/* --- ι�� */
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
*	�� �� ��: bsp_WriteCpuFlash
*	����˵��: д���ݵ�CPU �ڲ�Flash��
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpSrc : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 0-�ɹ���1-���ݳ��Ȼ��ַ�����2-дFlash����(����Flash������)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
	//uint32_t i;
	 uint32_t FlashProtection = 0;
  	uint8_t ucRet;
	//uint16_t usTemp;
	flash_status_t status = FLASH_OK;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* ����Ϊ0 ʱ����������  */
	if (_ulSize == 0)
	{
		return 0;
	}

	/* ����Ϊ����ʱ����������  */
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
		p_info("����FLASHIF_PROTECTION_NONE");
	}
	else
	{
		/* FLASH ���� */
		HAL_FLASH_Unlock();

		/* erase user application area */
		Flash_If_Erase(_ulFlashAddr);  

		Flash_If_Write(_ucpSrc, _ulFlashAddr,_ulSize);

		/* Flash ��������ֹдFlash���ƼĴ��� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
