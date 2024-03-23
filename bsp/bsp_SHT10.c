/**************************************************************************************/
//Filename:SHT10.c
//Date :   2008-8-26
//SHT10数字温湿度传感器的读写程序
/**************************************************************************************/

#include "bsp.h"
#include "bsp_SHT10.h"

uint8_t m_IFPosNega = 0x01;


void SHT10_Delay1us()
{
	bsp_DelayUS(5);
}


/*---------------------------------------------------------------------
 功能描述: 开始采集
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
void s_transstart(void)  
// generates a transmission start   
//       _____         ________  
// DATA:      |_______|  
//           ___     ___  
// SCK : ___|   |___|   |______  
{    
  SHT10_SDA_H; 
  SHT10_SCL_L;                           //Initial state  
  SHT10_Delay1us();  
  SHT10_SCL_H;  
  SHT10_Delay1us();  
  SHT10_SDA_L;  
  SHT10_Delay1us();  
  SHT10_SCL_L;    
  SHT10_Delay1us();SHT10_Delay1us();SHT10_Delay1us();  
  SHT10_SCL_H;  
  SHT10_Delay1us();  
  SHT10_SDA_H;         
  SHT10_Delay1us();  
  SHT10_SCL_L;         
}  

/*---------------------------------------------------------------------
 功能描述: 重新连接
 参数说明: 无
 函数返回: 无
 ---------------------------------------------------------------------*/
void s_connectionreset(void)  
// communication reset: DATA-line=1 and at least 9 SCK cycles followed by transstart  
//       _____________________________________________________         ________  
// DATA:                                                      |_______|  
//          _    _    _    _    _    _    _    _    _        ___     ___  
// SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______  
{    
  uint8_t i;   
  SHT10_SDA_H; 
  SHT10_SCL_L;                                             //Initial state 
  for(i=0;i<9;i++)                                      //9 SCK cycles  
  {  
    SHT10_SCL_H;
    SHT10_Delay1us(); 
    SHT10_SCL_L;
    SHT10_Delay1us(); 
  }  
  s_transstart();                                       //transmission start  
}  

/*---------------------------------------------------------------------
 功能描述: 写字节数据
 参数说明: value - 字节数据
 函数返回: 无
 ---------------------------------------------------------------------*/
uint8_t s_write_byte(uint8_t value)  
//----------------------------------------------------------------------------------  
// writes a byte on the Sensibus and checks the acknowledge   
{   
  uint8_t i,error=0;  
  
  for (i=0x80; i>0; i/=2)                                       //shift bit for masking  
  {   
    if (i&value) SHT10_SDA_H;                                     //masking value with i , write to SENSI-BUS  
    else SHT10_SDA_L;                          
    SHT10_SCL_H;                                                 //clk for SENSI-BUS  
    SHT10_Delay1us();                                            //pulswith approx. 5 us      
    SHT10_SCL_L;
    SHT10_Delay1us();
  }  
  SHT10_SDA_H;                                                    //release DATA-line 
  
  SHT10_SDA_I;                                                  //设置SDA为输入
  
  SHT10_SCL_H;                                                     //clk #9 for ack   
  error = SHT10_SDA_DAT;                                              //check ack (DATA will be pulled down by DHT90),DATA在第9个上升沿将被DHT90自动下拉为低电平。  
  SHT10_Delay1us();     
  SHT10_SCL_L;
  SHT10_Delay1us();
  
  SHT10_SDA_O;                                                  //设置SDA为输出
  
  return error;                                                //error=1 in case of no acknowledge //返回：0成功，1失败 
}  

/*---------------------------------------------------------------------
 功能描述: 读字节数据
 参数说明: ack - 读完成
 函数返回: 返回字节数据
 ---------------------------------------------------------------------*/
uint8_t s_read_byte(uint8_t ack)   
// reads a byte form the Sensibus and gives an acknowledge in case of "ack=1"   
{   
  uint8_t i,val=0;  
  SHT10_SDA_H;                                                    //release DATA-line 
  
  SHT10_SDA_I;                                                  //设置SDA为输入
  
  for (i=0x80;i>0;i/=2)                                         //shift bit for masking  
  { 
    SHT10_SCL_H;                                                 //clk for SENSI-BUS  
    if (SHT10_SDA_DAT) val=(val | i);                                 //read bit    
    SHT10_Delay1us();                                            //pulswith approx. 5 us 
    SHT10_SCL_L;
    SHT10_Delay1us();
  } 
  
  SHT10_SDA_O;                                                  //设置SDA为输出
  
  if (ack==1) SHT10_SDA_L;                                        //in case of "ack==1" pull down DATA-Line  
  else SHT10_SDA_H;                                               //如果是校验(ack==0)，读取完后结束通讯 
  SHT10_Delay1us();                                             //pulswith approx. 5 us  
  SHT10_SCL_H;                                                     //clk #9 for ack  
  SHT10_Delay1us();                                             //pulswith approx. 5 us   
  SHT10_SCL_L;                  
  SHT10_Delay1us();                                                //pulswith approx. 5 us  
  SHT10_SDA_H;                                                    //release DATA-line  
  return val;  
}  

/*---------------------------------------------------------------------
 功能描述: 采集温度和湿度数据
 参数说明: *p_value
          *p_checksum
          mode
 函数返回: 采集状态
 ---------------------------------------------------------------------*/
