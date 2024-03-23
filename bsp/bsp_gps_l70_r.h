//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : #include "bsp_gps_l70_r.h"
//  Date :     2018-1-15 22:11
//  Version :  V0001
//  History :  ��ʼ�����汾
//******************************************************************************
#ifndef BSP_GPS_L70_R_H
#define BSP_GPS_L70_R_H

#ifdef __cplusplus
extern "C"
{
#endif
  
#include <stdint.h>
  
typedef enum
{
  GPS_TYPE_RMC
}gps_type_e;
  
typedef void (*gps_receive_cb)(char*, uint16_t);

//gps��λ��Ϣ����֪ͨ�ص�����Ҫ��APP��ص�
typedef void (*gps_notify_cb)(gps_type_e);

//******************************************************************************
// typedef 
//
typedef struct
{
  char nmea_id[6];           //nmea message id
  char utc[11];              //utcʱ�䡣hhmmss
  char state[2];             // A=��λ��V=δ��λ
  char latitude_value[10];   //γ��ֵ
  char latitude[2];          // N ��S
  char longitude_value[11];  //����ֵ��dddmm.mmmm
  char longitude[2];         // W �� E
  char speed[10];            //Speed over ground in knots
  char cog[7];               //��λ�ǣ���
  char date[7];              //���ڣ�DDMMYY
  char magnetic_angle[3];    //��ƫ��  һֱΪ��
  char magnetic_direction[2]; //��λ   һ��Ϊ��
  char mode[2];               //ģʽ��A = �Զ���D=��֣�E=���⣬N=��Ч
}gps_gprmc_t;

typedef gps_gprmc_t* GpsrmcHandle;

typedef struct
{
  double longtitude;  //����ֵ
  char   longtitude_char;
  double latitude;    //γ��ֵ 
  char   latitude_char;
  
}gps_info_t;

typedef gps_info_t* GpsHandle;

typedef struct
{
	int year;
	int month;
	int day;
	
	int hour;
	int minute;
	int second;

}TDateTime;

//******************************************************************************
// fn : bsp_GPS_Init
//
// brief : ʵʼ��GPSģ��
//
// param : cb->���»ص�
//
// return : none
extern void bsp_GPS_Init(gps_notify_cb cb);

//******************************************************************************
// fn : bsp_GPS_Get
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
extern GpsrmcHandle bsp_GPS_Get(void);

//******************************************************************************
// fn : bsp_GPS_GetInfo
//
// brief : ��ȡGPS��λ��Ϣ
//
// param : none
//
// return : ��Ŷ�λ��Ϣ��ַ����Ч��λ��Ϣ������null
extern GpsHandle bsp_GPS_GetInfo(void);

void GPS_handle_msg(char*msg, uint16_t len);
#ifdef __cplusplus
}
#endif
#endif   //BSP_GPS_L70_R_H
