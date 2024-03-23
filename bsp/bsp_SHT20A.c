/*
���մ����� "���������ص��ӿƼ����޹�˾"�ṩ����ϵQQ:1746950285

��������ֲ˵��:  ������IOģ��I2Cʵ�ִ������ɼ�����Ҫ��ֲ�ļ� �������ͺŶ�Ӧ��.h�ļ���.c�����ļ���
DataType.h(��ѡ) 
			.h�ļ�(�磺B_LUX_V20.h��)������IO�ŵĺ궨�� �� ��������, ��Ҫ�޸�IO�ĺ궨�壬��Ӧ�û�ʹ�õ�IO
			.c�ļ�(��: B_LUX_V20.c��)������I2Cģ�����ʹ������ɼ�����, ������Ҫ�޸ĵط�,������ʱ�������޹�
			DataType.h							 �����Ͷ��� (��ѡ)
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
����������������ʼ��
��    ������
�� �� ֵ����
---------------------------------------------------------*/
vid SHT20_Init(vid)
{
	//RCC->APB2ENR |= (0x01<<3);							//��GPIOB����ʱ��
  
  SHT20_SCL0_O;                          	//����SCLKΪ���
  SHT20_SDA0_O;                          	//����SDAΪ���
	
	SHT20_SCL0_H;
	SHT20_SCL0_L;
	
	SHT20_SDA0_H;
	SHT20_SDA0_L;
	
	SHT20_Soft_Reset();
	
	bsp_DelayMS(1);
}


/*---------------------------------------------------------------------
 ��������: ��ʼ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
vid SHT20_Start()
{
  SHT20_SDA0_H;                         //����������
	SHT20_Delay1us(1);
  SHT20_SCL0_H;                         //����ʱ����
  SHT20_Delay1us(1);                     //��ʱ
  SHT20_SDA0_L;                         //�����½���
  SHT20_Delay1us(10);                     //��ʱ
  SHT20_SCL0_L;                         //����ʱ����
	SHT20_Delay1us(10);
}

/*---------------------------------------------------------------------
 ��������: ֹͣ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
vid SHT20_Stop()
{
  SHT20_SCL0_L;                         //����ʱ����
	SHT20_Delay1us(1);
	SHT20_SDA0_L;                         //����������
  SHT20_Delay1us(1);                     //��ʱ
  SHT20_SCL0_H;
  SHT20_Delay1us(10);                     //��ʱ
	SHT20_SDA0_H;                         //����������
  SHT20_Delay1us(10);
}

/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ack [in] - Ӧ���ź�(0:ACK 1:NAK)
 ��������: ��
 ---------------------------------------------------------------------*/
vid SHT20_SendACK(uint8 ack)
{
  if (ack&0x01)	SHT20_SDA0_H;		//дӦ���ź�
  else	SHT20_SDA0_L;
  
	SHT20_Delay1us(1);
  SHT20_SCL0_H;                         //����ʱ����
  SHT20_Delay1us(5);                     //��ʱ
  SHT20_SCL0_L;                         //����ʱ����
  SHT20_SDA0_H;
  SHT20_Delay1us(20);                     //��ʱ
}

/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ��
 ��������: ����Ӧ���ź�
 ---------------------------------------------------------------------*/
