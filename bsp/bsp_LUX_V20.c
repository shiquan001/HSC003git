//***************************************
// bsp_LUX_V20�ɼ�����
//****************************************
#include "bsp_LUX_V20.h"
#include "bsp.h"

uint8_t    BUF_0[8];                       //�������ݻ�����      	
uint16_t   dis_data_0;                     //����


/*---------------------------------------------------------------------
 ��������: ��ʱ���� ��ͬ�Ĺ�������,��Ҫ�����˺���
 ����˵��: ��	
 ��������: ��
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
 ��������: ��ʱ5΢��  ��ͬ�Ĺ�������,��Ҫ�����˺���
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
void bsp_LUX_Delay5us()
{
	bsp_DelayUS(5);
}

/*---------------------------------------------------------------------
 ��������: ��ʱ5����  ��ͬ�Ĺ�������,��Ҫ�����˺���
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
void bsp_LUX_Delay5ms()
{
	bsp_DelayMS(5);
}

/*---------------------------------------------------------------------
 ��������: ��ʼ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
void bsp_LUX_Start()
{
  bsp_LUX_SDA0_H;                         //����������
  bsp_LUX_SCL0_H;                         //����ʱ����
  bsp_LUX_Delay5us();                     //��ʱ
  bsp_LUX_SDA0_L;                         //�����½���
  bsp_LUX_Delay5us();                     //��ʱ
  bsp_LUX_SCL0_L;                         //����ʱ����
}

/*---------------------------------------------------------------------
 ��������: ֹͣ�ź�
 ����˵��: ��	
 ��������: ��
 ---------------------------------------------------------------------*/
void bsp_LUX_Stop()
{
  bsp_LUX_SDA0_L;                         //����������
  bsp_LUX_SCL0_H;                         //����ʱ����
  bsp_LUX_Delay5us();                     //��ʱ
  bsp_LUX_SDA0_H;                         //����������
  bsp_LUX_Delay5us();                     //��ʱ
  bsp_LUX_SCL0_L;
  bsp_LUX_Delay5us();
}

/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ack - Ӧ���ź�(0:ACK 1:NAK)
 ��������: ��
 ---------------------------------------------------------------------*/
void bsp_LUX_SendACK(uint8_t ack)
{
  if (ack&0x01)	bsp_LUX_SDA0_H;		//дӦ���ź�
  else	bsp_LUX_SDA0_L;
  
  bsp_LUX_SCL0_H;                         //����ʱ����
  bsp_LUX_Delay5us();                     //��ʱ
  bsp_LUX_SCL0_L;                         //����ʱ����
  bsp_LUX_SDA0_H;
  bsp_LUX_Delay5us();                     //��ʱ
}

/*---------------------------------------------------------------------
 ��������: ����Ӧ���ź�
 ����˵��: ��
 ��������: ����Ӧ���ź�
 ---------------------------------------------------------------------*/
