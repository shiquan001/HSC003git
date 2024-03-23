/*
*********************************************************************************************************
*
*	ģ������ : ����ͷ����BSPģ��(For NT99141)
*	�ļ����� : bsp_camera.c
*	��    �� : V1.0
*	˵    �� : NT99141��������
*
*			  ������ο��� guanfu_wang �ṩ�����ӡ�http://mcudiy.taobao.com/,���ο��˰�����V5��OV7670����
*
*********************************************************************************************************
*/


#include "bsp_camera.h"
#include "bsp.h"
#include "stdio.h"

#include "app_picture.h"


/*
	����ΪNT99141�ĳ�ʼ���õļĴ��������б�
*/

static const unsigned int  NT99141_Init_Reg[NT99141_INIT_REG_NUM][2] =
{
	{0x3109,0x04},
	{0x3040,0x04},
	{0x3041,0x02},
	{0x3042,0xFF},
	{0x3043,0x08},
	{0x3052,0xE0},
	{0x305F,0x33},
	{0x3100,0x07},
	{0x3105,0x01},
	{0x3106,0x03},
	{0x3108,0x05},
	{0x3110,0x22},
	{0x3111,0x57},
	{0x3112,0x22},
	{0x3113,0x55},
	{0x3114,0x05},
	{0x3135,0x00},
	{0x32F0,0x01},
	{0x3290,0x01},
	{0x3291,0xA0},
	{0x3296,0x01},
	{0x3297,0x73}	
};

/*
	����ΪNT99141��JPEG�õļĴ��������б�
*/

static const unsigned int  NT99141_JPEG_Reg[NT99141_JPEG_REG_NUM][2] =
{
	{0x32BF, 0x60}, 
	{0x32C0, 0x6A},
	{0x32C1, 0x6A},
	{0x32C2, 0x6A}, 
	{0x32C3, 0x00}, 
	{0x32C4, 0x20},
	{0x32C5, 0x20}, 
	{0x32C6, 0x20}, 
	{0x32C7, 0x00},
	{0x32C8, 0xDD}, 
	{0x32C9, 0x6A}, 
	{0x32CA, 0x8A}, 
	{0x32CB, 0x8A}, 
	{0x32CC, 0x8A}, 
	{0x32CD, 0x8A}, 
	{0x32DB, 0x7B}, 
	{0x32F0, 0x70}, 
	{0x3400, 0x08}, 
	{0x3400, 0x00}, 
	{0x3401, 0x4E}, 
	{0x3404, 0x00}, 
	{0x3405, 0x00}, 
	{0x3410, 0x00}, 
	{0x32E0, 0x02},
	{0x32E1, 0x80}, 
	{0x32E2, 0x01}, 
	{0x32E3, 0xE0}, 
	{0x32E4, 0x00}, 
	{0x32E5, 0x80}, 
	{0x32E6, 0x00},
	{0x32E7, 0x80}, 
	{0x3200, 0x3E}, 
	{0x3201, 0x0F}, 
	{0x3028, 0x24}, 
	{0x3029, 0x20}, 
	//{0x302A, 0x04},// PCLK = (PLL_CKOUT) / 2
	//{0x302A, 0x08},// PCLK = (PLL_CKOUT) / 4
	{0x302A, 0x0C}, // PCLK = (PLL_CKOUT)    / 8
	
	//{0x302A, 0x08},// PCLK = (PLL_CKOUT) / 4 = 15MHZ
	//{0x302A, 0x0C},// PCLK = (PLL_CKOUT) / 8 = 7.5MHZ
	{0x3022, 0x24},//0x24}, //��ֱ��ת��д0x25�������д0x27
	{0x3023, 0x24}, 
	{0x3002, 0x00}, 
	{0x3003, 0xA4},
	{0x3004, 0x00}, 
	{0x3005, 0x04}, 
	{0x3006, 0x04},
	{0x3007, 0x63}, 
	{0x3008, 0x02}, 
	{0x3009, 0xD3}, 
	{0x300A, 0x06}, 
	{0x300B, 0x8B},
	{0x300C, 0x02},
	{0x300D, 0xE0}, 
	{0x300E, 0x03}, 
	{0x300F, 0xC0}, 
	{0x3010, 0x02}, 
	{0x3011, 0xD0}, 
	{0x32B8, 0x3F}, 
	{0x32B9, 0x31}, 
	{0x32BB, 0x87}, 
	{0x32BC, 0x38}, 
	{0x32BD, 0x3C},
	{0x32BE, 0x34}, 
	{0x3201, 0x7F}, 
	{0x3021, 0x06}, 
	{0x3400, 0x01}, 
	{0x3060, 0x01} 
};


