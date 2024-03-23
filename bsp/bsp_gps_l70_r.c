//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : led_blink.h
//  Date :     2018-1-09 10:30
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bsp_gps_l70_r.h"
#include "gpio.h"
#include "usart.h"
#include "rtc.h"


#define NMEA_GPGSA  "$GPGSA"
#define NMEA_GPGGA  "$GPGGA"
#define NMEA_GPGSV  "$GPGSV"
#define NMEA_GPRMC  "$GPRMC"
#define NMEA_GPVTG  "$GPVTG"
#define NMEA_GPGLL  "$GPGLL"

static gps_notify_cb gps_app_cb;
static gps_gprmc_t   gps_gprmc;
static gps_info_t    gps_info;
//******************************************************************************
// �ڲ�����������
//
//static void GPS_handle_msg(char*msg, uint16_t len);

//����GPRMC ���ݸ�ʽ
static void GPS_parse_rmc(char *msg,uint16_t len);
//��ȡ����������
static uint16_t GPS_ExtractField(char *dst,char*src,uint16_t offset,uint8_t max_len);
//��ת����Ӧ�Ķ�λ���ݣ���ŵ�gps_info��
static uint8_t GPS_Convert(void);
//******************************************************************************
// fn : bsp_GPS_Init
//
// brief : ʵʼ��GPSģ��
//
// param : none
//
// return : none
void bsp_GPS_Init(gps_notify_cb cb)
{
	gps_app_cb = cb;
	//��λ����
	memset(&gps_gprmc,0,sizeof(gps_gprmc_t));
	
	HAL_UARTDMA_Init(GPS,(void*)GPS_handle_msg,0);
	
	//��gps��Դ
	#if 0
	Bsp_gps_en();
	#endif
}
//******************************************************************************
// fn : bsp_GPS_Get
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
GpsrmcHandle bsp_GPS_Get(void)
{
  if( gps_gprmc.state[0] == 'A')
  {
    return &gps_gprmc;
  }
  return NULL;
}
//******************************************************************************
// fn : bsp_GPS_GetInfo
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
extern GpsHandle bsp_GPS_GetInfo(void)
{
  return &gps_info;
}
//******************************************************************************
// fn : GPS_handle_msg
//
// brief : �������GPS�����Ϣ
//
// param : msg -> NMEA Э������
//         len -> ���ݳ���
//
// return : none
void GPS_handle_msg(char*msg, uint16_t len)
{
  char *pRmc = 0;
  char *pEnd = 0;
  pRmc = strstr(msg,NMEA_GPRMC);
  
  if(pRmc)
  {
    //�ҵ�GPRMC NMEAЭ������
    pEnd = strchr(pRmc,'\r');
    if(pEnd)
    {
      *pEnd = 0;
      //printf("RMC=%s\r\n",pRmc);
      //printf("len=%d\r\n",(uint16_t)(pEnd - pRmc));
      GPS_parse_rmc(pRmc,(uint16_t)(pEnd - pRmc));
      
      GPS_Convert();
      if(gps_app_cb)
      {
        gps_app_cb(GPS_TYPE_RMC);
      }
    }
    
  }
}
//******************************************************************************
// fn : GPS_parse_rmc
//
// brief : �������GPRMC�����Ϣ
//
// param : msg -> GPRMCЭ��������ʼ��ַ
//         len -> ���ݳ���
//
// return : none
void GPS_parse_rmc(char *msg,uint16_t len)
{
  uint16_t offset = 0;
  if(msg == NULL || len < 10)
  {
    return;
  }
  
  offset = GPS_ExtractField(gps_gprmc.nmea_id,msg,offset,sizeof(gps_gprmc.nmea_id)-1);
  //printf("len=%d,P_ID=%s,offset=%d\r\n",len,gps_gprmc.nmea_id,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.utc,msg,offset,sizeof(gps_gprmc.utc)-1);
  }
 // printf("utc=%s,offset=%d\r\n",gps_gprmc.utc,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.state,msg,offset,sizeof(gps_gprmc.state)-1);
  }
 // printf("state=%s,offset=%d\r\n",gps_gprmc.state,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.latitude_value,msg,offset,sizeof(gps_gprmc.latitude_value)-1);
  }
  printf("latitude=%s,offset=%d\r\n",gps_gprmc.latitude_value,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.latitude,msg,offset,sizeof(gps_gprmc.latitude)-1);
  }
  //printf("latitude=%s,offset=%d\r\n",gps_gprmc.latitude,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.longitude_value,msg,offset,sizeof(gps_gprmc.longitude_value)-1);
  }
  printf("longitude=%s,offset=%d\r\n",gps_gprmc.longitude_value,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.longitude,msg,offset,sizeof(gps_gprmc.longitude)-1);
  }
 // printf("longitude=%s,offset=%d\r\n",gps_gprmc.longitude,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.speed,msg,offset,sizeof(gps_gprmc.speed)-1);
  }
  //printf("speed=%s,offset=%d\r\n",gps_gprmc.speed,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.cog,msg,offset,sizeof(gps_gprmc.cog)-1);
  }
 // printf("cog=%s,offset=%d\r\n",gps_gprmc.cog,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.date,msg,offset,sizeof(gps_gprmc.date)-1);
  }