uint8 SHT20_RecvACK()
{
  uint8 CY = 0x00;
	uint16 vConter = 1000;
  SHT20_SDA0_H;
  
  SHT20_SDA0_I;
  
  SHT20_SCL0_H;                         //����ʱ����
  SHT20_Delay1us(1);                     //��ʱ
	while (vConter)
	{
		vConter--;
		CY |= SHT20_SDA0_DAT;                 //��Ӧ���ź�
		
		if(!CY)	break;
	}
  SHT20_Delay1us(20);                     //��ʱ
	SHT20_SCL0_L;
  SHT20_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߷���һ���ֽ�����
 ����˵��: dat [in] - д�ֽ�
 ��������: 0 - �ɹ�   1- ����
 ---------------------------------------------------------------------*/
uint8 SHT20_SendByte(uint8 dat)
{
	uint8 vRval = 0x01;
  uint8 i;
  
  for (i=0; i<8; i++)         			//8λ������
  {
    if (dat&0x80)	SHT20_SDA0_H;
    else	SHT20_SDA0_L;                   //�����ݿ�
    
    SHT20_Delay1us(4);             		//��ʱ
    SHT20_SCL0_H;                		//����ʱ����
    SHT20_Delay1us(5);             		//��ʱ
    SHT20_SCL0_L;                		//����ʱ����
    SHT20_Delay1us(1);             		//��ʱ
    dat <<= 1;              			//�Ƴ����ݵ����λ
  }
  
	vRval = SHT20_RecvACK();
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߽���һ���ֽ�����
 ����˵��: ��
 ��������: �����ֽ�
 ---------------------------------------------------------------------*/
uint8 SHT20_RecvByte()
{
  uint8 i;
  uint8 dat = 0;

  SHT20_SDA0_H;                         //ʹ���ڲ�����,׼����ȡ����,
	
	SHT20_SDA0_I;
	for (i=0; i<8; i++)         	        //8λ������
  {
    SHT20_SCL0_H;                       //����ʱ����
		SHT20_Delay1us(5);
		dat |= SHT20_SDA0_DAT;              //������             
    SHT20_SCL0_L;                       //����ʱ����
    SHT20_Delay1us(5);             			//��ʱ
    
    if (i<7) dat <<= 1;	
  }
  SHT20_SDA0_O;
  
  return dat;
}


/*---------------------------------------------------------------------
 ��������: �����¶� ռ�����ߵȴ�
 ����˵��: vBuf [out] - ������ȡ���
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_T_MeasureHoldMaster(uint8 *vBuf)
{
	uint8 vRval = 0;
	uint16 vTimeOut = 1000;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//��ַд
	if (!vRval) vRval |= SHT20_SendByte(SHT20_T_MEASURE_HOLD_MASTER);
	
	SHT20_Start();
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//��ַ��
	
	SHT20_Stop();
	SHT20_SCL0_I;
	SHT20_DelayMs(5);
	
	while(!SHT20_SCL0_DAT)
	{
		SHT20_DelayMs(1);
		
		vTimeOut--;
		if (vTimeOut == 0)	break;				//�ȴ���ʱ
	}
	SHT20_SCL0_O;
	
	SHT20_DelayMs(1);
	
	vBuf[0] = SHT20_RecvByte();						//��λ
	SHT20_SendACK(0);											//ACK
	vBuf[1] = SHT20_RecvByte();						//��λ
	SHT20_SendACK(0);											//ACK
	vBuf[2] = SHT20_RecvByte();						//У��
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ����ʪ�� ռ�����ߵȴ�
 ����˵��: vBuf [out] - ������ȡ���
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_RH_MeasureHoldMaster(uint8 *vBuf)
{
		uint8 vRval = 0;
	uint16 vTimeOut = 1000;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//��ַд
	if (!vRval) vRval |= SHT20_SendByte(SHT20_RH_MEASURE_HOLD_MASTER);
	
	SHT20_Start();
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//��ַ��
	
	SHT20_Stop();
	SHT20_SCL0_I;
	SHT20_DelayMs(5);
	
	while(!SHT20_SCL0_DAT)
	{
		SHT20_DelayMs(1);
		
		vTimeOut--;
		if (vTimeOut == 0)	break;				//�ȴ���ʱ
	}
	SHT20_SCL0_O;
	
	SHT20_DelayMs(1);
	
	vBuf[0] = SHT20_RecvByte();						//��λ
	SHT20_SendACK(0);											//ACK
	vBuf[1] = SHT20_RecvByte();						//��λ
	SHT20_SendACK(0);											//ACK
	vBuf[2] = SHT20_RecvByte();						//У��
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}



/*---------------------------------------------------------------------
 ��������: ��ȡ״̬�Ĵ���
 ����˵��: vp_Status [out] - ��ȡ���
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_ReadStaus(uint8 *vp_Status)
{
	uint8 vRval = 0;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//��ַд
	if (!vRval) vRval |= SHT20_SendByte(SHT20_READ_USER_REGISTER);
	
	SHT20_DelayMs(1);											//�ȴ�1ms
	
	SHT20_Stop();
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+1);					//��ַ��
	*vp_Status = SHT20_RecvByte();						//��λ
	SHT20_SendACK(1);											//NACK
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ���״̬�Ĵ���
 ����˵��: vCfg - ���ò���
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_WriteStaus(uint8 vCfg)
{
	/*-----  ���ò���˵��
	0λ �� 7λ ��� [0:1] ���ò������ݵ�λ��  00: RH 12bit  T 14bit			Ĭ��00
																						01:	RH 8bit   T 12bit
																						10: RH 10bit  T 13bit
																						11: RH 11bit  T 11bit
	6λ ���ģʽ 0: VDD>2.25  1: VDD<2.25    Ĭ��0
	3,4,5  ����
	2λ  ���ȿ��� 0-�ر� 1��
	1λ  
	------*/
	
	uint8 vRval = 0;
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//��ַд
	if (!vRval) vRval |= SHT20_SendByte(SHT20_WRITE_USER_REGISTER);
	if (!vRval) vRval |= SHT20_SendByte(vCfg);
	
	SHT20_Stop();
	
	return vRval;
}


/*---------------------------------------------------------------------
 ��������: SHT20��������λ
 ����˵��: ��
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_Soft_Reset()
{
	uint8 vRval = 0;
	
	SHT20_Start();
	
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SlaveAddress+0);					//��ַд
	if (!vRval) vRval |= SHT20_SendByte(SHT20_SOFT_RESET);
	
	SHT20_Stop();
	
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: SHT20��������
 ����˵��: vEnable - ����ʹ�ܿ��� 0-�ر� 1-��
 ��������: 0 - �ɹ�  ����1����
 ---------------------------------------------------------------------*/
uint8 SHT20_Heater(uint8 vEnable)
{
	uint8 vRval = 0;
	uint8 vCfg = 0;
	
	if (!vRval) vRval |= SHT20_ReadStaus(&vCfg);
	
	if (vEnable)							//��
	{
		vCfg |= (0x01<<2);
	}
	else											//�ر�
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
 ��������: SHT20 �����������
 ����˵��:  vBuf [in] - ������ȡ���
						vTemSymbol [out] - �����¶ȷ���
						vTem [out] - �¶�
						vHum [out] - ʪ��

 ��������: ��
 ---------------------------------------------------------------------*/
void SHT20_calc(uint8 *vBuf, uint8 *vTemSymbol, uint16 *vTem, uint16 *vHum)
{
	uint16 	vVal = 0x00;
	uint8 	vCrc = 0x00;
	float		vTemp = 0.00;
	
	//�¶�
	vCrc = SHT20_CheckCrc8(&vBuf[0], 2);
	if (vCrc == vBuf[2])
	{
		vVal = vBuf[0];
		vVal<<=8;
		vVal |= vBuf[1];
		
		vVal &= ~0x0003;											//��Ч����
		
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
		
		vVal &= ~0x000F; 											//��Ч����
		
		vTemp = (125.0/65536.0*vVal) - 6;
		*vHum = (vTemp*10);
	}
}

/*---------------------------------------------------------------------
 ��������: SHT20 �����������
 ����˵��:  vTemSymbol [out] - �����¶ȷ���
						vTem [out] - �¶�
						vHum [out] - ʪ��

 ��������: ��
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





