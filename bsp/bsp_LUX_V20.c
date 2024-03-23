//***************************************
// bsp_LUX_V20采集程序
//****************************************
#include "bsp_LUX_V20.h"
#include "bsp.h"

uint8_t    BUF_0[8];                       //接收数据缓存区      	
uint16_t   dis_data_0;                     //变量


/*---------------------------------------------------------------------
 功能描述: 延时纳秒 不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_delay_nms(uint16_t k)	
{
	uint16_t i=0;
	for (i=0; i<k; i++)
	{
		bsp_DelayMS(1);	
	}	
}					

/*---------------------------------------------------------------------
 功能描述: 延时5微秒  不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_Delay5us()
{
	bsp_DelayUS(5);
}

/*---------------------------------------------------------------------
 功能描述: 延时5毫秒  不同的工作环境,需要调整此函数
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_Delay5ms()
{
	bsp_DelayMS(5);
}

/*---------------------------------------------------------------------
 功能描述: 起始信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_Start()
{
  bsp_LUX_SDA0_H;                         //拉高数据线
  bsp_LUX_SCL0_H;                         //拉高时钟线
  bsp_LUX_Delay5us();                     //延时
  bsp_LUX_SDA0_L;                         //产生下降沿
  bsp_LUX_Delay5us();                     //延时
  bsp_LUX_SCL0_L;                         //拉低时钟线
}

/*---------------------------------------------------------------------
 功能描述: 停止信号
 参数说明: 无	
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_Stop()
{
  bsp_LUX_SDA0_L;                         //拉低数据线
  bsp_LUX_SCL0_H;                         //拉高时钟线
  bsp_LUX_Delay5us();                     //延时
  bsp_LUX_SDA0_H;                         //产生上升沿
  bsp_LUX_Delay5us();                     //延时
  bsp_LUX_SCL0_L;
  bsp_LUX_Delay5us();
}

/*---------------------------------------------------------------------
 功能描述: 发送应答信号
 参数说明: ack - 应答信号(0:ACK 1:NAK)
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_LUX_SendACK(uint8_t ack)
{
  if (ack&0x01)	bsp_LUX_SDA0_H;		//写应答信号
  else	bsp_LUX_SDA0_L;
  
  bsp_LUX_SCL0_H;                         //拉高时钟线
  bsp_LUX_Delay5us();                     //延时
  bsp_LUX_SCL0_L;                         //拉低时钟线
  bsp_LUX_SDA0_H;
  bsp_LUX_Delay5us();                     //延时
}

/*---------------------------------------------------------------------
 功能描述: 接收应答信号
 参数说明: 无
 函数返回: 返回应答信号
 ---------------------------------------------------------------------*/
