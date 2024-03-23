#ifndef _SHT30_T_H_
#define _SHT30_T_H_

/*--------------------------头文件引用--------------------------------*/
#include "bsp_SHT30A_DataType.h"

#include "bsp_LUX_V20.h"

/*-----------------------------结构体定义---------------------------------*/
#define SHT10_SCL_O    bsp_LUX_SCL0_O
#define SHT10_SCL_H    bsp_LUX_SCL0_H
#define SHT10_SCL_L    bsp_LUX_SCL0_L

#define SHT10_SDA_O    bsp_LUX_SDA0_O
#define SHT10_SDA_H    bsp_LUX_SDA0_H
#define SHT10_SDA_L    bsp_LUX_SDA0_L

#define SHT10_SDA_I    bsp_LUX_SDA0_I
#define SHT10_SDA_DAT  bsp_LUX_SDA0_DAT

/*-----------------------------宏定义---------------------------------*/
#define SHT30_SCL0_O    bsp_LUX_SCL0_O			//GPIOA12 开漏输出
#define SHT30_SCL0_H    bsp_LUX_SCL0_H
#define SHT30_SCL0_L    bsp_LUX_SCL0_L

#define SHT30_SDA0_O    bsp_LUX_SDA0_O			//GPIOA8开漏输出
#define SHT30_SDA0_H    bsp_LUX_SDA0_H
#define SHT30_SDA0_L    bsp_LUX_SDA0_L

#define SHT30_SDA0_I    bsp_LUX_SDA0_I   //浮空输入
#define SHT30_SDA0_DAT  bsp_LUX_SDA0_DAT

#define SHT30_SCL0_I    bsp_LUX_SCL0_I    //浮空输入
#define SHT30_SCL0_DAT  bsp_LUX_SCL0_DAT

#if 0
#define SHT30_SCL0_O    GPIOA->CRH &= ~(0x0F<<16); GPIOA->CRH |= (0x03<<16);  GPIOA->CRH |= (0x01<<18)				//GPIOA12 开漏输出
#define SHT30_SCL0_H    GPIOA->BSRR |= (0x01<<12)
#define SHT30_SCL0_L    GPIOA->BSRR |= (0x01<<28)

#define SHT30_SDA0_O    GPIOA->CRH &= ~(0x0F<<0); GPIOA->CRH |= (0x03<<0);  GPIOA->CRH |= (0x01<<2)     			//GPIOA8开漏输出
#define SHT30_SDA0_H    GPIOA->BSRR |=  (0x01<<8)
#define SHT30_SDA0_L    GPIOA->BSRR |=  (0x01<<24)

#define SHT30_SDA0_I    GPIOA->CRH &= ~(0x0F<<0); GPIOA->CRH |= (0x01<<2)      //浮空输入
#define SHT30_SDA0_DAT  ( (GPIOA->IDR>>8) & 0x01)

#define SHT30_SCL0_I    GPIOA->CRH &= ~(0x0F<<16); GPIOA->CRH |= (0x02<<18)      //浮空输入
#define SHT30_SCL0_DAT  ( (GPIOA->IDR>>12) & 0x01)
#endif

#define	SHT30_SlaveAddress	  		(0x44<<1)					//7位地址0x44 左移1位 0x45 -- 0x8A

#define noACK 0                                 //用于判断是否结束通讯 
#define ACK   1                                 //结束数据传输 

//命令
#define SHT30_SINGLE_H_MEASURE_EN					0x2C06			//单次高精度测量使能
#define SHT30_SINGLE_M_MEASURE_EN					0x2C0D			//单次普通精度测量使能
#define SHT30_SINGLE_L_MEASURE_EN					0x2C10			//单次低精度测量使能
#define SHT30_SINGLE_H_MEASURE_DIS				0x2400			//单次高精度测量关闭
#define SHT30_SINGLE_M_MEASURE_DIS				0x240B			//单次普通精度测量关闭
#define SHT30_SINGLE_L_MEASURE_DIS				0x2416			//单次低精度测量关闭

#define SHT30_PERIODOC_H_MEASURE_500MS		0x2032			//间隔500Ms高精度测量
#define SHT30_PERIODOC_M_MEASURE_500MS		0x2024			//间隔500Ms中精度测量
#define SHT30_PERIODOC_L_MEASURE_500MS		0x202F			//间隔500Ms低精度测量
#define SHT30_PERIODOC_H_MEASURE_1S				0x2130			//间隔1s高精度测量
#define SHT30_PERIODOC_M_MEASURE_1S				0x2126			//间隔1s中精度测量
#define SHT30_PERIODOC_L_MEASURE_1S				0x212D			//间隔1s低精度测量
#define SHT30_PERIODOC_H_MEASURE_2S				0x2236			//间隔2s高精度测量
#define SHT30_PERIODOC_M_MEASURE_2S				0x2220			//间隔2s中精度测量
#define SHT30_PERIODOC_L_MEASURE_2S				0x222B			//间隔2s低精度测量
#define SHT30_PERIODOC_H_MEASURE_4S				0x2334			//间隔4s高精度测量
#define SHT30_PERIODOC_M_MEASURE_4S				0x2322			//间隔4s中精度测量
#define SHT30_PERIODOC_L_MEASURE_4S				0x2329			//间隔4s低精度测量
#define SHT30_PERIODOC_H_MEASURE_10S			0x2737			//间隔10s高精度测量
#define SHT30_PERIODOC_M_MEASURE_10S			0x2721			//间隔10s中精度测量
#define SHT30_PERIODOC_L_MEASURE_10S			0x272A			//间隔10s低精度测量

#define SHT30_PERIODOC_MEASURE_READ			0xE000			//重复测量读取数据

#define	SHT30_SOFT_RESET									0x30A2			//软复位

#define SHT30_HEATER_EN										0x306D			//加热使能
#define SHT30_HEATER_DIS									0x3066			//加热关闭

#define SHT30_READ_STATUS									0xF32D			//读状态寄存器
#define SHT30_CLEAR_STATUS								0x3041			//清状态寄存器

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001



/*---------------------------外部变量声明-----------------------------*/

/*-----------------------------函数声明-------------------------------*/
void SHT30_DelayMs(uint8 k);
void SHT30_Delay1us(uint16 vTemp);
void bsp_SHT30_Init(void);
void SHT30_Start(void);                          //起始信号
void SHT30_Stop(void);                           //停止信号
void SHT30_SendACK(uint8 ack);                  //应答ACK
uint8 SHT30_RecvACK(void);                      //读ack
uint8 SHT30_SendByte(uint8 dat);               //IIC单个字节写
uint8 SHT30_RecvByte(void);                     //IIC单个字节读
uint8 SHT30_CheckCrc8(uint8 *vDat, uint8 vLen);//CRC8 校验

uint16 SHT30_ReadSerialNumber(void);
uint8 SHT30_Single_Measure(uint8 *vBuf);				//单次采集数据
uint8 SHT30_Periodic_Measure(uint16 vCmd);			//设置周期采集
uint8 SHT30_Periodic_Measure_Read(uint8 *vBuf);	//周期采集读取数据
uint8 SHT30_Soft_Reset(void);												//软复位
uint8 SHT30_Heater(uint16 vCmd);								//传感器加热
uint16 SHT30_ReadStaus(void);												//读取状态寄存器
uint8 SHT30_ClearStaus(void);												//清楚状态寄存器
void SHT30_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//计算采集数据
void bsp_SHT30_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//获取温湿度数据

#endif

