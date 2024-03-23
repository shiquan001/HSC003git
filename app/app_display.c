 

#include "bsp.h"
#include "app.h"
#include "app_display.h"

struct DISPLAY g_display ;

extern RTC_HandleTypeDef hrtc;

RTC_TimeTypeDef g_Time;
RTC_DateTypeDef g_Date;

uint32_t m_Lux = 0x00;
uint8_t   m_Symbol;
uint16_t m_T;
uint16_t m_H;
uint16_t ad_value = 0;
float ad_value_f = 0;

uint8_t lcd_number[]=
{
/*  0   -1	 	-2	 -3	  	-4	  -5	    -6	-7	-8	 -9	  -null							*/
0x3f, 0x06, 0x5b,0x4f,0x66,0x6d,0x7d, 0x07,0x7f,0x6f,0x00,0x7c,0x39,0x5e,0x79,0x71
};

#define DISPLYA_NULL 10 //����ܲ���ʾ

/*
*********************************************************************************************************
*	�� �� ��: App_display_clock_hour_shiwei
*	����˵��:     
*	��    �Σ���  
1A	COM4	SEG9
1B	COM3	SEG9
1C	COM2	SEG9
1D	COM1	SEG10
1E	COM2	SEG10
1F 	COM4	SEG10
1G	COM3	SEG10
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_hour_shiwei_Code[7][2]=
{
/* 1A */{LCD_ENUM_COM1,LCD_ENUM_SEG_24},
/* 2B */{LCD_ENUM_COM1,LCD_ENUM_SEG_23},
/* 3C */{LCD_ENUM_COM3,LCD_ENUM_SEG_23},
/* 4D */{LCD_ENUM_COM4,LCD_ENUM_SEG_24},
/* 5E */{LCD_ENUM_COM3,LCD_ENUM_SEG_24},
/* 6F */{LCD_ENUM_COM2,LCD_ENUM_SEG_24},
/* 7G */{LCD_ENUM_COM2,LCD_ENUM_SEG_23}
};

/*
*********************************************************************************************************
*	�� �� ��: App_display_clock_hour_gewei
*	����˵��:     
*	��    �Σ���  
1A	COM4	SEG7
1B	COM3	SEG7
1C	COM2	SEG7
1D	COM1	SEG8
1E	COM2	SEG8
1F 	COM4	SEG8
1G	COM3	SEG8

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_hour_gewei_Code[7][2]=
{
/* 1A */{LCD_ENUM_COM1,LCD_ENUM_SEG_22},
/* 2B */{LCD_ENUM_COM1,LCD_ENUM_SEG_21},
/* 3C */{LCD_ENUM_COM3,LCD_ENUM_SEG_21},
/* 4D */{LCD_ENUM_COM4,LCD_ENUM_SEG_22},
/* 5E */{LCD_ENUM_COM3,LCD_ENUM_SEG_22},
/* 6F */{LCD_ENUM_COM2,LCD_ENUM_SEG_22},
/* 7G */{LCD_ENUM_COM2,LCD_ENUM_SEG_21}

};



