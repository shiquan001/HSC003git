/*
*********************************************************************************************************
*
*	??????? : ????????
*	??????? : bsp_timer.c
*	??    ?? : V1.2
*	?    ?? : ????TIM4???????????δ??????TIM1??TIM2?????????????TIM4??????????????????TIM1
*			   ??TIM2??????????????????????????????????? TIM4????δ??????????????1ms??
*
*				?????4???????????????????????(????10ms)
*				?????ms????????????????2ms??
*				?????us??????????
*				?????????????�????10ms??λ??
*
*	????? :
*		?�??  ????       ????    ???
*		v1.0    2012-10-12 armfly  ST?????�?V2.1.0
*		v1.1    2014-06-22 armfly  ??? bsp_DelayUS?? ???16M??? 2000us??? 1.9ms
*		V1.2	2014-09-11 armfly  ????TIM2 ?????????????us??????????????3??CC??????????3??????
*									?????????????????????????????????????
*
*	Copyright (C), 2012-2013, ?????????? www.armfly.com
*
*********************************************************************************************************
*/


#include "stm32l4xx_hal.h"
#include "bsp_timer.h"
#include "bsp.h"


#define ENABLE_SYSTICK_INT()	//enableInterrupts()//TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE)
#define DISABLE_SYSTICK_INT()	//disableInterrupts()//TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE)

/*
	??????????????????????????
	???????????__IO ?? volatile???????????????ж????????????????????п????????????????????
*/

static __IO uint32_t s_uiDelayCount = 0;
static __IO uint8_t s_ucTimeOutFlag = 0;

SOFT_TMR g_Tmr[TMR_COUNT];

/* ????????????λ1ms */
 uint32_t g_iRunTime = 0;
#if 0
/* ???? TIM2????ж??????е?????????? */
static void (*s_TIM2_CallBack1)(void);
static void (*s_TIM2_CallBack2)(void);
static void (*s_TIM2_CallBack3)(void);
static void TIM4_Config(void);
#endif
static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

/*
*********************************************************************************************************
*	?? ?? ??: SysTick_ISR
*	???????: SysTick?ж??????????2ms????1??
*	??    ?Σ???
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/*
		STM8S ?????????????????HSI RC???(16MHz)??8???????fHSI/8 = 2MHz
	*/
	//bsp_ConfgiSysClk(CPU_SYS_CLK_INIT);		/* ???????? */

	/* ???????е?????????? */
	for (i = 0; i < TMR_COUNT; i++)
	{
		g_Tmr[i].Count = 0;
		g_Tmr[i].PreLoad = 0;
		g_Tmr[i].Flag = 0;
		g_Tmr[i].Mode = TMR_ONCE_MODE;	/* ????1????????? */
	}

	//TIM4_Config();
}

/*
*********************************************************************************************************
*	?? ?? ??: SysTick_ISR
*	???????: SysTick?ж??????????2ms????1??
*	??    ?Σ???
*	?? ?? ?: ??
*********************************************************************************************************
*/
extern void bsp_RunPer10ms(void);
extern void bsp_RunPer1ms(void);

