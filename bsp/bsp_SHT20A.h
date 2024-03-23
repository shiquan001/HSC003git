#ifndef _SHT20_T_H_
#define _SHT20_T_H_

/*--------------------------ͷ�ļ�����--------------------------------*/
//#include "StdInc.h"
#include "bsp_LUX_V20.h"
#include "bsp_SHT30A_DataType.h"

/*-----------------------------�궨��---------------------------------*/
#define SHT20_SCL0_O    bsp_LUX_SCL0_O			//GPIOA12 ��©���
#define SHT20_SCL0_H    bsp_LUX_SCL0_H
#define SHT20_SCL0_L    bsp_LUX_SCL0_L

#define SHT20_SDA0_O    bsp_LUX_SDA0_O			//GPIOA8��©���
#define SHT20_SDA0_H    bsp_LUX_SDA0_H
#define SHT20_SDA0_L    bsp_LUX_SDA0_L

#define SHT20_SDA0_I    bsp_LUX_SDA0_I   //��������
#define SHT20_SDA0_DAT  bsp_LUX_SDA0_DAT

#define SHT20_SCL0_I    bsp_LUX_SCL0_I    //��������
#define SHT20_SCL0_DAT  bsp_LUX_SCL0_DAT

/*-----------------------------�ṹ�嶨��---------------------------------*/

#define	SHT20_SlaveAddress	  		0x80					//7λ��ַ0x40

#define noACK 0                                 //�����ж��Ƿ����ͨѶ 
#define ACK   1                                 //�������ݴ��� 

//����
#define SHT20_T_MEASURE_HOLD_MASTER								0xE3								//�����¶� ռ�����ߵȴ�
#define SHT20_RH_MEASURE_HOLD_MASTER							0xE5								//����ʪ�� ռ�����ߵȴ�

#define SHT20_T_MEASURE_NO_HOLD_MASTER						0xF3								//�����¶� ��ռ�����ߵȴ�
#define SHT20_RH_MEASURE_NO_HOLD_MASTER					 	0xF5								//����ʪ�� ��ռ�����ߵȴ�
#define SHT20_NO_HOLD_MASTER_FIND									0x81								//��ռ������ ��ѯ�����Ƿ����

#define SHT20_WRITE_USER_REGISTER					 				0xE6								//д�û��Ĵ���
#define SHT20_READ_USER_REGISTER					 				0xE7								//���û��Ĵ���

#define	SHT20_SOFT_RESET													0xFE								//��λ

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001




/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
void SHT20_DelayMs(uint8 k);
void SHT20_Delay1us(uint16 vTemp);
void SHT20_Init(void);
void SHT20_Start(void);                         		//��ʼ�ź�
void SHT20_Stop(void);                          		//ֹͣ�ź�
void SHT20_SendACK(uint8 ack);                  		//Ӧ��ACK
uint8 SHT20_RecvACK(void);                      		//��ack
uint8 SHT20_SendByte(uint8 dat);               			//IIC�����ֽ�д
uint8 SHT20_RecvByte(void);                     		//IIC�����ֽڶ�

uint8 SHT20_CheckCrc8(uint8 *vDat, uint8 vLen);			//CRC8 У��

uint8 SHT20_T_MeasureHoldMaster(uint8 *vBuf);				//�����¶� ռ�����ߵȴ�
uint8 SHT20_RH_MeasureHoldMaster(uint8 *vBuf);			//����ʪ�� ռ�����ߵȴ�

uint8 SHT20_Soft_Reset(void);												//��λ
uint8 SHT20_Heater(uint8 vEnable);									//����������
uint8 SHT20_ReadStaus(uint8 *vp_Status);						//��ȡ״̬�Ĵ���
uint8 SHT20_WriteStaus(uint8 vCfg);									//���״̬�Ĵ���

void SHT20_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//����ɼ�����
uint8 SHT20_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//��ȡ��ʪ������

#endif

