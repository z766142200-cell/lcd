#ifndef __BSP_SCCB_H
#define __BSP_SCCB_H

#include "ov7725_GPIO_Function_Init.h"
#include <stdint.h>



#define ADDR_OV7725   0x42


#define SCCB_Delay()   void delay_us(u32 nus)

static int SCCB_Start(void);
static void SCCB_Stop(void);
//static void SCCB_Ack(void);
static void SCCB_NoAck(void);
//static int SCCB_WaitAck(void) ;	

static void SCCB_SendByte(uint8_t SendByte) ;
static int SCCB_ReceiveByte(void)  ;
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte );
// int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress);
int  SCCB_ReadByte(uint8_t ReadAddress);

#endif 

