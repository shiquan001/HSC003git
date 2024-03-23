#include "app.h"

#include "app_communication.h"

Communication_STRCUT g_Communication;

volatile unsigned char do_Fire_detection_flag = 0;
//PAOTOU_STRCUT g_Paotou;

PAOTOU_STRCUT * g_tXiangchang_address_paotou_master_CAN ;//���ص� _CAN
PAOTOU_STRCUT * g_tXiangchang_address_paotou_cur_CAN ;//��ǰͨѶ�� CAN

/*��1����*/
PAOTOU_STRCUT g_Paotou11;
PAOTOU_STRCUT g_Paotou12;
PAOTOU_STRCUT g_Paotou13;
PAOTOU_STRCUT g_Paotou14;
/*��2����*/
PAOTOU_STRCUT g_Paotou21;
PAOTOU_STRCUT g_Paotou22;
PAOTOU_STRCUT g_Paotou23;
PAOTOU_STRCUT g_Paotou24;
/*��3����*/
PAOTOU_STRCUT g_Paotou31;
PAOTOU_STRCUT g_Paotou32;
PAOTOU_STRCUT g_Paotou33;
PAOTOU_STRCUT g_Paotou34;
/*��4����*/
PAOTOU_STRCUT g_Paotou41;
PAOTOU_STRCUT g_Paotou42;
PAOTOU_STRCUT g_Paotou43;
PAOTOU_STRCUT g_Paotou44;

/*��5����*/
PAOTOU_STRCUT g_Paotou51;
PAOTOU_STRCUT g_Paotou52;
PAOTOU_STRCUT g_Paotou53;
PAOTOU_STRCUT g_Paotou54;
/*��6����*/
PAOTOU_STRCUT g_Paotou61;
PAOTOU_STRCUT g_Paotou62;
PAOTOU_STRCUT g_Paotou63;
PAOTOU_STRCUT g_Paotou64;
/*��7����*/
PAOTOU_STRCUT g_Paotou71;
PAOTOU_STRCUT g_Paotou72;
PAOTOU_STRCUT g_Paotou73;
PAOTOU_STRCUT g_Paotou74;
/*��8����*/
PAOTOU_STRCUT g_Paotou81;
PAOTOU_STRCUT g_Paotou82;
PAOTOU_STRCUT g_Paotou83;
PAOTOU_STRCUT g_Paotou84;

/*��9����*/
PAOTOU_STRCUT g_Paotou91;
PAOTOU_STRCUT g_Paotou92;
PAOTOU_STRCUT g_Paotou93;
PAOTOU_STRCUT g_Paotou94;
/*��10����*/
PAOTOU_STRCUT g_Paotou101;
PAOTOU_STRCUT g_Paotou102;
PAOTOU_STRCUT g_Paotou103;
PAOTOU_STRCUT g_Paotou104;
/*��11����*/
PAOTOU_STRCUT g_Paotou111;
PAOTOU_STRCUT g_Paotou112;
PAOTOU_STRCUT g_Paotou113;
PAOTOU_STRCUT g_Paotou114;
/*��12����*/
PAOTOU_STRCUT g_Paotou121;
PAOTOU_STRCUT g_Paotou122;
PAOTOU_STRCUT g_Paotou123;
PAOTOU_STRCUT g_Paotou124;

/*��13����*/
PAOTOU_STRCUT g_Paotou131;
PAOTOU_STRCUT g_Paotou132;
PAOTOU_STRCUT g_Paotou133;
PAOTOU_STRCUT g_Paotou134;
/*��14����*/
PAOTOU_STRCUT g_Paotou141;
PAOTOU_STRCUT g_Paotou142;
PAOTOU_STRCUT g_Paotou143;
PAOTOU_STRCUT g_Paotou144;
/*��15����*/
PAOTOU_STRCUT g_Paotou151;
PAOTOU_STRCUT g_Paotou152;
PAOTOU_STRCUT g_Paotou153;
PAOTOU_STRCUT g_Paotou154;
/*��16����*/
PAOTOU_STRCUT g_Paotou161;
PAOTOU_STRCUT g_Paotou162;
PAOTOU_STRCUT g_Paotou163;
PAOTOU_STRCUT g_Paotou164;



//#define TEST_TX_rever 1 //ѡ�����ֳ����Ƶ�    ����Ԥ�����Ƶ�485

#define TEST_TX_ADD_FRAME_HEADER 


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void PAOTOU_Init_Var(void)
{
	//PAOTOU_STRCUT * dest ;//���ص� _CAN
	//PAOTOU_STRCUT * souce ;//���ص� _CAN

	g_Paotou11.contrl_mode = FRAME_CMD_AUTO;
	g_Paotou11.contrl_mode_bak = FRAME_CMD_AUTO;
	//LED_STATUS.Hand_auto =False;
	
	g_Paotou11.Fire_On_State = FIRE_OFF;

	g_Paotou11.Pump_State = FIRE_OFF;

	g_Paotou11.Value_State = FIRE_OFF;	
	g_Paotou11.Value_State_return = FIRE_OFF;
	g_Paotou11.Value_State_qingtai= FIRE_OFF;// ��̨
	g_Paotou11.Value_State_xianchang = FIRE_OFF;
	g_Paotou11.Selfcheck_State = SELF_CHECK_OVER;

	g_Paotou11.xianchang_yuancheng_contrl_mode = 0;//
	g_Paotou11.beep_manul_mute_flag = FALSE;
	
	LED_STATUS.xianchang_yuancheng = True;//Ĭ��Զ�̡�

	/*״̬�ı���*/
	g_Paotou11.Fire_On_State_bak = g_Paotou11.Fire_On_State;
	g_Paotou11.Value_State_return_bak = g_Paotou11.Value_State_return;
	g_Paotou11.Pump_State_bak = g_Paotou11.Pump_State;
	
	g_Paotou11.com_error_bak = FRAME_CMD_COM_ERROR;
	g_Paotou11.com_error = FRAME_CMD_COM_ERROR;
	
	g_Paotou11.Selfcheck_State_bak = g_Paotou11.Selfcheck_State;
	
	/*�ṹ��ĺô����ǣ�������ֱ�Ӹ�ֵ����������*/

	memcpy(&g_Paotou12,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou13,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou14,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou21,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou22,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou23,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou24,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou31,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou32,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou33,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou34,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou41,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou42,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou43,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou44,&g_Paotou11,sizeof(PAOTOU_STRCUT));


	memcpy(&g_Paotou51,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou52,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou53,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou54,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou61,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou62,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou63,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou64,&g_Paotou11,sizeof(PAOTOU_STRCUT));


	memcpy(&g_Paotou71,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou72,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou73,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou74,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou81,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou82,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou83,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou84,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou91,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou92,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou93,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou94,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou101,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou102,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou103,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou104,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou111,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou112,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou113,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou114,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou121,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou122,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou123,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou124,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou131,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou132,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou133,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou134,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou141,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou142,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou143,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou144,&g_Paotou11,sizeof(PAOTOU_STRCUT));


	memcpy(&g_Paotou151,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou152,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou153,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou154,&g_Paotou11,sizeof(PAOTOU_STRCUT));

	memcpy(&g_Paotou161,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou162,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou163,&g_Paotou11,sizeof(PAOTOU_STRCUT));
	memcpy(&g_Paotou164,&g_Paotou11,sizeof(PAOTOU_STRCUT));
}

