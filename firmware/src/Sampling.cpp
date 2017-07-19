/*
 * Sampling.cpp
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#include <algorithm>

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_cortex.h"
#include "Sampling.h"

Sampling sampling;

Sampling::Sampling() :current(seconds1),next(seconds2){
    __HAL_RCC_TIM1_CLK_ENABLE();

    TIM_Base_InitTypeDef timerInitStructure;
    timerInitStructure.Prescaler = 72; // 1Mhz
    timerInitStructure.Period = 1000; // 1Khz;
    timerInitStructure.CounterMode = TIM_COUNTERMODE_UP;

    timerInitStructure.ClockDivision = 1;
    timerInitStructure.RepetitionCounter = 0;
    TIM_Base_SetConfig(TIMER, &timerInitStructure);

    HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0,1);

    runIsr = false;
    currentIter = current.begin();

    TIMER->DIER |= TIM_DIER_UIE;
}

uint16_t Sampling::meanSecond() {
    uint32_t tot=0;
    for(uint16_t val: next){
        tot+=val;
    }
    return tot/next.size();
}

void Sampling::isr() {
    if (!runIsr)
        return;
    volatile uint16_t hight = currentMeasure.getHighSensibility();
    volatile uint16_t low = currentMeasure.getLowSensibility();
    *currentIter = low < 50 ? hight : low;
    currentIter++;
    if (currentIter == current.end()) {
        std::swap(current, next);
        currentIter = current.begin();
    }
    runIsr=false;
}

extern "C" {

void TIM1_UP_IRQHandler(void) {
    TIM1->SR &= TIM_SR_UIF;
    sampling.enableISR();
}

}

