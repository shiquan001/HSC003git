/*
*********************************************************************************************************
*
*	ģ������ : �����ж�+FIFO����ģ��
*	�ļ����� : bsp_uart_fifo.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

/*
	�����Ҫ���Ĵ��ڶ�Ӧ�Ĺܽţ��������޸� bsp_uart_fifo.c�ļ��е� static void InitHardUart(void)����
*/

/* ����ʹ�ܵĴ���, 0 ��ʾ��ʹ�ܣ������Ӵ����С���� 1��ʾʹ�� */
/*
	������STM32-V4 ���ڷ��䣺
	������1�� RS232 оƬ��1·��
		PA9/USART1_TX	  --- ��ӡ���Կ�
		PA10/USART1_RX
*/

#define	UART1_FIFO_EN	1   //����1�� zigbee
#define	UART3_FIFO_EN	1   //����3�� gps

//#define	UART4_FIFO_EN	1
#define	UART5_FIFO_EN	1 //����5��485




/* ����˿ں� */
typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12, PD2 */
}COM_PORT_E;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200   //zigbee  �ж�
	#define UART1_TX_BUF_SIZE	1*256
	#define UART1_RX_BUF_SIZE	1*256
#endif
/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART3_FIFO_EN == 1
	#define UART3_BAUD			9600   //gps  �ж�
	#define UART3_TX_BUF_SIZE	2*1024
	#define UART3_RX_BUF_SIZE	1*1024
#endif


#if UART4_FIFO_EN == 1
	#define UART4_BAUD			115200 // ���ڵ���
	#define UART4_TX_BUF_SIZE	1*512
	#define UART4_RX_BUF_SIZE	1*512
#endif
#if UART5_FIFO_EN == 1					//485   �ж�
	#define UART5_BAUD			9600
	#define UART5_TX_BUF_SIZE	1*128
	#define UART5_RX_BUF_SIZE	1*1024
#endif

/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	__IO uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	__IO uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	__IO uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	__IO uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	__IO uint16_t usRxRead;			/* ���ջ�������ָ�� */
	__IO uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(uint8_t _byte);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;

void bsp_InitUart(void);

uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

extern  UART_T g_tUart1;
extern  UART_T g_tUart3;
extern  UART_T g_tUart4;
extern 	UART_T g_tUart5;

extern uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];	   /* ���ջ����� */

#define	ZIGBEE_HA_UART1	COM1
#define	ZIGBEE_UART1	COM1

#define	GPS_UART1	COM3

#define Reserve_485_uart COM5 // ������ʪ�ȴ���������ַע��

/* RS485оƬ����ʹ��GPIO, PB2 */
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
