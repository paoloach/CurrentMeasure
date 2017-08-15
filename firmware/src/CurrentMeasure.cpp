/*
 * CurrentMeasure.cpp
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#include <string.h>

#include "CurrentMeasure.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"

uint16_t count;

uint16_t CurrentMeasure::currents[480];
uint16_t CurrentMeasure::pos;
bool CurrentMeasure::sampleEnd;
bool CurrentMeasure::trigger;
uint16_t CurrentMeasure::triggerLevel;
uint16_t CurrentMeasure::triggerStart;
static TIM_HandleTypeDef handle;

void CurrentMeasure::init() {
    __HAL_RCC_TIM2_CLK_ENABLE()
    ;
    ADS7841::init();

    pos = 0;

    handle.Instance = TIM2;
    handle.Init.Prescaler = 168;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle.Init.Period = 100;
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
    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = GPIO_PIN_6;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    CurrentMeasure::pos = 0;
    sampleEnd = false;
    trigger = false;
    triggerLevel=10;
    triggerStart=50;
    HAL_TIM_Base_Start_IT(&handle); // start timer interrupts
}

extern "C" {

void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&handle, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
            {
        if (__HAL_TIM_GET_ITSTATUS(&handle, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_FLAG(&handle, TIM_FLAG_UPDATE);
            volatile uint16_t hight = CurrentMeasure::getHighSensibility();
            CurrentMeasure::currents[CurrentMeasure::pos] = hight;
            if (CurrentMeasure::pos < CurrentMeasure::triggerStart || CurrentMeasure::trigger) {
                CurrentMeasure::pos++;
                if (CurrentMeasure::pos >= 480) {
                    CurrentMeasure::sampleEnd = true;
                    HAL_TIM_Base_Stop_IT(&handle);
                }
            } else {
                if (hight < CurrentMeasure::triggerLevel){
                    memmove(CurrentMeasure::currents, CurrentMeasure::currents+1, sizeof(uint16_t)*(CurrentMeasure::triggerStart-1) );
                } else {
                    CurrentMeasure::trigger=true;
                    CurrentMeasure::pos++;
                }
            }
            count = TIM2->CNT;
        }
    }
}

}