/*
*********************************************************************************************************
*	�� �� ��: App_communication_Init_Var
*	����˵��:  UPDATE  AND 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_Init_Var(void)
{
	g_Communication.work_state = Communication_INIT;
	g_Communication.loop_time = 50;//50;//125;//150;//1*250;//200;//��//300;//;//5*1000;//10s

}

/*
*********************************************************************************************************
*	�� �� ��: App_communication_address_point_change_CAN
*	����˵��:   ��Ӧ��ͷ��ָ�� 
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_address_point_change_CAN(u8 address_fengqu_cur,u8  address_paotou_cur)
{
	/* ��ǰͨ�ŷ��� ָ��ı�� */
	switch(address_fengqu_cur)
	{
		case ENUM_ADDRESS_FENGQU_1:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou11;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou12;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou13;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou14;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_2:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou21;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou22;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou23;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou24;
				}
					break;	
				default:
					break;
			}	

		}
			break;
		case ENUM_ADDRESS_FENGQU_3:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou31;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou32;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou33;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou34;
				}
					break;	
				default:
					break;
			}	

		}
			break;
		case ENUM_ADDRESS_FENGQU_4:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou41;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou42;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou43;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou44;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_5:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou51;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou52;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou53;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou54;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_6:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou61;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou62;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou63;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou64;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_7:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou71;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou72;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou73;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou74;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_8:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou81;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou82;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou83;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou84;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_9:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou91;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou92;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou93;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou94;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_10:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou101;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou102;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou103;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou104;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_11:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou111;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou112;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou113;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou114;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_12:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou121;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou122;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou123;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou124;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_13:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou131;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou132;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou133;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou134;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_14:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou141;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou142;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou143;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou144;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_15:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou151;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou152;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou153;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou154;
				}
					break;	
				default:
					break;
			}		
		}
			break;

		case ENUM_ADDRESS_FENGQU_16:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(address_paotou_cur)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou161;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou162;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou163;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_tXiangchang_address_paotou_cur_CAN = &g_Paotou164;
				}
					break;	
				default:
					break;
			}		
		}
			break;			
		default:
			break;
	}

	g_Paotou11.master_manchine = 0;
	g_Paotou12.master_manchine = 0;
	g_Paotou13.master_manchine = 0;
	g_Paotou14.master_manchine = 0;

	g_Paotou21.master_manchine = 0;
	g_Paotou22.master_manchine = 0;
	g_Paotou23.master_manchine = 0;
	g_Paotou24.master_manchine = 0;

	g_Paotou31.master_manchine = 0;
	g_Paotou32.master_manchine = 0;
	g_Paotou33.master_manchine = 0;
	g_Paotou34.master_manchine = 0;	

	g_Paotou41.master_manchine = 0;
	g_Paotou42.master_manchine = 0;
	g_Paotou43.master_manchine = 0;
	g_Paotou44.master_manchine = 0;

	g_Paotou51.master_manchine = 0;
	g_Paotou52.master_manchine = 0;
	g_Paotou53.master_manchine = 0;
	g_Paotou54.master_manchine = 0;

	g_Paotou61.master_manchine = 0;
	g_Paotou62.master_manchine = 0;
	g_Paotou63.master_manchine = 0;
	g_Paotou64.master_manchine = 0;

	g_Paotou71.master_manchine = 0;
	g_Paotou72.master_manchine = 0;
	g_Paotou73.master_manchine = 0;
	g_Paotou74.master_manchine = 0;	

	g_Paotou81.master_manchine = 0;
	g_Paotou82.master_manchine = 0;
	g_Paotou83.master_manchine = 0;
	g_Paotou84.master_manchine = 0;

	g_Paotou91.master_manchine = 0;
	g_Paotou92.master_manchine = 0;
	g_Paotou93.master_manchine = 0;
	g_Paotou94.master_manchine = 0;

	g_Paotou101.master_manchine = 0;
	g_Paotou102.master_manchine = 0;
	g_Paotou103.master_manchine = 0;
	g_Paotou104.master_manchine = 0;

	g_Paotou111.master_manchine = 0;
	g_Paotou112.master_manchine = 0;
	g_Paotou113.master_manchine = 0;
	g_Paotou114.master_manchine = 0;

	g_Paotou121.master_manchine = 0;
	g_Paotou122.master_manchine = 0;
	g_Paotou123.master_manchine = 0;
	g_Paotou124.master_manchine = 0;

	g_Paotou131.master_manchine = 0;
	g_Paotou132.master_manchine = 0;
	g_Paotou133.master_manchine = 0;
	g_Paotou134.master_manchine = 0;

	g_Paotou141.master_manchine = 0;
	g_Paotou142.master_manchine = 0;
	g_Paotou143.master_manchine = 0;
	g_Paotou144.master_manchine = 0;

	g_Paotou151.master_manchine = 0;
	g_Paotou152.master_manchine = 0;
	g_Paotou153.master_manchine = 0;
	g_Paotou154.master_manchine = 0;

	g_Paotou161.master_manchine = 0;
	g_Paotou162.master_manchine = 0;
	g_Paotou163.master_manchine = 0;
	g_Paotou164.master_manchine = 0;	

	/* ��ǰͨ�ŷ��� ָ��ı�� */
	switch(g_QingTai.address_fengqu_master)
	{
		case ENUM_ADDRESS_FENGQU_1:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou11.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou11;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou12.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou12;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou13.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou13;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou14.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou14;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_2:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou21.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou21;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou22.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou22;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou23.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou23;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou24.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou24;
				}
					break;	
				default:
					break;
			}	

		}
			break;
		case ENUM_ADDRESS_FENGQU_3:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou31.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou31;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou32.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou32;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou33.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou33;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou34.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou34;
				}
					break;	
				default:
					break;
			}	

		}
			break;
		case ENUM_ADDRESS_FENGQU_4:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou41.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou41;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou42.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou42;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou43.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou43;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou44.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou44;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_5:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou51.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou51;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou52.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou52;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou53.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou53;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou54.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou54;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_6:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou61.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou61;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou62.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou62;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou63.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou63;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou64.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou64;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_7:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou71.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou71;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou72.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou72;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou73.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou73;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou74.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou74;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_8:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou81.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou81;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou82.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou82;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou83.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou83;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou84.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou84;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_9:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou91.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou91;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou92.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou92;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou93.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou93;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou94.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou94;
				}
					break;	
				default:
					break;
			}		
		}
			break;		
		case ENUM_ADDRESS_FENGQU_10:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou101.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou101;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou102.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou102;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou103.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou103;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou104.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou104;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_11:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou111.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou111;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou112.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou112;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou113.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou113;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou114.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou114;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_12:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou121.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou121;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou122.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou122;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou123.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou123;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou124.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou124;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_13:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou131.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou131;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou132.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou132;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou133.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou133;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou134.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou134;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_14:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou141.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou141;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou142.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou142;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou143.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou143;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou144.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou144;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_15:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou151.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou151;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou152.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou152;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou153.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou153;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou154.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou154;
				}
					break;	
				default:
					break;
			}		
		}
			break;
		case ENUM_ADDRESS_FENGQU_16:
		{
			/* ��ǰͨ�� ָ��ı�� */
			switch(g_QingTai.address_paotou_master)
			{
				case ENUM_ADDRESS_PAOTOU_1:
				{
					g_Paotou161.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN= &g_Paotou161;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_2:
				{
					g_Paotou162.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou162;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_3:
				{
					g_Paotou163.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou163;
				}
					break;
				case ENUM_ADDRESS_PAOTOU_4:
				{
					g_Paotou164.master_manchine = 1;					
					g_tXiangchang_address_paotou_master_CAN = &g_Paotou164;
				}
					break;	
				default:
					break;
			}		
		}
			break;			
		default:
			break;
	}

}
static u8 flag_poweron = 1;

