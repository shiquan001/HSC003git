#include "bsp_Ht1621.h"
#include "bsp.h"

 LCD_COM_1_4 g_LcdCom;

 /*****************
 us级的延时函数
 *************/

static void bsp_HT1621_delay_us(uint32_t tick)
{
	uint32_t i , j;
	for(i = 0; i < 100; i ++)
	{	
		for(j = 0; j < tick; j ++)
			tick --;
	}
}

													
 /***********************************************************
 *   @name  void bsp_HT1621_delay_us(uint32_t tick)
 *   @function  :  delay us
 *   @input  : time length
 *   @output : none                        
 *   @time   : 20170217
 *   @note	 :                          
 ***********************************************************/ 
void bsp_HT1621_delay_ms(uint32_t tick)
{
		bsp_HT1621_delay_us(500);
}
 /***********************************************************
 *   函数说明：bsp_HT1621写一个字节数据                         *
 *   输入：    无                                           *
 *   输出：    无                                           *
 *   调用函数：bsp_HT1621_delay_us(int)                               *
 ***********************************************************/
void bsp_HT1621_Write_Data(uint8_t Data,uint8_t cnt)
 {
	 uint8_t i;
	 for(i=0;i<cnt;i++)	
	 {
			 WR_LOW();
			 bsp_HT1621_delay_ms(1);
			 if(Data&0x80)
			 {DA_HIGH();}
			 else
			 DA_LOW();
			 bsp_HT1621_delay_ms(1);
			 WR_HIGH();
			 bsp_HT1621_delay_ms(1);
			 Data<<=1;
	 }
 }
 /***********************************************************
 *   函数说明：bsp_HT1621写命令                                 *
 *   输入：    无                                           *
 *   输出：    无                                           *
 *   调用函数：bsp_HT1621write_data(uint8_t add,uint8_t data)   *
                           bsp_HT1621_delay_us(int)                   *
 ***********************************************************/
 void bsp_HT1621_Write_Cmd(uint8_t Cmd)
 {
         CS_LOW();
         bsp_HT1621_delay_ms(1);
         bsp_HT1621_Write_Data(0x80,4);
         bsp_HT1621_Write_Data(Cmd,8);
         CS_HIGH();
         bsp_HT1621_delay_ms(1);
 }
 /***********************************************************
 *   函数说明：bsp_HT1621写数据                                 *
 *   输入：    无                                           *
 *   输出：    无                                           *
 *   调用函数：bsp_HT1621write_data(uint8_t add,uint8_t data)   * 
                          bsp_HT1621_delay_us(int)                    *
 ***********************************************************/
 void bsp_HT1621_Write0neData(uint8_t Addr,uint8_t Data)                        
{
         CS_LOW();
         bsp_HT1621_Write_Data(0xa0,3);
         bsp_HT1621_Write_Data(Addr<<2,6);
         bsp_HT1621_Write_Data(Data<<4,4);
         CS_HIGH();
         bsp_HT1621_delay_ms(1);
 }
 /***********************************************************
 *   函数说明：bsp_HT1621全部点亮                               *
 *   输入：    无                                           *
 *   输出：    无                                           *
 *   调用函数：bsp_HT1621write_data(uint8_t add,uint8_t data)   *
                           bsp_HT1621_delay_us(int)                   *
 ***********************************************************/
 void bsp_HT1621BWrite_AllData(uint8_t Addr,uint8_t *p,uint8_t cnt)
 {
         uint8_t i;
         CS_LOW();
         bsp_HT1621_Write_Data(0xa0,3);
         bsp_HT1621_Write_Data(Addr<<2,6);
         for(i=0;i<cnt;i++)
         {
                 bsp_HT1621_Write_Data(*p,8);
                 p++;
         }
         CS_HIGH();
         bsp_HT1621_delay_ms(1);
 }
 /*
 HT_IRQ   PD11
 HT_DATA	PD9
 HT_WR	PB15
 HT_RD	PB13
 HT_CS	PB12
 LED_GROUP1	PB14
 
 */
 
 void bsp_HT1621_Gpio_Init(void)
 {
	 GPIO_InitTypeDef GPIO_InitStruct;
	 
	WR_CLOCK_ENABLE();
	/*Configure GPIO pins : PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = WR_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(WR_PORTS, &GPIO_InitStruct);

	CS_CLOCK_ENABLE();
	CS_HIGH();
	/*Configure GPIO pins : PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = CS_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(CS_PORTS, &GPIO_InitStruct);

	DA_CLOCK_ENABLE();
	/*Configure GPIO pins : PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = DA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(DA_PORTS, &GPIO_InitStruct);


	RD_CLOCK_ENABLE();
	/*Configure GPIO pins : PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = RD_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(RD_PORTS, &GPIO_InitStruct);

	#if 0
	while(1)
	{
		p_info("GPIO_Led_Toggle");
	    GPIO_Led_Toggle();
		
		//CS_HIGH();
		WR_HIGH();
		HAL_Delay(100);
		//CS_LOW();
		WR_LOW();
		
		HAL_Delay(100);
	}
	#endif
 }
 
 
 /***********************************************************
 *   函数说明：bsp_HT1621 初始化                                *
 *   输入：    无                                           *
 *   输出：    无                                           *
 *   调用函数：bsp_HT1621write_Cmd(Cmd)                         *
                           _delay_ms(int)                   *
 ***********************************************************/
void bsp_HT1621_Init()
{
	bsp_HT1621_Gpio_Init();

	CS_HIGH();
	WR_HIGH();
	DA_HIGH();
	bsp_HT1621_delay_ms(2);
#if 0
	bsp_HT1621_Write_Cmd(BIAS);
	bsp_HT1621_Write_Cmd(RC256);                //内部时钟
	bsp_HT1621_Write_Cmd(SYSDIS);       				 //关振系统荡振器和LCD偏压发生器

	bsp_HT1621_Write_Cmd(WDTDIS);       				 //禁止看门狗

	bsp_HT1621_Write_Cmd(SYSEN);                //打开系统振荡漾器
	bsp_HT1621_Write_Cmd(LCDON);                //关LCD偏压
	bsp_HT1621_Write_Cmd(0x12);
	bsp_HT1621_delay_ms(1);
	bsp_HT1621_Write_Cmd(0x10);
#else
	bsp_HT1621_Write_Cmd(BIAS);
	bsp_HT1621_Write_Cmd(RC256);                //内部时钟
	bsp_HT1621_Write_Cmd(SYSDIS);       		//关振系统荡振器和LCD偏压发生器

	bsp_HT1621_Write_Cmd(IRQEN);       		
	bsp_HT1621_Write_Cmd(CLRWDT);       			
	bsp_HT1621_Write_Cmd(WDTEN);       				

	bsp_HT1621_Write_Cmd(SYSEN);                //打开系统振荡漾器
	bsp_HT1621_Write_Cmd(LCDON);                //
	bsp_HT1621_Write_Cmd(0x12);
	bsp_HT1621_delay_ms(1);
	bsp_HT1621_Write_Cmd(0x10);
#endif

	bsp_HT1621_Show_NULL();

}
void bsp_HT1621_Show_ALL(void)
{	
	uint8_t i;
	
	for(i = 0; i < DATA_LENTH; i ++)
		bsp_HT1621_Write0neData(i, 0xf);

} 
void bsp_HT1621_Show_NULL(void)
{	
	uint8_t i;
	
	for(i = 0; i < DATA_LENTH; i ++)
		bsp_HT1621_Write0neData(i, 0x00);

} 


void bsp_HT1621B_Send_AllData(void)
{
		uint8_t i=0;
		
		CS_LOW();
		bsp_HT1621_Write_Data(0xa0,3);
		bsp_HT1621_Write_Data(0<<2,6);
		
		for(i=0; i<DATA_LENTH; i++)
		{
				bsp_HT1621_Write_Data(g_LcdCom.SEG0_31[i]<<4,4);
		}
		CS_HIGH();
		bsp_HT1621_delay_ms(1);

}

void bsp_HT1621B_ClearDisplay(void)
{
		memset(g_LcdCom.SEG0_31,0,sizeof(g_LcdCom.SEG0_31));
}
void bsp_HT1621B_CloseDisplay(void)
{
	bsp_HT1621B_ClearDisplay();
	bsp_HT1621B_Send_AllData();
}
#if 0
void bsp_HT1621B_CloseLight(void)
{
}

void bsp_HT1621B_OpenLight(void)
{
}
/*
	检测lcd看门狗是否复位
*/
void bsp_HT1621B_WDT_OVER_INDICATER(void)
{
	//看门狗复位，重新初始化LCD
	if(LCD_WDT_IRQ_STATE() == Bit_RESET)
	{
		Ht1621b_Init();//重新初始化
		bsp_HT1621B_Send_AllData();//更新数据
		p_dbg_enter;
	}
	
}
#endif
