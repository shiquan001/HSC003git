//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : sht20_cfg.c
//  Date     : 2017-12-23 13:47
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// SHT20 IICͨ�ŷ�ʽ����˵��
//******************************************************************************
#include "sht20_cfg.h"


//******************************************************************************
// ����������
//******************************************************************************
#define SHT20_IIC_ADDR    0x80

//
#define SHT20_READ_ADDR   0X81
#define SHT20_WRITE_ADDR  0x80

// CMD
#define SHT20_MEASURE_TEMP_CMD      0xE3
#define SHT20_MEASURE_RH_CMD        0xE5

#define SHT20_SOFT_RESET_CMD        0xFE


//******************************************************************************
//  ���������붨����
//******************************************************************************

//******************************************************************************
// fn : SHT20_ReadTemp
//
// brief : read tempture from sht20 with iic
//
// param : none
//
// return : �¶ȵ�ԭʼ����
uint16_t SHT20_ReadTemp(void)
{
  uint16_t temp = 0;

  //�趨
  uint8_t  cmd = SHT20_MEASURE_TEMP_CMD;
  HAL_I2C_Send(SHT20_WRITE_ADDR,&cmd,1);
  
  uint8_t pDATA[3] = {0,0,0};
  HAL_I2C_Read(SHT20_READ_ADDR,pDATA,3);
  
  temp = pDATA[0];
  temp <<= 8;
  temp += (pDATA[1] & 0xfc);
  
  //printf("temp=%04X\r\n",temp);
  return temp;
}

//******************************************************************************
// fn : SHT20_ReadRH
//
// brief : read RH from sht20 with iic
//
// param : none
//
// return : none
uint16_t SHT20_ReadRH(void)
{
  uint16_t rh = 0;
  uint8_t  cmd = SHT20_MEASURE_RH_CMD;
  HAL_I2C_Send(SHT20_WRITE_ADDR,&cmd,1);
  
  uint8_t pDATA[3] = {0,0,0};
  HAL_I2C_Read(SHT20_READ_ADDR,pDATA,3);
  
  rh = pDATA[0];
  rh <<= 8;
  rh += (pDATA[1] & 0xf0);
  
  return rh;
}

//******************************************************************************
// fn : SHT20_SoftReset
//
// brief : softreset sht20
//
// param : none
//
// return : none
void SHT20_SoftReset(void)
{
  uint8_t  cmd = SHT20_MEASURE_RH_CMD;
  HAL_I2C_Send(SHT20_WRITE_ADDR,&cmd,1);
  
}
//******************************************************************************
// fn : SHT20_Convert
//
// brief : ��ԭʼ��ADֵת����Ӧ���¶�ֵ��ʪ��ֵ
//
// param : value-> adֵ
//         isTemp-> >0 �¶�ֵ��=0 ʪ��ֵ
//
// return : ֱ�۵��¶�ֵ
extern float SHT20_Convert(uint16_t value,uint8_t isTemp)
{
  float tmp = 0.0;
  if(isTemp)
  {
    tmp = -46.85 + (175.72* value)/(1 << 16);
  }
  else
  {
    tmp = -6 + (125.0 *value)/(1<<16);
  }
  return tmp;
}
