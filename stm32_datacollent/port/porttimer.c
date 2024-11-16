/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "bsp_timer2.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR( void );

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
  timer2_init(usTim1Timerout50us);
	timer2_nvic();
	return TRUE;
}


void vMBPortTimersEnable(  )
{
    /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM2,0x0000); 
	TIM_Cmd(TIM2, ENABLE);
}

void vMBPortTimersDisable(  )
{
    /* Disable any pending timers. */
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
  TIM_SetCounter(TIM2,0x0000); 
  TIM_Cmd(TIM2, DISABLE);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
static void prvvTIMERExpiredISR( void )
{
    ( void )pxMBPortCBTimerExpired(  );
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		prvvTIMERExpiredISR();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}