CAM_T g_tCam;

u8 JpegBuffer[ PIC_BUFF_LEN];

void bsp_Init_camera_Gpio(void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitI2C
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init_camera_rest_power_Gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = SEN_PWDN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SEN_PWDN_GPIO_Port, &GPIO_InitStruct);


	/*Configure GPIO pins : PCPin PCPin */
	GPIO_InitStruct.Pin = NRESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(NRESET_GPIO_Port, &GPIO_InitStruct);

}
/*
*********************************************************************************************************
*	�� �� ��: bsp_Init_OV2640_CaptureGpioInit
*	����˵��: 

VIN_D7--VIN_D0	PB7--PB0	����	
VIN_PCLK	PC10	����ʱ��	 DCMI_PCKPolarity_Rising 
VIN_VSYNC	PC12	֡ͬ��	DCMI_VSPolarity_Low;
VIN_HSYNC	PC11	��ͬ��	DCMI_HSPolarity_Low; 

*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_Init_camera_CaptureGpioInit(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;
	
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
#if 0
	/*Configure GPIO pins : PBPin PBPin PBPin PBPin 
	       PBPin PBPin PBPin PBPin */
	GPIO_InitStruct.Pin = VIN_D0_Pin|VIN_D1_Pin|VIN_D2_Pin|VIN_D3_Pin 
	      |VIN_D4_Pin|VIN_D5_Pin|VIN_D6_Pin|VIN_D7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = VIN_PCLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(VIN_PCLK_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PCPin PCPin */
	GPIO_InitStruct.Pin = VIN_HSYNC_Pin|VIN_VSYNC_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif

	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	
	bsp_camera_ov_ItDisable();// �ȹر��ж�
}

void bsp_camera_ov_ItEnable(void)
{
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
void bsp_camera_ov_ItDisable(void)
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);// �ȹر��ж�

}

