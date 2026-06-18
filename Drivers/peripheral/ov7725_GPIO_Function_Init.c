#include "ov7725_GPIO_Function_Init.h"
#include "stdint.h"
void VSYNC_EXIT_Init()
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//    EXTI_InitTypeDef EXTI_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//    /*初始化时钟，注意中断要开AFIO*/
//    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO|OV7725_VSYNC_GPIO_CLK, ENABLE );

//    /*初始化引脚*/
//    GPIO_InitStructure.GPIO_Pin =  OV7725_VSYNC_GPIO_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(OV7725_VSYNC_GPIO_Port, &GPIO_InitStructure);

//    /*配置中断*/
//    GPIO_EXTILineConfig(OV7725_VSYNC_EXTI_PORT,OV7725_VSYNC_EXTI_SOURCE_PIN);
//    EXTI_InitStructure.EXTI_Line = OV7725_VSYNC_EXTI_LINE;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//    EXTI_GenerateSWInterrupt(OV7725_VSYNC_EXTI_LINE);

//    /*配置优先级*/
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//    NVIC_InitStructure.NVIC_IRQChannel = OV7725_VSYNC_EXTI_IRQ;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}



// 建议将句柄放在类内或全局，以便后续可能的中断或其他函数调用
TIM_HandleTypeDef htim1 = {};

void ov7725_tim_Init(void)
{
    // 1. 开启时钟
    __HAL_RCC_TIM1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE(); // 假设 OV7725_XCLK_GPIO_Port 为 GPIOA

    // 2. GPIO 初始化 (PA8 -> TIM1_CH1)
    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin = OV7725_XCLK_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 对应标准库的 50MHz
    HAL_GPIO_Init(OV7725_XCLK_GPIO_Port, &GPIO_InitStruct);

    // 3. 时基结构体初始化
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 1;
    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // 对应预装载配置
    HAL_TIM_PWM_Init(&htim1);

    // 4. 输出比较结构体初始化
    TIM_OC_InitTypeDef sConfigOC = {};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    // HAL库的该函数内部会自动使能 OCPreload
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

    // 5. 刹车和死区结构体初始化
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {};
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_ENABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
    sBreakDeadTimeConfig.DeadTime = 11;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
    HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

    // 6. 使能主输出以及通道的 PWM 信号输出
    // 此函数在高级定时器下等同于标准库的 TIM_Cmd + TIM_CtrlPWMOutputs + TIM_CCxCmd
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

