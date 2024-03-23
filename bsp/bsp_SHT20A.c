/*
参照代码由 "淮安市蓝控电子科技有限公司"提供，联系QQ:1746950285

传感器移植说明:  代码以IO模拟I2C实现传感器采集，需要移植文件 传感器型号对应的.h文件和.c两个文件、
DataType.h(可选) 
			.h文件(如：B_LUX_V20.h等)：包含IO脚的宏定义 和 函数声明, 需要修改IO的宏定义，对应用户使用的IO
			.c文件(如: B_LUX_V20.c等)：包含I2C模拟代码和传感器采集函数, 可能需要修改地方,两个延时函数的修过
			DataType.h							 ：类型定义 (可选)
*/
#include "bsp_SHT20A.h"
#include "bsp.h"


vid SHT20_DelayMs(uint8 k)
{
	bsp_DelayMS(k);
}

vid SHT20_Delay1us(uint16 vTemp)
{
	bsp_DelayUS(vTemp);
}

/*-------------------------------------------------------
功能描述：函数初始化
参    数：无
返 回 值：无
---------------------------------------------------------*/
vid SHT20_Init(vid)
{
	//RCC->APB2ENR |= (0x01<<3);							//打开GPIOB外设时钟
  
  SHT20_SCL0_O;                          	//设置SCLK为输出
  SHT20_SDA0_O;                          	//设置SDA为输出
	
	SHT20_SCL0_H;
	SHT20_SCL0_L;
	
	SHT20_SDA0_H;
	SHT20_SDA0_L;
	
	SHT20_Soft_Reset();
	
	bsp_DelayMS(1);
}


/*---------------------------------------------------------------------
 功能描述: 起始信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid SHT20_Start()
{
  SHT20_SDA0_H;                         //拉高数据线
	SHT20_Delay1us(1);
  SHT20_SCL0_H;                         //拉高时钟线
  SHT20_Delay1us(1);                     //延时
  SHT20_SDA0_L;                         //产生下降沿
  SHT20_Delay1us(10);                     //延时
  SHT20_SCL0_L;                         //拉低时钟线
	SHT20_Delay1us(10);
}

/*---------------------------------------------------------------------
 功能描述: 停止信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
vid SHT20_Stop()
{
  SHT20_SCL0_L;                         //拉低时钟线
	SHT20_Delay1us(1);
	SHT20_SDA0_L;                         //拉低数据线
  SHT20_Delay1us(1);                     //延时
  SHT20_SCL0_H;
  SHT20_Delay1us(10);                     //延时
	SHT20_SDA0_H;                         //产生上升沿
  SHT20_Delay1us(10);
}

/*---------------------------------------------------------------------
 功能描述: 发送应答信号
 参数说明: ack [in] - 应答信号(0:ACK 1:NAK)
 函数返回: 无
 ---------------------------------------------------------------------*/
vid SHT20_SendACK(uint8 ack)
{
  if (ack&0x01)	SHT20_SDA0_H;		//写应答信号
  else	SHT20_SDA0_L;
  
	SHT20_Delay1us(1);
  SHT20_SCL0_H;                         //拉高时钟线
  SHT20_Delay1us(5);                     //延时
  SHT20_SCL0_L;                         //拉低时钟线
  SHT20_SDA0_H;
  SHT20_Delay1us(20);                     //延时
}

/*---------------------------------------------------------------------
 功能描述: 接收应答信号
 参数说明: 无
 函数返回: 返回应答信号
 ---------------------------------------------------------------------*/