void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;
	#if 0
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
	#endif
	//TIM4_ClearFlag(TIM4_FLAG_UPDATE);

	/* ???1ms????1?? */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
        {
			s_ucTimeOutFlag = 1;
		}
	}

	/* ???1ms ??????? */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&g_Tmr[i]);
	}
	
	g_iRunTime++;	/* ???????????1ms??1,  24.8???????? */

	bsp_RunPer1ms();	/* ??????? bsp.c ?????(???????) */
		
	if (++s_count < (10 / SYSTICK_PERIOD))
	{
		return;
	}

	s_count = 0;

	bsp_RunPer10ms();	/* ??????? bsp.c ?????(???????) */
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_SoftTimerDec
*	???????: ?????ж??????????1??????SysTick_ISR????????á?
*	??    ?Σ?_tmr : ????????????
*	?? ?? ?: ??
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
//	if (_tmr->Flag == 0)
	{
		if (_tmr->Count > 0)
		{
			/* ????????????????1???????????????? */
			if (--_tmr->Count == 0)
			{
				_tmr->Flag = 1;

				/* ?????????????????????????? */
				if(_tmr->Mode == TMR_AUTO_MODE)
				{
					_tmr->Count = _tmr->PreLoad;
				}
			}
		}
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_DelayMS
*	???????: ms?????????????????2ms
*	??    ?Σ?n : ?????????λ1 ms?? n ?????2
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	DISABLE_SYSTICK_INT();  			/* ???ж? */
	s_uiDelayCount = n / SYSTICK_PERIOD;
	s_ucTimeOutFlag = 0;
	ENABLE_SYSTICK_INT();  				/* ???ж? */

	while (1)
	{
		bsp_Idle();

		/* ????????? */
		/* ???????????? s_ucTimeOutFlag = 0????????????????
		??? s_ucTimeOutFlag ?????????????_IO_?? */
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_StartTimer
*	???????: ???????????????????????????
*	??    ?Σ?	_id     : ?????ID?????0,TMR_COUNT-1???????????????????????ID??????????ID?????
*				_period : ??????????λ10ms
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ???? */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  			/* ???ж? */

	g_Tmr[_id].Count = _period;			/* ??????????? */
	g_Tmr[_id].PreLoad = _period;		/* ???????????????????????????? */
	g_Tmr[_id].Flag = 0;				/* ????????? */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* 1????????? */

	ENABLE_SYSTICK_INT();  				/* ???ж? */
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_StartAutoTimer
*	???????: ??????????????????????????????
*	??    ?Σ?	_id     : ?????ID?????0,TMR_COUNT-1???????????????????????ID??????????ID?????
*				_period : ??????????λ10ms
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ???? */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  		/* ???ж? */

	g_Tmr[_id].Count = _period;			/* ??????????? */
	g_Tmr[_id].PreLoad = _period;		/* ???????????????????????????? */
	g_Tmr[_id].Flag = 0;				/* ????????? */
	g_Tmr[_id].Mode = TMR_AUTO_MODE;	/* ????????? */

	ENABLE_SYSTICK_INT();  		/* ???ж? */
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_StopTimer
*	???????: ??????????
*	??    ?Σ?	_id     : ?????ID?????1,TMR_COUNT-1???????????????????????ID??????????ID?????
*						  ?????ID = 0 ??????bsp_DelayMS()????
*	?? ?? ?: ??
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ???? */
		return;
	}

	DISABLE_SYSTICK_INT();  	/* ???ж? */

	g_Tmr[_id].Count = 0;				/* ??????????? */
	g_Tmr[_id].Flag = 0;				/* ????????? */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* ????????? */

	ENABLE_SYSTICK_INT();  		/* ???ж? */
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_CheckTimer
*	???????: ??????????
*	??    ?Σ?	_id     : ?????ID?????0,TMR_COUNT-1???????????????????????ID??????????ID?????
*	?? ?? ?: ???? 0 ??????δ???? 1????????
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (g_Tmr[_id].Flag == 1)
	{
		g_Tmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_SetTimer
*	???????: ????id
*	??    ?Σ?	_id     : ?????ID?????0,TMR_COUNT-1???????????????????????ID??????????ID?????
*	?? ?? ?: ???? 0 ??????δ???? 1????????
*********************************************************************************************************
*/
uint8_t bsp_SetTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	//if (g_Tmr[_id].Flag == 1)
	{
		g_Tmr[_id].Flag = 1;
		return 1;
	}

}

/*
*********************************************************************************************************
*	?? ?? ??: bsp_GetRunTime
*	???????: ???CPU???????
*	??    ?Σ???
*	?? ?? ?: CPU?????????λ10ms
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	uint16_t runtime;

	DISABLE_SYSTICK_INT();  		/* ???ж? */

	runtime = g_iRunTime;	        /* ??????Systick?ж????д???????ж???б??? */

	ENABLE_SYSTICK_INT();  		    /* ???ж? */

	return runtime;
}
#if 0

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer
*	����˵��: ���� TIMx������us����Ӳ����ʱ��TIMx���������У�����ֹͣ.
*			TIMx������TIM2 - TIM5 ֮���TIM, ��ЩTIM��4��ͨ��, ���� APB1 �ϣ�����ʱ��=SystemCoreClock / 2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint32_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

	/* ʹ��TIMʱ�� */
	RCC_APB1PeriphClockCmd(TIM_HARD_RCC, ENABLE);

	/*-----------------------------------------------------------------------
		system_stm32f4xx.c �ļ��� void SetSysClock(void) ������ʱ�ӵ��������£�

		HCLK = SYSCLK / 1	  (AHB1Periph)
		PCLK2 = HCLK / 2	  (APB2Periph)
		PCLK1 = HCLK / 4	  (APB1Periph)

		��ΪAPB1 prescaler != 1, ���� APB1�ϵ�TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		��ΪAPB2 prescaler != 1, ���� APB2�ϵ�TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 ��ʱ���� TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
		APB2 ��ʱ���� TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	uiTIMxCLK = SystemCoreClock / 2;

	usPrescaler = uiTIMxCLK / 1000000 ; /* ��Ƶ������ 1us */

