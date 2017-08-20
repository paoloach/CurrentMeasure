/*
 * CurrentMeasure.cpp
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#include <string.h>
#include "diag/Trace.h"

#include "CurrentMeasure.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"

uint16_t CurrentMeasure::count;

uint16_t CurrentMeasure::currents[480];
uint16_t CurrentMeasure::pos;

bool CurrentMeasure::sampleEnd;
bool CurrentMeasure::trigger;
uint16_t CurrentMeasure::triggerLevel;
uint16_t CurrentMeasure::triggerStart;
uint16_t CurrentMeasure::meanData[1000];
uint16_t CurrentMeasure::meanPos;
uint32_t CurrentMeasure::mean;
bool     CurrentMeasure::meanAvailable;
TIM_HandleTypeDef CurrentMeasure::handle;

void CurrentMeasure::init() {
    __HAL_RCC_TIM2_CLK_ENABLE()
    ;
    ADS7841::init();

    pos = 0;

    handle.Instance = TIM2;
    handle.Init.Prescaler = 168;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle.Init.Period = 1000;
    handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    handle.Lock = HAL_UNLOCKED;
    HAL_TIM_Base_Init(&handle);     // Init timer

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    sampleEnd = false;
}

void CurrentMeasure::start() {
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;

    pos = 0;
    meanPos=0;
    sampleEnd = false;
    trigger = false;
    triggerLevel = 12;
    triggerStart = 50;
    HAL_TIM_Base_Start_IT(&handle); // start timer interrupts
}

void CurrentMeasure::restartAcquire() {
    pos = 0;
    sampleEnd = false;
    trigger = false;
}

void CurrentMeasure::saveData() {
    uint16_t current;
    auto lowSens = getLowSensibility();
    if (lowSens < 70) {
        current = getHighSensibility();
    } else {
        current = lowSens;
    }
    currents[pos] = current;
    if (pos < 480) {
        if (pos < triggerStart || trigger) {
            pos++;
        } else {
            if (current < triggerLevel) {
                memmove(currents, currents + 1, sizeof(uint16_t) * (triggerStart));
            } else {
                trigger = true;
                pos++;
            }
        }
        if (pos == 480) {
            sampleEnd = true;
        }
    }
    meanData[meanPos] = current;
    meanPos++;
    if (meanPos == 1000){
        mean=0;
        for(auto data: meanData){
            mean += data;
        }
        meanPos =0;
        meanAvailable=true;
    }
}

extern "C" {

void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&CurrentMeasure::handle, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
            {
        if (__HAL_TIM_GET_ITSTATUS(&CurrentMeasure::handle, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_FLAG(&CurrentMeasure::handle, TIM_FLAG_UPDATE);
            ADS7841::get();
        }
    }
}

}