/*
*********************************************************************************************************
*	�� �� ��: App_communication_address_change_to_zero
*	����˵��:  ��ͷ��ʼɨ����ͷ ��������ͷ��ַ��
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_address_change_to_zero(void)
{
	flag_poweron = 1;
}

/*
*********************************************************************************************************
*	�� �� ��: App_communication_address_change_pro
*	����˵��:  ѭ��ɨ�� ��������ͷ��ַ��
*	��    �Σ���

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_address_change_pro(void)
{
	
	if(flag_poweron == 1)
	{
		flag_poweron = 0;
		
		/*������ ͨѶ���� ��ͷ*/
		g_QingTai.address_fengqu_cur = g_QingTai.address_fengqu_master;
		g_QingTai.address_paotou_cur = g_QingTai.address_paotou_master;
		return ;
	}
	
	switch(g_QingTai.address_fengqu_cur)
	{
		case ENUM_ADDRESS_FENGQU_1:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_2;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1;
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_1;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_2:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur =  ENUM_ADDRESS_FENGQU_3;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1;
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_2;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}	
		}
			break;
		case ENUM_ADDRESS_FENGQU_3:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_4;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_3;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}	
		}
			break;
		case ENUM_ADDRESS_FENGQU_4:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_5;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_4;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}	
		}
			break;
		case ENUM_ADDRESS_FENGQU_5:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_6;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_5;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}	
		}
			break;
		case ENUM_ADDRESS_FENGQU_6:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_7;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_6;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}	
		}
			break;
		case ENUM_ADDRESS_FENGQU_7:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_8;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_7;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_8:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_9;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_8;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_9:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_10;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_9;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_10:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_11;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_10;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_11:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_12;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_11;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_12:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_13;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_12;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_13:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_14;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_13;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_14:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_15;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_14;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_15:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_16;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_15;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
		case ENUM_ADDRESS_FENGQU_16:
		{
			g_QingTai.address_paotou_cur++;
			if(g_QingTai.address_paotou_cur > ENUM_ADDRESS_PAOTOU_4)
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_1;
				g_QingTai.address_paotou_cur = ENUM_ADDRESS_PAOTOU_1 ;// Ŀǰ���� ���� 1-4����ͷ 1-4 ��
			}
			else
			{
				g_QingTai.address_fengqu_cur = ENUM_ADDRESS_FENGQU_16;
				g_QingTai.address_paotou_cur = g_QingTai.address_paotou_cur ;
			}			
		}
			break;
			
		default:
			break;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: App_communication_rx_data_pro
*	����˵��: ���ڽ����жϷ������ ��Ҫ����������ִ�С�
*	��    �Σ���
COMMUNICATE_TYPE_CHAXUN = 0X06,// ��ѯ֡
COMMUNICATE_TYPE_CMD   = 0X05,	//����֡

*	�� �� ֵ: ��
*********************************************************************************************************
*/
	u8 tx_data[FRAME_LENTH];
