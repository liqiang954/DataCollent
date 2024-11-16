#include "stm32f10x.h"
#include "OLED.h"

// SysTick定时器的频率（单位：Hz）
#define SYSTICK_FREQ 10000  // 10 kHz

// 初始化SysTick定时器
void SysTick_Init(uint32_t freq)
{
    if (freq > 0)
    {
        // 设置SysTick定时器的加载值
        SysTick->LOAD = (SystemCoreClock / freq) - 1;
        // 清除当前值寄存器
        SysTick->VAL = 0;
        // 启用SysTick定时器
        SysTick->CTRL = SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT | SysTick_CTRL_CLKSOURCE;
    }
}

// 获取当前的SysTick计数值
uint32_t GetSysTickValue(void)
{
    return SysTick->VAL;
}

// 测量程序运行时间
float MeasureAverageTime(void (*func)(void), uint32_t iterations)
{
    uint32_t start, end;
    uint32_t total_time = 0;

    for (uint32_t i = 0; i < iterations; i++)
    {
        start = GetSysTickValue();
        func();
        end = GetSysTickValue();
        total_time += (end - start);
    }

    // 计算平均时间（单位：微秒）
    return (float)total_time / iterations * (1000000.0 / SYSTICK_FREQ);
}

void ito_a(int num, char *str) 
	{
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int i = 0;
    int negative = num < 0;
    if (negative) {
        num = -num;
    }

    while (num > 0) {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // 反转字符串
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}
	
void oled_showError(char* err)
{
	OLED_ShowString(4,1,err);
}
