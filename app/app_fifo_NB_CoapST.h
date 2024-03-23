/*
*********************************************************************************************************
*
*	模块名称 :  
*	文件名称 : app_fifo_NB_CoapST.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef _APP_FIFO_NB_COAPST_H_
#define _APP_FIFO_NB_COAPST_H_



/* 供外部调用的函数声明 */

void app_fifo_NB_CoapST_Init(void);
uint8_t app_fifo_NB_CoapST_Put(char * p_byte_array, uint16_t * p_size);
uint8_t app_fifo_NB_CoapST_Get(char * p_byte,uint16_t * p_size);
void app_fifo_NB_CoapST_Clear(void);
void App_fifo_NB_CoapST_Loop(void);
void App_fifoNBCoapST_putAgain(void);

#endif

/*****************************    (END OF FILE) *********************************/