#if defined (USE_TIM2) || defined (USE_TIM5) 
	usPeriod = 0xFFFFFFFF;
#else
	usPeriod = 0xFFFF;
#endif
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_HARD, &TIM_TimeBaseStructure);

	//TIM_ARRPreloadConfig(TIMx, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM_HARD, ENABLE);

	/* ����TIM��ʱ�ж� (Update) */
	{
		NVIC_InitTypeDef NVIC_InitStructure;	/* �жϽṹ���� misc.h �ж��� */

		NVIC_InitStructure.NVIC_IRQChannel = TIM_HARD_IRQn;

		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	/* �ȴ������ȼ��� */
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM2-5�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����4����ʱ�����������š�
*             ��ʱ��������10us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*			 TIM2��TIM5 ��32λ��ʱ������ʱ��Χ�ܴ�
*			 TIM3��TIM4 ��16λ��ʱ����
*	��    ��: _CC : ����ͨ������1��2��3, 4
*             _uiTimeOut : ��ʱʱ��, ��λ 1us.       ����16λ��ʱ������� 65.5ms; ����32λ��ʱ������� 4294��
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = TIM_GetCounter(TIM_HARD);    	/* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _uiTimeOut;			/* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIM_HARD, cnt_tar);      	/* ���ò���Ƚϼ�����CC1 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC1, ENABLE);	/* ʹ��CC1�ж� */

    }
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIM_HARD, cnt_tar);      	/* ���ò���Ƚϼ�����CC2 */
		TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC2, ENABLE);	/* ʹ��CC2�ж� */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIM_HARD, cnt_tar);      	/* ���ò���Ƚϼ�����CC3 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC3, ENABLE);	/* ʹ��CC3�ж� */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIM_HARD, cnt_tar);      	/* ���ò���Ƚϼ�����CC4 */
		TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC4, ENABLE);	/* ʹ��CC4�ж� */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
*	�� �� ��: TIMx_IRQHandler
*	����˵��: TIM �жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

#ifdef USE_TIM2
void TIM2_IRQHandler(void)
#endif

#ifdef USE_TIM3
void TIM3_IRQHandler(void)
#endif

#ifdef USE_TIM4
void TIM4_IRQHandler(void)
#endif

#ifdef USE_TIM5
void TIM5_IRQHandler(void)
#endif
{
    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC1, DISABLE);	/* ����CC1�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC2, DISABLE);	/* ����CC2�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC3, DISABLE);	/* ����CC3�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC4, DISABLE);	/* ����CC4�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM_CallBack4();
    }
}

#endif
/***************************** ?????????? www.armfly.com (END OF FILE) *********************************/