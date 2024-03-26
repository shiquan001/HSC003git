/*
*********************************************************************************************************
*
*   ģ������ :   ͨ��ģ��
*   �ļ����� : app_main.c
*   ��    �� : V1.0
*   ˵    �� : ���� Ӧ�ò�   ����ģ����ļ���
              ��Ҫ�ṩ app_tasks() ��������������á�
*   �޸ļ�¼ :
*       �汾��  ����        ����     ˵��
*       V1.0    2022-01-01    ��ʽ����
*
*   Copyright (C), , 2022-2032,   
*
*********************************************************************************************************
*/
#include "app_main.h"
#include "stm32l4xx_hal.h"
#include "main.h"
#include "app.h"
#include "bsp.h"
#include "bsp_timer.h"

#define TEST_ENABLE  //����ʹ�ܿ���

// ʡ��MX_GPIO_Init������SystemClock_Config������ʵ��...
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// ����һ���ṹ��������GPIO�˿ں�������Ϣ
typedef struct 
{
    GPIO_TypeDef* GPIOx;//GPIO_TypeDef
    uint16_t GPIO_Pin;
} 
GPIO_PinInfo;

// ����һ�����飬��������Ҫ���Ƶ�GPIO����,p400
GPIO_PinInfo gpioPinsP400[] = 
{
    {GPIOB, GPIO_PIN_12},
    {GPIOB, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_14},
    {GPIOB, GPIO_PIN_15},
    {GPIOC, GPIO_PIN_7},
    {GPIOC, GPIO_PIN_8},
    {GPIOA, GPIO_PIN_8},
    {GPIOA, GPIO_PIN_9},
    {GPIOA, GPIO_PIN_10},
    {GPIOA, GPIO_PIN_11},
};
int numPinsP400 = sizeof(gpioPinsP400) / sizeof(GPIO_PinInfo);

static void MX_GPIO_Init_P400(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

    /*Configure GPIO pins : PB12 PB13 PB14 PB15 */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : PC7 PC8 */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PA8 PA9 PA10 PA11 */
    GPIO_InitStruct.Pin =  GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


// ����P400����
void app_test_p400task(void)
{
    //test MX_GPIO_Init_P400
    for (int i = 0; i < numPinsP400; ++i) 
    {
        // ����ǰ������Ϊ�ߵ�ƽ
        HAL_GPIO_WritePin(gpioPinsP400[i].GPIOx, gpioPinsP400[i].GPIO_Pin, GPIO_PIN_SET);
        bsp_DelayMS(100); // �ȴ�1��
        // ����ǰ������Ϊ�͵�ƽ
        HAL_GPIO_WritePin(gpioPinsP400[i].GPIOx, gpioPinsP400[i].GPIO_Pin, GPIO_PIN_RESET);
    }
}


// ����һ�����飬��������Ҫ���Ƶ�GPIO����,p400
GPIO_PinInfo gpioPinsP500[] = 
{
    {GPIOB, GPIO_PIN_10},
    {GPIOA, GPIO_PIN_7},
    {GPIOA, GPIO_PIN_6},
    {GPIOA, GPIO_PIN_5},
    {GPIOA, GPIO_PIN_4},
    {GPIOA, GPIO_PIN_15},
    {GPIOB, GPIO_PIN_6},
    {GPIOB, GPIO_PIN_7},
    {GPIOB, GPIO_PIN_8},
    {GPIOB, GPIO_PIN_9},
};
int numPinsP500 = sizeof(gpioPinsP500) / sizeof(GPIO_PinInfo);

static void MX_GPIO_Init_P500(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_15, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

    /*Configure GPIO pins */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


// ����P400����
void app_test_p500task(void)
{
    //test MX_GPIO_Init_P400
    for (int i = 0; i < numPinsP500; ++i) 
    {
        // ����ǰ������Ϊ�ߵ�ƽ
        HAL_GPIO_WritePin(gpioPinsP500[i].GPIOx, gpioPinsP500[i].GPIO_Pin, GPIO_PIN_SET);
        bsp_DelayMS(100); // �ȴ�1��
        // ����ǰ������Ϊ�͵�ƽ
        HAL_GPIO_WritePin(gpioPinsP500[i].GPIOx, gpioPinsP500[i].GPIO_Pin, GPIO_PIN_RESET);
    }
}



// ����һ�����飬��������Ҫ���Ƶ�GPIO����,p401
GPIO_PinInfo gpioPinsP401[] = 
{
    {GPIOB, GPIO_PIN_11},
    {GPIOA, GPIO_PIN_3},
    {GPIOB, GPIO_PIN_2},
    {GPIOB, GPIO_PIN_1},
};
int numPinsP401 = sizeof(gpioPinsP401) / sizeof(GPIO_PinInfo);

static void MX_GPIO_Init_P401(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_11, GPIO_PIN_RESET);

    /*Configure GPIO pins */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_2|GPIO_PIN_1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


// ����P401����
void app_test_p401task(void)
{
    //test MX_GPIO_Init_P400
    for (int i = 0; i < numPinsP401; ++i) 
    {
        // ����ǰ������Ϊ�ߵ�ƽ
        HAL_GPIO_WritePin(gpioPinsP401[i].GPIOx, gpioPinsP401[i].GPIO_Pin, GPIO_PIN_SET);
        bsp_DelayMS(100); // �ȴ�1��
        // ����ǰ������Ϊ�͵�ƽ
        HAL_GPIO_WritePin(gpioPinsP401[i].GPIOx, gpioPinsP401[i].GPIO_Pin, GPIO_PIN_RESET);
    }
}
/*
*********************************************************************************************************
*   �� �� ��: app_main_test
*   ����˵��: ����ʹ��
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_test_task(void)
{
    #ifdef TEST_ENABLE

    MX_GPIO_Init_P400();//GPIO��ʼ��
    MX_GPIO_Init_P500();//GPIO��ʼ��
    MX_GPIO_Init_P401();//GPIO��ʼ��

    ENABLE_INT();//���ж�

    while (1)
    {
        bsp_Idle();//����

        //test MX_GPIO_Init_P400
        app_test_p400task();
        app_test_p500task();
        app_test_p401task();
    }
    
    #endif
}

/*
*********************************************************************************************************
*   �� �� ��: app_tasks
*   ����˵��: 
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void app_tasks(void)
{
    app_test_task();//����ʹ��
    
}


/*****************************    (END OF FILE) *********************************/

