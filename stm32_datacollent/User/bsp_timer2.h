#ifndef __TIMER2_H
#define	__TIMER2_H

#include "stm32f10x.h"

void timer2_init(uint16_t period);
void timer2_nvic(void);

#endif /* __TIMER2_H */

