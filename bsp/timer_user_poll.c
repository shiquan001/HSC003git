//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : timer_user_poll.c
//  Date     : 2017-12-04 16:11
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// BC95ϵ��NB-IOTģ���ATָ���������ָ����Ӧ���н���
//******************************************************************************
#include "timer_user_poll.h"
#include "nb_bc95.h"

bc95_timeout_cb  nb_timeoutCb;

uint32_t  gTickStart = 0;
uint32_t  gTickDelta = 0;
uint8_t   gTimeOpenFlag = 0;

void MX_TIM_Set(bc95_timeout_cb cb)
{
  nb_timeoutCb = cb;
}

void MX_TIM_Start(uint32_t ms)
{
  gTickDelta = ms;
  gTickStart = HAL_GetTick();
  gTimeOpenFlag = 1;
}

void MX_TIM_Stop(void)
{
  gTimeOpenFlag = 0;
}


void MX_TimerPoll()
{
  uint32_t tick;
  if(gTimeOpenFlag)
  {
    tick = HAL_GetTick();
    uint32_t de = tick >= gTickStart ? tick - gTickStart : tick + UINT32_MAX - gTickStart;
    if(gTickDelta > de)
    {
      gTickDelta -= de;
      gTickStart = tick;
    }
    else
    {
      gTickDelta = 0;
      gTimeOpenFlag = 0;
      if(nb_timeoutCb)
      {
        nb_timeoutCb();
      }
    }
    
  }
}



/* USER CODE BEGIN 4 */


//******************************************************************************
// fn : isGetTime
//
// brief : 5s һ��
//
// param : none
//
// return : true -> yes; false -> wait
uint8_t isGetTime(void)
{
  static uint32_t start_tick = 0;
  uint32_t cur_tick = 0;
  uint8_t  readFlag = 0;
  
	//5 s
  cur_tick = HAL_GetTick();
  if(cur_tick >= start_tick )
  {
    if(cur_tick - start_tick >= 5000)
    {
      readFlag = 1;
    }
    else
    {
      readFlag = 0;
    }
  }
  else
  {
    if((UINT32_MAX - start_tick + cur_tick) >= 5000)
    {
      readFlag = 1;
    }
    else
    {
      readFlag = 0;
    }
  }
  
  if(readFlag)
  {
    start_tick = cur_tick;
  } 
  return readFlag;
}

