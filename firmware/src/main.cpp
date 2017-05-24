//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"

#include "Timer.h"
#include "Delay.h"
#include "ILI9325.h"
#include "ILI9341.h"
#include "HX8367.h"
#include "Current.h"
#include "SampleHour.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define ADC1_DR    ((uint32_t)0x4001244C)
volatile uint16_t ADC_values[2];
constexpr uint16_t CURRENT_SAMPLE_SIZE = 2000;
volatile uint16_t currentSamples[CURRENT_SAMPLE_SIZE];
volatile uint16_t writeIndex = 0;
volatile bool update;
uint16_t distribution[240];

void ADCInit(void);
void initDMA(void);
void InitializeTimer();
void InitializePWMChannel(int pulse);
void EnableTimerInterrupt();
void EnableADCEOCInterrupt();
void EnableDMAInterrupt();

Current getMeanSecond();
GFX * gfx;

constexpr Point INSTANTANEUS_VALUE(128, 8);
constexpr Point MAX_VALUE(128, 24);
constexpr Point MEAN_VALUE(128, 40);
constexpr Point TOT(128, 56);

int main(int argc, char* argv[]) {
    trace_puts("Current measure!");
    Timer timer;
    SampleHour sampleHour;
    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    timer.start();


    GFX::initIO();
   // EnableADCEOCInterrupt();

    while (true) {
        if (ILI9341::ILI9341::checkPresence()) {
            trace_printf("Found ILI9341\n");
            gfx = new ILI9341::ILI9341 { };
            break;
        } else if (ILI9325::checkPresence()) {
            trace_printf("Found ILI925\n");
            gfx = new ILI9325 { };
            break;
        } else if (HX8367::HX8367::checkPresence()) {
            trace_printf("Found HX8367\n");
            gfx = new HX8367::HX8367 { };
            break;
        }
    }

    gfx->setForeground(WHITE);
    gfx->setBackground(BLACK);
    gfx->setFont(&bigFont);

    writeIndex = 0;
    update = false;
    TIM_Cmd(TIM1, ENABLE);

    gfx->drawString(Point(0, INSTANTANEUS_VALUE.y), "Actual: ");
    gfx->drawString(Point(0, MAX_VALUE.y), "Max: ");
    gfx->drawString(Point(0, MEAN_VALUE.y), "Mean: ");
    gfx->drawString(Point(0, TOT.y), "mA/h: ");

    EnableDMAInterrupt();
    EnableTimerInterrupt();
    ADCInit();
    initDMA();
    InitializeTimer();
    TIM_Cmd(TIM1, ENABLE);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (true) {
        if (update) {
            auto mean = getMeanSecond();
            sampleHour.addSample(mean);
            update = false;
            mean.printMilliAmpere(INSTANTANEUS_VALUE);
            auto stats= sampleHour.getStats();
            std::get<0>(stats).printMilliAmpere(MAX_VALUE);
            std::get<1>(stats).printMilliAmpere(MEAN_VALUE);
            Current::printMilliAmpere(TOT, std::get<2>(stats));

            gfx->fillRect(Point(0,72),239,320-72, WHITE );
            for(uint16_t x=0; x < 240; x++){
                gfx->drawFastVLine(Point(239-x, 319-distribution[x]), distribution[x], BLACK);
            }
        }
    };

}

void ADCInit(void) {
    //--Enable ADC1 and GPIOA--
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    //ADC1 configuration

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //!
    //select no external triggering
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_DiscModeChannelCountConfig(ADC1, 2);
    //configure each channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
    //Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    //enable DMA for ADC
    ADC_DMACmd(ADC1, ENABLE);
    //Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1);
    //Check the end of ADC1 reset calibration register
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;
    //Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    //Check the end of ADC1 calibration
    while (ADC_GetCalibrationStatus(ADC1)) {

    }
}

Current getMeanSecond() {
    uint32_t mean = 0;
    volatile uint16_t * iter;
    volatile uint16_t * iterBegin;
    volatile uint16_t * iterEnd;
    if (writeIndex > 1000) {
        iterBegin = currentSamples;
        iterEnd = currentSamples + 1000;
    } else {
        iterBegin = currentSamples + 1000;
        iterEnd = currentSamples + 2000;
    }
    for (iter = iterBegin; iter != iterEnd; iter++) {
        mean += *iter;
    }
    mean /= 1000;
    int16_t error;

    memset(distribution,0, 240*2);

    for (iter = iterBegin; iter != iterEnd; iter++) {
        error = *iter - mean;
        error += 120;
        if (error >= 240){
            distribution[239]++;
        } else if (error < 0){
            distribution[0]++;
        } else {
            distribution[error]++;
        }
    }

    uint16_t max =0;
    for (uint16_t  d : distribution){
        if (d > max){
            max = d;
        }
    }
    for (uint16_t &  d : distribution){
        d = (uint32_t)d*240/max;
    }

    return Current(mean);
}

void initDMA(void) {
    //enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //create DMA structure
    DMA_InitTypeDef DMA_InitStructure;
    //reset DMA1 channe1 to default values;
    DMA_DeInit(DMA1_Channel1);
    //channel will be used for memory to memory transfer
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //medium priority
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //source and destination data size word=32bit
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //automatic memory destination increment enable.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //source address increment disable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //chunk of data to be transfered
    DMA_InitStructure.DMA_BufferSize = 2;
    //source and destination start addresses
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) ADC1_DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADC_values;
    //send values to DMA registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Enable DMA1 Channel Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1


}

void EnableDMAInterrupt(){
    NVIC_InitTypeDef NVIC_InitStructure;
    //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EnableADCEOCInterrupt() {
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 1;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void EnableTimerInterrupt() {
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 2;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void InitializeTimer() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 72; // 1Mhz
    timerInitStructure.TIM_Period = 1000; // 1Khz;
    //timerInitStructure.TIM_Prescaler = 36000; // 2k
    //timerInitStructure.TIM_Period = 2000; // 1H;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &timerInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    InitializePWMChannel(500);
}

void InitializePWMChannel(int pulse) {

    TIM_OCInitTypeDef outputChannelInit;
    memset(&outputChannelInit, 0, sizeof(outputChannelInit));

    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = pulse;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OutputNState = TIM_OutputNState_Disable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
    outputChannelInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
    outputChannelInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    outputChannelInit.TIM_OCIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &outputChannelInit);
    // TIM_OC1PreloadConfig(TIM1RCC_APB2PeriphClockCmd, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
}

extern "C" {

void DMA1_Channel1_IRQHandler(void) {
    DMA_ClearITPendingBit(DMA1_IT_GL1);
//    if (ADC_values[1] < 100) {
//        currentSamples[writeIndex] = ADC_values[0] * 10;
//    } else {
        currentSamples[writeIndex] = ADC_values[0];
//    }
    writeIndex++;
    if (writeIndex % 1000 == 0) {
        update = true;
    }
    if (writeIndex == CURRENT_SAMPLE_SIZE) {
        writeIndex = 0;
    }
}

void TIM1_UP_IRQHandler(void) {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
