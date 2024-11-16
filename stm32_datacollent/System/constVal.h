#ifndef CONSTVAL_H
#define CONSTVAL_H
#include <stdint.h>
#include <time.h>

typedef struct{
	struct{
		time_t clock_count;		//utc时间，在主机中转换
	}clock;	
	struct{
		int16_t AccX;
		int16_t AccY;
		int16_t AccZ;
		int16_t GyroX;
		int16_t GyroY;
		int16_t GyroZ;
	}MPU6050_data;		//6050数据
	struct{
		int16_t data_hum;	//湿度
		int16_t data_temp;	//温度
	}DHT11_data;
}buf_struct;

extern uint16_t collectFrequance;
extern uint16_t MyRTC_Time[6];
extern uint8_t  DHT11_rec_data[4]; 
extern char ref_stringLocalTime[24];
extern uint16_t TIM3_count;	
extern int16_t MPU6050_data[6];
extern uint8_t modbusSlave_id;


extern buf_struct* my_buf_struct;
//错误状态码
extern uint8_t DHT11_errorCode;

#endif

