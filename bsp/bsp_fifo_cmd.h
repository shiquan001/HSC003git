/*
*********************************************************************************************************
*
*	ģ������ :  
*	�ļ����� : bsp_fifo_cmd.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef __BSP_FIFO_CMD_H
#define __BSP_FIFO_CMD_H


#define Cmd_NONE 0

typedef struct
{
    uint16_t mid;
    uint8_t valuetype;
    uint8_t occupied;
    int32_t value;
}
RXVALUE_TYPE_ST;

typedef union
{
    RXVALUE_TYPE_ST    rxVale;
    char         rxdata[8];
}RXVALUE_TYPE_UN;



/* ����FIFO�õ����� */
#define Cmd_FIFO_SIZE	72//60//12
typedef struct
{
	RXVALUE_TYPE_ST Buf[Cmd_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}CMD_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitCmd(void);
void bsp_PutCmd(RXVALUE_TYPE_ST * _CmdCode);
uint32_t bsp_GetCmd(RXVALUE_TYPE_ST * _CmdCode);
void bsp_ClearCmd(void);
uint32_t bsp_GetFIFOState_IsNull(void);
uint32_t bsp_GetLastrCmd(RXVALUE_TYPE_ST * _CmdCode);

#endif

/*****************************    (END OF FILE) *********************************/
