/*
 * Delay.cpp
 *
 *  Created on: 11 gen 2017
 *      Author: paolo
 */

#include "stm32f10x.h"
#include "Delay.h"

Delay::Delay() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    // Time base configuration
    TIM_TimeBaseInitTypeDef TIM;
    TIM_TimeBaseStructInit(&TIM);
    TIM.TIM_Prescaler = SystemCoreClock/1000000;
    TIM.TIM_Period = 0xffff;
    TIM.TIM_ClockDivision = 0;
    TIM.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2,&TIM);
    TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Global);
}

void Delay::delayUs(uint32_t us){
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2,ENABLE);
    while(TIM_GetCounter(TIM2) < us);
}
