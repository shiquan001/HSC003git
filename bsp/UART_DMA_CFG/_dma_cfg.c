//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : _dma_cfg.c
//  Date     : 2017-12-18 20:43
//  Version  : V0001
// ��ʷ��¼  : ��һ�δ���
//******************************************************************************
#include <string.h>
#include "_dma_cfg.h"
#include "usart.h"

#define HI_UINT16(DATA)       ((DATA)>>8)
#define LO_UINT16(DATA)       ((DATA) & 0xFF)
#define BUILD_UINT16(hi,lo)   (uint16_t)(((hi) & 0x00FF) << 8 + ((lo) & 0x00FF))

#define DMA_NEW_RX_BYTE(TYPE,IDX)  (0x00 == HI_UINT16(dmaCfg[TYPE].buf[(IDX)]))// default oxff
#define DMA_GET_RX_BYTE(TYPE,IDX)  (LO_UINT16(dmaCfg[TYPE].buf[(IDX)]))
#define DMA_CLR_RX_BYTE(TYPE,IDX)  (dmaCfg[TYPE].buf[(IDX)] = 0xFFFF)

#define HAL_UART_DMA_IDLE        20//60 // 255  //5ms  10ms  50ms//500ms


#define HAL_UART_DMA_FULL          (RECE_BUF_MAX_LEN - 0)

#define TRUE                        1
#define FALSE                       0

 dmaCfg_t               dmaCfg[2];
static sendData_cb            dmaSendCb[2];
static USART_TypeDef*         hDmaUart[2] = {0,0};
//*****************************************************************************
// fn :    findTail
//
// brief :  �ҵ���ǰDMA���ջ��������ڲ�����λ��
//
// param : None.
//
// return : Index of tail of rxBuf.
//*****************************************************************************/
static uint16_t findTail(uint8_t type)
{
  uint16_t idx = dmaCfg[type].rxHead;

  do
  {
    if (!DMA_NEW_RX_BYTE(type,idx))
    {
      break;
    }
    
    if (++idx >= RECE_BUF_MAX_LEN)
    {
      idx = 0;
    }
  } while (idx != dmaCfg[type].rxHead);

  return idx;
}

