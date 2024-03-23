#ifndef _APP_GPS_H__
#define _APP_GPS_H__
#include "bsp_gps_l70_r.h"


//定义与GPS相关
#define GPS_EVENT_GPRMC        0x01
   

typedef struct
{
	u8 update_flag;
	
}GPS_STRCUT;

extern gps_info_t	  g_GpsInfo;

void App_gps_Msg_cb(gps_type_e type);
void App_gps_Get(void);
void App_gps_test(void);


#endif
