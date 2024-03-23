//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : json_format.h
//  Date :     2018-03-01 21:51
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************
#ifndef _JSON_FORMAT_H
#define _JSON_FORMAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

//******************************************************************************
// fn : JSON_Temp
//
// brief : ���߱����¶�ֵת������Ӧ��json��ʽ
//
// param : buf ->���json��ʽ���ݵ�ַ
//         tempValue -> ������¶�ֵ
//
// return : ת��֮��json���ݳ���
uint16_t JSON_Temp(char* buf,float tempValue);  
 
//******************************************************************************
// fn : JSON_Humi
//
// brief : ���߱���ʪ��ֵת������Ӧ��json��ʽ
//
// param : buf ->���json��ʽ���ݵ�ַ
//         humiValue -> �����ʪ��ֵ
//
// return : ת��֮��json���ݳ���
uint16_t JSON_Humi(char* buf,float humiValue); 

//******************************************************************************
// fn : JSON_gps
//
// brief : ����γ����Ϣת������Ӧ��json��ʽ
//
// param : buf ->���json��ʽ���ݵ�ַ
//         longitude -> ����
//         latiude -> γ��
//
// return : ת��֮��json���ݳ���
uint16_t JSON_gps(char* buf,double longitude,double latitude); 
  
#ifdef __cplusplus
}
#endif
#endif   //_KEY_HANDLE_H