uint8 SHT20_RecvACK()
{
  uint8 CY = 0x00;
	uint16 vConter = 1000;
  SHT20_SDA0_H;
  
  SHT20_SDA0_I;
  
  SHT20_SCL0_H;                         //拉高时钟线
  SHT20_Delay1us(1);                     //延时
	while (vConter)
	{
		vConter--;
		CY |= SHT20_SDA0_DAT;                 //读应答信号
		
		if(!CY)	break;
	}
  SHT20_Delay1us(20);                     //延时
	SHT20_SCL0_L;
  SHT20_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 功能描述: 向IIC总线发送一个字节数据
 参数说明: dat [in] - 写字节
 函数返回: 0 - 成功   1- 出错
 ---------------------------------------------------------------------*/
uint8 SHT20_SendByte(uint8 dat)
{
	uint8 vRval = 0x01;
  uint8 i;
  
  for (i=0; i<8; i++)         			//8位计数器
  {
    if (dat&0x80)	SHT20_SDA0_H;
    else	SHT20_SDA0_L;                   //送数据口
    
    SHT20_Delay1us(4);             		//延时
    SHT20_SCL0_H;                		//拉高时钟线
    SHT20_Delay1us(5);             		//延时
    SHT20_SCL0_L;                		//拉低时钟线
    SHT20_Delay1us(1);             		//延时
    dat <<= 1;              			//移出数据的最高位
  }
  
	vRval = SHT20_RecvACK();
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 从IIC总线接收一个字节数据
 参数说明: 无
 函数返回: 接收字节
 ---------------------------------------------------------------------*/
uint8 SHT20_RecvByte()
{
  uint8 i;
  uint8 dat = 0;

  SHT20_SDA0_H;                         //使能内部上拉,准备读取数据,
	
	SHT20_SDA0_I;
	for (i=0; i<8; i++)         	        //8位计数器
  {
    SHT20_SCL0_H;                       //拉高时钟线
		SHT20_Delay1us(5);
		dat |= SHT20_SDA0_DAT;              //读数据             
    SHT20_SCL0_L;                       //拉低时钟线
    SHT20_Delay1us(5);             			//延时
    
    if (i<7) dat <<= 1;	
  }
  SHT20_SDA0_O;
  
  return dat;
}


/*---------------------------------------------------------------------
 功能描述: 测量温度 占用总线等待
 参数说明: vBuf [out] - 测量读取结果
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_T_MeasureHoldMaster(uint8 *vBuf)
{
	uint8 vRval = 0;
	uint16 vTimeOut = 1000;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//地址写
	if (!vRval) vRval |= SHT20_SendByte(SHT20_T_MEASURE_HOLD_MASTER);
	
	SHT20_Start();
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//地址度
	
	SHT20_Stop();
	SHT20_SCL0_I;
	SHT20_DelayMs(5);
	
	while(!SHT20_SCL0_DAT)
	{
		SHT20_DelayMs(1);
		
		vTimeOut--;
		if (vTimeOut == 0)	break;				//等待超时
	}
	SHT20_SCL0_O;
	
	SHT20_DelayMs(1);
	
	vBuf[0] = SHT20_RecvByte();						//高位
	SHT20_SendACK(0);											//ACK
	vBuf[1] = SHT20_RecvByte();						//低位
	SHT20_SendACK(0);											//ACK
	vBuf[2] = SHT20_RecvByte();						//校验
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 测量湿度 占用总线等待
 参数说明: vBuf [out] - 测量读取结果
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_RH_MeasureHoldMaster(uint8 *vBuf)
{
		uint8 vRval = 0;
	uint16 vTimeOut = 1000;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//地址写
	if (!vRval) vRval |= SHT20_SendByte(SHT20_RH_MEASURE_HOLD_MASTER);
	
	SHT20_Start();
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//地址度
	
	SHT20_Stop();
	SHT20_SCL0_I;
	SHT20_DelayMs(5);
	
	while(!SHT20_SCL0_DAT)
	{
		SHT20_DelayMs(1);
		
		vTimeOut--;
		if (vTimeOut == 0)	break;				//等待超时
	}
	SHT20_SCL0_O;
	
	SHT20_DelayMs(1);
	
	vBuf[0] = SHT20_RecvByte();						//高位
	SHT20_SendACK(0);											//ACK
	vBuf[1] = SHT20_RecvByte();						//低位
	SHT20_SendACK(0);											//ACK
	vBuf[2] = SHT20_RecvByte();						//校验
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}



/*---------------------------------------------------------------------
 功能描述: 读取状态寄存器
 参数说明: vp_Status [out] - 读取结果
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_ReadStaus(uint8 *vp_Status)
{
	uint8 vRval = 0;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//地址写
	if (!vRval) vRval |= SHT20_SendByte(SHT20_READ_USER_REGISTER);
	
	SHT20_DelayMs(1);											//等待1ms
	
	SHT20_Stop();
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//地址度
	*vp_Status = SHT20_RecvByte();						//高位
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 清楚状态寄存器
 参数说明: vCfg - 配置参数
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_WriteStaus(uint8 vCfg)
{
	/*-----  配置参数说明
	0位 和 7位 组合 [0:1] 配置测量数据的位数  00: RH 12bit  T 14bit			默认00
																						01:	RH 8bit   T 12bit
																						10: RH 10bit  T 13bit
																						11: RH 11bit  T 11bit
	6位 电池模式 0: VDD>2.25  1: VDD<2.25    默认0
	3,4,5  保留
	2位  加热开关 0-关闭 1打开
	1位  
	------*/
	
	uint8 vRval = 0;
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//地址写
	if (!vRval) vRval |= SHT20_SendByte(SHT20_WRITE_USER_REGISTER);
	if (!vRval) vRval |= SHT20_SendByte(vCfg);
	
	SHT20_Stop();
	
	return vRval;
}


/*---------------------------------------------------------------------
 功能描述: SHT20传感器软复位
 参数说明: 无
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_Soft_Reset()
{
	uint8 vRval = 0;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//地址写
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SOFT_RESET);
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: SHT20加热命令
 参数说明: vEnable - 加热使能开关 0-关闭 1-打开
 函数返回: 0 - 成功  大于1出错
 ---------------------------------------------------------------------*/
uint8 SHT20_Heater(uint8 vEnable)
{
	uint8 vRval = 0;
	uint8 vCfg = 0;
	
	if (!vRval) vRval |= SHT20_ReadStaus(&vCfg);
	
	if (vEnable)							//打开
	{
		vCfg |= (0x01<<2);
	}
	else											//关闭
	{
		vCfg &= ~(0x01<<2);
	}
	
	if (!vRval) vRval |= SHT20_WriteStaus(vCfg);
	
	return vRval;
}

uint8 SHT20_CheckCrc8(uint8 *vDat, uint8 vLen)  
{
  uint8 vCrc = 0;	
  uint8 vByteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (vByteCtr = 0; vByteCtr < vLen; ++vByteCtr)
  { vCrc ^= (vDat[vByteCtr]);
    for (uint8 vBit = 8; vBit > 0; --vBit)
    { if (vCrc & 0x80) vCrc = (vCrc << 1) ^ POLYNOMIAL;
      else vCrc = (vCrc << 1);
    }
  }
	return vCrc;
}
 

/*---------------------------------------------------------------------
 功能描述: SHT20 测量结果计算
 参数说明:  vBuf [in] - 测量读取结果
						vTemSymbol [out] - 返回温度符号
						vTem [out] - 温度
						vHum [out] - 湿度

 函数返回: 无
 ---------------------------------------------------------------------*/
void SHT20_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
	uint16 	vVal = 0x00;
	uint8 	vCrc = 0x00;
	float		vTemp = 0.00;
	
	//温度
	vCrc = SHT20_CheckCrc8(&vBuf[0], 2);
	if (vCrc == vBuf[2])
	{
		vVal = vBuf[0];
		vVal<<=8;
		vVal |= vBuf[1];
		
		vVal &= ~0x0003;											//有效数据
		
		vTemp = 175.72*(vVal/65536.0);
		
		if (vTemp >= 46.85)
		{
			*vTemSymbol = 1;
			*vTem = (vTemp - 46.85)*10.0;
		}
		else
		{
			*vTemSymbol = 0;
			*vTem = (46.85 - vTemp)*10.0;
		}
		
	}
	
	vVal = 0x00;
	vCrc = SHT20_CheckCrc8(&vBuf[3], 2);
	if (vCrc == vBuf[5])
	{
		vVal = vBuf[3];
		vVal<<=8;
		vVal |= vBuf[4];
		
		vVal &= ~0x000F; 											//有效数据
		
		vTemp = (125.0/65536.0*vVal) - 6;
		*vHum = (vTemp*10);
	}
}

/*---------------------------------------------------------------------
 功能描述: SHT20 测量结果计算
 参数说明:  vTemSymbol [out] - 返回温度符号
						vTem [out] - 温度
						vHum [out] - 湿度

 函数返回: 无
 ---------------------------------------------------------------------*/
	uint8 vDat[8];
uint8 SHT20_Get_TH(uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{

	uint8 vRval = 0;
	
	if (!vRval) vRval |= SHT20_T_MeasureHoldMaster(&vDat[0]);
	if (!vRval) vRval |= SHT20_RH_MeasureHoldMaster(&vDat[3]);
	if (!vRval) SHT20_calc(vDat, vTemSymbol, vTem, vHum);
	
	return vRval;
}





