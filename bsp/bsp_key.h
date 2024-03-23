/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define KEY_COUNT   12 /*��������*/

///* ����Ӧ�ó���Ĺ��������������� */
//#define KEY_DOWN_K1		KEY_1_DOWN
//#define KEY_UP_K1		KEY_1_UP
//#define KEY_LONG_K1		KEY_1_LONG

//#define KEY_DOWN_K2		KEY_2_DOWN
//#define KEY_UP_K2		KEY_2_UP
//#define KEY_LONG_K2		KEY_2_LONG

//#define KEY_DOWN_K3		KEY_3_DOWN
//#define KEY_UP_K3		KEY_3_UP
//#define KEY_LONG_K3		KEY_3_LONG

//#define JOY_DOWN_U		KEY_4_DOWN		/* �� */
//#define JOY_UP_U		KEY_4_UP
//#define JOY_LONG_U		KEY_4_LONG

//#define JOY_DOWN_D		KEY_5_DOWN		/* �� */
//#define JOY_UP_D		KEY_5_UP
//#define JOY_LONG_D		KEY_5_LONG

//#define JOY_DOWN_L		KEY_6_DOWN		/* �� */
//#define JOY_UP_L		KEY_6_UP
//#define JOY_LONG_L		KEY_6_LONG

//#define JOY_DOWN_R		KEY_7_DOWN		/* �� */
//#define JOY_UP_R		KEY_7_UP
//#define JOY_LONG_R		KEY_7_LONG

//#define JOY_DOWN_OK		KEY_8_DOWN		/* ok */
//#define JOY_UP_OK		KEY_8_UP
//#define JOY_LONG_OK		KEY_8_LONG

//#define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 ��ϼ� */
//#define SYS_UP_K1K2	    KEY_9_UP
//#define SYS_LONG_K1K2	KEY_9_LONG

//#define SYS_DOWN_K2K3	KEY_10_DOWN		/* K2 K3 ��ϼ� */
//#define SYS_UP_K2K3  	KEY_10_UP
//#define SYS_LONG_K2K3	KEY_10_LONG

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_K1 = 0,
	KID_K2,
	KID_K3,
	KID_JOY_U,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
	KID_JOY_OK
}KEY_ID_E;


/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   2//5ms
#define KEY_LONG_TIME     1000/* ��λ1ms�� ����1�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_select_DOWN,				/* ѡ����ͷ��ַ������ */
	KEY_select_UP,					/* 1������ */
	KEY_select_LONG,				/* 1������ */

	KEY_DOWN_DOWN,				/* 2������ */
	KEY_DOWN_UP,				/* 2������ */
	KEY_DOWN_LONG,				/* 2������ */

	KEY_SelfCheck_DOWN,				/* 3������ */
	KEY_SelfCheck_UP,				/* 3������ */
	KEY_SelfCheck_LONG,				/* 3������ */

	KEY_Valve_DOWN,				/* 4������ */
	KEY_Valve_UP,				/* 4������ */
	KEY_Valve_LONG,				/* 4������ */

	KEY_Right_DOWN,				/* 5������ */
	KEY_Right_UP,				/* 5������ */
	KEY_Right_LONG,				/* 5������ */

	KEY_Keyboard_Lock_DOWN,				/* 6������ */
	KEY_Keyboard_Lock_UP,				/* 6������ */
	KEY_Keyboard_Lock_LONG,				/* 6������ */

	KEY_Hand_Switch_DOWN,				/* 7������ */
	KEY_Hand_Switch_UP,				/* 7������ */
	KEY_Hand_Switch_LONG,				/* 7������ */

	KEY_UP_DOWN,				/* 8������ */
	KEY_UP_UP,				/* 8������ */
	KEY_UP_LONG,				/* 8������ */

	/* ��ϼ� */
	KEY_Pump_DOWN,				/* 9������ */
	KEY_Pump_UP,				/* 9������ */
	KEY_Pump_LONG,				/* 9������ */

	KEY_Mute_DOWN,			/* 10������ */
	KEY_Mute_UP,				/* 10������ */
	KEY_Mute_LONG,			/* 10������ */
	
	KEY_Left_DOWN,			/* 10������ */
	KEY_Left_UP,				/* 10������ */
	KEY_Left_LONG,			/* 10������ */
	KEY_Reset_DOWN,			/* 10������ */
	KEY_Reset_UP,				/* 10������ */
	KEY_Reset_LONG,			/* 10������ */
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	12
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif

/*****************************    (END OF FILE) *********************************/