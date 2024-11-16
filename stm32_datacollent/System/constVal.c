#include "stm32f10x.h"                  // Device header
#include "time.h"
#include "bsp_modbus.h"
#include "constVal.h"


buf_struct* my_buf_struct=(buf_struct*)REG_INPUT_START;		//modbus输入寄存器

uint16_t collectFrequance=100; //采样周期T=100ms
uint16_t MyRTC_Time[] = {2000, 1, 1, 0, 0, 0};	//定义全局的时间数组，数组内容分别为年、月、日、时、分、秒
uint16_t TIM3_count=0;		//每100ms计数一次
char 	 ref_stringLocalTime[24]={0};		//string格式的标准时间 2024-12-12 12:12:12:100


int16_t MPU6050_data[6]={0};
uint8_t  DHT11_rec_data[4]={0}; //DHT11湿度+温度
uint8_t DHT11_errorCode=0;
uint8_t modbusSlave_id=0x01;	//modbus从机地址










