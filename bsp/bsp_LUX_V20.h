#ifndef _BSP_B_LUX_V20_H
#define _BSP_B_LUX_V20_H

/*--------------------------ͷ�ļ�����--------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

void MX_GPIO_Init_IIC1_SCL_Pin_OD(void);
void MX_GPIO_Init_IIC1_SDA_Pin_OD(void);
void MX_GPIO_Init_IIC1_SDA_Pin_Input(void);
void MX_GPIO_Init_IIC1_SCL_Pin_Input(void);

/*-----------------------------�ṹ�嶨��---------------------------------*/
//IIC1_SCL	PB13
//IIC1_SDA	PB14


/*-----------------------------�궨��---------------------------------*/
//���Ŷ���
#if 0
#define IIC1_SCL_GPIO_Port 	GPIOB
#define IIC1_SCL_Pin		GPIO_PIN_13

#define IIC1_SDA_GPIO_Port 	GPIOB
#define IIC1_SDA_Pin		GPIO_PIN_14
#endif
#define bsp_LUX_SCL0_O    MX_GPIO_Init_IIC1_SCL_Pin_OD()  // ��©���
#define bsp_LUX_SCL0_H    HAL_GPIO_WritePin(IIC1_SCL_GPIO_Port, IIC1_SCL_Pin, GPIO_PIN_SET) 
#define bsp_LUX_SCL0_L    HAL_GPIO_WritePin(IIC1_SCL_GPIO_Port, IIC1_SCL_Pin, GPIO_PIN_RESET) 

#define bsp_LUX_SDA0_O    MX_GPIO_Init_IIC1_SDA_Pin_OD()// ��©���
#define bsp_LUX_SDA0_H    HAL_GPIO_WritePin(IIC1_SDA_GPIO_Port, IIC1_SDA_Pin, GPIO_PIN_SET) 
#define bsp_LUX_SDA0_L    HAL_GPIO_WritePin(IIC1_SDA_GPIO_Port, IIC1_SDA_Pin, GPIO_PIN_RESET) 

#define bsp_LUX_SDA0_I    MX_GPIO_Init_IIC1_SDA_Pin_Input()//��������
#define bsp_LUX_SDA0_DAT  HAL_GPIO_ReadPin(IIC1_SDA_GPIO_Port,IIC1_SDA_Pin)//( (IIC1_SDA_GPIO_Port->IDR& IIC1_SDA_Pin)>>14)

#define bsp_LUX_SCL0_I    MX_GPIO_Init_IIC1_SCL_Pin_Input()//��������
#define bsp_LUX_SCL0_DAT  HAL_GPIO_ReadPin(IIC1_SCL_GPIO_Port,IIC1_SCL_Pin)//( (IIC1_SCL_GPIO_Port->IDR& IIC1_SCL_Pin)>>13)



#define	bsp_LUX_SlaveAddress	  0x46                                          //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

/*-----------------------------��������-------------------------------*/
void bsp_LUX_delay_nms(uint16_t k);
uint8_t bsp_LUX_Init(void);

uint8_t  bsp_LUX_Single_Write(uint8_t REG_Address);                    //����д������
uint8_t bsp_LUX_Single_Read(uint8_t REG_Address);                     //������ȡ�ڲ��Ĵ�������
uint8_t  bsp_LUX_Multiple_read(void);                               //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
void bsp_LUX_Delay5us(void);
void bsp_LUX_Delay5ms(void);
void bsp_LUX_Start(void);                          //��ʼ�ź�
void bsp_LUX_Stop(void);                           //ֹͣ�ź�
void bsp_LUX_SendACK(uint8_t ack);                  //Ӧ��ACK
uint8_t  bsp_LUX_RecvACK(void);                     //��ack
uint8_t bsp_LUX_SendByte(uint8_t dat);                 //IIC�����ֽ�д
uint8_t bsp_LUX_RecvByte(void);                     //IIC�����ֽڶ�

uint8_t bsp_LUX_GetLux(uint32_t *vLux);
#endif