uint8_t s_measure(uint16_t* p_value, uint8_t *p_checksum, uint8_t mode)  
// makes a measurement (humidity/temperature) with checksum  
{   
  uint8_t  error=0;  
  uint16_t i;  
  
  s_transstart();                                                //transmission start  
  switch(mode){                                                 //send command to sensor  
  case 0x01  : error += s_write_byte(MEASURE_TEMP); break;  
  case 0x02  : error += s_write_byte(MEASURE_HUMI); break;  
  default    : break;     
  }
  
  SHT10_SDA_I;                                                  //设置SDA为输入
  
  for (i=0;i<65535;i++) 
  {
    SHT10_Delay1us();
    if(!SHT10_SDA_DAT) break;                                      //wait until sensor has finished the measurement
  }
  if(SHT10_SDA_DAT) error+=1;                                         // or timeout (~2 sec.) is reached
  
  SHT10_SDA_O;                                                  //设置SDA为输出
  
  (*p_value)  =s_read_byte(ACK);                                //read the first byte (MSB)
  (*p_value) <<= 8;
  (*p_value) |=s_read_byte(ACK);                                //read the second byte (LSB)  
  (*p_checksum) =s_read_byte(noACK);                            //read checksum  
  return error;  
}


/*---------------------------------------------------------------------
 功能描述: 解析温湿度数据
 参数说明: *p_humidity - 湿度数据
          *p_temperature - 温度数据
 函数返回: 无
 ---------------------------------------------------------------------*/
void calc_sth11(float *p_humidity ,float *p_temperature)
// calculates temperature [C] and humidity [%RH]
// input : humi [Ticks] (12 bit)
// temp [Ticks] (14 bit)
// output: humi [%RH]
// temp [C]
{
  const float C1=-4.0f;                                  // for 12 Bit 
  const float C2=+0.0405f;                               // for 12 Bit 
  const float C3=-0.00028f;                              // for 12 Bit   (0.0000028*100)
  const float T1=0.01f;                                  // for 12 Bit
  const float T2=0.00008f;                               // for 12 Bit
  
  const float d2= 0.01f;                                 // for 14 Bit @ 5V
  
  
  float rh=*p_humidity;                                 // rh:      Humidity [Ticks] 12 Bit  
  float t=*p_temperature;                               // t:       Temperature [Ticks] 14 Bit 
  float rh_lin;                                         // rh_lin:  Humidity linear 
  float rh_true;                                        // rh_true: Temperature compensated humidity 
  float t_C;                                            // t_C   :  Temperature [C] 
  
  float C3h;
  float C2h;
  float T2h;
  float rhh;
  float D2T;
  
  rhh = rh/100.0f;
  C3h = C3*rh;
  C3h *= rhh;
  C2h = C2*rh;
  T2h = (T2*rh)+T1;
  
  D2T = d2*t;
  
  if (D2T > 40.0f)
  {
    m_IFPosNega = 0x01;
    t_C=D2T - 40.0f; 		                        //calc. temperature from ticks to [C] 
  }
  else
  {
    m_IFPosNega = 0;
    t_C=40.0f - D2T; 		                        //calc. temperature from ticks to [C]
  }
  rh_lin = C1 + C2h + C3h;                              //calc. humidity from ticks to [%RH]  
  rh_true = (t_C-25.0f)*T2h + rh_lin;                    //calc. temperature compensated humidity [%RH]
  if(rh_true > 99.9f) rh_true = 99.9f;                    //cut if the value is outside of 
  if(rh_true < 0.1f) rh_true = 0.1f;                      //the physical possible range 
  
  *p_humidity = rh_true;
  *p_temperature = t_C;                                 //return temperature [C] 
  
}


/*-------------------------------------------------------
功能描述：函数初始化
参    数：无
返 回 值：无
---------------------------------------------------------*/
void bsp_SHT10_Init(void)
{
  
  SHT10_SCL_O;                          //设置SCLK为输出
  SHT10_SDA_O;                          //设置SDA为输出
  
  s_connectionreset();
  
  /*STHSCK = 0;
  STHDATA = 0;
  STHSCK = 1;
  STHDATA = 1;*/
  
}

/*---------------------------------------------------------------------
 功能描述: 读取温湿度数据
 参数说明: *vTemSymbol - 温度符号位、
          *vTem - 温度数据
          *vHum - 湿度数据
 函数返回: 无
 ---------------------------------------------------------------------*/
void bsp_Get_TH(uint8_t *vTemSymbol, uint16_t *vTem, uint16_t *vHum)
{
  value humi_val,temp_val;
  uint8_t error,checksum;
  
  uint16_t vtem;
  uint16_t vhum;
  
  s_connectionreset();
  error=0;
  error+=s_measure(&(humi_val.i), &checksum, 0x02);     //measure humidity
  error+=s_measure(&(temp_val.i), &checksum, 0x01);     //measure temperature
  if (error != 0)
  {
    s_connectionreset();                                //in case of an error: connection reset
  }
  else
  {
    humi_val.f=(float)humi_val.i;                         //converts integer to float
    temp_val.f=(float)temp_val.i;                         //converts integer to float
    calc_sth11(&(humi_val.f), &(temp_val.f));            //calculate humidity, temperature
    
    if (humi_val.f<99.9f && humi_val.f>0.1f && temp_val.f>-40.0f && temp_val.f<120.0f)
    {
      vtem = (uint16_t)(temp_val.f*10.0f);
      vhum = (uint16_t)(humi_val.f*10.0f);
      
      if (m_IFPosNega)
      {
        if (vtem>10)
        {
          vtem = (vtem-10);
        }
        else
        {
          m_IFPosNega = 0;
          vtem = (10 - vtem);
        }
      }
      else
      {
        vtem = (vtem+10);
      }
      
      *vTemSymbol = m_IFPosNega;
      *vTem = vtem;
      *vHum = vhum;
    }
  }
  
}

