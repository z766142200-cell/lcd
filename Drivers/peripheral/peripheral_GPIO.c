
#include "peripheral_GPIO.h"
#include "device_lcd.h"
 void  LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RES_Pin|DC_Pin|BL_Pin|LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_Pin */
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RES_Pin DC_Pin BL_Pin LED0_Pin */
  GPIO_InitStruct.Pin = RES_Pin|DC_Pin|BL_Pin|LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

void SCCB_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 开启 GPIOD 和 GPIOG 的时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // 配置 SIOC 引脚 (开漏输出，无上下拉)
    GPIO_InitStructure.Pin   = OV7725_SIOC_GPIO_PIN;
    GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull  = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 对应 50MHz
    HAL_GPIO_Init(OV7725_SIOC_GPIO_Port, &GPIO_InitStructure);
    
    // 配置 SIOD 引脚 (同样为开漏输出，支持双向 I2C 读写)
    GPIO_InitStructure.Pin   = OV7725_SIOD_GPIO_PIN;
    HAL_GPIO_Init(OV7725_SIOD_GPIO_Port, &GPIO_InitStructure);
}

// ====================================================================
// 2. 摄像头 D2~D9 数据引脚及 PCLK 时钟引脚输入初始化
// ====================================================================
void OV7725_Data_Input_Init(void) // 8位并口输入
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 开启 GPIOC 和 GPIOB 的时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置 D2 ~ D9 数据引脚 (浮空输入)
    // 注意：既然 D0 和 D1 不用，这些宏应该直接对应你的硬件引脚（如 PC0~PC7）
    GPIO_InitStructure.Pin = OV7725_DATA9_GPIO_PIN | OV7725_DATA8_GPIO_PIN | 
                             OV7725_DATA7_GPIO_PIN | OV7725_DATA6_GPIO_PIN | 
                             OV7725_DATA5_GPIO_PIN | OV7725_DATA4_GPIO_PIN | 
                             OV7725_DATA3_GPIO_PIN | OV7725_DATA2_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;    // 对应标准库的输入模式
    GPIO_InitStructure.Pull = GPIO_NOPULL;       // 对应浮空（无上下拉）
    HAL_GPIO_Init(OV7725_DATA9_GPIO_Port, &GPIO_InitStructure);

    // 配置 PCLK 像素时钟引脚 (浮空输入)
    GPIO_InitStructure.Pin = OV7725_PCLK_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OV7725_PCLK_GPIO_Port, &GPIO_InitStructure);
}

void per_workingInit(void)
{
    VSYNC_EXIT_Init();
    SCCB_GPIO_Init();
    OV7725_Data_Input_Init();
    ov7725_tim_Init();
}

int Ovcamera_data_read()
{      
    uint8_t camera_data = 0x00 ;    
    camera_data = GPIOC->IDR & 0x00FF;
    return camera_data ;
} 



