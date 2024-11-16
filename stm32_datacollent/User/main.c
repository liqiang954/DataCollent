#include "stm32f10x.h"                  // Device header
#include "bsp_modbus.h"
#include "constVal.h"
#include "OLED.h"
#include "DHT11.h"
#include "Delay.h"
#include "time.h"
#include "mytool.h"
#include "system_32.h"
#include "mpu6050.h"

#include "string.h"

//******函数声明*************
void BSP_init(void);
//*******************


uint8_t break_flag=0; //中断标志
int main(void)
{	
	//初始化
	BSP_init();
	
	while(1)
	{
		if(break_flag)
		{
		
			/***100ms时间片***/
			//数据采集
			MPU6050_GetData(MPU6050_data);
			//等待主站轮询
			
			/***100ms时间片退出***/
			++TIM3_count;
			break_flag=0;	
			if(TIM3_count>=10)	//整数秒
			{	/***整数秒***/
				TIM3_count=0;
				//低速传感器操作
				TIME_acq();
				DHT11_acq();
					
				
			}
			set_acqFinish_Flags(modbusSlave_id);	//置采集完成标志位
		}
		else		//剩余时间片全轮询,需要确保剩余时间片内可以传输完数据
		{
			(void)eMBPoll();
		}
		
	}		
}	
	
	
void BSP_init(void)
{
	Delay_s(1);//等待芯片上电
	MyRTC_Init();//RTC时钟	
	DH11_GPIO_Init();//温湿度传感器
	MPU6050_Init();
	OLED_Init();
	TIM3_initConfig();	
	bsp_modbus_init();//modbus初始化
	Delay_s(1);		//DH11温湿度传感器需要至少等待1s
	while(0xff!=check_BSP_initCondition())	//等待广播 使能tim3中断
	{
		(void)eMBPoll();
	}
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3中断	
	time_t clock_count=(*(uint16_t*)REG_HOLDING_START)<<16 | *(uint16_t*)(REG_HOLDING_START+1);
	MyRTC_SetTime_count(clock_count);//设置秒计数器
	memcpy(&my_buf_struct->clock,&clock_count,sizeof(time_t));
}


void oled_test(void)
{
	OLED_ShowString(1,1,"oled OK");
	get_currentTime(ref_stringLocalTime,0);
	OLED_ShowString(3,1,ref_stringLocalTime);
	OLED_ShowString(4,1,ref_stringLocalTime+11);
	Delay_s(1);OLED_Clear();	//显示1s后清屏
}

	
//	Delay_s(1);
//	MyRTC_Init();	
//	DH11_GPIO_Init();	
//	MPU6050_Init();
//	OLED_Init();
//	//初始化 RTU模式 参数二：不用管，参数3：从机地址为1 参数4:9600 参数5：无效验 	
//	eMBInit(MB_RTU, 0x01, 0x01, 9600, MB_PAR_NONE);
//	eMBEnable(); 
//	
//	TIM3_initConfig();
//	Delay_s(1);
//	
//	//测试oled
////	OLED_ShowString(1,1,"oled OK");
////	get_currentTime(ref_stringLocalTime,0);
////	OLED_ShowString(4,1,ref_stringLocalTime+11);
////	Delay_s(1);OLED_Clear();
//while(1)
//{
//	while(break_flag)
//	{
//		//do something
//		//get_currentTime(ref_stringLocalTime,0);		
//		MPU6050_GetData(MPU6050_data);
//			
//		
//		++TIM3_count;
//		break_flag=0;			
//	}	
//	if(TIM3_count>=10)	
//	{// 1s
//		TIM3_count=0;
//		//acq
//		DHT11_acq();
//		
//		//testfun();
//		
//			
//		//show	stringTime and DHT11_data
//		if(0)
//		{
//			get_currentTime(ref_stringLocalTime,0);		
//			OLED_ShowString(1,1,ref_stringLocalTime+11);
//			DHT11_showData();
//		}
//		//show	MPU6050_data
//		if(1)
//		{
//			//MPU6050_showData();
//			//OLED_ShowNum(4,10,++count_ceshi,5);
//			DHT11_showData();
//		}
//			
//		
//	}
//	
//}	



void testfun(void)
{
int16_t AX, AY, AZ, GX, GY, GZ;
MPU6050_GetData_ood(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
	//OLED_ShowSignedNum(1, 1, AZ, 7);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	Delay_ms(20);
}



//100ms中断
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		break_flag=1;		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		
	}
}

