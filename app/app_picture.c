#include "app.h"
#include "bsp.h"

#include "app_picture.h"
PICTURE_T g_Picture;

#define DEBUG_LOG_CAMER 1


u8 *p_camera = (u8 *)JpegBuffer;



 /*****************************************************************************
 *   @name 			 void  Uart_DebugSendDat(uint8_t *dat,uint16_t length)
 *   @function  :  
 *   @input  : 
								dat: the point of the data  to send
								length : the length of the data 
 *   @output : none                        
 *   @time   : 20170306
 *   @note	 :                          
 ********************************************************************************/ 
void  Uart_DebugSendDat(uint8_t *dat,uint16_t length)
{
	//	u8 data = 0;
	
	 if((length >0)&&(length < 65535))
	{
		while(length--)
		{
			/* Loop until the end of transmission */
			//HAL_UART_Transmit(&hlpuart1, dat, 1, 0xFFFF);
			printf("%02X",*dat);
			dat++;
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: App_picture_Init_Var
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_picture_Init_Var(void)
{
	g_Picture.data_packet_total = NULL;
	g_Picture.data_packet_cur   = NULL;//
	
	g_Picture.data_packet_size  = FRAME_PICTURE_DATA_SIZE;//

	g_Picture.data_packet_last_size = NULL;
	g_Picture.work_state = PICTURE_INIT;

}




/*
*********************************************************************************************************
*	函 数 名: App_picture_send_proccess
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_picture_send_proccess(void)
{
	u32 image_size_bytes = 0;

	//u16 i = 0;

	/*从后往前搜索 起点 6k*/			
	/*从后往前搜索 起点 6k*/			
	for (image_size_bytes = PIC_BUFF_LEN; image_size_bytes > 0; image_size_bytes--)
	{
		if(p_camera[image_size_bytes-1] == 0xff && p_camera[image_size_bytes] == 0xd9)
		{
			g_tCam.Capture_End = 1;
			g_tCam.Capture_End_Cur = image_size_bytes+1;
			
			break;
		}
	}		

	/* 打印日志 */
	if(g_tCam.Capture_End == 1)
	{	
		/* 根据捕获的图片，计算*/
		g_Picture.data_packet_total = (u16)(g_tCam.Capture_End_Cur/(u32)g_Picture.data_packet_size);
		if((u32)(g_Picture.data_packet_total*g_Picture.data_packet_size)== g_tCam.Capture_End_Cur)
		{
			g_Picture.data_packet_last_size = FRAME_PICTURE_DATA_SIZE;
			g_Picture.data_packet_total = g_Picture.data_packet_total ;
		}
		else/* 有余数计算 余数之后和加一*/
		{
			g_Picture.data_packet_last_size = g_tCam.Capture_End_Cur - (u32)(g_Picture.data_packet_total*g_Picture.data_packet_size);
			g_Picture.data_packet_total = g_Picture.data_packet_total + 1;
		}
		
		#if (DEBUG_LOG_CAMER == 1)
		p_dbg("Capture_End:%d", g_tCam.Capture_End);
		p_dbg("Capture_End_Cur:%d", g_tCam.Capture_End_Cur);
		
		p_dbg("data_packet_total:%d", g_Picture.data_packet_total);
		p_dbg("data_packet_last_size:%d", g_Picture.data_packet_last_size);
		#endif			

		#if 0
		/*一包一包的发送*/
		for (image_size_bytes = 0; image_size_bytes < g_Picture.data_packet_total; image_size_bytes++)
		{
			g_Picture.data_packet_cur = image_size_bytes;//
			
			//ServerProtocol_Frame_PictureReport(&p_camera[g_Picture.data_packet_cur*FRAME_PICTURE_DATA_SIZE]);
			
			bsp_DelayMS(20);//延时1秒钟
		}
		#else
	
			Uart_DebugSendDat(p_camera, g_tCam.Capture_End_Cur);
		#endif
		
	}
	else
	{
		g_tCam.Capture_End = 0; 				
		#if (DEBUG_LOG_CAMER == 1)
		p_dbg("Capture_End:%d", g_tCam.Capture_End);				
		#endif
	}					
	
	memset(JpegBuffer,0,PIC_BUFF_LEN);//清零
	//Camera_NT99141_Start();	

}



/*
*********************************************************************************************************
*	函 数 名: App_picture_Loop
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_picture_Loop(void)
{


	switch(g_Picture.work_state)
	{
		case PICTURE_INIT :
		{				
			bsp_Init_Camera_NT99141();
			
			g_Picture.work_state = PICTURE_START_CAPTURE;
			break;
		}
		case PICTURE_START_CAPTURE :
		{							
			if((g_wenshi.camera_state ==1)||(g_wenshi.camera_state ==2))
			{
				Camera_NT99141_Start(); 								
				g_Picture.work_state = PICTURE_LOOP;
			}			
			break;
		}

		case PICTURE_LOOP :
		{
			if(g_tCam.CaptureOk == 1)
			{
				g_tCam.CaptureOk = 0;
				p_info("CaptureOk --- ");
				if(g_wenshi.camera_state == 1)//单拍
					g_wenshi.camera_state = 0;
				p_info("JpegDataCnt:%d --- ",g_tCam.JpegDataCnt);
				
				App_picture_send_proccess();// 处理图片数据
				g_key_press = 1;

				g_Picture.work_state = PICTURE_START_CAPTURE;
			}	
			
			break;	
		}

		case PICTURE_TIMER_OUT :
		{
			
			break;
		}

		
		default:
			break;
	}

	

}


