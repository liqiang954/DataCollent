#ifndef BSP_MODBUS_H
#define BSP_MODBUS_H

#include "mb.h"
#include "mbutils.h"
#include "stm32f10x.h" 
#include "stdint.h"

/* ----------------------- Defines ------------------------------------------*/
//输入寄存器起始地址
#define REG_INPUT_START       0x0000
//输入寄存器数量
#define REG_INPUT_NREGS       100
//保持寄存器起始地址
// 保持寄存器起始地址	前两位为时间计数器
#define REG_HOLDING_START     0x0064

//保持寄存器数量
#define REG_HOLDING_NREGS     8

//线圈起始地址  定义第一位为初始化使能位
#define REG_COILS_START       0x006C  
//线圈数量
#define REG_COILS_SIZE        16

//开关寄存器起始地址
#define REG_DISCRETE_START    0x007C
//开关寄存器数量
#define REG_DISCRETE_SIZE     16


/* Private variables ---------------------------------------------------------*/
//输入寄存器内容
extern uint16_t usRegInputBuf[REG_INPUT_NREGS] ;
//保持寄存器内容
extern uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];
//线圈状态
extern uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] ;
//开关输入状态
extern uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8];




eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode );
eMBErrorCode 
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete );

void timer3_init(void);
void bsp_modbus_init(void);
#endif

