/*
*********************************************************************************************************
*
*	模块名称 : 串口中断+FIFO驱动模块
*	文件名称 : bsp_uart_fifo.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

/*
	如果需要更改串口对应的管脚，请自行修改 bsp_uart_fifo.c文件中的 static void InitHardUart(void)函数
*/

/* 定义使能的串口, 0 表示不使能（不增加代码大小）， 1表示使能 */
/*
	安富莱STM32-V4 串口分配：
	【串口1】 RS232 芯片第1路。
		PA9/USART1_TX	  --- 打印调试口
		PA10/USART1_RX
*/

#define	UART1_FIFO_EN	1   //串口1、 zigbee
#define	UART3_FIFO_EN	1   //串口3、 gps

//#define	UART4_FIFO_EN	1
#define	UART5_FIFO_EN	1 //串口5、485




/* 定义端口号 */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200   //zigbee  中断
	#define UART1_TX_BUF_SIZE	1*256
	#define UART1_RX_BUF_SIZE	1*256
#endif
/* 定义串口波特率和FIFO缓冲区大小，分为发送缓冲区和接收缓冲区, 支持全双工 */
#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600   //gps  中断
	#define UART3_TX_BUF_SIZE	2*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif


#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200 // 串口调试
	#define UART4_TX_BUF_SIZE	1*512
	#define UART4_RX_BUF_SIZE	1*512
#endif
#if UART5_FIFO_EN == 1					//485   中断
	#define UART5_BAUD			9600
	#define UART5_TX_BUF_SIZE	1*128
	#define UART5_RX_BUF_SIZE	1*1024
#endif

/* 串口设备结构体 */
typedef struct
{
	USART_TypeDef *uart;		/* STM32内部串口设备指针 */
	uint8_t *pTxBuf;			/* 发送缓冲区 */
	uint8_t *pRxBuf;			/* 接收缓冲区 */
	uint16_t usTxBufSize;		/* 发送缓冲区大小 */
	uint16_t usRxBufSize;		/* 接收缓冲区大小 */
	__IO uint16_t usTxWrite;			/* 发送缓冲区写指针 */
	__IO uint16_t usTxRead;			/* 发送缓冲区读指针 */
	__IO uint16_t usTxCount;			/* 等待发送的数据个数 */

	__IO uint16_t usRxWrite;			/* 接收缓冲区写指针 */
	__IO uint16_t usRxRead;			/* 接收缓冲区读指针 */
	__IO uint16_t usRxCount;			/* 还未读取的新数据个数 */

	void (*SendBefor)(void); 	/* 开始发送之前的回调函数指针（主要用于RS485切换到发送模式） */
	void (*SendOver)(void); 	/* 发送完毕的回调函数指针（主要用于RS485将发送模式切换为接收模式） */
	void (*ReciveNew)(uint8_t _byte);	/* 串口收到数据的回调函数指针 */
}UART_T;

void bsp_InitUart(void);

uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

extern  UART_T g_tUart1;
extern  UART_T g_tUart3;
extern  UART_T g_tUart4;
extern 	UART_T g_tUart5;

extern uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];	   /* 接收缓冲区 */

#define	ZIGBEE_HA_UART1	COM1
#define	ZIGBEE_UART1	COM1

#define	GPS_UART1	COM3

#define Reserve_485_uart COM5 // 土壤温湿度传感器，地址注释

/* RS485芯片发送使能GPIO, PB2 */
#define RCC_RS485_TXEN 	RCC_AHB1Periph_GPIOA
#define PORT_RS485_TXEN  RS485_EN_GPIO_Port
#define PIN_RS485_TXEN	 RS485_EN_Pin

#define RS485_RX_EN()	HAL_GPIO_WritePin(PORT_RS485_TXEN, PIN_RS485_TXEN, GPIO_PIN_RESET)
#define RS485_TX_EN()	HAL_GPIO_WritePin(PORT_RS485_TXEN, PIN_RS485_TXEN, GPIO_PIN_SET)

void RS485_SendBuf(uint8_t *_ucaBuf, uint16_t _usLen);
void RS485_SendStr(char *_pBuf);
void RS485_SendOver(void);

void bsp_Set485Baud(uint32_t _baud);

#endif

/*****************************    (END OF FILE) *********************************/
