#ifndef MCU6050
#define MCU6050

void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData_ood(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
uint8_t MPU6050_ReadDatas(uint8_t RegAddress,uint8_t* arr_12);
void MPU6050_GetData(int16_t* arr_6);



//void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
//uint8_t MPU6050_ReadReg(uint8_t RegAddress);
//uint8_t MPU6050_ReadDatas(uint8_t RegAddress,uint8_t* arr_12);
//void MPU6050_Init(void);
//uint8_t MPU6050_GetID(void);
//void MPU6050_GetData_old(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
//						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
//void MPU6050_GetData(int16_t* arr_6);
//void MPU6050_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
//void MPU6050_GetData_ood(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
//						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
#endif

