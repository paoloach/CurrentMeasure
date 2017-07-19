/*
 * Delay.cpp
 *
 *  Created on: 11 gen 2017
 *      Author: paolo
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include "Delay.h"

Delay::Delay() {
    __HAL_RCC_TIM2_CLK_ENABLE();

    // Time base configuration
    TIM_Base_InitTypeDef TIM;
    TIM.Prescaler = SystemCoreClock/1000000;
    TIM.Period = 0xffff;
    TIM.ClockDivision = 0;
    TIM.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Base_SetConfig(TIM2,&TIM);
    //TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Global);
}

void Delay::delayUs(uint32_t us){
    TIM2->CNT = 0;
    TIM2->CR1 |= 1;
    while(TIM2->CNT < us);
    TIM2->CR1 &= 0xFFFFFFFE;
}
