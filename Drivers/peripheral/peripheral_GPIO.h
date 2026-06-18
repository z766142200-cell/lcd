#ifndef __PERIPHERAL_GPIO_H__
#define __PERIPHERAL_GPIO_H__

#include "stm32f1xx_hal.h"

/******************************************************************************
//本程序适用与STM32F103C8
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB0
//              DC    接PB1
//              CS    接PA4 
//							BL		接PB10
*******************************************************************************/
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOE
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define RES_Pin GPIO_PIN_0
#define RES_GPIO_Port GPIOB
#define DC_Pin GPIO_PIN_1
#define DC_GPIO_Port GPIOB
#define BL_Pin GPIO_PIN_10
#define BL_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOB

void LCD_GPIO_Init(void);



/**@
 * OV7725_VSYNC PB3
 * OV7725_SIOC  PD3
 * OV7725_SIOD  PD6
 * OV7725_XCLK  PA8
 * OV7725_PCLK  PB4
 * OV7725_RESET PG14
 * OV7725_PWDN  PD14
 * OV7725_DATA9 PC7
 * OV7725_DATA8 PC6
 * OV7725_DATA7 PC5
 * OV7725_DATA6 PC4
 * OV7725_DATA5 PC3
 * OV7725_DATA4 PC2
 * OV7725_DATA3 PC1
 * OV7725_DATA2 PC0
 * 
 * 
 * lcd rst:PB2, lcd dc:PB1, lcd cs:PA4, lcd bl:PB11
 *   lcd data:PD0~PD7, lcd clk:PA5
 * 
 * 
 * */
#define OV7725_VSYNC_GPIO_Port GPIOB//PB3 WEN
#define OV7725_VSYNC_GPIO_PIN GPIO_PIN_3//PB3 WEN
#define OV7725_VSYNC_EXTI_PORT HAL_GPIO_EXTI_PORT_GPIOB
#define OV7725_VSYNC_EXTI_SOURCE_PIN GPIO_PIN_3
#define OV7725_VSYNC_EXTI_IRQ EXTI3_IRQn
#define OV7725_VSYNC_GPIO_CLK RCC_APB2Periph_GPIOB
#define OV7725_VSYNC_EXTI_LINE EXTI_Line3

#define OV7725_SIOC_GPIO_Port GPIOD//PD3
#define OV7725_SIOC_GPIO_PIN GPIO_PIN_3//PD3

#define OV7725_SIOD_GPIO_Port GPIOG//PG13
#define OV7725_SIOD_GPIO_PIN GPIO_PIN_13//
////SCCB
#define SCCB_SIOD_H()  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET)
#define SCCB_SIOD_L()  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET)
#define SCCB_SIOC_H()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET)
#define SCCB_SIOC_L()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET)

#define OV7725_XCLK_GPIO_Port GPIOA//vsync
#define OV7725_XCLK_GPIO_PIN GPIO_PIN_8//vsync PA8

#define OV7725_PCLK_GPIO_Port GPIOB//RCLK
#define OV7725_PCLK_GPIO_PIN GPIO_PIN_4//RCLK PB4
#define OV7725_PCLK_RCC RCC_APB2Periph_GPIOB 

#define OV7725_RST_GPIO_Port GPIOG//RRST
#define OV7725_RST_GPIO_PIN GPIO_PIN_15//RRST PG14

#define OV7725_PWDN_GPIO_Port GPIOD//RRST
#define OV7725_PWDN_GPIO_PIN GPIO_PIN_14//RRST PD6


#define OV7725_DATA9_GPIO_Port GPIOC
#define OV7725_DATA9_GPIO_PIN GPIO_PIN_7

#define OV7725_DATA8_GPIO_Port GPIOC
#define OV7725_DATA8_GPIO_PIN GPIO_PIN_6

#define OV7725_DATA7_GPIO_Port GPIOC
#define OV7725_DATA7_GPIO_PIN GPIO_PIN_5

#define OV7725_DATA6_GPIO_Port GPIOC
#define OV7725_DATA6_GPIO_PIN GPIO_PIN_4

#define OV7725_DATA5_GPIO_Port GPIOC
#define OV7725_DATA5_GPIO_PIN GPIO_PIN_3

#define OV7725_DATA4_GPIO_Port GPIOC
#define OV7725_DATA4_GPIO_PIN GPIO_PIN_2

#define OV7725_DATA3_GPIO_Port GPIOC
#define OV7725_DATA3_GPIO_PIN GPIO_PIN_1

#define OV7725_DATA2_GPIO_Port GPIOC
#define OV7725_DATA2_GPIO_PIN GPIO_PIN_0

void per_workingInit(void);

#endif