/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_mini_shiwei_Code
*	����˵��:     
*	��    �Σ���  
1A	COM4	SEG5
1B	COM3	SEG5
1C	COM2	SEG5
1D	COM1	SEG6
1E	COM2	SEG6
1F 	COM4	SEG6
1G	COM3	SEG6

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_mini_shiwei_Code[7][2]=
{
	/* 1A */{LCD_ENUM_COM1,LCD_ENUM_SEG_20},
	/* 2B */{LCD_ENUM_COM1,LCD_ENUM_SEG_19},
	/* 3C */{LCD_ENUM_COM3,LCD_ENUM_SEG_19},
	/* 4D */{LCD_ENUM_COM4,LCD_ENUM_SEG_20},
	/* 5E */{LCD_ENUM_COM3,LCD_ENUM_SEG_20},
	/* 6F */{LCD_ENUM_COM2,LCD_ENUM_SEG_20},
	/* 7G */{LCD_ENUM_COM2,LCD_ENUM_SEG_19}

};

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_mini_shiwei_Code
*	����˵��:     
*	��    �Σ���  
1A	COM4	SEG3
1B	COM3	SEG3
1C	COM2	SEG3
1D	COM1	SEG4
1E	COM2	SEG4
1F 	COM4	SEG4
1G	COM3	SEG4

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_mini_gewei_Code[7][2]=
{
	/* 1A */{LCD_ENUM_COM1,LCD_ENUM_SEG_18},
	/* 2B */{LCD_ENUM_COM1,LCD_ENUM_SEG_17},
	/* 3C */{LCD_ENUM_COM3,LCD_ENUM_SEG_17},
	/* 4D */{LCD_ENUM_COM4,LCD_ENUM_SEG_18},
	/* 5E */{LCD_ENUM_COM3,LCD_ENUM_SEG_18},
	/* 6F */{LCD_ENUM_COM2,LCD_ENUM_SEG_18},
	/* 7G */{LCD_ENUM_COM2,LCD_ENUM_SEG_17}


};





/*
*********************************************************************************************************
*	�� �� ��: App_display_position_process
*	����˵��:     
*	��    �Σ���com2  seg21
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_position_process_seg_show(uint8_t com,uint8_t seg,uint8_t show)
{
	#if 0
	switch(com)
	{
		case LCD_ENUM_COM1:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x01;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x01;
			
			break;
		}
		case LCD_ENUM_COM2:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x02;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x02;
			
			
			break;
		}
		case LCD_ENUM_COM3:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x04;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x04;
			
			
			break;
		}
		case LCD_ENUM_COM4:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x08;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x08;
			
			
			break;
		}
		
		default:
		{
			p_err("com outof range !");
			while(1);//�ȴ���������ʾ�����⡣
		}
			break;
	}
	#else
	switch(com)
	{
		case LCD_ENUM_COM1:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x08;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x08;
			
			break;
		}
		case LCD_ENUM_COM2:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x04;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x04;
			
			
			break;
		}
		case LCD_ENUM_COM3:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x02;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x02;
			
			
			break;
		}
		case LCD_ENUM_COM4:
		{	
			if(show == TRUE)
				g_LcdCom.SEG0_31[seg] |= 0x01;
			else
				g_LcdCom.SEG0_31[seg]&= ~0x01;
			
			
			break;
		}
		
		default:
		{
			p_err("com outof range !");
			//while(1);//�ȴ���������ʾ�����⡣
		}
			break;
	}


	#endif
}


/*
*********************************************************************************************************
*	�� �� ��: App_display_position_process
*	����˵��:     
*	��    �Σ���com2  seg21
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_position_process(uint8_t com,uint8_t seg,uint8_t show)
{
	App_display_position_process_seg_show(com,seg,show);
}

/*
*********************************************************************************************************
*	�� �� ��: App_display_Var_to_Ram
*	����˵��:     
*	��    �Σ���com2  seg21
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_Var_to_Ram(void)
{

	bsp_HT1621B_Send_AllData();
}

/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��:     
*	��    �Σ���com4  seg17
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_logo_set(uint8_t show)
{
	App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_17,show);

	App_display_Var_to_Ram();

}

#if 0
/*
*********************************************************************************************************
*	�� �� ��: App_display_battery_value
*	����˵��:     
*	��    �Σ�
M5	COM1	SEG1
M4	COM2	SEG1
M3	COM3	SEG1
M2	COM4	SEG1

M1	COM4	SEG2 

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_battery_value(uint8_t show_level, uint8_t show)
{
	if(show_level == 0)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
		
	}
	else if(show_level == 1)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 2)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 3)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 4)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,TRUE);	
	}

	/*��ʾ���*/
	if(show == TRUE)
	{
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_19,TRUE);		
	}
	else
	{
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_19,FALSE);		
	}

	App_display_Var_to_Ram();

}
#endif
/*
*********************************************************************************************************
*	�� �� ��: App_display_battery_n
*	����˵��:     
*	��    �Σ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_battery_n(uint8_t show_level )
{
	if(show_level == 0)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
		
	}
	else if(show_level == 1)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 2)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 3)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,FALSE);	
	}
	else if(show_level == 4)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_15,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_15,TRUE);	
	}

	/*��ʾ���*/
	App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_19,TRUE);		

	App_display_Var_to_Ram();

}


