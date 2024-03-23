#ifndef _BSP_SHT10_T_H
#define _BSP_SHT10_T_H

/*--------------------------ͷ�ļ�����--------------------------------*/

#include "stm32l4xx_hal.h"

#include "bsp_LUX_V20.h"

/*-----------------------------�ṹ�嶨��---------------------------------*/
typedef union
{ 
  uint16_t i;
  float   f;
  
}value;


/*-----------------------------�궨��---------------------------------*/
#define SHT10_SCL_O    bsp_LUX_SCL0_O
#define SHT10_SCL_H    bsp_LUX_SCL0_H
#define SHT10_SCL_L    bsp_LUX_SCL0_L

#define SHT10_SDA_O    bsp_LUX_SDA0_O
#define SHT10_SDA_H    bsp_LUX_SDA0_H
#define SHT10_SDA_L    bsp_LUX_SDA0_L

#define SHT10_SDA_I    bsp_LUX_SDA0_I
#define SHT10_SDA_DAT  bsp_LUX_SDA0_DAT


#define noACK 0                                 //�����ж��Ƿ����ͨѶ 
#define ACK   1                                 //�������ݴ��� 
                                                //adr  command  r/w  
//#define STATUS_REG_W 0x06                     //000   0011    0  
//#define STATUS_REG_R 0x07                     //000   0011    1  
#define MEASURE_TEMP 0x03                       //000   0001    1  
#define MEASURE_HUMI 0x05                       //000   0010    1  
//#define RESET        0x1e                     //000   1111    0 

/*---------------------------�ⲿ��������-----------------------------*/

/*-----------------------------��������-------------------------------*/
void SHT10_Delay1us(void);

uint8_t s_measure(uint16_t* p_value, uint8_t *p_checksum, uint8_t mode);
void calc_sth11(float *p_humidity ,float *p_temperature);
void s_transstart(void);
void s_connectionreset(void);
uint8_t s_write_byte(uint8_t value);
uint8_t s_read_byte(uint8_t ack);

void bsp_SHT10_Init(void);
void bsp_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum);

#endif
