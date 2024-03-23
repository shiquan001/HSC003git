//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : key_handle.c
//  Date :     2018-1-12 9:55
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************

#include "bsp.h"
#include "key_handle.h"

#define KEY_DELAY_TICK   20

typedef struct
{
  uint8_t key_event;
  int     start_tick;
}key_press_t;

static key_press_t key_check_press = {0,0};

static key_cb   pFkey_cb;
//**************************************
// fn : KEY_RegisterCb
//
// brief : ע�ᰴť�¼��ص�
//
// param : cb -> ����ť�¼�����ָ��
//
// return : none
void KEY_RegisterCb(key_cb cb)
{
  if(cb != 0)
  {
    pFkey_cb = cb;
  }
}

//**************************************
// fn : KEY_Poll
//
// brief : ��ѯ��ť�¼�
//
// param : none
//
// return : none
void KEY_Poll(void)
{
  uint8_t key_event = 0 ;
  if(key_check_press.key_event)
  {
    if(HAL_GetTick() - key_check_press.start_tick >= KEY_DELAY_TICK )
    {
      if(key_check_press.key_event & KEY_UP)
      {
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET)
        {
          key_event |= KEY_UP;
        }
        key_check_press.key_event ^= KEY_UP;
      }
      #if 0
      if(key_check_press.key_event & KEY_LEFT)
      {
        if(HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port,KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {
          key_event |= KEY_LEFT;
        }
        key_check_press.key_event ^= KEY_LEFT;
      }
      if(key_check_press.key_event & KEY_DOWN)
      {
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port,KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
          key_event |= KEY_DOWN;
        }
        key_check_press.key_event ^= KEY_DOWN;
      }
      if(key_check_press.key_event & KEY_RIGHT)
      {
        if(HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port,KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {
          key_event |= KEY_RIGHT;
        }
        key_check_press.key_event ^= KEY_RIGHT;
      }
	  #endif
    }
  }
  //�������а�ť���£���ִ�лص�����
  if(key_event && pFkey_cb)
  {
    pFkey_cb(key_event);
  }
}

//************************************************************************
// fn : HAL_GPIO_EXTI_Callback
//
// brief : �����жϻص�����
//
// param : GPIO_Pin-> ���ű��
//
// return : none
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//�������жϷ�����Ҫ������������ȷ���Ƿ�Ϊ��Ч����
	if(GPIO_Pin == KEY1_Pin)
	{
		key_check_press.key_event = KEY_UP;
		key_check_press.start_tick = HAL_GetTick();
	}
	
	#if 0
	if(GPIO_Pin == KEY_LEFT_Pin)
	{
		key_check_press.key_event = KEY_LEFT;
		key_check_press.start_tick = HAL_GetTick();
	}
	if(GPIO_Pin == KEY_DOWN_Pin)
	{
		key_check_press.key_event = KEY_DOWN;
		key_check_press.start_tick = HAL_GetTick();
	}
	if(GPIO_Pin == KEY_RIGHT_Pin)
	{
		key_check_press.key_event = KEY_RIGHT;
		key_check_press.start_tick = HAL_GetTick();
	}
	#endif

    #if 0
	//�������жϷ�����Ҫ������������ȷ���Ƿ�Ϊ��Ч����
	if(GPIO_Pin == KEY1_Pin)
	{
		g_key_press = 1;
	}
	#endif
	/* ֡ͬ�� */
	if(GPIO_Pin == VIN_VSYNC_Pin)
	{
		/* ������->�ߵ�ƽ->��Ч��ͬ���ź�*/
		if(VIN_VSYNC_GPIO_Port->IDR & VIN_VSYNC_Pin)
		{	
			#if 0
			//p_info("VIN_VSYNC_Pin 1--");
		
			/*//ʹ�� ��ͬ���ж�*/
			g_tCam.VsyncActive = VsyncActive_START;
			g_tCam.JpegDataCnt =4;
			#endif
			g_tCam.VsyncActive_rise++;
		}
		else
		{	/*//�½���->��ͬ���źŽ���*/
			#if 0
			if(g_tCam.VsyncActive == VsyncActive_START)
			{	
				/* �Ѿ�������һ�������ĳ�ͬ���׶�*/						
				g_tCam.VsyncActive = VsyncActive_START_HIGH;
				g_tCam.CaptureOk = 1;
				Camera_NT99141_Stop();	/*//�ر�����ͬ���ж�*/
			}
			else if(g_tCam.VsyncActive == VsyncActive_NULL)
			{
				//p_info("VIN_VSYNC_Pin 2--");
			
				g_tCam.VsyncActive = VsyncActive_NULL;
			}
			#endif
			g_tCam.VsyncActive_rise--;
		}														
	}  
}
