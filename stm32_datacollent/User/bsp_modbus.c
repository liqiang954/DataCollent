#include "bsp_modbus.h"
#include "constVal.h"

/* Private variables ---------------------------------------------------------*/
//输入寄存器内容
uint16_t usRegInputBuf[REG_INPUT_NREGS] = {0};
//输入寄存器起始地址
uint16_t usRegInputStart = REG_INPUT_START;

//保持寄存器内容
uint16_t usRegHoldingBuf[REG_HOLDING_NREGS] = {0};
//保持寄存器起始地址
uint16_t usRegHoldingStart = REG_HOLDING_START;

//线圈状态
uint8_t ucRegCoilsBuf[REG_COILS_SIZE / 8] = {0};
//开关输入状态
uint8_t ucRegDiscreteBuf[REG_DISCRETE_SIZE / 8] = {0};

//uint32_t addr_val=(uint32_t)&usRegInputBuf;




/*********************************************END OF FILE**********************/

/****************************************************************************
* 名	  称：eMBRegInputCB 
* 功    能：读取输入寄存器，对应功能码是 04 eMBFuncReadInputRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读取的寄存器个数
* 出口参数：
* 注	  意：上位机发来的 帧格式是: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*							3 区
****************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
			//printf("Read register %04X: %04X\n", usAddress + usNRegs, usRegInputBuf[iRegIndex - 1]);
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegHoldingCB 
* 功    能：对应功能码有：06 写保持寄存器 eMBFuncWriteHoldingRegister 
*													16 写多个保持寄存器 eMBFuncWriteMultipleHoldingRegister
*													03 读保持寄存器 eMBFuncReadHoldingRegister
*													23 读写多个保持寄存器 eMBFuncReadWriteMultipleHoldingRegister
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNRegs: 要读写的寄存器个数
*						eMode: 功能码
* 出口参数：
* 注	  意：4 区
****************************************************************************/
//eMBErrorCode
//eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
//{
//	eMBErrorCode    eStatus = MB_ENOERR;
//	int             iRegIndex;


//	if((usAddress >= REG_HOLDING_START)&&\
//		((usAddress+usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
//	{
//		iRegIndex = (int)(usAddress - usRegHoldingStart);
//		switch(eMode)
//		{                                       
//			case MB_REG_READ://读 MB_REG_READ = 0
//        while(usNRegs > 0)
//				{
//					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);            
//					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF); 
//          iRegIndex++;
//          usNRegs--;					
//				}                            
//        break;
//			case MB_REG_WRITE://写 MB_REG_WRITE = 0
//				while(usNRegs > 0)
//				{         
//					usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
//          usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
//          iRegIndex++;
//          usNRegs--;
//        }				
//			}
//	}
//	else//错误
//	{
//		eStatus = MB_ENOREG;
//	}	
//	
//	return eStatus;
//}

extern void xMBUtilSetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits,
                UCHAR ucValue );
extern UCHAR xMBUtilGetBits( UCHAR * ucByteBuf, USHORT usBitOffset, UCHAR ucNBits );
/****************************************************************************
* 名	  称：eMBRegCoilsCB 
* 功    能：对应功能码有：01 读线圈 eMBFuncReadCoils
*													05 写线圈 eMBFuncWriteCoil
*													15 写多个线圈 eMBFuncWriteMultipleCoils
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 线圈地址
*						usNCoils: 要读写的线圈个数
*						eMode: 功能码
* 出口参数：
* 注	  意：如继电器 
*						0 区
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode )
{
  //错误状态
  eMBErrorCode eStatus = MB_ENOERR;
  //寄存器个数
  int16_t iNCoils = ( int16_t )usNCoils;
  //寄存器偏移量
  int16_t usBitOffset;
  
  //检查寄存器是否在指定范围内
  if( ( (int16_t)usAddress >= REG_COILS_START ) &&
     ( usAddress + usNCoils <= REG_COILS_START + REG_COILS_SIZE ) )
  {
    //计算寄存器偏移量
    usBitOffset = ( int16_t )( usAddress - REG_COILS_START );
    switch ( eMode )
    {
      //读操作
    case MB_REG_READ:
      while( iNCoils > 0 )
      {
        *pucRegBuffer++ = xMBUtilGetBits( ucRegCoilsBuf, usBitOffset,
                                         ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ) );
        iNCoils -= 8;
        usBitOffset += 8;
      }
      break;
      
      //写操作
    case MB_REG_WRITE:
      while( iNCoils > 0 )
      {
        xMBUtilSetBits( ucRegCoilsBuf, usBitOffset,
                       ( uint8_t )( iNCoils > 8 ? 8 : iNCoils ),
                       *pucRegBuffer++ );
        iNCoils -= 8;
      }
      break;
    }
    
  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

/****************************************************************************
* 名	  称：eMBRegDiscreteCB 
* 功    能：读取离散寄存器，对应功能码有：02 读离散寄存器 eMBFuncReadDiscreteInputs
* 入口参数：pucRegBuffer: 数据缓存区，用于响应主机   
*						usAddress: 寄存器地址
*						usNDiscrete: 要读取的寄存器个数
* 出口参数：
* 注	  意：1 区
****************************************************************************/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
  //错误状态
  eMBErrorCode eStatus = MB_ENOERR;
  //操作寄存器个数
  int16_t iNDiscrete = ( int16_t )usNDiscrete;
  //偏移量
  uint16_t usBitOffset;
  
  //判断寄存器时候再制定范围内
  if( ( (int16_t)usAddress >= REG_DISCRETE_START ) &&
     ( usAddress + usNDiscrete <= REG_DISCRETE_START + REG_DISCRETE_SIZE ) )
  {
    //获得偏移量
    usBitOffset = ( uint16_t )( usAddress - REG_DISCRETE_START );
    
    while( iNDiscrete > 0 )
    {
      *pucRegBuffer++ = xMBUtilGetBits( ucRegDiscreteBuf, usBitOffset,
                                       ( uint8_t)( iNDiscrete > 8 ? 8 : iNDiscrete ) );
      iNDiscrete -= 8;
      usBitOffset += 8;
    }
    
  }
  else
  {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}



// 保持寄存器回调函数
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    // 检查地址范围是否合法
    if ((usAddress >= REG_HOLDING_START) && ((usAddress + usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
    {
        // 计算寄存器索引
        iRegIndex = (int)(usAddress - REG_HOLDING_START);

        // 根据操作模式处理读写操作
        switch (eMode)
        {
            case MB_REG_READ: // 读取保持寄存器
                while (usNRegs > 0)
                {
                    *pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] >> 8);  // 存储高字节
                    *pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] & 0xFF); // 存储低字节
                    iRegIndex++;
                    usNRegs--;
                }
                break;

            case MB_REG_WRITE: // 写入保持寄存器
                while (usNRegs > 0)
                {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;  // 读取高字节
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;      // 读取低字节
                    iRegIndex++;
                    usNRegs--;

                    // 检查广播命令写保持寄存器第一位为 0xFF
                    if (usAddress == REG_HOLDING_START && usRegHoldingBuf[0] == 0xFF)
                    {
                        timer3_init();  // 初始化定时器3
                    }
                }
                break;
        }
    }
    else // 地址超出范围
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}


void timer3_init(void)
{
	uint8_t i=0;
	++i;

}

void bsp_modbus_init(void)
{

	//初始化 RTU模式 参数二：不用管，参数3：从机地址为1 参数4:9600 参数5：无效验 	
	eMBInit(MB_RTU, 0x01, modbusSlave_id, 9600, MB_PAR_NONE);
	eMBEnable(); 

}