/*
*********************************************************************************************************
*	�� �� ��: App_display_battery_and_charge
*	����˵��:     ͬʱ��ʾ��ص����ͳ��״̬
*	��    �Σ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_battery_and_charge(uint8_t show_level, uint8_t charge)
{
	static uint8_t battery_show_n = 0;
	uint8_t l_show_level = 0;
	
	//charge 
	if(charge ==CHARGE_YES)
	{
		//�ֲ���ʾ��©����0����ʱ��Ҳ��˸			
		l_show_level = show_level;
		if(l_show_level == 0)
			l_show_level = 1;
		
		App_display_battery_n(battery_show_n);
		battery_show_n++;
		if(battery_show_n > l_show_level)
			battery_show_n = 0;				
	}
	else
	{
		App_display_battery_n(show_level);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: App_display_wifi_value
*	����˵��:     
*	��    �Σ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_wifi_value(uint8_t show_level, uint8_t show)
{
	if(show_level == 0)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_16,FALSE);		
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_16,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_16,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_16,FALSE);			
	}
	else if(show_level == 1)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_16,FALSE);		
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_16,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_16,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_16,TRUE);	

	}
	else if(show_level == 2)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_16,FALSE);		
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_16,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_16,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_16,TRUE);	
	}
	else if(show_level == 3)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_16,FALSE);		
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_16,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_16,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_16,TRUE);
	}
	else if(show_level == 4)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_16,TRUE);		
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_16,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_16,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_16,TRUE);
	}
	App_display_Var_to_Ram();
}


/*
*********************************************************************************************************
*	�� �� ��: App_display_gprs_value
*	����˵��:     
*	��    �Σ�
M4	COM4	SEG0
M3	COM3	SEG0
M2	COM2	SEG0

M1	COM1	SEG0 

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_gprs_value(uint8_t show_level, uint8_t show)
{
	if(show_level == 0)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_14,FALSE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_14,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_14,FALSE);			
	}
	else if(show_level == 1)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_14,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_14,FALSE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_14,FALSE);	
	}
	else if(show_level == 2)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_14,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_14,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_14,FALSE);	
	}
	else if(show_level == 3)
	{
		App_display_position_process(LCD_ENUM_COM2,LCD_ENUM_SEG_14,TRUE);	
		App_display_position_process(LCD_ENUM_COM3,LCD_ENUM_SEG_14,TRUE);	
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_14,TRUE);	
	}

	/*��ʾ�ź����� ���*/
	if(show == TRUE)
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_14,TRUE);		
	}
	else
	{
		App_display_position_process(LCD_ENUM_COM1,LCD_ENUM_SEG_14,FALSE);		
	}	
	App_display_Var_to_Ram();

}

void App_display_clock_hour_shiwei(uint8_t show_number)
{
	uint8_t i= 0;

	/*0-6; A B C D E F G */
	for(i=0;i<7;i++)
	{	
		if(lcd_number[show_number]&(0x01<<i))
		{
			App_display_position_process(ucShuMaGuan_hour_shiwei_Code[i][0],ucShuMaGuan_hour_shiwei_Code[i][1],TRUE);	
		}
		else
		{
			App_display_position_process(ucShuMaGuan_hour_shiwei_Code[i][0],ucShuMaGuan_hour_shiwei_Code[i][1],FALSE);	
		}
	}
	//App_display_Var_to_Ram();
	
}

void App_display_clock_hour_gewei(uint8_t show_number)
{
	uint8_t i= 0;

	/*0-6; A B C D E F G */
	for(i=0;i<7;i++)
	{	
		if(lcd_number[show_number]&(0x01<<i))
		{
			App_display_position_process(ucShuMaGuan_hour_gewei_Code[i][0],ucShuMaGuan_hour_gewei_Code[i][1],TRUE);	
		}
		else
		{
			App_display_position_process(ucShuMaGuan_hour_gewei_Code[i][0],ucShuMaGuan_hour_gewei_Code[i][1],FALSE);	
		}
	}
	//App_display_Var_to_Ram();
	
}

