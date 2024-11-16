#ifndef __DHT11_H
#define __DHT11_H




#define dht11_high GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define dht11_low GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define Read_Data GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)


void WriteIO(uint8_t BitValue);
void DH11_GPIO_Init(void);
void DHT11_Start(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadData(uint8_t *Hum,uint8_t *FHum,uint8_t *Temp,uint8_t *FTemp);
void DHT11_acq(void);
void DHT11_showData(void);	

void ceshi(void);

#endif

