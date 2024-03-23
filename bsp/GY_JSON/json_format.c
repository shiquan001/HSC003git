//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : json_format.c
//  Date :     2018-03-01 22:03
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************
#include <stdio.h>
#include "json_format.h"

//******************************************************************************
// fn : JSON_Temp
//
// brief : ���߱����¶�ֵת������Ӧ��json��ʽ
//
// param : buf ->���json��ʽ���ݵ�ַ
//         tempValue -> ������¶�ֵ
//
// return : ת��֮��json���ݳ���
//
// ע�⣬buf �ĳ���һ��Ҫ�㹻��.Temp Json��������Ҫ35����
// ת��֮��������ʽ
//{
//	"dataType": "TEMP",
//	"data": "32.5"
//}
uint16_t JSON_Temp(char* buf,float tempValue)
{
	uint16_t msgLen = 0;
	if(buf == NULL)
	{
		return 0;
	}
	msgLen = sprintf(buf,"{\"dataType\":\"TEMP\",\"data\":\"%0.1f\"}",tempValue);
	return msgLen;
}
//******************************************************************************
// fn : JSON_Humi
//
// brief : ���߱���ʪ��ֵת������Ӧ��json��ʽ
//
// param : buf ->���json��ʽ���ݵ�ַ
//         humiValue -> �����ʪ��ֵ
//
// return : ת��֮��json���ݳ���
//
// ע�⣬buf �ĳ���һ��Ҫ�㹻��.Temp Json��������Ҫ35����
// ת��֮��������ʽ
//{
//	"dataType": "HUM",
//	"data": "60"
//}
uint16_t JSON_Humi(char* buf,float humiValue)
{
  uint16_t msgLen = 0;
	if(buf == NULL)
	{
		return 0;
	}
	msgLen = sprintf(buf,"{\"dataType\":\"HUM\",\"data\":\"%.1f\"}",humiValue);
	return msgLen;
}
