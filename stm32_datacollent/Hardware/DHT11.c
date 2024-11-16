#include "stm32f10x.h"                  // Device header
#include  "DHT11.h"
#include  "Delay.h"
#include "constVal.h"
#include "mytool.h"
#include "OLED.h"
#include <string.h>
//数据
//extern uint32_t DHT11_rec_data[4];



void DH11_GPIO_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD; //开漏输出
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOB,GPIO_Pin_6);	//默认高电平
}

/*
@brief:置PA7高低电平
@param:BitValue:1|0
@retval:无
*/
void WriteIO(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_6,(BitAction)BitValue);
}

/*
@brief:DHT11起始信号
@param:无
@retval:无
*/
//void DHT11_Start(void)
//{
//	WriteIO(0);		//主机拉低总线
//	Delay_ms(20);	//至少拉低18ms
//	WriteIO(1);		//释放总线
//	Delay_us(30);	//释放总线20~40us
//}

//主机发送开始信号
void DHT11_Start(void)
{
	dht11_low; //拉低电平至少18us
	Delay_ms(20);
	
	dht11_high; //拉高电平20~40us
	Delay_us(30);
}

/*
@brief:起始信号以后检测DHT11的响应信号
@param:无
@retval:stateData,1表示DHT11响应,-1表示DHT11无响应
*/
uint8_t DHT11_Check(void)
{
	uint8_t stateData = 0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == RESET)//DHT11收到起始信号后会把总线拉低
	{
		Delay_us(80);//等待80us
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == SET)//DHT11拉高表示响应信号有用
		{
			stateData = 1;
		}
		else
		{
			stateData = 2;
		}
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == SET);//引脚恢复低电平则表示传感器准备发数据
	}
	return stateData;
}

/*
@brief:接收DHT11传感器的一个字节数据
@param:无
@retval:Byte:接收的数据
*/
uint8_t DHT11_ReadByte(void)
{
	uint8_t i,Byte = 0x00;
	for(i=0;i<8;i++)
	{
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == RESET);//DHT11每发一位数据之前,都先拉低,所以等待总线拉高
		Delay_us(40);//等待40us
		//高电平持续时间26-28us为0,70us为1
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6))
		{
			Byte |= (0x80>>i);//高位在前
		}
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == SET);//等待高电平结束,为下一次接收数据做准备
	}
	return Byte;
}


/*
@brief:接收DHT11传感器的温度和湿度
@param1:
@param1:
@retval:Byte:接收的数据
*/
uint8_t DHT11_ReadData(uint8_t *Hum,uint8_t *FHum,uint8_t *Temp,uint8_t *FTemp)
{
	uint8_t Buf[5];
	uint8_t i,Flag = 0x00;
	DHT11_Start();//起始信号
	if(DHT11_Check() == 1)//DHT11正确响应
	{
		for(i=0;i<5;i++)
		{
			Buf[i] = DHT11_ReadByte();//读取5个字节存在数组中
		}
		Delay_us(60);//保证完整读取
		if(Buf[0] + Buf[1] + Buf[2] + Buf[3] == Buf[4])//校验数据是否有效,如果有效
		{
			*Hum = Buf[0];		//湿度整数部分
			*FHum = Buf[1];		//湿度小数部分
			*Temp = Buf[2];		//温度整数部分
			*FTemp = Buf[3];	//温度小数部分
			Flag = 0;			//验证读取数据是否正确
			return Flag;
		}
		else//校验失败
		{
			*Hum = 0xFF;
			*FHum = 0xFF;
			*Temp = 0xFF;
			*FTemp = 0xFF;
			return 1;
		}
	}
	else//DHT11无响应
	{
		*Hum = 0xFF;
		*FHum = 0xFF;
		*Temp = 0xFF;
		*FTemp = 0xFF;
		return 2;
	}


}

void DHT11_acq(void)
{//每秒采集一次
	uint8_t flag=0;
	flag=DHT11_ReadData(&DHT11_rec_data[0],&DHT11_rec_data[1],&DHT11_rec_data[2],&DHT11_rec_data[3]);	
//	uint16_t DHT11_Hum=(uint16_t)DHT11_rec_data[0]<<8 | DHT11_rec_data[1];
//	uint16_t DHT11_temp=(uint16_t)DHT11_rec_data[2]<<8 | DHT11_rec_data[3];
	memcpy(&my_buf_struct->DHT11_data,&DHT11_rec_data,8*4);
	if(flag!=0)
		DHT11_errorCode=1;
		//oled_showError("DHT11 ERROR");
	else{
		DHT11_errorCode=0;
		//oled_showError("OK Normal");
	}

}
void DHT11_showData(void)
{
	OLED_ShowNum(2,1,DHT11_rec_data[2],2);
	//Delay_us(200);	
	OLED_ShowNum(2,4,DHT11_rec_data[3],2);
	//Delay_us(200);
	OLED_ShowNum(2,8,DHT11_rec_data[0],2);
	//Delay_us(200);
	OLED_ShowNum(2,12,DHT11_rec_data[1],2);
	Delay_us(200);

}




void ceshi(void)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_INITStruct;
//	GPIO_INITStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_INITStruct.GPIO_Pin= GPIO_Pin_3|GPIO_Pin_6; 
//	GPIO_INITStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB,&GPIO_INITStruct);
//	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_6);
	//DH11_GPIO_Init_OUT();
	//DHT11_rec_data[1]=12;
	//dht11_high;
//	Delay_s(1);
//	dht11_low;
//	Delay_s(3);
}



