#ifndef  _APP_PICTURE_H_
#define  _APP_PICTURE_H_

#include "stm32l4xx_hal.h"
#include "app_wenshi.h"



/* 帧同步*/
typedef enum
{
	VsyncActive_NULL = 0,//帧同步
	VsyncActive_START ,
	VsyncActive_START_HIGH ,
	VsyncActive_MAX ,

} ENUM_VsyncActive;

/* 行同步*/
typedef enum
{
	HSYNCActive_NULL = 0,//行同步*/
	HSYNCActive_START ,
	HSYNCActive_MAX ,

} ENUM_HSYNCActive;


typedef enum 
{
	PICTURE_INIT=1,// 初始化态
	PICTURE_START_CAPTURE,
	PICTURE_LOOP,
	PICTURE_TIMER_OUT,
		
	PICTURE_MAX,
}ENUM_PICTURE;


typedef struct
{
	u8 work_state;
	
	u16 data_packet_total;//????????
	u16 data_packet_cur;	 //??????????
	u16 data_packet_size; //????????С
	u16 data_packet_last_size; //最后一包的数据大小
}
PICTURE_T;



extern PICTURE_T g_Picture;
extern u8 JpegBuffer[ PIC_BUFF_LEN];

void App_picture_Init_Var(void);
void App_picture_Loop(void);
void App_picture_send_proccess(void);

#endif