void App_display_clock_mini_shiwei(uint8_t show_number)
{
	uint8_t i= 0;

	/*0-6; A B C D E F G */
	for(i=0;i<7;i++)
	{	
		if(lcd_number[show_number]&(0x01<<i))
		{
			App_display_position_process(ucShuMaGuan_mini_shiwei_Code[i][0],ucShuMaGuan_mini_shiwei_Code[i][1],TRUE);	
		}
		else
		{
			App_display_position_process(ucShuMaGuan_mini_shiwei_Code[i][0],ucShuMaGuan_mini_shiwei_Code[i][1],FALSE);	
		}
	}
	//App_display_Var_to_Ram();
	
}

void App_display_clock_mini_gewei(uint8_t show_number)
{
	uint8_t i= 0;

	/*0-6; A B C D E F G */
	for(i=0;i<7;i++)
	{	
		if(lcd_number[show_number]&(0x01<<i))
		{
			App_display_position_process(ucShuMaGuan_mini_gewei_Code[i][0],ucShuMaGuan_mini_gewei_Code[i][1],TRUE);	
		}
		else
		{
			App_display_position_process(ucShuMaGuan_mini_gewei_Code[i][0],ucShuMaGuan_mini_gewei_Code[i][1],FALSE);	
		}
	}
	//App_display_Var_to_Ram();
	
}
/*
*********************************************************************************************************
*	�� �� ��: App_display_logo_yijing
*	����˵��:     
*	��    �Σ���com1  seg39

T5	COM4	SEG0
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_clock(uint8_t hour,uint8_t mini,uint8_t dot_toggle,uint8_t hour_toggle,uint8_t mini_toggle)
{
	static uint8_t l_toggle_dot = 1;
	static uint8_t l_toggle_hour = 1;
	static uint8_t l_toggle_mini = 1;
	
	/*	ʼ��ͼ����˸	*/
	if(dot_toggle == TRUE)
	{
		if(l_toggle_dot == 1)
		{
			l_toggle_dot = 0;
			App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_21,TRUE);	
		}
		else
		{
			l_toggle_dot = 1;
			App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_21,FALSE);	
		}
		//else
	}
	else
	{
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_21,TRUE);	
	}
	/*	Сʱ��˸	*/
	if(hour_toggle == TRUE)
	{
		if(l_toggle_hour == 1)
		{
			l_toggle_hour = 0;
			App_display_clock_hour_shiwei(hour/10);
			App_display_clock_hour_gewei(hour%10);
		}
		else
		{
			l_toggle_hour = 1;
			App_display_clock_hour_shiwei(DISPLYA_NULL);//����ʾ
			App_display_clock_hour_gewei(DISPLYA_NULL);//����ʾ
		}
		//else
	}
	else
	{
		App_display_clock_hour_shiwei(hour/10);
		App_display_clock_hour_gewei(hour%10);
	}

	/*	������˸	*/
	if(mini_toggle == TRUE)
	{
		if(l_toggle_mini == 1)
		{
			l_toggle_mini = 0;
			App_display_clock_mini_shiwei(mini/10);
			App_display_clock_mini_gewei(mini%10);
		}
		else
		{
			l_toggle_mini = 1;
			App_display_clock_mini_shiwei(DISPLYA_NULL);//����ʾ
			App_display_clock_mini_gewei(DISPLYA_NULL);//����ʾ
		}
		//else
	}
	else
	{
		App_display_clock_mini_shiwei(mini/10);
		App_display_clock_mini_gewei(mini%10);
	}


	App_display_Var_to_Ram();
	
}

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_illumination_baiwei_Code
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG12
11B	COM2	SEG12
11C	COM3	SEG12
11D	COM4	SEG11
11E	COM3	SEG11
11F COM1	SEG11
11G	COM2	SEG11

