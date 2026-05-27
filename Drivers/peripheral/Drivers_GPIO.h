#ifndef __DRIVERS_GPIO_H__
#define __DRIVERS_GPIO_H__

#include "stm32f1xx_hal.h"

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

void MX_GPIO_Init(void);

#endif
