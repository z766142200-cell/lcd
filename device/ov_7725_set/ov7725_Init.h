#ifndef __OV7725_INIT_H__
#define __OV7725_INIT_H__

#include "stm32f1xx.h"



#define OV7725_MID				0X7FA2    
#define OV7725_PID				0X7721

// #define OV7725_VSYNC  	PAin(8)			//同步信号检测IO
// #define OV7725_WRST		PDout(6)		//写指针复位
// #define OV7725_WREN		PBout(3)		//写入FIFO使能
// #define OV7725_RCK_H	GPIOB->BSRR=1<<4//设置读数据时钟高电平
// #define OV7725_RCK_L	GPIOB->BRR=1<<4	//设置读数据时钟低电平
// #define OV7725_RRST		PGout(14)  		//读指针复位
// #define OV7725_CS		PGout(15)  		//片选信号(OE)
//GPIO_Redefine
/**@
 * OV7725_SIOC_GPIO_PIN  PD3
 * OV7725_SIOD_GPIO_PIN  PD6
 * OV7725_VSYNC_GPIO_PIN PG13
 * OV7725_HREF_GPIO_PIN  PG14
 *  OV7725_PCLK_GPIO_PIN  PC0
 * OV7725_XCLK_GPIO_PIN   PG15
 *  
 * OV7725_DATA9_GPIO_PIN  PC2
 * OV7725_DATA8_GPIO_PIN  PC1
 * OV7725_DATA7_GPIO_PIN  PC4
 * OV7725_DATA6_GPIO_PIN  PC3
 * OV7725_DATA5_GPIO_PIN  PC6
 * OV7725_DATA4_GPIO_PIN  PC5
 *      
 * OV7725_DATA3_GPIO_PIN  PB4
 * OV7725_DATA2_GPIO_PIN  PC7
 * 
 * OV7725_RESET_GPIO_PIN  PB3
 * OV7725_PWDN_GPIO_PIN   PA8
 * 
**/
				 
u8   OV7725_Init(void);		  	   		 
void OV7725_Light_Mode(u8 mode);
void OV7725_Color_Saturation(s8 sat);
void OV7725_Brightness(s8 bright);
void OV7725_Contrast(s8 contrast);
void OV7725_Special_Effects(u8 eft);
void OV7725_Window_Set(u16 width,u16 height,u8 mode);
void OV7725_MODE_Init(void);


#endif
























