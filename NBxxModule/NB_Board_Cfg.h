//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : NB_Board_Cfg.h
//  Date     : 2017-12-21 5:45
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// NB_IOT��ƽ̨��ص����ñ�������
//******************************************************************************
#include "nb_board.h"
extern NB_Config  nb_config;
extern void HAL_UART_NB_Init(bc95_receive_cb cb, uint32_t baud);
extern void HAL_UART_NB_Write(uint8_t*buf,uint16_t len);
