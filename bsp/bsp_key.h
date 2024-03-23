/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014,   
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define KEY_COUNT   12 /*按键个数*/

///* 根据应用程序的功能重命名按键宏 */
//#define KEY_DOWN_K1		KEY_1_DOWN
//#define KEY_UP_K1		KEY_1_UP
//#define KEY_LONG_K1		KEY_1_LONG

//#define KEY_DOWN_K2		KEY_2_DOWN
//#define KEY_UP_K2		KEY_2_UP
//#define KEY_LONG_K2		KEY_2_LONG

//#define KEY_DOWN_K3		KEY_3_DOWN
//#define KEY_UP_K3		KEY_3_UP
//#define KEY_LONG_K3		KEY_3_LONG

//#define JOY_DOWN_U		KEY_4_DOWN		/* 上 */
//#define JOY_UP_U		KEY_4_UP
//#define JOY_LONG_U		KEY_4_LONG

//#define JOY_DOWN_D		KEY_5_DOWN		/* 下 */
//#define JOY_UP_D		KEY_5_UP
//#define JOY_LONG_D		KEY_5_LONG

//#define JOY_DOWN_L		KEY_6_DOWN		/* 左 */
//#define JOY_UP_L		KEY_6_UP
//#define JOY_LONG_L		KEY_6_LONG

//#define JOY_DOWN_R		KEY_7_DOWN		/* 右 */
//#define JOY_UP_R		KEY_7_UP
//#define JOY_LONG_R		KEY_7_LONG

//#define JOY_DOWN_OK		KEY_8_DOWN		/* ok */
//#define JOY_UP_OK		KEY_8_UP
//#define JOY_LONG_OK		KEY_8_LONG

//#define SYS_DOWN_K1K2	KEY_9_DOWN		/* K1 K2 组合键 */
//#define SYS_UP_K1K2	    KEY_9_UP
//#define SYS_LONG_K1K2	KEY_9_LONG

//#define SYS_DOWN_K2K3	KEY_10_DOWN		/* K2 K3 组合键 */
//#define SYS_UP_K2K3  	KEY_10_UP
//#define SYS_LONG_K2K3	KEY_10_LONG

/* 按键ID, 主要用于bsp_KeyState()函数的入口参数 */
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
	按键滤波时间50ms, 单位10ms。
	只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
	即使按键电路不做硬件滤波，该滤波机制也可以保证可靠地检测到按键事件
*/
#define KEY_FILTER_TIME   2//5ms
#define KEY_LONG_TIME     1000/* 单位1ms， 持续1秒，认为长按事件 */

/*
	每个按键对应1个全局的结构体变量。
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t  Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t  RepeatSpeed;	/* 连续按键周期 */
	uint8_t  RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/*
	定义键值代码, 必须按如下次序定时每个键的按下、弹起和长按事件

	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_select_DOWN,				/* 选择炮头地址键按下 */
	KEY_select_UP,					/* 1键弹起 */
	KEY_select_LONG,				/* 1键长按 */

	KEY_DOWN_DOWN,				/* 2键按下 */
	KEY_DOWN_UP,				/* 2键弹起 */
	KEY_DOWN_LONG,				/* 2键长按 */

	KEY_SelfCheck_DOWN,				/* 3键按下 */
	KEY_SelfCheck_UP,				/* 3键弹起 */
	KEY_SelfCheck_LONG,				/* 3键长按 */

	KEY_Valve_DOWN,				/* 4键按下 */
	KEY_Valve_UP,				/* 4键弹起 */
	KEY_Valve_LONG,				/* 4键长按 */

	KEY_Right_DOWN,				/* 5键按下 */
	KEY_Right_UP,				/* 5键弹起 */
	KEY_Right_LONG,				/* 5键长按 */

	KEY_Keyboard_Lock_DOWN,				/* 6键按下 */
	KEY_Keyboard_Lock_UP,				/* 6键弹起 */
	KEY_Keyboard_Lock_LONG,				/* 6键长按 */

	KEY_Hand_Switch_DOWN,				/* 7键按下 */
	KEY_Hand_Switch_UP,				/* 7键弹起 */
	KEY_Hand_Switch_LONG,				/* 7键长按 */

	KEY_UP_DOWN,				/* 8键按下 */
	KEY_UP_UP,				/* 8键弹起 */
	KEY_UP_LONG,				/* 8键长按 */

	/* 组合键 */
	KEY_Pump_DOWN,				/* 9键按下 */
	KEY_Pump_UP,				/* 9键弹起 */
	KEY_Pump_LONG,				/* 9键长按 */

	KEY_Mute_DOWN,			/* 10键按下 */
	KEY_Mute_UP,				/* 10键弹起 */
	KEY_Mute_LONG,			/* 10键长按 */
	
	KEY_Left_DOWN,			/* 10键按下 */
	KEY_Left_UP,				/* 10键弹起 */
	KEY_Left_LONG,			/* 10键长按 */
	KEY_Reset_DOWN,			/* 10键按下 */
	KEY_Reset_UP,				/* 10键弹起 */
	KEY_Reset_LONG,			/* 10键长按 */
}KEY_ENUM;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	12
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针1 */
	uint8_t Write;					/* 缓冲区写指针 */
	uint8_t Read2;					/* 缓冲区读指针2 */
}KEY_FIFO_T;

/* 供外部调用的函数声明 */
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
