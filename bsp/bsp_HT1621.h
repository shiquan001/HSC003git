#ifndef __HT1621_H
#define __HT1621_H


#include "stm32l4xx_hal.h"
//#include "my_conf.h"


	
//ϵͳ�궨��
 #define BIAS                  0x52        //0b100001010010    1/3duty 4com
 //#define BIAS                0x42        //0b100001001010        1/3duty 3com
 //#define BIAS                0x4a        //0b100001010010        1/3duty 2com
 #define SYSDIS                0x00        //����ϵͳ��������LCDƫѹ������
 #define SYSEN                 0x02        //��ϵͳ������
 
#define LCDOFF                 0x04                //��LCDƫѹ
 #define LCDON                 0x06                //
 
#define XTAL                   0x28                //���ʱ��
 #define RC256                 0x30                //�ڲ�ʱ��
 
#define TONEON                 0x12                //������
 #define TONEOFF               0x10                //�ر�����
 
#define WDTDIS                 0x0a                //��ֹ���Ź�
 // ���Ź�
#define WDTEN                0x0e                //ʹ�ܿ��Ź�
#define CLRWDT   0x1c                //ʹ�ܿ��Ź�
#define IRQEN 0x01 //ʹ��IRQ output

/********** 

GPIO_CLOCK_ENABLE 

HT_CS	PB15
HT_RD	PC6
HT_WR	PC7
HT_DATA	PC8

***********/	
#define CS_CLOCK_ENABLE()  	__HAL_RCC_GPIOB_CLK_ENABLE()
#define WR_CLOCK_ENABLE()  	__HAL_RCC_GPIOC_CLK_ENABLE()
#define DA_CLOCK_ENABLE()  	__HAL_RCC_GPIOC_CLK_ENABLE()
#define RD_CLOCK_ENABLE()  	__HAL_RCC_GPIOC_CLK_ENABLE()

#define CS_PORTS  	HT_CS_GPIO_Port
#define RD_PORTS  	HT_RD_GPIO_Port
#define WR_PORTS  	HT_WR_GPIO_Port
#define DA_PORTS  	HT_DATA_GPIO_Port

#define CS_GPIO		HT_CS_Pin
#define RD_GPIO		HT_RD_Pin
#define WR_GPIO		HT_WR_Pin
#define DA_GPIO		HT_DATA_Pin

//�����ø�  
// HAL_GPIO_WritePin(NB_EN_GPIO_Port, NB_EN_Pin, GPIO_PIN_RESET);
 #define        WR_HIGH()          HAL_GPIO_WritePin(WR_PORTS,WR_GPIO,GPIO_PIN_SET)
 #define        CS_HIGH()          HAL_GPIO_WritePin(CS_PORTS,CS_GPIO,GPIO_PIN_SET)
 #define        DA_HIGH()          HAL_GPIO_WritePin(DA_PORTS,DA_GPIO,GPIO_PIN_SET)
 #define        RD_HIGH()          HAL_GPIO_WritePin(RD_PORTS,RD_GPIO,GPIO_PIN_SET) 
 //�����õ�
 #define        WR_LOW()           HAL_GPIO_WritePin(WR_PORTS, WR_GPIO, GPIO_PIN_RESET)
 #define        CS_LOW()           HAL_GPIO_WritePin(CS_PORTS, CS_GPIO, GPIO_PIN_RESET)
 #define        DA_LOW()           HAL_GPIO_WritePin(DA_PORTS, DA_GPIO, GPIO_PIN_RESET)
 #define        RD_LOW()           HAL_GPIO_WritePin(RD_PORTS, RD_GPIO, GPIO_PIN_RESET)
 



//��ʾwifi �źŵȼ�
#define  FLAG_SHOW_WIFI_LEVEL_0		0
#define  FLAG_SHOW_WIFI_LEVEL_1		1
#define  FLAG_SHOW_WIFI_LEVEL_2		2
#define  FLAG_SHOW_WIFI_LEVEL_3		3
#define  FLAG_SHOW_WIFI_LEVEL_4		4

//������ʾ������ʼʱ��ͽ���ʱ��ı�־
//��ʾ��ʼ����ʱ��
#define FLAG_SHOW_CON_LIGHT_ON					0x01
//��ʾ��������ʱ��
#define FLAG_SHOW_CON_LIGHT_OFF					0x02
//��������ʾ
#define FLAG_SHOW_CON_LIGHT_ON_OFF			0x03

//������ʾʱ��ı�־
//������ʾʱ���Сʱ
#define FLAG_SHOW_TIME_HOURS						0x01
//������ʾʱ��ķ���
#define FLAG_SHOW_TIME_MINS							0x02
//��������ʾ
#define FLAG_SHOW_TIME_HOURS_MINS				0x03



#define DATA_LENTH 32 			// ram ���� 34			

typedef struct
{
	/* 0000,D0 D1 D2 D3*/
	uint8_t SEG0_31[DATA_LENTH];
}LCD_COM_1_4;

extern LCD_COM_1_4 g_LcdCom;


extern  void bsp_HT1621_Init(void);

extern  void bsp_HT1621B_CloseDisplay(void);
extern  void bsp_HT1621B_ClearDisplay(void);

extern void bsp_HT1621_Show_NULL(void);

void bsp_HT1621B_Send_AllData(void);
void bsp_HT1621_Show_ALL(void);
void bsp_HT1621_Show_NULL(void);


#endif