DP5	COM4	SEG12	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_illumination_baiwei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_8},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_9},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_9},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_8},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_8},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_8},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_9}
};

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_temp_shiwei_Code
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG12
11B	COM2	SEG12
11C	COM3	SEG12
11D	COM4	SEG11
11E	COM3	SEG11
11F COM1	SEG11
11G	COM2	SEG11

DP5	COM4	SEG12	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_illumination_shiwei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_10},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_11},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_11},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_10},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_10},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_10},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_11}

};

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_illumination_gewei_Code
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG14
11B	COM2	SEG14
11C	COM3	SEG14
11D	COM4	SEG13
11E	COM3	SEG13
11F COM1	SEG13
11G	COM2	SEG13

DP5	COM4	SEG14	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_illumination_gewei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_12},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_13},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_13},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_12},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_12},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_12},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_13}

};

/*
*********************************************************************************************************
*	�� �� ��: App_display_illumination
*	����˵��:     
*	��    �Σ�

DP5	COM4	SEG14	

S1	COM4	SEG17	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_illumination(uint32_t illumination_value,uint8_t show_flag)
{
	uint16_t illumination_ug_m3=0;
	uint8_t baiwei = 0;
	uint8_t shiwei = 0;
	uint8_t gewei = 0;
	uint8_t i = 0;

	uint16_t illumination_value_k = 0 ;

	if(show_flag == TRUE)
	{
		/* ����1ǧ����ʾklux��С��1ǧ����ʾ��ʮ����*/
		if(illumination_value>=1000)
		{
			illumination_value_k = illumination_value/1000;
			
			baiwei = illumination_value_k/100;
			illumination_ug_m3 = illumination_value_k%100;//���÷Ŵ��� 20171221
			shiwei = illumination_ug_m3/10;
			gewei = illumination_ug_m3%10;		
			
			/*��ʾ klux*/
			App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_13,TRUE);	
			

		}
		else
		{	
			#if  1
			baiwei = illumination_value/100;
			illumination_ug_m3 = illumination_value%100;//���÷Ŵ��� 20171221
			shiwei = illumination_ug_m3/10;
			gewei = illumination_ug_m3%10;		
		
			/*����ʾ klux*/
			App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_13,FALSE);	
			#else
			baiwei = 0;
			shiwei = 0;
			gewei = 0;				
			/*����ʾ klux*/
			App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_13,TRUE);	
			#endif			
		}

		/*illumination	 ��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[baiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_baiwei_Code[i][0],ucShuMaGuan_illumination_baiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_baiwei_Code[i][0],ucShuMaGuan_illumination_baiwei_Code[i][1],FALSE);	
			}
		}		
		/*illumination	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_shiwei_Code[i][0],ucShuMaGuan_illumination_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_shiwei_Code[i][0],ucShuMaGuan_illumination_shiwei_Code[i][1],FALSE);	
			}
		}
		/*illumination	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_gewei_Code[i][0],ucShuMaGuan_illumination_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_gewei_Code[i][0],ucShuMaGuan_illumination_gewei_Code[i][1],FALSE);	
			}
		}	
		

		/*��ʾlight */
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_11,TRUE);	
	}
	else
	{
		baiwei	 = DISPLYA_NULL;
		shiwei	 = DISPLYA_NULL;
		gewei	 = DISPLYA_NULL; 
		/*illumination	 ��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[baiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_baiwei_Code[i][0],ucShuMaGuan_illumination_baiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_baiwei_Code[i][0],ucShuMaGuan_illumination_baiwei_Code[i][1],FALSE);	
			}
		}				
		/*illumination	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_shiwei_Code[i][0],ucShuMaGuan_illumination_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_shiwei_Code[i][0],ucShuMaGuan_illumination_shiwei_Code[i][1],FALSE);	
			}
		}
		/*illumination	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_illumination_gewei_Code[i][0],ucShuMaGuan_illumination_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_illumination_gewei_Code[i][0],ucShuMaGuan_illumination_gewei_Code[i][1],FALSE);	
			}
		}	
		/*��ʾ klux*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_13,FALSE);	
		
		/*��ʾlight */
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_11,FALSE);		

	}
	App_display_Var_to_Ram();
	
}



