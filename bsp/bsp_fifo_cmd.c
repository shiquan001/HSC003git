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
#include "bsp.h"

#include "bsp_fifo_cmd.h"


CMD_FIFO_T s_tCMD;		/* 按键FIFO变量,结构体 */
static void bsp_InitCmdVar(void);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitCmd
*	功能说明: 初始化按键. 该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitCmd(void)
{
	bsp_InitCmdVar();		/* 初始化按键变量 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutCmd
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参:  _CmdCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutCmd(RXVALUE_TYPE_ST * _CmdCode)
{
	memcpy((uint8_t *) &s_tCMD.Buf[s_tCMD.Write],(uint8_t *)_CmdCode,sizeof(RXVALUE_TYPE_ST));
	//s_tCMD.Buf[s_tCMD.Write] = _CmdCode;

	if (++s_tCMD.Write  >= Cmd_FIFO_SIZE)
	{
		s_tCMD.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetLastrCmd
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint32_t bsp_GetLastrCmd(RXVALUE_TYPE_ST * _CmdCode)
{
	memcpy((uint8_t *)_CmdCode,(uint8_t *) &s_tCMD.Buf[s_tCMD.Read],sizeof(RXVALUE_TYPE_ST));
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetCmd
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint32_t bsp_GetCmd(RXVALUE_TYPE_ST * _CmdCode)
{
//	uint32_t ret;

	if (s_tCMD.Read == s_tCMD.Write)
	{
		return Cmd_NONE;
	}
	else
	{
		//ret = s_tCMD.Buf[s_tCMD.Read];
		memcpy((uint8_t *)_CmdCode,(uint8_t *) &s_tCMD.Buf[s_tCMD.Read],sizeof(RXVALUE_TYPE_ST));

		if (++s_tCMD.Read >= Cmd_FIFO_SIZE)
		{
			s_tCMD.Read = 0;
		}
		return 1;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetFIFOState_IsNull
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint32_t bsp_GetFIFOState_IsNull(void)
{
//	uint32_t ret;

	if (s_tCMD.Read == s_tCMD.Write)
	{
		return 1;//没有cmd了，返回1
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearCmd
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearCmd(void)
{
	s_tCMD.Read = s_tCMD.Write;
}


/*
*********************************************************************************************************
*	函 数 名: bsp_InitCmdVar
*	功能说明: 初始化按键变量
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitCmdVar(void)
{
	memset(&s_tCMD,0,sizeof(s_tCMD));
	
	/* 对按键FIFO读写指针清零 */
	s_tCMD.Read = 0;
	s_tCMD.Write = 0;
	s_tCMD.Read2 = 0;
}


/*****************************    (END OF FILE) *********************************/
