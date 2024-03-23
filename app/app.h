#ifndef _APP_H__
#define _APP_H__

/*include */

#include "rtc.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

/*include */

#include "main.h"

#include "json_format.h"

#include "timer_user_poll.h"

#include "app_display.h"
#include "app_battery.h"

#include "app_picture.h"
#include "app_wenshi.h"
#include "app_zigbee.h"

#include "app_zigbee_HA.h"
#include "app_gps.h"
#include "app_nb.h"


#include "app_caiji.h"
#include "app_config.h"
#include "app_update.h"
#include "GMT_time.h"

#include "app_camera.h"
#include "heart_led.h"
#include "app_caiji_PLC.h"

#include "app_sheshisuo_PLC.h"

#include "app_fault_analysis.h"

#include "app_fifo_NB_CoapST.h"
#include "app_4g.h"
#include "app_update_4g.h"


#define VERSION_WENSHI_cur "V1.18"//
#define VERSION_WENSHI_CODE 118//

#if 0
#define VERSION_bootloarder_code  
#else
#define VERSION_app_code  
#endif
//#define  APPLICATION_ADDRESS 0x08020000


#define ENABLE_WATCHDOG 1////.....///1  

/* PLC  config */
//#define ENABLE_TIANJING_PLC  //兼容设施所plc
#define ENABLE_SHESHISUO_PLC //设施所plc 新

//#define ALL_SENSORS // 原来的传感器
#define ONLY_8SAME_AIRTHSENSOR // 1个控制器带5个：11、12、13、14、15；  1个控制器带3个；16、20、30；

/* COAP address config */
#if 0
#define REMOTE_COAP_INFO        "180.101.147.115,5683"//NB网关 中国电信物联网开放平台
#else
#define REMOTE_COAP_INFO        "lwm2m.ctwing.cn,5683"//"221.229.214.202,5682"//ctwing

#endif

#endif