uint8_t bsp_LUX_RecvACK()
{
  uint8_t CY = 0x00;
	uint16 vConter = 1000;  
  bsp_LUX_SDA0_H;
  
  bsp_LUX_SDA0_I;
  
  bsp_LUX_SCL0_H;                         //����ʱ����
  bsp_LUX_Delay5us();                     //��ʱ
  
  	while (vConter)
	{
		vConter--;
		
		if (bsp_LUX_SDA0_DAT)
			CY = 1;                 		//��Ӧ���ź�
		
		if(!CY)	break;
	}
  
  bsp_LUX_Delay5us();                     //��ʱ
  
  bsp_LUX_SCL0_L;                         //����ʱ����
  
  bsp_LUX_SDA0_O;
  
  return CY;
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߷���һ���ֽ�����
 ����˵��: dat - д�ֽ�
 ��������: ��
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_SendByte(uint8_t dat)
{
	uint8_t i;
	uint8 vRval = 0x00;
	
  for (i=0; i<8; i++)         			//8λ������
  {
    if (dat&0x80)	bsp_LUX_SDA0_H;
    else	bsp_LUX_SDA0_L;                   //�����ݿ�
    
    bsp_LUX_Delay5us();             		//��ʱ
    bsp_LUX_SCL0_H;                		//����ʱ����
    bsp_LUX_Delay5us();             		//��ʱ
    bsp_LUX_SCL0_L;                		//����ʱ����
    bsp_LUX_Delay5us();             		//��ʱ
    dat <<= 1;              			//�Ƴ����ݵ����λ
  }
  
    vRval = bsp_LUX_RecvACK();
	return vRval;  
}

/*---------------------------------------------------------------------
 ��������: ��IIC���߽���һ���ֽ�����
 ����˵��: ��
 ��������: �����ֽ�
 ---------------------------------------------------------------------*/
uint8_t bsp_LUX_RecvByte()
{
  uint8_t i;
  uint8_t dat = 0;
  bsp_LUX_SDA0_I;
  
  bsp_LUX_SDA0_H;                         //ʹ���ڲ�����,׼����ȡ����,
  for (i=0; i<8; i++)         	        //8λ������
  {
    bsp_LUX_SCL0_H;                       //����ʱ����
    bsp_LUX_Delay5us();             	//��ʱ

	if (bsp_LUX_SDA0_DAT)
		dat |= 1;              			//������ 
            
    bsp_LUX_SCL0_L;                       //����ʱ����
    bsp_LUX_Delay5us();             	//��ʱ
    
    if (i<7) dat <<= 1;
  }
  bsp_LUX_SDA0_O;
  
  return dat;
}

/*---------------------------------------------------------------------
 ��������: дBH1750
 ����˵��: REG_Address - �Ĵ�����ַ
 ��������: ��
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Single_Write(uint8_t REG_Address)
{
	uint8 vRval = 0;

  bsp_LUX_Start();                                //��ʼ�ź�
  vRval +=bsp_LUX_SendByte(bsp_LUX_SlaveAddress);           //�����豸��ַ+д�ź�
  vRval +=bsp_LUX_SendByte(REG_Address);                  //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
  //  BH1750_SendByte(REG_data);                //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
  bsp_LUX_Stop();                                 //����ֹͣ�ź�

	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ��������BH1750�ڲ�����
 ����˵��: ��
 ��������: ��
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Multiple_read(void)
{   
  uint8_t i;
	uint8 vRval = 0;  
  bsp_LUX_Start();                                //��ʼ�ź�
  vRval +=bsp_LUX_SendByte(bsp_LUX_SlaveAddress+1);         //�����豸��ַ+���ź�
  
  for (i=0; i<3; i++)                           //������ȡ6����ַ���ݣ��洢��BUF
  {
    BUF_0[i] = bsp_LUX_RecvByte();                //BUF[0]�洢0x32��ַ�е�����
    if (i == 0x02)
    {      
      bsp_LUX_SendACK(1);                         //���һ��������Ҫ��NOACK
    }
    else
    {		
      bsp_LUX_SendACK(0);                         //��ӦACK
    }
  }
  
  bsp_LUX_Stop();                                 //ֹͣ�ź�
//  bsp_LUX_Delay5ms();
	return vRval;  
}

/*
*********************************************************************************************************
*	�� �� ��: MX_GPIO_Init_IIC1_SCL_Pin_OD
*	����˵��: 
*	��    ��: 
*			  
*	�� �� ֵ: no 
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
*	�� �� ��: MX_GPIO_Init_IIC1_SCL_Pin_Input
*	����˵��: 
*	��    ��: 
*			  
*	�� �� ֵ: no 
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
*	�� �� ��: MX_GPIO_Init_IIC1_SDA_Pin_OD
*	����˵��: 
*	��    ��: 
*			  
*	�� �� ֵ: no 
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
*	�� �� ��: MX_GPIO_Init_IIC1_SDA_Pin_OD
*	����˵��: 
*	��    ��: 
*			  
*	�� �� ֵ: no 
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
 ��������: ��ʼ�����մ�����
 ����˵��: ��
 ��������: ��
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_Init()
{
	uint8 vRval = 0 ;

	bsp_LUX_SCL0_O;
	bsp_LUX_SDA0_O;

	bsp_LUX_delay_nms(100);	                        //��ʱ100ms

	vRval += bsp_LUX_Single_Write(0x01); 
	return vRval;
}

/*---------------------------------------------------------------------
 ��������: ���ն�ȡ����
 ����˵��: ��
 ��������: ���ع���ֵ
 ---------------------------------------------------------------------*/
uint8 bsp_LUX_GetLux(uint32_t *vLux)
{  
	uint8 vRval = 0;

	float temp;
	vRval += bsp_LUX_Single_Write(0x01);                     // power on
	vRval += bsp_LUX_Single_Write(0x10);                     // H- resolution mode 

	bsp_LUX_delay_nms(180);                         //��ʱ180ms

	vRval += bsp_LUX_Multiple_read();                        //�����������ݣ��洢��BUF��

	vRval +=  bsp_LUX_Single_Write(0x00);                     // power off

	dis_data_0=BUF_0[0];
	dis_data_0=(dis_data_0<<8)+BUF_0[1];          //�ϳ����ݣ�����������

	temp=(float)((float)dis_data_0/1.2f);
	*vLux = (uint32_t)(temp*1.4f);
	return vRval;  
} 

