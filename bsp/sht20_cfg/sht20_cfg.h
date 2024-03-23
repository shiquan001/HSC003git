//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : sht20_cfg.h
//  Date     : 2017-12-23 13:48
//  Version  : V0001
// ��ʷ��¼  : 1.��һ�δ���
//
// ˵����
// sht20 iic ��ʽͨ������ʵ��
//******************************************************************************
#ifndef __SHT20_CFG_H
#define __SHT20_CFG_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "i2c.h"
  
//******************************************************************************
// fn : SHT20_ReadTemp
//
// brief : read tempture from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadTemp(void);

//******************************************************************************
// fn : SHT20_ReadRH
//
// brief : read RH from sht20 with iic
//
// param : none
//
// return : none
extern uint16_t SHT20_ReadRH(void);

//******************************************************************************
// fn : SHT20_SoftReset
//
// brief : softreset sht20
//
// param : none
//
// return : none
extern void SHT20_SoftReset(void);

//******************************************************************************
// fn : SHT20_Convert
//
// brief : ��ԭʼ��ADֵת����Ӧ���¶�ֵ��ʪ��ֵ
//
// param : value-> adֵ
//         isTemp-> >0 �¶�ֵ��=0 ʪ��ֵ
//
// return : ֱ�۵��¶�ֵ
extern float SHT20_Convert(uint16_t value,uint8_t isTemp);
  

#ifdef __cplusplus
}
#endif
#endif   //__SHT20_CFG_H