/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG16
11B	COM2	SEG16
11C	COM3	SEG16
11D	COM4	SEG15
11E	COM3	SEG15
11F COM1	SEG15
11G	COM2	SEG15

DP5	COM4	SEG16	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_humidity_shiwei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_4},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_5},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_5},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_4},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_4},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_4},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_5}
};

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_humidity_gewei_Code
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG20
11B	COM2	SEG20
11C	COM3	SEG20
11D	COM4	SEG19
11E	COM3	SEG19
11F COM1	SEG19
11G	COM2	SEG19

DP5	COM4	SEG20	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_humidity_gewei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_6},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_7},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_7},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_6},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_6},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_6},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_7}

};

/*
*********************************************************************************************************
*	�� �� ��: App_display_humidity
*	����˵��:     
*	��    �Σ�

DP5	COM4	SEG14	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_humidity(uint16_t humidity_value,uint8_t show_flag)
{
	uint16_t humidity_ug_m3;

	uint8_t shiwei = 0;
	uint8_t gewei = 0;
	uint8_t i = 0;

	if(show_flag == TRUE)
	{
		humidity_ug_m3 = humidity_value%100;//���÷Ŵ��� 20171221
		shiwei	 = humidity_ug_m3/10;
		gewei = humidity_ug_m3%10;		
		
		/*humidity	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_humidity_shiwei_Code[i][0],ucShuMaGuan_humidity_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_humidity_shiwei_Code[i][0],ucShuMaGuan_humidity_shiwei_Code[i][1],FALSE);	
			}
		}
		/*humidity	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_humidity_gewei_Code[i][0],ucShuMaGuan_humidity_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_humidity_gewei_Code[i][0],ucShuMaGuan_humidity_gewei_Code[i][1],FALSE);	
			}
		}	
				
		/*��ʾT5 %*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_7,TRUE);	
		/*��ʾ hum*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_9,TRUE);	
			
	}
	else
	{
		shiwei	 = DISPLYA_NULL;
		gewei	 = DISPLYA_NULL; 
		
		/*humidity	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_humidity_shiwei_Code[i][0],ucShuMaGuan_humidity_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_humidity_shiwei_Code[i][0],ucShuMaGuan_humidity_shiwei_Code[i][1],FALSE);	
			}
		}
		/*humidity	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_humidity_gewei_Code[i][0],ucShuMaGuan_humidity_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_humidity_gewei_Code[i][0],ucShuMaGuan_humidity_gewei_Code[i][1],FALSE);	
			}
		}	

		/*��ʾT5 %*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_7,FALSE);	
		/*��ʾ hum*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_9,FALSE);	

	}
	App_display_Var_to_Ram();
	
}


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG22
11B	COM2	SEG22
11C	COM3	SEG22
11D	COM4	SEG21
11E	COM3	SEG21
11F COM1	SEG21
11G	COM2	SEG21

DP5	COM4	SEG22	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_temp_shiwei_Code[7][2]=
{
/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_0},
/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_1},
/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_1},
/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_0},
/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_0},
/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_0},
/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_1}

};

/*
*********************************************************************************************************
*	�� �� ��: ucShuMaGuan_temp_gewei_Code
*	����˵��:     
*	��    �Σ���  
11A	COM1	SEG24
11B	COM2	SEG24
11C	COM3	SEG24
11D	COM4	SEG23
11E	COM3	SEG23
11F COM1	SEG23
11G	COM2	SEG23

DP5	COM4	SEG24	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
unsigned char ucShuMaGuan_temp_gewei_Code[7][2]=
{
	/* A */{LCD_ENUM_COM1,LCD_ENUM_SEG_2},
	/* B */{LCD_ENUM_COM1,LCD_ENUM_SEG_3},
	/* C */{LCD_ENUM_COM3,LCD_ENUM_SEG_3},
	/* D */{LCD_ENUM_COM4,LCD_ENUM_SEG_2},
	/* E */{LCD_ENUM_COM3,LCD_ENUM_SEG_2},
	/* F */{LCD_ENUM_COM2,LCD_ENUM_SEG_2},
	/* G */{LCD_ENUM_COM2,LCD_ENUM_SEG_3}
};

