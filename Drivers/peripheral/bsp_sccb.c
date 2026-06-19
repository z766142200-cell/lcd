//bsp_sccb.c
#include "peripheral_GPIO.h"
static int SCCB_Start(void)
{
    SCCB_SIOD_H();
    SCCB_SIOC_H();
    SCCB_Delay();
    SCCB_SIOD_L();
    SCCB_Delay();
    SCCB_SIOC_L();
    SCCB_Delay();
    return 1;
}
static void SCCB_Stop(void)
{
    SCCB_SIOD_L();
    SCCB_Delay();
    SCCB_SIOC_H();
    SCCB_Delay();
    SCCB_SIOD_H();
    SCCB_Delay();
}
//static void SCCB_Ack(void)
//{
//    SCCB_SIOD_L();
//    SCCB_Delay();
//    SCCB_SIOC_H();
//    SCCB_Delay();
//    SCCB_SIOC_L();
//    SCCB_Delay();
//}

static void SCCB_NoAck(void)
{
    SCCB_SIOD_H();
    SCCB_Delay();
    SCCB_SIOC_H();
    SCCB_Delay();
    SCCB_SIOC_L();
    SCCB_Delay();
}

static int SCCB_WaitAck(void) 
{
    SCCB_SIOD_H();
    SCCB_Delay();
    SCCB_SIOC_H();
    SCCB_Delay();

    SCCB_SIOC_L();
    SCCB_Delay();
    return 1;

}

static void SCCB_SendByte(uint8_t SendByte)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
        if(SendByte&0x80)
            SCCB_SIOD_H();
        else
            SCCB_SIOD_L();
        SendByte<<=1;
        SCCB_SIOC_L();
        SCCB_Delay();
        SCCB_SIOC_H();
        SCCB_Delay();
    }
    SCCB_SIOC_L();
}

static int SCCB_ReceiveByte(void)  
{  
    uint8_t i;
    uint8_t ReceiveByte=0;
    SCCB_SIOD_H();
    for(i=0;i<8;i++)
    {
        ReceiveByte<<=1;
        SCCB_SIOC_L();
        SCCB_Delay();
        SCCB_SIOC_H();
        SCCB_Delay();
    
    }
    SCCB_SIOC_L();
    return ReceiveByte;
}

int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{
    if(!SCCB_Start())return 1;
    SCCB_SendByte(ADDR_OV7725);
    if(!SCCB_WaitAck()){SCCB_Stop();return 1;}
    SCCB_SendByte((uint8_t)WriteAddress);
    SCCB_WaitAck();
    SCCB_SendByte(SendByte);
    SCCB_WaitAck();
    SCCB_Stop();
    return 0;

}

// int  SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)


int  SCCB_ReadByte(uint8_t ReadAddress)
{
    uint8_t ReceiveByte;
    if(!SCCB_Start())return 1;
    SCCB_SendByte(ADDR_OV7725);
    if(!SCCB_WaitAck()){SCCB_Stop();return 1;}
    SCCB_SendByte(ReadAddress);
    SCCB_WaitAck();
    SCCB_Stop();
    if(!SCCB_Start())return 1;
    SCCB_SendByte(ADDR_OV7725+1);
    if(!SCCB_WaitAck()){SCCB_Stop();return 1;}
    ReceiveByte=SCCB_ReceiveByte();
    SCCB_NoAck();
    SCCB_Stop();
    return ReceiveByte;
}




