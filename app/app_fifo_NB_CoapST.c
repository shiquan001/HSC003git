/*
*********************************************************************************************************
*
*	模块名称 :  
*	文件名称 :  .c
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		
*********************************************************************************************************
*/
#include "app.h"

#include "app_fifo_NB_CoapST.h"
#include "NB_BC95.h"

#define NRF_ERROR_BASE_NUM      (0x0)       ///< Global error base
#define NRF_ERROR_NULL                        (NRF_ERROR_BASE_NUM + 14) ///< Null Pointer

/**@brief Macro for verifying statement to be false. It will cause the exterior function to return
 *        err_code if the statement is not false.
 *
 * @param[in]   statement   Statement to test.
 * @param[in]   err_code    Error value to return if test was invalid.
 *
 * @retval      nothing, but will cause the exterior function to return @p err_code if @p statement
 *              is true.
 */
#define VERIFY_FALSE(statement, err_code)   \
do                                          \
{                                           \
    if ((statement))                        \
    {                                       \
        return err_code;                    \
    }                                       \
} while (0)


/**@brief Macro for verifying that the module is initialized. It will cause the exterior function to
 *        return if not.
 *
 * @param[in] param  The variable to check if is NULL.
 */
#define VERIFY_PARAM_NOT_NULL(param) VERIFY_FALSE(((param) == NULL), NRF_ERROR_NULL)


#define _NONE 0

/* 按键FIFO用到变量 */
#define _FIFO_SIZE	 36//10
typedef struct
{
	char Buf[_FIFO_SIZE][JSON_BUF_LEN+sizeof(uint16)];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}_FIFO_T;

 _FIFO_T s_tFIFO;		/* 按键FIFO变量,结构体 */
static void app_fifo_NB_CoapST_InitVar(void);
//char m_bufCopy[JSON_BUF_LEN+sizeof(uint16)];		/* 备份区域 */
json_info_t m_bufCopy;

/*
*********************************************************************************************************
*	函 数 名: app_fifo_NB_CoapST_Init
*	功能说明: 初始化按键. 该函数被 app_fifo_NB_CoapST_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void app_fifo_NB_CoapST_Init(void)
{
	app_fifo_NB_CoapST_InitVar();		/* 初始化按键变量 */
	bsp_StartAutoTimer(TMR_ID_app_fifo_NB_CoapST,TMR_DELAY_0_5S);	
}

/*
*********************************************************************************************************
*	函 数 名: app_fifo_NB_CoapST_Put
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _Code : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t app_fifo_NB_CoapST_Put(char * p_byte_array, uint16_t * p_size)
{
	VERIFY_PARAM_NOT_NULL(p_size);
    uint16_t requested_len   = (*p_size);

	memcpy(&s_tFIFO.Buf[s_tFIFO.Write][0],&requested_len,sizeof(requested_len));
	memcpy(&s_tFIFO.Buf[s_tFIFO.Write][0+sizeof(requested_len)],p_byte_array,requested_len);
	
	if (++s_tFIFO.Write  >= _FIFO_SIZE)
	{
		s_tFIFO.Write = 0;
	}
	return TRUE;
}

/*
*********************************************************************************************************
*	函 数 名: app_fifo_NB_CoapST_Get
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t app_fifo_NB_CoapST_Get(char * p_byte,uint16_t * p_size)
{

    uint16_t requested_len   = 0;

	uint8_t ret;

	if (s_tFIFO.Read == s_tFIFO.Write)
	{
		return _NONE;
	}
	else
	{
		memcpy(&requested_len,&s_tFIFO.Buf[s_tFIFO.Read][0],sizeof(requested_len));
		memcpy(p_byte,&s_tFIFO.Buf[s_tFIFO.Read][0+sizeof(requested_len)],requested_len);
	
		ret = TRUE;
		memcpy((uint8_t *)p_size,(uint8_t *)&requested_len,sizeof(requested_len));

		if (++s_tFIFO.Read >= _FIFO_SIZE)
		{
			s_tFIFO.Read = 0;
		}
		return ret;
	}
}


/*
*********************************************************************************************************
*	函 数 名: app_fifo_NB_CoapST_Clear
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void app_fifo_NB_CoapST_Clear(void)
{
	s_tFIFO.Read = s_tFIFO.Write;
}


/*
*********************************************************************************************************
*	函 数 名: app_fifo_NB_CoapST_InitVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void app_fifo_NB_CoapST_InitVar(void)
{
	memset(&s_tFIFO,0,sizeof(s_tFIFO));
	
	/* 对按键FIFO读写指针清零 */
	s_tFIFO.Read = 0;
	s_tFIFO.Write = 0;
	s_tFIFO.Read2 = 0;
}
/*
*********************************************************************************************************
*	函 数 名: App_fifoNBCoapST_putAgain
*	功能说明: 发送失败  重新加入fifo
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_fifoNBCoapST_putAgain(void)
{
	app_fifo_NB_CoapST_Put(m_bufCopy.json_buf,&m_bufCopy.json_len);
}

/*
*********************************************************************************************************
*	函 数 名: App_caiji_PLC_Loop
*	功能说明: 采集 PLC
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_fifo_NB_CoapST_Loop(void)
{
	/* 0.5S	定时查询，控制节奏 */
	if(App_nb_get_APPSTATE()&&state_get()&&bsp_CheckTimer(TMR_ID_app_fifo_NB_CoapST))
	//if(App_nb_get_APPSTATE()&&state_get())
	{
		uint8_t Code = 0;
		Code = app_fifo_NB_CoapST_Get(&json_info.json_buf[0],&json_info.json_len); 		
		if(Code != _NONE)
		{
			/* 备份数据*/
			memcpy(&m_bufCopy,&json_info.json_buf[0],sizeof(json_info_t));
			
				
			APP_STATE = NB_CoAP_ST;   //发送数据					
		}
	}				
}


/*****************************    (END OF FILE) *********************************/