/*
*********************************************************************************************************
*	�� �� ��: App_display_temp
*	����˵��:     
*	��    �Σ�

DP5	COM4	SEG14	

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_temp(uint16_t temp_value,uint8_t show_flag)
{
	uint16_t temp_ug_m3;

	uint8_t shiwei = 0;
	uint8_t gewei = 0;
	uint8_t i = 0;

	if(show_flag == TRUE)
	{
		temp_ug_m3 = temp_value%100;//���÷Ŵ��� 20171221
		shiwei	 = temp_ug_m3/10;
		gewei = temp_ug_m3%10;		
		
		/*temp	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_temp_shiwei_Code[i][0],ucShuMaGuan_temp_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_temp_shiwei_Code[i][0],ucShuMaGuan_temp_shiwei_Code[i][1],FALSE);	
			}
		}
		/*temp	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_temp_gewei_Code[i][0],ucShuMaGuan_temp_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_temp_gewei_Code[i][0],ucShuMaGuan_temp_gewei_Code[i][1],FALSE);	
			}
		}	
		
		/*��ʾ du*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_3,TRUE);			
		/*��ʾ temp*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_5,TRUE);	
		
	}
	else
	{
		shiwei	 = DISPLYA_NULL;
		gewei	 = DISPLYA_NULL; 
		
		/*temp	 ʮλ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[shiwei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_temp_shiwei_Code[i][0],ucShuMaGuan_temp_shiwei_Code[i][1],TRUE);	
			}
			else
			{
				App_display_position_process(ucShuMaGuan_temp_shiwei_Code[i][0],ucShuMaGuan_temp_shiwei_Code[i][1],FALSE);	
			}
		}
		/*temp	��λ	0-6; A B C D E F G */
		for(i=0;i<7;i++)
		{	
			if(lcd_number[gewei]&(0x01<<i))
			{
				App_display_position_process(ucShuMaGuan_temp_gewei_Code[i][0],ucShuMaGuan_temp_gewei_Code[i][1],TRUE); 
			}
			else
			{
				App_display_position_process(ucShuMaGuan_temp_gewei_Code[i][0],ucShuMaGuan_temp_gewei_Code[i][1],FALSE);	
			}
		}		
		/*��ʾ du*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_3,FALSE);			
		/*��ʾ temp*/
		App_display_position_process(LCD_ENUM_COM4,LCD_ENUM_SEG_5,FALSE);	

	}
	App_display_Var_to_Ram();
	
}