//******************************************************************************
// fn : UartDma_Init
//
// brief : ��ʼ��dmaReceCfg�Ľṹ
//
// param : none
//
// return : none
uint8_t* UartDma_Init(uint8_t type,sendData_cb sendCb ,USART_TypeDef* hUart)
{
	#if 1
  memset(dmaCfg[type].buf,0xff,RECE_BUF_MAX_LEN<<1);
	#else
	memset(dmaCfg[type].buf,0xffff,RECE_BUF_MAX_LEN);
	#endif 
  dmaCfg[type].rxHead = 0;
  dmaCfg[type].rxTail = 0;
  dmaCfg[type].rxTick = 0;
  dmaCfg[type].rxShdw = 0;

  memset(dmaCfg[type].txBuf,0x00,SENT_BUF_MAX_LEN);
  dmaCfg[type].txIdx[0] = 0;
  dmaCfg[type].txIdx[1] = 0;
  dmaCfg[type].txSel = 0;
  
  dmaCfg[type].txTick = 0;  
  dmaCfg[type].txShdw = 0;  	
  dmaCfg[type].txShdwValid = FALSE;
  dmaCfg[type].txDMAPending = FALSE;
  
  dmaSendCb[type] = sendCb;
  hDmaUart[type] = hUart;
  return (uint8_t*)dmaCfg[type].buf;
}
//******************************************************************************
// fn :     UartDma_Read
//
// brief :  �ӽ��ջ������ȡָ�����ȴ������ݣ����ͷ�ռ�õĿռ�
//
// param :  buf  - ��Ч���ȵ�Ŀ�껺��
//          len  - Ҫ��ȡ�ĳ���
//
// return : length of buffer that was read
//******************************************************************************
uint16_t UartDma_Read(uint8_t type,uint8_t *buf, uint16_t len)
{
  uint16_t cnt;

  for (cnt = 0; cnt < len; cnt++)
  {
    if (!DMA_NEW_RX_BYTE(type,dmaCfg[type].rxHead))
    {
      break;
    }
    *buf++ = DMA_GET_RX_BYTE(type,dmaCfg[type].rxHead);
    
    //�ͷ�ռ�ÿռ�
    DMA_CLR_RX_BYTE(type,dmaCfg[type].rxHead);

    if (++(dmaCfg[type].rxHead) >= RECE_BUF_MAX_LEN)
    {
      dmaCfg[type].rxHead = 0;
    }
  }
//	if(cnt >= RECE_BUF_MAX_LEN)
//		cnt = RECE_BUF_MAX_LEN-1;
	
  return cnt;
}
//******************************************************************************
// fn :     UartDma_Write
//
// brief :  ��Ҫ���͵����ݣ����������ͻ�����
//
// param :  buf  - ��Ч���ȵ�Ŀ�껺��
//          len  - Ҫ��ȡ�ĳ���
//
// return : length of buffer that was write
//******************************************************************************
uint16_t UartDma_Write(uint8_t type,uint8_t *buf, uint16_t len)
{
  uint16_t cnt;
  uint8_t txSel;
  uint16_t txIdx;

  // Enforce all or none.
  if ((len + dmaCfg[type].txIdx[dmaCfg[type].txSel]) > SENT_BUF_MAX_LEN)
  {
    return 0;
  }

  txSel = dmaCfg[type].txSel;
  txIdx = dmaCfg[type].txIdx[txSel];

  for (cnt = 0; cnt < len; cnt++)
  {
    dmaCfg[type].txBuf[txSel][txIdx++] = buf[cnt];
  }
  
  if (txSel != dmaCfg[type].txSel)
  {
    txSel = dmaCfg[type].txSel;
    txIdx = dmaCfg[type].txIdx[txSel];

    for (cnt = 0; cnt < len; cnt++)
    {
      dmaCfg[type].txBuf[txSel][txIdx++] = buf[cnt];
    }
  }

  dmaCfg[type].txIdx[txSel] = txIdx;

  if (dmaCfg[type].txIdx[(txSel ^ 1)] == 0)
  {
    // TX DMA is expected to be fired
    dmaCfg[type].txDMAPending = TRUE;
  }

  return cnt;
}
//**************************************************************************************************
// fn  :  UartDma_Avail
//
// brief :  ���㻺���������ݸ��� - the number of bytes in the buffer.
//
// param :  none
//
// return : the number of bytes
//**************************************************************************************************/
extern uint16_t UartDma_Avail(uint8_t type)
{
  uint16_t cnt = 0;

  if (DMA_NEW_RX_BYTE(type,dmaCfg[type].rxHead))
  {
    uint16_t idx;

	#if 1  // 20191109
    for (idx = 0; idx < RECE_BUF_MAX_LEN; idx++)
	#else
    for (idx = dmaCfg[type].rxHead; idx < RECE_BUF_MAX_LEN; idx++)
	#endif
    {
      if (DMA_NEW_RX_BYTE(type,idx))
      {
        cnt++;
      }
    }
  }

  return cnt;
}
//******************************************************************************
// fn : UartDma_Poll
//
// brief : ��ѯ���ջ������ݳ���
//����֡��Ҫ������ʱ����100ms
// param : none
//
// return : none
//****************************************************************************/
uint8_t UartDma_Poll(void)
{
  uint16_t cnt = 0;
  uint8_t evt = 0;
  uint16_t tail = 0;
  
  for(uint8_t i = 0 ; i < 2 ; i++)
  {
	  cnt = 0;// clear zero 
	  tail = 0;
	  
    if(DMA_NEW_RX_BYTE(i,dmaCfg[i].rxHead))
    {
      tail = findTail(i);
      
      // If the DMA has transferred in more Rx bytes, reset the Rx idle timer.
      if (dmaCfg[i].rxTail != tail)
      {
        dmaCfg[i].rxTail = tail;
		#if 0
        if (dmaCfg[i].rxTick == 0)
        {
          dmaCfg[i].rxShdw = HAL_GetTick();
        }
		#else
		dmaCfg[i].rxShdw = HAL_GetTick();// 20191109ֻҪβ�������ݱ仯�ͽ������ʱ�̵ĸ���
		#endif
		if(i ==NB)
        	dmaCfg[i].rxTick = HAL_UART_DMA_IDLE;
		if(i ==GPS)
    		dmaCfg[i].rxTick = 200;//200;	//200ms ���
      }
      else if (dmaCfg[i].rxTick)
      {
        uint32_t Tick = HAL_GetTick();
        uint32_t delta = Tick >= dmaCfg[i].rxShdw ?
                                 (Tick - dmaCfg[i].rxShdw ): 
                                 (Tick + (UINT32_MAX - dmaCfg[i].rxShdw));
        
        if (dmaCfg[i].rxTick > delta)
        {
          dmaCfg[i].rxTick -= delta;
          dmaCfg[i].rxShdw = Tick;
        }
        else
        {
          dmaCfg[i].rxTick = 0;
        }
      }
      cnt = UartDma_Avail(i);
    }
    else
    {
      dmaCfg[i].rxTick = 0;
    }

    if (cnt >= HAL_UART_DMA_FULL)
    {
      evt |= HAL_UART_RX_FULL<<(i<<1);
    }
    else if (cnt && !dmaCfg[i].rxTick)// ������ͬʱ���ʱ�䵽��
    {
      evt |= HAL_UART_RX_TIMEOUT<<(i<<1);
   	    if(evt>=256)
	  	{
	  	__nop();

	  	}
	}
    
    if (dmaCfg[i].txShdwValid)
    {
      uint32_t decr = HAL_GetTick() - dmaCfg[i].txShdw;
    
      if (decr > dmaCfg[i].txTick)
      {
        // No protection for txShdwValid is required
        // because while the shadow was valid, DMA ISR cannot be triggered
        // to cause concurrent access to this variable.
        dmaCfg[i].txShdwValid = FALSE;
      }
    }
    
    if (dmaCfg[i].txDMAPending && !dmaCfg[i].txShdwValid)
    {
      // Clear the DMA pending flag
      dmaCfg[i].txDMAPending = FALSE;
      //Send  data
      if(dmaSendCb[i])
      {
        (dmaSendCb[i])(i,dmaCfg[i].txBuf[dmaCfg[i].txSel],dmaCfg[i].txIdx[dmaCfg[i].txSel]);
      }
      dmaCfg[i].txSel ^= 1;
    } 
  }

  return evt;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  uint8_t index = 0xff;
  
  if(huart->Instance == hDmaUart[0])
  {
    index= 0;  //��ʾNB
  }
  else if(huart->Instance == hDmaUart[1])
  {
    index = 1; //��ʾGPS
  }
  
  if(index != 0xff)
  {
    // Indicate that the other buffer is free now.
    dmaCfg[index].txIdx[(dmaCfg[index].txSel ^ 1)] = 0;
    
    // Set TX shadow
    dmaCfg[index].txShdw = HAL_GetTick();
    dmaCfg[index].txShdwValid = TRUE;
	dmaCfg[index].txTick = 0;

    // If there is more Tx data ready to go, re-start the DMA immediately on it.
    if (dmaCfg[index].txIdx[dmaCfg[index].txSel])
    {
      // UART TX DMA is expected to be fired
      dmaCfg[index].txDMAPending = TRUE;
    }
  }
}


