/*
*********************************************************************************************************
*
*	ģ������ :  
*	�ļ����� :  .c
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
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

/* ����FIFO�õ����� */
#define _FIFO_SIZE	 36//10
typedef struct
{
	char Buf[_FIFO_SIZE][JSON_BUF_LEN+sizeof(uint16)];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}_FIFO_T;

 _FIFO_T s_tFIFO;		/* ����FIFO����,�ṹ�� */
static void app_fifo_NB_CoapST_InitVar(void);
//char m_bufCopy[JSON_BUF_LEN+sizeof(uint16)];		/* �������� */
json_info_t m_bufCopy;

/*
*********************************************************************************************************
*	�� �� ��: app_fifo_NB_CoapST_Init
*	����˵��: ��ʼ������. �ú����� app_fifo_NB_CoapST_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void app_fifo_NB_CoapST_Init(void)
{
	app_fifo_NB_CoapST_InitVar();		/* ��ʼ���������� */
	bsp_StartAutoTimer(TMR_ID_app_fifo_NB_CoapST,TMR_DELAY_0_5S);	
}

/*
*********************************************************************************************************
*	�� �� ��: app_fifo_NB_CoapST_Put
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _Code : ��������
*	�� �� ֵ: ��
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
*	�� �� ��: app_fifo_NB_CoapST_Get
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
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
*	�� �� ��: app_fifo_NB_CoapST_Clear
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void app_fifo_NB_CoapST_Clear(void)
{
	s_tFIFO.Read = s_tFIFO.Write;
}


/*
*********************************************************************************************************
*	�� �� ��: app_fifo_NB_CoapST_InitVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void app_fifo_NB_CoapST_InitVar(void)
{
	memset(&s_tFIFO,0,sizeof(s_tFIFO));
	
	/* �԰���FIFO��дָ������ */
	s_tFIFO.Read = 0;
	s_tFIFO.Write = 0;
	s_tFIFO.Read2 = 0;
}
/*
*********************************************************************************************************
*	�� �� ��: App_fifoNBCoapST_putAgain
*	����˵��: ����ʧ��  ���¼���fifo
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_fifoNBCoapST_putAgain(void)
{
	app_fifo_NB_CoapST_Put(m_bufCopy.json_buf,&m_bufCopy.json_len);
}

/*
*********************************************************************************************************
*	�� �� ��: App_caiji_PLC_Loop
*	����˵��: �ɼ� PLC
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_fifo_NB_CoapST_Loop(void)
{
	/* 0.5S	��ʱ��ѯ�����ƽ��� */
	if(App_nb_get_APPSTATE()&&state_get()&&bsp_CheckTimer(TMR_ID_app_fifo_NB_CoapST))
	//if(App_nb_get_APPSTATE()&&state_get())
	{
		uint8_t Code = 0;
		Code = app_fifo_NB_CoapST_Get(&json_info.json_buf[0],&json_info.json_len); 		
		if(Code != _NONE)
		{
			/* ��������*/
			memcpy(&m_bufCopy,&json_info.json_buf[0],sizeof(json_info_t));
			
				
			APP_STATE = NB_CoAP_ST;   //��������					
		}
	}				
}


/*****************************    (END OF FILE) *********************************/