/*
*********************************************************************************************************
*	�� �� ��: App_display_Init_Var
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_Init_Var(void)
{	

	g_display.work_state = DISPLAY_INIT;

}

/*
*********************************************************************************************************
*	�� �� ��: App_wenshi_Loop
*	����˵��:     
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_Loop(void)
{			
	static  u16 l_count = 0;
	
	switch(g_display.work_state)
	{
		case DISPLAY_INIT :
		{				
			g_display.work_state = DISPLAY_LOOP;							

			bsp_HT1621_Show_ALL();
			HAL_Delay(100);			
			bsp_HT1621_Show_NULL();
			#if 0			
			App_display_logo_set(TRUE);
			
			App_display_battery_value(2,TRUE);// 2
			App_display_gprs_value(2,TRUE);// 2
			
			App_display_illumination(18,TRUE);//���� 18
			App_display_humidity(88,TRUE);//ʪ��88
			App_display_temp(11,TRUE);// �¶�11��
			#endif	
			bsp_StartTimer(TMR_ID_dispaly_refresh,TMR_DELAY_0_5S);

			p_info("g_display DISPLAY_INIT");
		}
			break;
			
		case DISPLAY_LOOP :
		{	
			App_address_sensor_Led_Loop();
			
			/*  1������ʾһ��*/
			if(bsp_CheckTimer(TMR_ID_dispaly_refresh))
			{
				bsp_StartTimer(TMR_ID_dispaly_refresh,TMR_DELAY_0_5S);
				
				/* Get the RTC current Time */
				if (HAL_RTC_GetTime(&hrtc, &g_Time, RTC_FORMAT_BIN) != HAL_OK)
				{
					/* HAL Error */
					Error_Handler();
				}				
				if (HAL_RTC_GetDate(&hrtc, &g_Date, RTC_FORMAT_BIN) != HAL_OK)
				{
					/* HAL Error */
					Error_Handler();
				}	

				
				App_display_clock(g_Time.Hours,g_Time.Minutes,TRUE,FALSE,FALSE);

				l_count++;
				if(l_count >= 15)
				{
					l_count = 0;
					//p_info("App_display_clock :%02d-%02d-%02d ",g_Date.Year,g_Date.Month,g_Date.Date);
					//p_info("App_display_clock :%02d:%02d:%02d ",g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
				}

				/*	��ʾ����ǿ��*/
				App_display_illumination(g_wenshi.m_Lux,TRUE);

				/*	��ʾ��ʪ��*/
				App_display_temp(g_wenshi.m_T/10,TRUE);
				App_display_humidity(g_wenshi.m_H/10,TRUE);

				/*	��ʾ��ص���*/
				//App_display_battery_value(g_wenshi.m_voltage_level,TRUE);// 2
				App_display_battery_and_charge(g_wenshi.m_voltage_level,g_tBattery.charge_flag);

				
				/*	��ʾnb���ź�ǿ��*/
				App_display_gprs_value(g_wenshi.m_nb_signal_level,TRUE);

				if(g_tZigbeeHACoordinator.power_flag == TRUE)
				{
					App_display_wifi_value(4,TRUE);
				}
				else
				{
					App_display_wifi_value(0,TRUE);
				}

			}			
			
		}		
			break;	
			
		default:
			break;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: App_display_TEST
*	����˵��:     
*	��    �Σ���  
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_display_TEST(void)
{
	#if 0
	bsp_HT1621_Show_ALL();
	HAL_Delay(1000);
	
	bsp_HT1621_Show_NULL();
	HAL_Delay(1000);

	App_display_logo_set(TRUE);

	App_display_battery_value(2,TRUE);// 2
	App_display_gprs_value(2,TRUE);// 2

	App_display_illumination(18,TRUE);//���� 18
	App_display_humidity(88,TRUE);//ʪ��88
	App_display_temp(11,TRUE);// �¶�11��
	

	bsp_InitTimer();
	bsp_StartTimer(TMR_ID_433M_1s_MATCH,1000);

	while(1)
	{

		//��ʱ500ms
		if(bsp_CheckTimer(TMR_ID_433M_1s_MATCH))
		{
			bsp_StartTimer(TMR_ID_433M_1s_MATCH,1000);
			
			HAL_RTC_GetTime(&hrtc,&g_Time,RTC_FORMAT_BIN);
			
			App_display_clock(g_Time.Hours,g_Time.Minutes,TRUE,FALSE,FALSE);
			
			p_info("App_display_clock :%02d:%02d:%02d ",g_Time.Hours,g_Time.Minutes,g_Time.Seconds);
			#if 1 // 20190222
			bsp_LUX_GetLux(&m_Lux);
			App_display_illumination(m_Lux%100,TRUE);
			
			//p_info("m_Lux=%.3flux\n", (float)m_Lux);
			
			bsp_Get_TH(&m_Symbol, &m_T, &m_H);
			App_display_temp(m_T/10,TRUE);
			App_display_humidity(m_H/10,TRUE);
			
			//p_info("m_Symbol=%d; m_T=%d; m_H=%d; ", m_Symbol,m_T,m_H);
			
			App_Battery_Voltage_Display_Data();// loge
			App_display_battery_value(g_tBattery.voltage_level_g,TRUE);// 2
			#endif
		}

	}
	#endif
}


