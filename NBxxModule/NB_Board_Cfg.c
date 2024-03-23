//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board_Cfg.c
//  Date     : 2017-12-21 5:45
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// NB_IOT��ƽ̨��ص����ñ�������
//******************************************************************************
#include "usart.h"

#include "timer_user_poll.h"

#include "NB_Board.h"
#include "NB_BC95.h"
extern int NB_MsgreportCb(msg_types_t,int ,char*);
//==============================================================================
//���ڽӿں�����NBҪ��ĺ����ӿ��β�ͬ������������˽ӿڲ���ת��
//******************************************************************************
// fn : HAL_UART_NB_Init
//
// brief : ��NB_95ʹ�ô���
//
// param : cb -> �������ݽ��ջص�����
//         baud -> ������
//
// return : none
//ע�⣬��ʱ������û�������ã�
void HAL_UART_NB_Init(bc95_receive_cb cb, uint32_t baud)
{
  HAL_UARTDMA_Init(NB,(void*)cb,baud);
}

//******************************************************************************
// fn : HAL_UART_NB_Write
//
// brief : ����ͨ�����ڷ���
//
// param : buf -> �������ݻ����ַ
//         len -> ��Ч���ݳ���
//
// return : none
void HAL_UART_NB_Write(uint8_t*buf,uint16_t len)
{
  HAL_UART_Write(NB,buf,len);
}

//******************************************************************************
// fn : HAL_UART_NB_Close
//
// brief : �ر������õĴ���
//
// param : none
//
// return : none
void HAL_UART_NB_Close(void)
{
  HAL_UART_Close(NB);
}
//==============================================================================
com_fxnTable  com_fxn = 
{
  .openFxn = HAL_UART_NB_Init,
  .sendFxn = HAL_UART_NB_Write,
  .closeFxn = HAL_UART_NB_Close
};

time_fxnTable time_fxn = 
{
  .initTimerFxn = MX_TIM_Set,
  .startTimerFxn = MX_TIM_Start,
  .stopTimerFxn = MX_TIM_Stop
};

hw_object_t  HWAtrrs_object = 
{
  .baudrate = 9600,
  .uart_fxnTable = &com_fxn,
  .timer_fxnTable = &time_fxn
};

NB_Config  nb_config = 
{
  .fxnTablePtr = NULL,
  .object = (void*)&HWAtrrs_object,
  .AppReceCB = NB_MsgreportCb,
  .log = NULL
};