uint8_t bsp_LUX_RecvACK()
{
  uint8_t CY = 0x00;
	uint16 vConter = 1000;  
  bsp_LUX_SDA0_H;
  
  bsp_LUX_SDA0_I;
  
  bsp_LUX_SCL0_H;                         //拉高时钟线
  bsp_LUX_Delay5us();                     //延时
  
  	while (vConter)
	{
		vConter--;
		
		if (bsp_LUX_SDA0_DAT)
			CY = 1;                 		//读应答信号
		
		if(!CY)	break;
	}
  
  bsp_LUX_Delay5us();                     //延时
  
  bsp_LUX_SCL0_L;                         //拉低时钟线
  
  bsp_LUX_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 功能描述: 向IIC总线发送一个字节数据
 参数说明: dat - 写字节
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_SendByte(uint8_t dat)
{
	uint8_t i;
	uint8 vRval = 0x00;
	
  for (i=0; i<8; i++)         			//8位计数器
  {
    if (dat&0x80)	bsp_LUX_SDA0_H;
    else	bsp_LUX_SDA0_L;                   //送数据口
    
    bsp_LUX_Delay5us();             		//延时
    bsp_LUX_SCL0_H;                		//拉高时钟线
    bsp_LUX_Delay5us();             		//延时
    bsp_LUX_SCL0_L;                		//拉低时钟线
    bsp_LUX_Delay5us();             		//延时
    dat <<= 1;              			//移出数据的最高位
  }
  
    vRval = bsp_LUX_RecvACK();
	return vRval;  
}

/*---------------------------------------------------------------------
 功能描述: 从IIC总线接收一个字节数据
 参数说明: 无
 函数返回: 接收字节
 ---------------------------------------------------------------------*/
uint8_t bsp_LUX_RecvByte()
{
  uint8_t i;
  uint8_t dat = 0;
  bsp_LUX_SDA0_I;
  
  bsp_LUX_SDA0_H;                         //使能内部上拉,准备读取数据,
  for (i=0; i<8; i++)         	        //8位计数器
  {
    bsp_LUX_SCL0_H;                       //拉高时钟线
    bsp_LUX_Delay5us();             	//延时

	if (bsp_LUX_SDA0_DAT)
		dat |= 1;              			//读数据 
            
    bsp_LUX_SCL0_L;                       //拉低时钟线
    bsp_LUX_Delay5us();             	//延时
    
    if (i<7) dat <<= 1;
  }
  bsp_LUX_SDA0_O;
  
  return dat;
}

/*---------------------------------------------------------------------
 功能描述: 写BH1750
 参数说明: REG_Address - 寄存器地址
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Single_Write(uint8_t REG_Address)
{
	uint8 vRval = 0;

  bsp_LUX_Start();                                //起始信号
  vRval +=bsp_LUX_SendByte(bsp_LUX_SlaveAddress);           //发送设备地址+写信号
  vRval +=bsp_LUX_SendByte(REG_Address);                  //内部寄存器地址，请参考中文pdf22页 
  //  BH1750_SendByte(REG_data);                //内部寄存器数据，请参考中文pdf22页 
  bsp_LUX_Stop();                                 //发送停止信号

	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 连续读出BH1750内部数据
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Multiple_read(void)
{   
  uint8_t i;
	uint8 vRval = 0;  
  bsp_LUX_Start();                                //起始信号
  vRval +=bsp_LUX_SendByte(bsp_LUX_SlaveAddress+1);         //发送设备地址+读信号
  
  for (i=0; i<3; i++)                           //连续读取6个地址数据，存储中BUF
  {
    BUF_0[i] = bsp_LUX_RecvByte();                //BUF[0]存储0x32地址中的数据
    if (i == 0x02)
    {      
      bsp_LUX_SendACK(1);                         //最后一个数据需要回NOACK
    }
    else
    {		
      bsp_LUX_SendACK(0);                         //回应ACK
    }
  }
  
  bsp_LUX_Stop();                                 //停止信号
//  bsp_LUX_Delay5ms();
	return vRval;  
}

/*
*********************************************************************************************************
*	函 数 名: MX_GPIO_Init_IIC1_SCL_Pin_OD
*	功能说明: 
*	形    参: 
*			  
*	返 回 值: no 
*********************************************************************************************************
*/
void MX_GPIO_Init_IIC1_SCL_Pin_OD(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = IIC1_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC1_SCL_GPIO_Port, &GPIO_InitStruct);

}

/*
*********************************************************************************************************
*	函 数 名: MX_GPIO_Init_IIC1_SCL_Pin_Input
*	功能说明: 
*	形    参: 
*			  
*	返 回 值: no 
*********************************************************************************************************
*/
void MX_GPIO_Init_IIC1_SCL_Pin_Input(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = IIC1_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC1_SCL_GPIO_Port, &GPIO_InitStruct);

}

/*
*********************************************************************************************************
*	函 数 名: MX_GPIO_Init_IIC1_SDA_Pin_OD
*	功能说明: 
*	形    参: 
*			  
*	返 回 值: no 
*********************************************************************************************************
*/
void MX_GPIO_Init_IIC1_SDA_Pin_OD(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = IIC1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC1_SDA_GPIO_Port, &GPIO_InitStruct);

}

/*
*********************************************************************************************************
*	函 数 名: MX_GPIO_Init_IIC1_SDA_Pin_OD
*	功能说明: 
*	形    参: 
*			  
*	返 回 值: no 
*********************************************************************************************************
*/
void MX_GPIO_Init_IIC1_SDA_Pin_Input(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */

  __HAL_RCC_GPIOB_CLK_ENABLE();


  /*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = IIC1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(IIC1_SDA_GPIO_Port, &GPIO_InitStruct);

}

/*---------------------------------------------------------------------
 功能描述: 初始化光照传感器
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Init()
{
	uint8 vRval = 0 ;

	bsp_LUX_SCL0_O;
	bsp_LUX_SDA0_O;

	bsp_LUX_delay_nms(100);	                        //延时100ms

	vRval += bsp_LUX_Single_Write(0x01); 
	return vRval;
}

/*---------------------------------------------------------------------
 功能描述: 光照读取函数
 参数说明: 无
 函数返回: 返回光照值
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_GetLux(uint32_t *vLux)
{  
	uint8 vRval = 0;

	float temp;
	vRval += bsp_LUX_Single_Write(0x01);                     // power on
	vRval += bsp_LUX_Single_Write(0x10);                     // H- resolution mode 

	bsp_LUX_delay_nms(180);                         //延时180ms

	vRval += bsp_LUX_Multiple_read();                        //连续读出数据，存储在BUF中

	vRval +=  bsp_LUX_Single_Write(0x00);                     // power off

	dis_data_0=BUF_0[0];
	dis_data_0=(dis_data_0<<8)+BUF_0[1];          //合成数据，即光照数据

	temp=(float)((float)dis_data_0/1.2f);
	*vLux = (uint32_t)(temp*1.4f);
	return vRval;  
} 