u8 tx_data_[FRAME_LENTH];
void App_communication_tx_chaxun_data_pro(u8 input_CMD,u8 input_type)
{
	u16 l_sum = 0;
	u8 i = 0;
	u8 low_bit4 = 0;
	u8 low_bit5 = 0;
	u8 low_bit6 = 0;
	u8 low_bit7 = 0;

	u8 low_bit3 = 0;
	u8 low_bit2 = 0;
	u8 low_bit1 = 0;
	u8 low_bit0 = 0;

	u8 high_4bit = 0;
	u8 low_4bit = 0;
	u16 Horizontal_angle_current = 0;
	u8 Vertical_angle_current = 0;
	/*��������� �ֳ����ƺ��ӣ���������*/

	tx_data[FRAME_POS_HEAD] = FRAME_HEAD;// byte 0

	high_4bit = ((g_QingTai.address_fengqu_cur-1)<<4);
	low_4bit = 	g_QingTai.address_paotou_cur;
	tx_data[FRAME_POS_address] = high_4bit|low_4bit;// byte 1

	high_4bit = (input_type <<4);	
	low_bit3 = 	(g_tXiangchang_address_paotou_cur_CAN->contrl_mode<<3);
	low_bit2 = (g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode<<2);	
	low_bit1 = (g_tXiangchang_address_paotou_cur_CAN->master_manchine<<1);// 1��ʾ��ǰ��������
	low_bit0 = (g_tXiangchang_address_paotou_cur_CAN->com_error<<0);// 0��ʾ���ǡ�
	low_4bit = low_bit3|low_bit2|low_bit1|low_bit0;	
	tx_data[FRAME_POS_WORK_MODE] = high_4bit|low_4bit;// byte 2

	low_bit7 = (g_tXiangchang_address_paotou_cur_CAN->Fire_On_State<<7);
	low_bit6 = (g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag<<6);
	low_bit5 = (g_tXiangchang_address_paotou_cur_CAN->Value_State_return<<5);
	low_bit4 = (g_tXiangchang_address_paotou_cur_CAN->Pump_State<<4);	
	low_bit3 = (g_tXiangchang_address_paotou_cur_CAN->Selfcheck_State_rx_485<<3);
	low_bit2 = (g_QingTai.pump_state_feedback<<2);// ������̨ʵ��ˮ�õ�״̬
	low_bit1 = (g_tXiangchang_address_paotou_cur_CAN->Value_State_qingtai<<1);
	low_bit0 = 0;
	tx_data[FRAME_POS_FIRE_ON_STATE] = low_bit7|low_bit6|low_bit5|low_bit4|low_bit3|low_bit2|low_bit1|low_bit0;// byte 3

	/*ˮƽ�Ƕ� ��ֱ�Ƕ�*/	
	Horizontal_angle_current = (u16)g_tXiangchang_address_paotou_cur_CAN->Horizontal_angle_current;
	Vertical_angle_current = (u8)g_tXiangchang_address_paotou_cur_CAN->Vertical_angle_current;
	tx_data[FRAME_POS_HOR_ANGLE] = Horizontal_angle_current>>1;/*ˮƽ�Ƕ�*/  // byte 4
	if(Horizontal_angle_current&0x0001)
		low_bit7 = 0x80; 
	else
		low_bit7 = 0x00; 
	tx_data[FRAME_POS_VER_ANGLE] = low_bit7 | Vertical_angle_current;/*��ֱ�Ƕ�*/  // byte 5
	
	tx_data[FRAME_POS_CMD] = input_CMD; // byte 6

	l_sum = 0;
	for(i=0;i<FRAME_SUM_LENTH;i++)
	{
		l_sum = l_sum ^ tx_data[FRAME_POS_address+i];
	}
	
	tx_data[FRAME_POS_SUM] = (u8)l_sum;// byte 7
		
	#if 0
	tx_data_[0] = 0x11;	
	tx_data_[1] = 0x11;	
	//tx_data_[2] = 0x11;	
	comSendBuf(Reserve_485_uart, tx_data_, 2);
	#endif

	#ifdef TEST_TX_rever
	Reserve_RS485_SendBuf(tx_data,FRAME_LENTH);
	#else
	Xianchang_RS485_SendBuf(tx_data,FRAME_LENTH);
	#endif

	if(bsp_can_Tx_data(tx_data,FRAME_LENTH)==0)
	{
		//p_info("bsp_can_Tx_data  ok");
	}
	else
	{
		p_info("bsp_can_Tx_data  error");
	}

}  
/*
*********************************************************************************************************
*	�� �� ��: App_communication_tx_cmd_data_pro
*	����˵��: 
*	��    �Σ���
COMMUNICATE_TYPE_CHAXUN = 0X06,// ��ѯ֡
COMMUNICATE_TYPE_CMD   = 0X05,	//����֡

*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_tx_cmd_data_pro(u8 input_CMD,u8 input_type)
{
	u16 l_sum = 0;
	u8 i = 0;
	u8 low_bit4 = 0;
	u8 low_bit5 = 0;
	u8 low_bit6 = 0;
	u8 low_bit7 = 0;

	u8 low_bit3 = 0;
	u8 low_bit2 = 0;
	u8 low_bit1 = 0;
	u8 low_bit0 = 0;

	u8 high_4bit = 0;
	u8 low_4bit = 0;
	u16 Horizontal_angle_current = 0;
	u8 Vertical_angle_current = 0;
	/*��������� �ֳ����ƺ��ӣ���������*/

	tx_data[FRAME_POS_HEAD] = FRAME_HEAD;// byte 0

	high_4bit = ((g_QingTai.address_fengqu_master-1)<<4);
	low_4bit = 	g_QingTai.address_paotou_master;
	tx_data[FRAME_POS_address] = high_4bit|low_4bit;// byte 1

	high_4bit = (input_type <<4);	
	low_bit3 = 	(g_tXiangchang_address_paotou_cur_CAN->contrl_mode<<3);
	low_bit2 = (g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode<<2);
	low_bit1 = (g_tXiangchang_address_paotou_cur_CAN->master_manchine<<1);// 1��ʾ��ǰ��������
	low_bit0 = (g_tXiangchang_address_paotou_cur_CAN->com_error<<0);// 0��ʾ���ǡ�
	low_4bit = low_bit3|low_bit2|low_bit1|low_bit0;	
	tx_data[FRAME_POS_WORK_MODE] = high_4bit|low_4bit;// byte 2
	
	low_bit7 = (g_tXiangchang_address_paotou_cur_CAN->Fire_On_State<<7);
	low_bit6 = (g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag<<6);
	low_bit5 = (g_tXiangchang_address_paotou_cur_CAN->Value_State_return<<5);
	low_bit4 = (g_tXiangchang_address_paotou_cur_CAN->Pump_State<<4);	
	low_bit3 = (g_tXiangchang_address_paotou_cur_CAN->Selfcheck_State_rx_485<<3);
	low_bit2 = (g_QingTai.pump_state_feedback<<2);// ������̨ʵ��ˮ�õ�״̬
	low_bit1 = (g_tXiangchang_address_paotou_cur_CAN->Value_State_qingtai<<1);
	low_bit0 = 0;
	tx_data[FRAME_POS_FIRE_ON_STATE] = low_bit7|low_bit6|low_bit5|low_bit4|low_bit3|low_bit2|low_bit1|low_bit0;// byte 3

	/*ˮƽ�Ƕ� ��ֱ�Ƕ�*/	
	Horizontal_angle_current = (u16)g_tXiangchang_address_paotou_cur_CAN->Horizontal_angle_current;
	Vertical_angle_current = (u8)g_tXiangchang_address_paotou_cur_CAN->Vertical_angle_current;
	tx_data[FRAME_POS_HOR_ANGLE] = Horizontal_angle_current>>1;/*ˮƽ�Ƕ�*/  // byte 4
	if(Horizontal_angle_current&0x0001)
		low_bit7 = 0x80; 
	else
		low_bit7 = 0x00; 
	tx_data[FRAME_POS_VER_ANGLE] = low_bit7 | Vertical_angle_current;/*��ֱ�Ƕ�*/  // byte 5
	
	tx_data[FRAME_POS_CMD] = input_CMD;// byte 6

	l_sum = 0;
	for(i=0;i<FRAME_SUM_LENTH;i++)
	{
		l_sum = l_sum ^ tx_data[FRAME_POS_address+i];
	}
	tx_data[FRAME_POS_SUM] = (u8)l_sum;// byte 7

		
	#if 0
	tx_data_[0] = 0x11;	
	tx_data_[1] = 0x11;	
	//tx_data_[2] = 0x11;	
	comSendBuf(Reserve_485_uart, tx_data_, 2);
	#endif

	#ifdef TEST_TX_rever
	Reserve_RS485_SendBuf(tx_data,FRAME_LENTH);
	#else
	Xianchang_RS485_SendBuf(tx_data,FRAME_LENTH);
	#endif

	if(bsp_can_Tx_data(tx_data,FRAME_LENTH)==0)
	{
		//p_info("bsp_can_Tx_data  ok");
	}
	else
	{
		p_info("bsp_can_Tx_data  error");
	}
}  

