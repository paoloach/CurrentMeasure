/*
 * CurrentMeasure.cpp
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#include <algorithm>
#include <string.h>
#include "diag/Trace.h"

#include "MessageQueue.h"
#include "CurrentMeasure.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"

uint16_t CurrentMeasure::count;

Current CurrentMeasure::samples1[480];
Current CurrentMeasure::samples2[480];
Current * CurrentMeasure::writingSamples;
Current * CurrentMeasure::readingSamples;
uint16_t CurrentMeasure::samplePos;

bool CurrentMeasure::trigger;
uint16_t CurrentMeasure::triggerLevel;
uint16_t CurrentMeasure::triggerStart;
uint32_t CurrentMeasure::meanUsed;
uint16_t CurrentMeasure::meanPos;
uint32_t CurrentMeasure::mean;
TIM_HandleTypeDef CurrentMeasure::handle;

void CurrentMeasure::init() {
    __HAL_RCC_TIM2_CLK_ENABLE()
    ;
    ADS7841::init();
    writingSamples = samples1;
    readingSamples = samples2;

    samplePos = 0;

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
}

void CurrentMeasure::start() {
    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOC_CLK_ENABLE()
    ;

    samplePos = 0;
    meanPos = 0;
    trigger = false;
    triggerLevel = 12;
    triggerStart = 50;
    HAL_TIM_Base_Start_IT(&handle); // start timer interrupts
}

void CurrentMeasure::restartAcquire() {
    samplePos = 0;
    trigger = false;
}

constexpr Current threashold(70);

void CurrentMeasure::saveData() {
    Current current;
    volatile auto a = ADS7841::resultHigh;
    volatile auto b = ADS7841::resultLow;
    auto lowSens = getLowSensibility();
    if (lowSens < threashold) {
        current = getHighSensibility();
    } else {
        current = lowSens;
    }
    volatile auto c = current.value;
    writingSamples[samplePos] = current;
    if (samplePos < 480) {
        if (samplePos < triggerStart || trigger) {
            samplePos++;
        } else {
            if (current < triggerLevel) {
                memmove(writingSamples, writingSamples + 1, sizeof(uint16_t) * (triggerStart));
            } else {
                trigger = true;
                samplePos++;
            }
        }
        if (samplePos == 480) {
            std::swap(readingSamples, writingSamples);
            MessageQueue::addMessage(Message(MessageType::SampleEnd));
        }
    }
    meanUsed += current.value;
    meanPos++;
    if (meanPos == 1000) {
        mean = meanUsed;
        meanPos = 0;
        meanUsed =0;
        MessageQueue::addMessage(Message(MessageType::MeanAvailable));
    }
}

extern "C" {

// Called every 100uS
void TIM2_IRQHandler(void) {
    if (__HAL_TIM_GET_FLAG(&CurrentMeasure::handle, TIM_FLAG_UPDATE) != RESET) {
        if (__HAL_TIM_GET_ITSTATUS(&CurrentMeasure::handle, TIM_IT_UPDATE) != RESET) {
            __HAL_TIM_CLEAR_FLAG(&CurrentMeasure::handle, TIM_FLAG_UPDATE);
            ADS7841::get();
        }
    }
}

}
