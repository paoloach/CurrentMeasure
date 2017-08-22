//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"
#include "HX8357.h"
#include "Timer.h"
#include "CurrentMeasure.h"
#include "Sampling.h"
#include "Buttons.h"
#include "MessageQueue.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

static uint16_t printInt(Point point, uint32_t value);
static void printTriggerLevel();
static void printMean();
static void drawGraph();

HX8357::HX8357 * gfx;

constexpr Point INSTANTANEUS_VALUE_POS(128, 0);
constexpr Point TRIGGER_LABEL_POS(0, 20);
constexpr Point TRIGGER_VALUE_POS(TRIGGER_LABEL_POS.x+9*16, TRIGGER_LABEL_POS.y);

uint8_t triggerLevelColorValue;

int main(int argc, char* argv[]) {
    trace_puts("Current measure!");
    Timer timer;
    CurrentMeasure currentMeasure;
    RCC_OscInitTypeDef rccOscInit;

    rccOscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    rccOscInit.HSEState = RCC_HSE_ON;
    rccOscInit.LSEState = RCC_LSE_OFF;
    rccOscInit.HSIState = RCC_HSI_OFF;
    rccOscInit.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    rccOscInit.LSIState = RCC_LSI_OFF;
    ;
    rccOscInit.PLL.PLLState = RCC_PLL_ON;
    rccOscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    rccOscInit.PLL.PLLM = 25;
    rccOscInit.PLL.PLLN = 336;
    rccOscInit.PLL.PLLP = RCC_PLLP_DIV2;
    rccOscInit.PLL.PLLQ = 7;

    HAL_RCC_OscConfig(&rccOscInit);

    __HAL_RCC_MCO1_CONFIG(RCC_MCO1SOURCE_HSE, RCC_MCODIV_5);

    RCC_ClkInitTypeDef clkInitStruct;
    clkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
    //clkInitStruct.SYSCLKSource = RCC_CFGR_SW_HSE;
    clkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
    clkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    clkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&clkInitStruct, FLASH_LATENCY_5);

    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    __HAL_RCC_GPIOA_CLK_ENABLE()
    ;
    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = GPIO_PIN_8;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF0_MCO;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    Buttons::init();
    timer.start();

    currentMeasure.init();
    GFX<Color16Bit>::initIO();

    gfx = new HX8357::HX8357 { LcdID::ID_HX8357D };

    gfx->setForeground(BLACK);
    gfx->setBackground(WHITE);
    gfx->setFont(&bigFont);

    gfx->drawString(Point(0, INSTANTANEUS_VALUE_POS.y), "Actual: ");
    CurrentMeasure::start();
    drawGraph();

    trace_printf("EXTI->IMR = %08X\n", EXTI->IMR);
    trace_printf("EXTI->RTSR = %08X\n", EXTI->RTSR);
    trace_printf("EXTI->FTSR = %08X\n", EXTI->FTSR);
    trace_printf("SYSCFG->EXTICR[0] = %08X\n", SYSCFG->EXTICR[0]);
    trace_printf("SYSCFG->EXTICR[1] = %08X\n", SYSCFG->EXTICR[1]);
    trace_printf("SYSCFG->EXTICR[2] = %08X\n", SYSCFG->EXTICR[2]);
    trace_printf("SYSCFG->EXTICR[3] = %08X\n", SYSCFG->EXTICR[3]);

    while (true) {
        if (!MessageQueue::empty()) {
            auto message = MessageQueue::get();
            switch (message.type) {
            case MessageType::Button1:
                CurrentMeasure::triggerLevel++;
                triggerLevelColorValue=255;
                drawGraph();
                printTriggerLevel();
                timer.delayMessage(MessageType::RemoveTrigger, 4000);
                break;
            case MessageType::Button3:
                CurrentMeasure::triggerLevel--;
                triggerLevelColorValue=255;
                drawGraph();
                printTriggerLevel();
                timer.delayMessage(MessageType::RemoveTrigger, 4000);
                break;
            case MessageType::RemoveTrigger:
                trace_printf("tick %d\n", HAL_GetTick());
                if(triggerLevelColorValue <30){
                    triggerLevelColorValue=0;
                    drawGraph();
                } else {
                    triggerLevelColorValue-=30;
                    timer.delayMessage(MessageType::RemoveTrigger, 500);
                }
                printTriggerLevel();
                break;
            }
            MessageQueue::pop();
        }
        if (CurrentMeasure::meanAvailable) {
            printMean();
            CurrentMeasure::meanAvailable = false;
        }
        if (CurrentMeasure::sampleEnd) {
            drawGraph();
            CurrentMeasure::restartAcquire();
        }
        if (ADS7841::resultPresent) {
            ADS7841::resultPresent = false;
            CurrentMeasure::saveData();
        }
    }
}

static void drawGraph() {
    uint16_t * samples = CurrentMeasure::readingSamples;
    for (uint16_t x = 0; x < 480; x++) {
        gfx->drawFastSample(x, *samples);
        samples++;
    }
}

static void printMean() {
    gfx->setBackground( WHITE);
    gfx->setForeground(BLACK);
    Point p(printInt(INSTANTANEUS_VALUE_POS, CurrentMeasure::mean) * 16 + INSTANTANEUS_VALUE_POS.x, INSTANTANEUS_VALUE_POS.y);
    gfx->drawString(std::move(p), "   ");
}

static void printTriggerLevel() {
    gfx->setForeground( Color16Bit(triggerLevelColorValue, 0, 0));
    gfx->setBackground(BLACK);
    gfx->drawString(TRIGGER_LABEL_POS,"Trigger: ");
    auto nChar = printInt(TRIGGER_VALUE_POS, CurrentMeasure::triggerLevel);
    Point leftTop(TRIGGER_VALUE_POS.x + nChar*16, TRIGGER_VALUE_POS.y);
    gfx->drawRect(leftTop, 32, 16, BLACK);
}

static uint16_t printInt(Point point, uint32_t value) {
    char buffer[10];
    itoa(value, buffer, 10);

    return gfx->drawString(Point(point), buffer);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