/*
*********************************************************************************************************
*	�� �� ��: App_communication_tx_data_Retry
*	����˵��:�ؼ����ݵ��ط���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 App_communication_tx_data_Retry(u8 input_CMD)
{
	u8 i = 0;
	//u8 j = 0;
	g_QingTai.address_paotou_cur= g_QingTai.address_paotou_master;
	g_tXiangchang_address_paotou_cur_CAN = g_tXiangchang_address_paotou_master_CAN; // �������ص� ��ǰ��ͨ��

	for(i = 0;i< 4;i++)
	{		
		App_communication_tx_cmd_data_pro(input_CMD,COMMUNICATE_TYPE_CMD);
		//for(j = 0;j< 6;j++)
		{
			bsp_DelayMS(25*3);//(25*4)	(40)
			if(App_communication_rx_data_pro() == Communication_ASK_OK)
			{
				App_communication_address_change_to_zero();//�������óɹ��ˣ��Ϳ��Դ��¿�ʼ��ѯ��
				return Communication_ASK_OK;
			}
		}
	}
	App_communication_address_change_to_zero();//�������óɹ��ˣ��Ϳ��Դ��¿�ʼ��ѯ��
	return Communication_ASK_NOT_OK;

}

/*
*********************************************************************************************************
*	�� �� ��: App_communication_rx_data_pro
*	����˵��: ���ڽ����жϷ������ ��Ҫ����������ִ�С�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern	u8 fault ;
//extern u8 update_dis_flag;//����led��ʾ��־
u8 rx_data[512];

uint8_t App_communication_rx_data_pro(void)
{
	//u8 tx_to_xiangchang_ansewer = 0;// �Ƿ���Ҫ�ظ����ֳ�
	//uint8_t ucData;
	uint16_t cur_pos = 0;
	uint16_t pos = 0;
	uint8_t ret;
	uint8_t i;
	u8 low_bit0 = 0;
	u8 low_bit1 = 0;
	u8 low_bit2 = 0;
	u8 low_bit3 = 0;
	u8 low_bit4 = 0;
	u8 low_bit5 = 0;
	u8 low_bit6 = 0;
	u8 low_bit7 = 0;
	u16 Horizontal_angle_current = 0;
	//u8 Vertical_angle_current = 0;
	
	u8 high_4bit = 0;
	u8 low_4bit = 0;
	//u8 low_2bit = 0;
	u8 address_fengqu = 0;
	u8 address_paotou = 0;
	u16 l_sum = 0;
	u8 l_sum_low8 = 0;
	
	g_QingTai.value_open_answer= 0;
	
	if (bsp_CheckTimer(TMR_ID_xianghcang_UART_RS485))
	{
		memset(rx_data,0,sizeof(rx_data));
		
		//p_info("rx_data");
		#if 0
		
		while(1)
		{
			#ifdef TEST_TX_rever
			if(comGetChar(Reserve_485_uart, &ucData))
			#else
			if(comGetChar(xianchang_485_uart, &ucData))
			#endif
			{			
				rx_data[pos++] = ucData;
				printf("%02X ",ucData);
			}
			else
				break;

			if(pos >= 500)
				break;
		}
		
		#else
		for(pos=0;pos<8;pos++)
		{
			//printf("%02X ",g_tCanRxMsg.Data[pos]);
			rx_data[pos] = g_tCanRxMsg.Data[pos];
		}
		#endif

		/* log */
		if(pos <= 4)
		{
			ret = 4;
			return ret;
		}
		
		while(1)
		{
			/*����֡ͷ*/
			if((rx_data[cur_pos] == FRAME_HEAD)||(rx_data[cur_pos] == 0))
			{
				break;
			}
			else
			{
				cur_pos++;
			}		
		}
		
		if((rx_data[FRAME_POS_HEAD+cur_pos] == FRAME_HEAD))//&&(rx_data[FRAME_POS_END+cur_pos] == FRAME_END))
		{
		
			App_SelfCheck_CAN_Init_Var();
			
			l_sum = 0;
			for(i=0;i<FRAME_SUM_LENTH;i++)
			{
				l_sum = l_sum ^ rx_data[FRAME_POS_address+cur_pos+i];
			}
			l_sum_low8 = (u8)l_sum&0xff;
			
			//У���
			if(l_sum_low8 == rx_data[FRAME_POS_SUM+cur_pos])		
			{
				address_fengqu = (rx_data[FRAME_POS_address+cur_pos]>>4)+1;
				address_paotou =  rx_data[FRAME_POS_address+cur_pos]&0x0f;
				#if 0
				/*ͬʱ������� ��ͷ�ĵ�ַ*/
				if((address_fengqu == g_QingTai.address_fengqu_cur)&&(address_paotou== g_QingTai.address_paotou_cur))
					{
				#else
				App_communication_address_point_change_CAN(address_fengqu,address_paotou);// ָ��ı��
				#endif
				/*����ģʽ�ĸ��� */
				high_4bit = rx_data[FRAME_POS_WORK_MODE+cur_pos]>>4;
				
				low_bit3 =  (rx_data[FRAME_POS_WORK_MODE+cur_pos]&0x08)>>3;// BIT3
				low_bit2 =  (rx_data[FRAME_POS_WORK_MODE+cur_pos]&0x04)>>2;	// BIT2
				low_bit1 =	(rx_data[FRAME_POS_WORK_MODE+cur_pos]&0x02)>>1;// BIT1
				low_bit0 =	(rx_data[FRAME_POS_WORK_MODE+cur_pos]&0x01)>>0; // BIT0
				
				if(low_bit0 == FRAME_CMD_COM_ERROR)// ����
				{
					g_tXiangchang_address_paotou_cur_CAN->com_error = FRAME_CMD_COM_ERROR;

					if(g_tXiangchang_address_paotou_cur_CAN->com_error!=g_tXiangchang_address_paotou_cur_CAN->com_error_bak)
					{
						App_Record_com_error_Paotou( address_fengqu, address_paotou);//��¼������Ϣ��
					}
					g_tXiangchang_address_paotou_cur_CAN->com_error_bak= g_tXiangchang_address_paotou_cur_CAN->com_error;
					ret = Communication_ASK_OK;
					return ret;//ͨѶ����ֱ�ӷ��ء�
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->com_error = FRAME_CMD_COM_OK;
					g_tXiangchang_address_paotou_cur_CAN->com_error_bak= g_tXiangchang_address_paotou_cur_CAN->com_error;
				}
				/* ͬһ��������ͬʱ��������ͷ������µ�ַ���� ����̨*/
				g_tXiangchang_address_paotou_cur_CAN->master_manchine = low_bit1;
				if((address_fengqu == g_QingTai.address_fengqu_master)&&(low_bit1 == FRAME_CMD_MASTER_MANCHINE))// ����  ���µ�ַ
				{
					address_fengqu = (rx_data[FRAME_POS_address+cur_pos]>>4)+1;
					address_paotou =  rx_data[FRAME_POS_address+cur_pos]&0x0f;
					#if 0
					/*���� ���� ��ͷ�ĵ�ַ*/
					g_QingTai.address_fengqu_master =address_fengqu;
					g_QingTai.address_paotou_master =address_paotou;
					App_communication_address_point_change_CAN(0,0);// cur ָ�벻�䣬masterָ���
					#endif
				}
				

				/** ����ģʽcontrl_mode **/
				g_tXiangchang_address_paotou_cur_CAN->contrl_mode = low_bit3;
				
				if(g_tXiangchang_address_paotou_cur_CAN->contrl_mode == FRAME_CMD_AUTO)
				{	
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					//if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					//	LED_STATUS.Hand_auto =False;
				}			
				else if(g_tXiangchang_address_paotou_cur_CAN->contrl_mode == FRAME_CMD_MANUL)
				{
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					//if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					//LED_STATUS.Hand_auto =True;
				}	
				if(g_tXiangchang_address_paotou_cur_CAN->contrl_mode_bak!=g_tXiangchang_address_paotou_cur_CAN->contrl_mode)
					App_Record_mode_change_Paotou( address_fengqu, address_paotou,g_tXiangchang_address_paotou_cur_CAN->contrl_mode);//��¼�رշ��ŵ���Ϣ��								
				
				g_tXiangchang_address_paotou_cur_CAN->contrl_mode_bak =g_tXiangchang_address_paotou_cur_CAN->contrl_mode; 			
					
				//�ֳ���Զ��ָʾ�� xianchang_yuancheng_contrl_mode
				g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode = low_bit2;
				if(g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode == FRAME_CMD_XIANGCHANG)
				{						
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{
						if(g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode !=g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode_bak)						
							LED_STATUS.xianchang_yuancheng =False;
					}
				}			
				else if(g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode == FRAME_CMD_YUANCHEN)
				{
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{
						if(g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode !=g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode_bak)						
							LED_STATUS.xianchang_yuancheng =True;
					}
				}	
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode = 0;
				}
				g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode_bak = g_tXiangchang_address_paotou_cur_CAN->xianchang_yuancheng_contrl_mode; 		

				
				/*����״̬ Ԥ��״̬�����ŵĿ���*/
				low_bit7 = (rx_data[FRAME_POS_FIRE_ON_STATE+cur_pos]&BIT7)>>7;/*����״̬*/
				low_bit6 =	(rx_data[FRAME_POS_FIRE_YUJING_STATE+cur_pos]&BIT6)>>6; /*Ԥ��״̬*/
				low_bit5 =	(rx_data[FRAME_POS_VALUE_STATE+cur_pos]&BIT5)>>5;	/*���ŷ�����״̬*/
				g_tXiangchang_address_paotou_cur_CAN->Value_State_return = low_bit5;
				
				low_bit4 = (rx_data[FRAME_POS_PUMP_STATE+cur_pos]&BIT4)>>4;/*ˮ��״̬Bit4*/				
				low_bit3 =	(rx_data[FRAME_POS_SELFCHECK_STATE+cur_pos]&BIT3)>>3;/*�Լ�״̬Bit3*/
				
				low_bit1 =	(rx_data[FRAME_POS_SELFCHECK_STATE+cur_pos]&BIT1)>>1;/* �ֳ������ж�״̬*/
				g_tXiangchang_address_paotou_cur_CAN->Value_State_xianchang = low_bit1;
				/*��ͷ�Ƿ񿪷��� */
				#if 0
				if( low_bit5 == FIRE_ON)
				{
					g_tXiangchang_address_paotou_cur_CAN->Value_State = FIRE_ON;
					g_tXiangchang_address_paotou_cur_CAN->Value_State_return = FIRE_ON;
					
					if(g_tXiangchang_address_paotou_cur_CAN->Value_State_return_bak !=g_tXiangchang_address_paotou_cur_CAN->Value_State_return)
						App_Record_value_open_Paotou( address_fengqu, address_paotou,FIRE_ON);//��¼��������Ϣ��						
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->Value_State = FIRE_OFF;
					g_tXiangchang_address_paotou_cur_CAN->Value_State_return = FIRE_OFF;


					if(g_tXiangchang_address_paotou_cur_CAN->Value_State_return_bak !=g_tXiangchang_address_paotou_cur_CAN->Value_State_return)
						App_Record_value_open_Paotou( address_fengqu, address_paotou,FIRE_OFF);//��¼��������Ϣ��						
					
				}	
				g_tXiangchang_address_paotou_cur_CAN->Value_State_return_bak =g_tXiangchang_address_paotou_cur_CAN->Value_State_return;			
				#else
				/*ֻ�����Զ�ģʽ�£��Ž��п������ŵ�����ͳ��*/
				if(g_tXiangchang_address_paotou_cur_CAN->contrl_mode == FRAME_CMD_AUTO)
				{
					App_value_action_process(address_fengqu,address_paotou,low_bit1);// �߼��жϣ��Ƿ�������  ��ŷ�������ͳ��
					
				}
				else// �ֶ�ģʽ�£��������ŵļ�¼
				{
				}
				if(g_tXiangchang_address_paotou_cur_CAN->Value_State_return_bak !=g_tXiangchang_address_paotou_cur_CAN->Value_State_return)
					App_Record_value_open_Paotou( address_fengqu, address_paotou,g_tXiangchang_address_paotou_cur_CAN->Value_State_return);//��¼�رշ��ŵ���Ϣ��								
				
				g_tXiangchang_address_paotou_cur_CAN->Value_State_return_bak =g_tXiangchang_address_paotou_cur_CAN->Value_State_return; 			
				

				#endif
				/*����״̬�ĸ��� */
				if(low_bit7 ==FIRE_ON)
				{
					//App_Fire_Init_Var();
					g_tXiangchang_address_paotou_cur_CAN->Fire_On_State = FIRE_ON;
	
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{	
						//LED_STATUS.Alarm_falg = True;
						//bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,TRUE);
					}
					if(g_tXiangchang_address_paotou_cur_CAN->Fire_On_State_bak!=g_tXiangchang_address_paotou_cur_CAN->Fire_On_State)
					{
						//LED_STATUS.Alarm_falg = True;
						#if 0
						bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						bsp_HC_595_OUT_DATA(FIRE_BEPP_OUTPUT_BEIYONG,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						bsp_HC_595_OUT_DATA(FIRE_BEPP_OUTPUT,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						bsp_HC_595_OUT_DATA(FIRE_PRE_ALARM_OUTPUT,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						bsp_HC_595_OUT_DATA(FIRE_ALARM_OUTPUT,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						#endif
						App_Record_Alarm_Paotou( address_fengqu, address_paotou,FIRE_ON);//��¼������Ϣ��
						#if 0
						// �б仯��ʱ�� �Ž��е�ַ���л� 20180712
						g_QingTai.address_fengqu_master = address_fengqu;
						g_QingTai.address_paotou_master = address_paotou;
						App_communication_address_point_change_CAN(0,0);// ָ��ı��
						#endif
					}
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->Fire_On_State = FIRE_OFF;
					//bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,FALSE);
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					//if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					//{
					//	if(LED_STATUS.Valve_flag == False)//��ʱ��ô��ƣ����Źرյ�ʱ�򣬱���ָʾ�Ʋſ��Թرա�
					//	 LED_STATUS.Alarm_falg = False;
						
						//bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,FALSE);
					//}
					if(g_tXiangchang_address_paotou_cur_CAN->Fire_On_State_bak!=g_tXiangchang_address_paotou_cur_CAN->Fire_On_State)
					{
						//  ��������ǹرյ�ʱ�� �ſ��Թرշ�������
						//if(g_tXiangchang_address_paotou_cur_CAN->Value_State == FIRE_OFF)
					}					
				}
				g_tXiangchang_address_paotou_cur_CAN->Fire_On_State_bak=g_tXiangchang_address_paotou_cur_CAN->Fire_On_State;					
				/*Ԥ��״̬�ĸ��� */
				if(low_bit6 ==FIRE_ON)
				{
					//App_Fire_Init_Var();
					g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag = FIRE_ON;
	
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{	
						//LED_STATUS.Alarm_falg = True;
						//bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,TRUE);
					}
					if(g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag_bak != g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag)
					{
						//LED_STATUS.Alarm_falg = True;
						#if 0
						bsp_HC_595_OUT_DATA(ENUM_HC_595_BEEP,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						bsp_HC_595_OUT_DATA(FIRE_PRE_ALARM_OUTPUT,TRUE);// �б仯��ʱ�� �Ž������ⱨ��
						#endif
						App_Record_YuJing_Alarm_Paotou( address_fengqu, address_paotou,FIRE_ON);//��¼Ԥ����Ϣ��
						// �б仯��ʱ�� �Ž��е�ַ���л� 20180712//
						/*ֻ��ת��һ����������ͷ*/
						if(g_QingTai.address_first_jump_flag == 0)
						{
							g_QingTai.address_first_jump_flag = 1;
							g_QingTai.address_fengqu_first_jump_record= address_fengqu;
							g_QingTai.address_paotou_first_jump_record= address_paotou;

							
							g_QingTai.address_fengqu_master = address_fengqu;
							g_QingTai.address_paotou_master = address_paotou;
							App_communication_address_point_change_CAN(0,0);// ָ��ı��
						}
					}
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag = FIRE_OFF;

					if(g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag_bak != g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag)
					{
						//  ��������ǹرյ�ʱ�� �ſ��Թرշ�������
						//if(g_tXiangchang_address_paotou_cur_CAN->Value_State == FIRE_OFF)
						/*��һ����������ͷ���Ԥ��֮��*/
						if((g_QingTai.address_fengqu_first_jump_record== address_fengqu)&&(g_QingTai.address_paotou_first_jump_record== address_paotou))
						{
							g_QingTai.address_first_jump_flag = 0;
							g_QingTai.address_fengqu_first_jump_record= address_fengqu;
							g_QingTai.address_paotou_first_jump_record= address_paotou;
						}
					}		
				}
				g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag_bak = g_tXiangchang_address_paotou_cur_CAN->Fire_yujing_flag;					
								
				/*ˮ�õ�״̬ */
				if(low_bit4 == FIRE_ON)
				{
					g_tXiangchang_address_paotou_cur_CAN->Pump_State= FIRE_ON;
					App_pump_action_process(FIRE_ON);
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					//if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{
						//LED_STATUS.Pump_flag =True;
						//bsp_HC_595_OUT_DATA(ENUM_HC_595_Water_Pump,TRUE);
					}
					
					if(g_tXiangchang_address_paotou_cur_CAN->Pump_State_bak!=g_tXiangchang_address_paotou_cur_CAN->Pump_State)
						App_Record_pump_open_Paotou( address_fengqu, address_paotou,FIRE_ON);//��¼������Ϣ��						
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->Pump_State = FIRE_OFF;
					App_pump_action_process(FIRE_OFF);
					/*�ǵ�ǰ���ص���ͷ�������ʾ */
					//if(g_tXiangchang_address_paotou_cur_CAN == g_tXiangchang_address_paotou_master_CAN)
					{
						//LED_STATUS.Pump_flag =False;
						//bsp_HC_595_OUT_DATA(ENUM_HC_595_Water_Pump,FALSE);
					}
					
					if(g_tXiangchang_address_paotou_cur_CAN->Pump_State_bak!=g_tXiangchang_address_paotou_cur_CAN->Pump_State)
						App_Record_pump_open_Paotou( address_fengqu, address_paotou,FIRE_OFF);//��¼������Ϣ��											
				}
 				g_tXiangchang_address_paotou_cur_CAN->Pump_State_bak =g_tXiangchang_address_paotou_cur_CAN->Pump_State;				
				/*�Լ�״̬ */
				//g_tXiangchang_address_paotou_cur_CAN->Selfcheck_State = low_4bit;				
				if(low_bit3 == SELF_CHECK_XIANCHANG_OK)
				{
					p_info("��̨  ���յ����Լ� SELF_CHECK_PAOTOU_OK %d",low_4bit);				
					g_tSelfCheck.flag_xianchang_ok[address_fengqu] = True;
					//if(g_tSelfCheck.flag_xianchang_ok[address_fengqu] != g_tSelfCheck.flag_xianchang_ok_bak[address_fengqu])
					//	App_Record_SelfcCheck_Fenqu( address_fengqu,FIRE_ON);//��¼�����Լ�������Ϣ��											
					//g_tSelfCheck.flag_paotou_ok = True;
					//LED_STATUS.Self_check_flag = True;
				}
				else
				{
					p_info("��̨ ���յ����Լ� not ok %d",low_4bit);
					g_tSelfCheck.flag_xianchang_ok[address_fengqu] = False;
					//if(g_tSelfCheck.flag_xianchang_ok[address_fengqu] != g_tSelfCheck.flag_xianchang_ok_bak[address_fengqu])
					//	App_Record_SelfcCheck_Fenqu( address_fengqu,FIRE_OFF);//��¼�����Լ�������Ϣ��					
					//g_tSelfCheck.flag_paotou_ok = False;
					//LED_STATUS.Self_check_flag = False;
				}
 				//g_tSelfCheck.flag_xianchang_ok_bak[address_fengqu] = g_tSelfCheck.flag_xianchang_ok[address_fengqu];				
				
				/*ˮƽ�Ƕ� ��ֱ�Ƕ�*/	
				Horizontal_angle_current = rx_data[FRAME_POS_HOR_ANGLE +cur_pos];
				Horizontal_angle_current = (Horizontal_angle_current<<1);
				g_tXiangchang_address_paotou_cur_CAN->Horizontal_angle_current = Horizontal_angle_current|(rx_data[FRAME_POS_VER_ANGLE +cur_pos]>>7);/*ˮƽ�Ƕ�*/  // byte 4
				
				g_tXiangchang_address_paotou_cur_CAN->Vertical_angle_current = rx_data[FRAME_POS_VER_ANGLE +cur_pos]&0x7f;/*��ֱ�Ƕ�*/	// byte 5
				
				/*��ͷ�Ƿ��ڳ�ʼλ�� */
				if(rx_data[FRAME_POS_CMD +cur_pos]== READY)
				{
					g_tXiangchang_address_paotou_cur_CAN->move_position_75_ready = READY;
				}
				else
				{
					g_tXiangchang_address_paotou_cur_CAN->move_position_75_ready = READY_NO;
				}				
				#if 0
					}
					else;
				#endif

				if(g_QingTai.value_open_answer == 1)
				{
					g_QingTai.value_open_answer= 0;
					
					bsp_StartTimer(TMR_COUNT_Communication,g_Communication.loop_time);
					
					App_communication_tx_chaxun_data_pro(0,COMMUNICATE_TYPE_CHAXUN);//�����ظ�
				}
				ret = Communication_ASK_OK;
				
			}
			else
			{
				ret = 2;

			}
		}
		else
			ret = 1;

    }
	else
		ret = 0;
	
    return ret;
}  

/*
*********************************************************************************************************
*	�� �� ��: App_communication_Loop
*	����˵��:  UPDATE  AND 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
u8 App_communication_Loop(void)
{	

	switch(g_Communication.work_state)
	{
		case Communication_INIT :
		{				
			App_communication_Init_Var();//���´��жϣ���ʼ������
			
			PAOTOU_Init_Var();
			
			g_Communication.work_state = Communication_LOOP;
			bsp_StartTimer(TMR_COUNT_Communication,g_Communication.loop_time);
			
			break;
		}

		case Communication_LOOP :
		{
			App_communication_rx_data_pro();// receive procce

			#if 1
			if(bsp_CheckTimer(TMR_COUNT_Communication))
			{
				bsp_StartTimer(TMR_COUNT_Communication,g_Communication.loop_time);
			
				g_Communication.work_state = Communication_LOOP;
				
				App_communication_address_change_pro();//
				App_communication_address_point_change_CAN(g_QingTai.address_fengqu_cur,g_QingTai.address_paotou_cur);// ָ��ı��
				App_communication_tx_chaxun_data_pro(0,COMMUNICATE_TYPE_CHAXUN);
			}
			#endif
			
			break;	
		}

		case Communication_TIMER_OUT :
		{
			
			break;
		}

		
		default:
			break;
	}



	return 0;


}







/*
*********************************************************************************************************
*	�� �� ��: App_communication_Test
*	����˵��:  UPDATE  AND 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void App_communication_Test(void)
{
	#if 1

	while(1)
	{

		App_communication_Loop();
	
	}
	#endif
}

