#ifndef MYTOOL_H
#define MYTOOL_H

void SysTick_Init(uint32_t freq);
uint32_t GetSysTickValue(void);
float MeasureAverageTime(void (*func)(void), uint32_t iterations);
void ito_a(int num, char *str); 
void oled_showError(char* err);



#endif
