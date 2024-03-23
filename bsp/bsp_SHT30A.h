#ifndef _SHT30_T_H_
#define _SHT30_T_H_

/*--------------------------ͷ�ļ�����--------------------------------*/
#include "bsp_SHT30A_DataType.h"

#include "bsp_LUX_V20.h"

/*-----------------------------�ṹ�嶨��---------------------------------*/
#define SHT10_SCL_O    bsp_LUX_SCL0_O
#define SHT10_SCL_H    bsp_LUX_SCL0_H
#define SHT10_SCL_L    bsp_LUX_SCL0_L

#define SHT10_SDA_O    bsp_LUX_SDA0_O
#define SHT10_SDA_H    bsp_LUX_SDA0_H
#define SHT10_SDA_L    bsp_LUX_SDA0_L

#define SHT10_SDA_I    bsp_LUX_SDA0_I
#define SHT10_SDA_DAT  bsp_LUX_SDA0_DAT

/*-----------------------------�궨��---------------------------------*/
#define SHT30_SCL0_O    bsp_LUX_SCL0_O			//GPIOA12 ��©���
#define SHT30_SCL0_H    bsp_LUX_SCL0_H
#define SHT30_SCL0_L    bsp_LUX_SCL0_L

#define SHT30_SDA0_O    bsp_LUX_SDA0_O			//GPIOA8��©���
#define SHT30_SDA0_H    bsp_LUX_SDA0_H
#define SHT30_SDA0_L    bsp_LUX_SDA0_L

#define SHT30_SDA0_I    bsp_LUX_SDA0_I   //��������
#define SHT30_SDA0_DAT  bsp_LUX_SDA0_DAT

#define SHT30_SCL0_I    bsp_LUX_SCL0_I    //��������
#define SHT30_SCL0_DAT  bsp_LUX_SCL0_DAT

#if 0
#define SHT30_SCL0_O    GPIOA->CRH &= ~(0x0F<<16); GPIOA->CRH |= (0x03<<16);  GPIOA->CRH |= (0x01<<18)				//GPIOA12 ��©���
#define SHT30_SCL0_H    GPIOA->BSRR |= (0x01<<12)
#define SHT30_SCL0_L    GPIOA->BSRR |= (0x01<<28)

#define SHT30_SDA0_O    GPIOA->CRH &= ~(0x0F<<0); GPIOA->CRH |= (0x03<<0);  GPIOA->CRH |= (0x01<<2)     			//GPIOA8��©���
#define SHT30_SDA0_H    GPIOA->BSRR |=  (0x01<<8)
#define SHT30_SDA0_L    GPIOA->BSRR |=  (0x01<<24)

#define SHT30_SDA0_I    GPIOA->CRH &= ~(0x0F<<0); GPIOA->CRH |= (0x01<<2)      //��������
#define SHT30_SDA0_DAT  ( (GPIOA->IDR>>8) & 0x01)

#define SHT30_SCL0_I    GPIOA->CRH &= ~(0x0F<<16); GPIOA->CRH |= (0x02<<18)      //��������
#define SHT30_SCL0_DAT  ( (GPIOA->IDR>>12) & 0x01)
#endif

#define	SHT30_SlaveAddress	  		(0x44<<1)					//7λ��ַ0x44 ����1λ 0x45 -- 0x8A

#define noACK 0                                 //�����ж��Ƿ����ͨѶ 
#define ACK   1                                 //�������ݴ��� 

//����
#define SHT30_SINGLE_H_MEASURE_EN					0x2C06			//���θ߾��Ȳ���ʹ��
#define SHT30_SINGLE_M_MEASURE_EN					0x2C0D			//������ͨ���Ȳ���ʹ��
#define SHT30_SINGLE_L_MEASURE_EN					0x2C10			//���ε;��Ȳ���ʹ��
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			//���θ߾��Ȳ����ر�
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			//������ͨ���Ȳ����ر�
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			//���ε;��Ȳ����ر�

#define SHT30_PERIODOC_H_MEASURE_500MS		0x2032			//���500Ms�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_500MS		0x2024			//���500Ms�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_500MS		0x202F			//���500Ms�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			//���1s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//���1s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//���1s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//���2s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//���2s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//���2s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//���4s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//���4s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//���4s�;��Ȳ���
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//���10s�߾��Ȳ���
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//���10s�о��Ȳ���
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//���10s�;��Ȳ���

#define SHT30_PERIODOC_MEASURE_READ			0xE000			//�ظ�������ȡ����

#define	SHT30_SOFT_RESET									0x30A2			//��λ

#define SHT30_HEATER_EN										0x306D			//����ʹ��
#define SHT30_HEATER_DIS									0x3066			//���ȹر�

#define SHT30_READ_STATUS									0xF32D			//��״̬�Ĵ���
#define SHT30_CLEAR_STATUS								0x3041			//��״̬�Ĵ���

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001



/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
void SHT30_DelayMs(uint8 k);
void SHT30_Delay1us(uint16 vTemp);
void bsp_SHT30_Init(void);
void SHT30_Start(void);                          //��ʼ�ź�
void SHT30_Stop(void);                           //ֹͣ�ź�
void SHT30_SendACK(uint8 ack);                  //Ӧ��ACK
uint8 SHT30_RecvACK(void);                      //��ack
uint8 SHT30_SendByte(uint8 dat);               //IIC�����ֽ�д
uint8 SHT30_RecvByte(void);                     //IIC�����ֽڶ�
uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen);//CRC8 У��

uint16 SHT30_ReadSerialNumber(void);
uint8 SHT30_Single_Measure(uint8 *vBuf);				//���βɼ�����
uint8 SHT30_Periodic_Measure(uint16 vCmd);			//�������ڲɼ�
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf);	//���ڲɼ���ȡ����
uint8 SHT30_Soft_Reset(void);												//��λ
uint8 SHT30_Heater(uint16 vCmd);								//����������
uint16 SHT30_ReadStaus(void);												//��ȡ״̬�Ĵ���
uint8 SHT30_ClearStaus(void);												//���״̬�Ĵ���
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//����ɼ�����
void bsp_SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//��ȡ��ʪ������

#endif