//  printf("date=%s,offset=%d\r\n",gps_gprmc.date,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.magnetic_angle,msg,offset,sizeof(gps_gprmc.magnetic_angle)-1);
  }
  //printf("magnetic_angle=%s,offset=%d\r\n",gps_gprmc.magnetic_angle,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.magnetic_direction,msg,offset,sizeof(gps_gprmc.magnetic_direction)-1);
  }
  //printf("magnetic_direction=%s,offset=%d\r\n",gps_gprmc.magnetic_direction,offset);
  if(offset < len)
  {
    offset = GPS_ExtractField(gps_gprmc.mode,msg,offset,sizeof(gps_gprmc.mode)-1);
  }
  //printf("mode=%s,offset=%d\r\n",gps_gprmc.mode,offset);
}
//******************************************************************************
// fn : GPS_ExtractField
//
// brief : ��ȡ����������
//
// param : dst -> Ŀ�����ݵ�ַ
//         src -> ԭ���ݵ�ַ
//         offset -> ����ƫ��
//         max_len -> �����ȡ���ݸ���
//
// return : none
static uint16_t GPS_ExtractField(char *dst,char*src,uint16_t offset,uint8_t max_len)
{
  uint8_t j = 0;
  if(*(src + offset) == '$' ||
     *(src + offset) == ',')
  {
    offset++;
  }
  
  j = 0;
  while((src[offset] != ',') && (src[offset] != '*') && (j < max_len))
  {
    dst[j] = src[offset];
    j++;
    offset++;
  }
  
  dst[j] = 0;
  
  
  return offset;
}

void CGpsParseWx_UTC2BTC( TDateTime *in_Param )
{
	if( in_Param == NULL )
	{
		return;
	}

	in_Param->second++;

	if( in_Param->second>59 )
	{
		in_Param->second = 0;
		in_Param->minute++;
		if( in_Param->minute>59 )
		{
		in_Param->minute = 0;
		in_Param->hour++;
		}
	}

	in_Param->hour+=8;
	if( in_Param->hour>23 )
	{
		in_Param->hour-=24;
		in_Param->day+=1;
		if( in_Param->month==2 || in_Param->month==4 || in_Param->month==6 || in_Param->month==9 || in_Param->month==11 )
		{
		if( in_Param->day>30 )
		{
		in_Param->day = 1;
		in_Param->month++;
		}
		}
		else
		{
		if( in_Param->day>31 )
		{
		in_Param->day = 1;
		in_Param->month++;
		}
		}
		if( in_Param->year % 4 == 0 )
		{
		if( in_Param->day > 29 && in_Param->month == 2 )
		{
		in_Param->day = 1;
		in_Param->month++;
		}
		}
		else
		{
		if( in_Param->day>28 && in_Param->month ==2 )
		{
		in_Param->day = 1;
		in_Param->month++;
		}
		}
		if( in_Param->month>12 )
		{
		in_Param->month-=12;
		in_Param->year++;
		}
	}
}