/*
*********************************************************************************************************
*	�� �� ��: NT99141_Hardware_Init
*	����˵��: Ӳ����ʼ��NT99141
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void NT99141_Hardware_Init(void)
{
	// when have PCB pin map, do it.
	//PE0---PWDN, PE1---RESET
	
	START_RESET_NT99141();
	bsp_DelayMS(20);
	END_RESET_NT99141();
	bsp_DelayMS(20);

	END_POWER_DOWN();
	bsp_DelayMS(20);
	bsp_DelayMS(20);
}



/*
*********************************************************************************************************
*	�� �� ��: NT99141_InitReg
*	����˵��: ��λnt99141, ����nt99141�ļĴ���
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��ȷ��1��ʾʧ��
*********************************************************************************************************
*/
uint8_t NT99141_InitReg(void)
{
  	uint8_t i;
	uint8_t reset_register = 0;
//  CAM_ConfigCPU();      /* ����DCMI����, bsp_InitCamera()���Ѿ���������Ͳ������ˣ�ֻ��������NT99141�ļĴ��� */
	//bsp_InitI2C();		/* ����I2C����, �� bsp.c �ļ�ִ���� */

	reset_register = NT99141_ReadReg16(0X3021);	/* software Reset */
	reset_register |= 0x01; /* bit0 set: to initial the NT99141,and the hardware will clear this bit autoly */
	NT99141_WriteReg16(0X3021, reset_register);
	
	bsp_DelayMS(5);

	/* ���� NT99141��ʼ�Ĵ��� */
  	for (i = 0; i < NT99141_INIT_REG_NUM; i++)
  	{
		NT99141_WriteReg16((uint16_t)NT99141_Init_Reg[i][0], (uint8_t)NT99141_Init_Reg[i][1]);
  	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: NT99141_InitJPEG
*	����˵��: ����NT99141����JPEG��ʽ���
*	��    ��: ��
*	�� �� ֵ: 0 ��ʾ��ȷ��1��ʾʧ��
*********************************************************************************************************
*/
uint8_t NT99141_JPEG_Config(void)
{
  	uint8_t i;

	/* ���� JPEG�üĴ��� */
  	for (i = 0; i < NT99141_JPEG_REG_NUM; i++)
  	{
		NT99141_WriteReg16((uint16_t)NT99141_JPEG_Reg[i][0], (uint8_t)NT99141_JPEG_Reg[i][1]);
  	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: Camera_NT99141_Start
*	����˵��: ����DMA��DCMI����ʼ����ͼ�����ݵ�LCD�Դ�
*	��    ��: _uiDispMemAddr �Դ��ַ
*	�� �� ֵ: ��
*	note	:���紫��Ļ�����Ҫ��д�ú�����������д���������������2017��7��17��12:15:26
*********************************************************************************************************
*/
void Camera_NT99141_Start(void)
{
	p_info("Camera_NT99141_Start--");
	g_tCam.CaptureOk = 0;
	
	g_tCam.JpegDataCnt = 0;

	g_tCam.VsyncActive_count =0;
	
	g_tCam.VsyncActive	= VsyncActive_NULL;
	g_tCam.HSYNCActive = HSYNCActive_NULL;

	g_tCam.VsyncActive_fall_count = 0;
	g_tCam.VsyncActive_rise_count = 0;
	
	g_tCam.HSYNCActive_fall_Count = 0;
	g_tCam.HSYNCActive_rise_Count= 0;

	g_tCam.pclk_count = 0;
	
	bsp_camera_ov_ItEnable();//
	
}

/*
*********************************************************************************************************
*	�� �� ��: Camera_NT99141_Stop
*	����˵��: ֹͣDMA��DCMI
*	��    ��: ��
*	�� �� ֵ: ��
*	note	:���紫��Ļ���������Ҫ��д�ú�����������д���������������2017��7��17��12:15:26
*********************************************************************************************************
*/
void Camera_NT99141_Stop(void)
{
	START_POWER_DOWN();

	bsp_camera_ov_ItDisable();
	g_tCam.VsyncActive	= VsyncActive_NULL;
	g_tCam.HSYNCActive = HSYNCActive_NULL;
	p_info("Camera_NT99141_Stop--");
}


/*
*********************************************************************************************************
*	�� �� ��: NT99141_WriteReg16
*	����˵��: дNT99141
*	��    ��: _ucRegAddr  : �Ĵ�����ַ //�Ĵ�����ַ��Ϊ16λ
*			  _ucRegValue : �Ĵ���ֵ //�Ĵ���ֵ��Ȼ��8λ
*	�� �� ֵ: no 
*********************************************************************************************************
*/
void NT99141_WriteReg16(uint16_t _ucRegAddr, uint8_t _ucRegValue)
{
    i2c_Start();							/* ���߿�ʼ�ź� */

    i2c_SendByte(NT99141_SLAVE_ADDRESS_WR);		/* �����豸��ַ+д�ź� */
	if (i2c_WaitAck() != (u8)0)
	{
#if	(DEGUG_CAMERA == 0x01)
		printf("write nt99141 device addr no ack\r\n");
#endif
	}

    i2c_SendByte((u8)((u16)_ucRegAddr >> 8));				/* ���ͼĴ����ߵ�ַ */
	if (i2c_WaitAck() != (u8)0)
	{
#if (DEGUG_CAMERA == 0x01)
		printf("write nt99141 reg high addr no ack\r\n");
#endif
	}

    i2c_SendByte((u8)_ucRegAddr);				/* ���ͼĴ����͵�ַ */
	if (i2c_WaitAck() != (u8)0)
	{
#if (DEGUG_CAMERA == 0x01)
		printf("write nt99141 reg low addr no ack\r\n");
#endif
	}
    i2c_SendByte(_ucRegValue);				/* ���ͼĴ�����ֵ */
	if (i2c_WaitAck() != (u8)0)
	{
#if (DEGUG_CAMERA == 0x01)
		printf("write nt99141 reg value no ack\r\n");
#endif
	}

    i2c_Stop();                   			/* ����ֹͣ�ź� */
	
}

/*
*********************************************************************************************************
*	�� �� ��: NT99141_ReadReg16
*	����˵��: //��NT99141
*	��    ��: _ucRegAddr  : �Ĵ�����ַ //�Ĵ�����ַ��Ϊ16λ
*			  
*	�� �� ֵ: �Ĵ���ֵ //�Ĵ���ֵ��Ȼ��8λ
*********************************************************************************************************
*/
uint8_t NT99141_ReadReg16(uint16_t _ucRegAddr)
{
	uint8_t ret = 0;
    i2c_Start();							/* ���߿�ʼ�ź� */

    i2c_SendByte(NT99141_SLAVE_ADDRESS_WR);		/* �����豸��ַ+д�ź� */
	if (i2c_WaitAck() != (u8)0)
	{
#if	(DEGUG_CAMERA == 0x01)
		printf("write nt99141 device addr no ack\r\n");
#endif
	}

    i2c_SendByte((u8)((u16)_ucRegAddr >> 8));				/* ���ͼĴ����ߵ�ַ */
	if (i2c_WaitAck() != (u8)0)
	{
#if (DEGUG_CAMERA == 0x01)
		printf("write nt99141 reg high addr no ack\r\n");
#endif
	}

    i2c_SendByte((u8)_ucRegAddr);				/* ���ͼĴ����͵�ַ */
	if (i2c_WaitAck() != (u8)0)
	{
#if (DEGUG_CAMERA == 0x01)
		printf("write nt99141 reg low addr no ack\r\n");
#endif
	}
    i2c_Start();							/* �ظ���ʼ�ź� */

    i2c_SendByte(NT99141_SLAVE_ADDRESS_RD);		/* �����豸��ַ+���ź� */
	if (i2c_WaitAck() != (u8)0)
	{
#if	(DEGUG_CAMERA == 0x01)
		printf("rewrite nt99141 device addr no ack\r\n");
#endif
	}

    ret = i2c_ReadByte();				/* ���ͼĴ�����ֵ */

	i2c_NAck();								/* ������Ӧ�����������ζ����� */

    i2c_Stop();                   			/* ����ֹͣ�ź� */
	return (ret);
}

/*
*********************************************************************************************************
*	�� �� ��: NT99141_ReadID
*	����˵��: ��NT99141��оƬID
*	��    ��: ��
*	�� �� ֵ: оƬID. bit15-bit4 : chip number, bit3-bit0: version
*********************************************************************************************************
*/
uint16_t NT99141_ReadID(void)
{
	uint16_t id = 0;

	id = (uint16_t)NT99141_ReadReg16(0x3000);
	id = ((uint16_t)id << 8);
	id += (uint16_t)NT99141_ReadReg16(0x3001);
	return (id);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitCameraNT99141
*	����˵��: ��������ͷGPIO��CAMERA�豸.
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init_Camera_NT99141(void)
{
	uint16_t id = 0;

	bsp_InitI2C(); //IIC initial
	
	//DCMI_Init_Stm32f407(); //DCMI interface init
	
	bsp_Init_camera_CaptureGpioInit();/* ���ݲɼ����ų�ʼ��*/
	bsp_camera_ov_ItDisable();

	bsp_Init_camera_rest_power_Gpio();
	NT99141_Hardware_Init(); //init nt99141 hareware

	
	NT99141_InitReg(); //init NT99141 register

	NT99141_JPEG_Config(); // config NT99141 output 640*480 JPEG image

	bsp_DelayMS(100);
	id = NT99141_ReadID();
	p_info("NT99141_ReadID:%d",id);
}

