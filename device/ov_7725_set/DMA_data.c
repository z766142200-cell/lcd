#include "DMA_data.h"
#include "ov7725_GPIO_Function_Init.h"
#include "usart.h"



void DMA_ParaSet(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&GPIOC->IDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&SPI1->DR);
    DMA_InitStructure.DMA_DIR =  DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 8;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel3 ,&DMA_InitStructure);
    DMA_Cmd(DMA1_Channel3, ENABLE);
	
    // DMA_ITConfig(DMA1_Channel3 , DMA_IT_TC, ENABLE);


    // NVIC_InitStructure.NVIC_IRQChannel = DMA_Channelx_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);
}



void dma1_to_spi1(void) {
	//设置一个超时变量
	u16 timeout = 0;
	//使能 DMA1
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	//判断 DMA 是否完成数据传输，若超过一定时限，则中断本次传输
	while(DMA_GetFlagStatus( DMA1_FLAG_TC3) == RESET) {
		if(timeout ++ > 200)
        printf("DMA1 to SPI1 error\n\r");
        break;
	}
	// 清除 DMA 标志位
	DMA_ClearFlag( DMA1_FLAG_TC3);
}


