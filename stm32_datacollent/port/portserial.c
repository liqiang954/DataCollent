#include "port.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
  //STM32串口接收中断使能
	if(xRxEnable == TRUE)
	{
		//UART中断使能
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
	else
	{
	  //禁止接收和接收中断
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
	}
  //STM32串口发送中断使能
	if(xTxEnable == TRUE)
	{
	  //使能发送中断
		USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	}
	else
	{
    //禁止发送中断
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	}
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	//串口初始化
  USART1_Config((uint16_t)ulBaudRate);  
	USART_NVIC();
	return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
    /* Put a byte in the UARTs transmit buffer. This function is called
     * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
     * called. */
	//串口发送函数
	USART_SendData(USART1, ucByte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
    /* Return the byte in the UARTs receive buffer. This function is called
     * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
     */
	//串口接收函数
  *pucByte = USART_ReceiveData(USART1); 
	return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
    pxMBFrameCBTransmitterEmpty(  );
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
    pxMBFrameCBByteReceived(  );
}

/**
  * @brief  This function handles usart1 Handler.
  * @param  None
  * @retval None
  */
//串口中断函数
void USART1_IRQHandler(void)
{
  //发生接收中断
  if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
    prvvUARTRxISR(); //串口接收中断调用函数
    //清除中断标志位    
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);   
  }
	
	if(USART_GetITStatus(USART1, USART_IT_ORE) == SET)
  {  
    USART_ClearITPendingBit(USART1, USART_IT_ORE);
		prvvUARTRxISR(); 	//串口发送中断调用函数
  }
  
  //发生完成中断
  if(USART_GetITStatus(USART1, USART_IT_TC) == SET)
  {
    prvvUARTTxReadyISR();
    //清除中断标志
    USART_ClearITPendingBit(USART1, USART_IT_TC);
  }
}
