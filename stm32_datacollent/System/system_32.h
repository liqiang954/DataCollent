#ifndef SYSTEM_32
#define SYSTEM_32



void TIM3_initConfig(void);
void MyRTC_SetTime(void);
void MyRTC_Init(void);
void MyRTC_ReadTime(void);
size_t get_currentTime(char* buffer, size_t limit_size);
void MPU6050_showData(void);
uint8_t check_BSP_initCondition(void);
void set_acqFinish_Flags(uint8_t slave_address);
time_t MyRTC_ReadTime_count(void);
void MyRTC_SetTime_count(time_t time_cnt);
void TIME_acq(void);



#endif


