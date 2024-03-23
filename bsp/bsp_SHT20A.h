#ifndef _SHT20_T_H_
#define _SHT20_T_H_

/*--------------------------头文件引用--------------------------------*/
//#include "StdInc.h"
#include "bsp_LUX_V20.h"
#include "bsp_SHT30A_DataType.h"

/*-----------------------------宏定义---------------------------------*/
#define SHT20_SCL0_O    bsp_LUX_SCL0_O			//GPIOA12 开漏输出
#define SHT20_SCL0_H    bsp_LUX_SCL0_H
#define SHT20_SCL0_L    bsp_LUX_SCL0_L

#define SHT20_SDA0_O    bsp_LUX_SDA0_O			//GPIOA8开漏输出
#define SHT20_SDA0_H    bsp_LUX_SDA0_H
#define SHT20_SDA0_L    bsp_LUX_SDA0_L

#define SHT20_SDA0_I    bsp_LUX_SDA0_I   //浮空输入
#define SHT20_SDA0_DAT  bsp_LUX_SDA0_DAT

#define SHT20_SCL0_I    bsp_LUX_SCL0_I    //浮空输入
#define SHT20_SCL0_DAT  bsp_LUX_SCL0_DAT

/*-----------------------------结构体定义---------------------------------*/

#define	SHT20_SlaveAddress	  		0x80					//7位地址0x40

#define noACK 0                                 //用于判断是否结束通讯 
#define ACK   1                                 //结束数据传输 

//命令
#define SHT20_T_MEASURE_HOLD_MASTER								0xE3								//测量温度 占用总线等待
#define SHT20_RH_MEASURE_HOLD_MASTER							0xE5								//测量湿度 占用总线等待

#define SHT20_T_MEASURE_NO_HOLD_MASTER						0xF3								//测量温度 不占用总线等待
#define SHT20_RH_MEASURE_NO_HOLD_MASTER					 	0xF5								//测量湿度 不占用总线等待
#define SHT20_NO_HOLD_MASTER_FIND									0x81								//不占用总线 轮询测量是否接收

#define SHT20_WRITE_USER_REGISTER					 				0xE6								//写用户寄存器
#define SHT20_READ_USER_REGISTER					 				0xE7								//读用户寄存器

#define	SHT20_SOFT_RESET													0xFE								//软复位

// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001




/*---------------------------外部变量声明-----------------------------*/

/*-----------------------------函数声明-------------------------------*/
void SHT20_DelayMs(uint8 k);
void SHT20_Delay1us(uint16 vTemp);
void SHT20_Init(void);
void SHT20_Start(void);                         		//起始信号
void SHT20_Stop(void);                          		//停止信号
void SHT20_SendACK(uint8 ack);                  		//应答ACK
uint8 SHT20_RecvACK(void);                      		//读ack
uint8 SHT20_SendByte(uint8 dat);               			//IIC单个字节写
uint8 SHT20_RecvByte(void);                     		//IIC单个字节读

uint8 SHT20_CheckCrc8(uint8 *vDat, uint8 vLen);			//CRC8 校验

uint8 SHT20_T_MeasureHoldMaster(uint8 *vBuf);				//测量温度 占用总线等待
uint8 SHT20_RH_MeasureHoldMaster(uint8 *vBuf);			//测量湿度 占用总线等待

uint8 SHT20_Soft_Reset(void);												//软复位
uint8 SHT20_Heater(uint8 vEnable);									//传感器加热
uint8 SHT20_ReadStaus(uint8 *vp_Status);						//读取状态寄存器
uint8 SHT20_WriteStaus(uint8 vCfg);									//清楚状态寄存器

void SHT20_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);	//计算采集数据
uint8 SHT20_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum);							//获取温湿度数据

#endif

