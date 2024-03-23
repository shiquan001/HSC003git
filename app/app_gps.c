#include "bsp.h"
#include "app.h"

#include "app_gps.h"

//ȫ�ֱ���
static  uint8_t gps_event_table = 0;

 
 gps_info_t    g_GpsInfo;


/*
*********************************************************************************************************
*	�� �� ��: App_gps_Msg_cb
*	����˵��: �ص�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_gps_Msg_cb(gps_type_e type)
{
	switch(type)
	{
		case GPS_TYPE_RMC:
		{
			gps_event_table |= GPS_EVENT_GPRMC;
		}
			break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: App_gps_Get
*	����˵��: ��ȡGPS��γ��ֵ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_gps_Get(void)
{
  if(gps_event_table)
  {
    if(gps_event_table & GPS_EVENT_GPRMC)
    {
      GpsHandle gps =  bsp_GPS_GetInfo();
 
      if(gps)
      {
		/* ��Ч���������˳�gps�ɼ���*/
	  	if( bsp_GPS_Get()!=NULL)
  		{
			//memcpy(&g_GpsInfo,gps,sizeof(GpsHandle));// 
			
	        //ddmm.mmmm;
	        //printf("Latitude=%c:%.6f\r\n",g_GpsInfo.latitude_char,g_GpsInfo.latitude);
	        //dddmm.mmmm;
	        //printf("Longitude=%c:%06f\r\n",g_GpsInfo.longtitude_char,g_GpsInfo.longtitude);
			g_wenshi.m_flag_gps = TRUE;
			g_wenshi.m_longtitude = gps->longtitude;
			g_wenshi.m_latitude = gps->latitude;
		
	        //ddmm.mmmm;
	        p_info("Latitude=%c:%.07f\r\n",gps->latitude_char,gps->latitude);
	        //dddmm.mmmm;
	        p_info("Longitude=%c:%07f\r\n",gps->longtitude_char,gps->longtitude);
			  		
			bsp_SetTimer(TMR_ID_caiji_gps_refresh );// �����ɼ�gps
			p_info("g_caiji �����ɼ�gps");	
  		}
		else
		{
			//g_wenshi.m_flag_gps = FALSE;
			//g_wenshi.m_longtitude = 0;
			//g_wenshi.m_latitude = 0;
		}

      }

	  gps_event_table ^= GPS_EVENT_GPRMC;
    }
  }
}

/*
*********************************************************************************************************
*	�� �� ��: App_Battery_test
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_gps_test(void)
{

#ifdef TEST_App_gps_test

	while(1)
	{

	}
#endif
	
}