//******************************************************************************
// fn : GPS_Convert
//
// brief : ����λ��Ϣ����ת��
//
// param : none
//
// return : none
uint8_t GPS_Convert(void)
{
#if 0
	TDateTime pDateTime ;
	
	int year =0;
	int mon =0;
	int day =0;
	int hour =0;
	int min =0;
	int sec =0;
#endif
  double tmp = 0;
  uint32_t dd = 0;
  if( gps_gprmc.state[0] == 'A')
  {
    //GPRMC��γ��ֵ��ʽΪddmm.mmmm,Ҫת����dd.dddddd
    gps_info.latitude_char = gps_gprmc.latitude[0];
    tmp = atof(gps_gprmc.latitude_value); 
    dd = (uint32_t)(tmp / 100);  //ȡ��������
    gps_info.latitude = dd + ((tmp - dd * 100)/60);
    
    //GPRMC�ľ��ȸ�ʽΪdddmm.mmmm��Ҫת����dd.dddddd
    gps_info.longtitude_char = gps_gprmc.longitude[0];
    tmp = atof(gps_gprmc.longitude_value);
    dd = (uint32_t)(tmp / 100);
    gps_info.longtitude = dd + ((tmp - dd * 100)/60);

	#if 0
	pDateTime.day = (gps_gprmc.date[0] - '0') * 10 + gps_gprmc.date[1]- '0'; //ʱ��
	pDateTime.month = (gps_gprmc.date[2] - '0') * 10 + gps_gprmc.date[3]- '0'; 
	pDateTime.year =(gps_gprmc.date[4] - '0') * 10 + gps_gprmc.date[5]- '0'; 
	
	pDateTime.hour = (gps_gprmc.utc[0] - '0') * 10 + gps_gprmc.utc[1]- '0';//utcʱ��
	pDateTime.minute = (gps_gprmc.utc[2] - '0') * 10 + gps_gprmc.utc[3]- '0';
	pDateTime.second = (gps_gprmc.utc[4] - '0') * 10 + gps_gprmc.utc[5]- '0';

	CGpsParseWx_UTC2BTC(&pDateTime);// utc ת ����ʱ��
	printf("set %d-%d-%d, %d:%d:%d",pDateTime.year,pDateTime.month,pDateTime.day,pDateTime.hour,pDateTime.minute,pDateTime.second);
	MX_RTC_Set(pDateTime.year,pDateTime.month,pDateTime.day,pDateTime.hour,pDateTime.minute,pDateTime.second);
	#endif
	
    return 1;
  }
  else
  {
    gps_info.latitude_char = 'X';
    gps_info.latitude = 0.0;
    
    gps_info.longtitude_char = 'X';
    gps_info.longtitude = 0.0;
	
	#if 0
	day = (gps_gprmc.date[0] - '0') * 10 + gps_gprmc.date[1]- '0'; //ʱ��
	mon = (gps_gprmc.date[2] - '0') * 10 + gps_gprmc.date[3]- '0'; 
	year =(gps_gprmc.date[4] - '0') * 10 + gps_gprmc.date[5]- '0'; 
	
	hour = (gps_gprmc.utc[0] - '0') * 10 + gps_gprmc.utc[1]- '0'+8;//����ʱ��
	min = (gps_gprmc.utc[2] - '0') * 10 + gps_gprmc.utc[3]- '0';
	sec = (gps_gprmc.utc[4] - '0') * 10 + gps_gprmc.utc[5]- '0';
	
	printf("set %d-%d-%d, %d:%d:%d",year,mon,day,hour,min,sec);
	MX_RTC_Set(year,mon,day,hour,min,sec);
	#endif
				
  }
  return 0;
}


