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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

static uint16_t printInt(Point point, uint32_t value);
static uint16_t printActual(uint16_t highSensibility, uint16_t lowSensibility);
static void printMean();
static void printGraph();

HX8357::HX8357 * gfx;

extern uint16_t count;

constexpr Point INSTANTANEUS_VALUE(128, 0);
constexpr Point MAX_VALUE(128, 24);
constexpr Point MEAN_VALUE(128, 40);
constexpr Point TOT(128, 56);

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

    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    timer.start();

    currentMeasure.init();
    GFX<Color16Bit>::initIO();

    gfx = new HX8357::HX8357 { LcdID::ID_HX8357D };

    gfx->setForeground(BLACK);
    gfx->setBackground(WHITE);
    gfx->setFont(&bigFont);

    gfx->drawString(Point(0, INSTANTANEUS_VALUE.y), "Actual: ");
    CurrentMeasure::start();
    printGraph();
    while (true) {
        if (CurrentMeasure::meanAvailable) {
            printMean();
            CurrentMeasure::meanAvailable = false;
        }
        if (CurrentMeasure::sampleEnd) {
            printGraph();
            CurrentMeasure::restartAcquire();
        }
        if (ADS7841::resultPresent) {
            ADS7841::resultPresent = false;
            CurrentMeasure::saveData();
        }
    }
}

static void printGraph() {
    for (uint16_t x = 0; x < 480; x++) {
        uint16_t val = CurrentMeasure::currents[x];
        gfx->drawFastSample(x, val);
    }
}

static void printMean() {
    Point p(printInt(INSTANTANEUS_VALUE, CurrentMeasure::mean) * 16 + INSTANTANEUS_VALUE.x, INSTANTANEUS_VALUE.y);
    gfx->drawString(std::move(p), "   ");
}

static uint16_t printActual(uint16_t hightSensibility, uint16_t lowSensibility) {
    uint16_t actual;
    if (lowSensibility < 50) {
        actual = hightSensibility;
    } else {
        actual = lowSensibility;
    }
    Point p(printInt(INSTANTANEUS_VALUE, actual) * 16 + INSTANTANEUS_VALUE.x + 16, INSTANTANEUS_VALUE.y);
    gfx->drawString(Point(p), "(");
    p.x += 16;
    p.x += printInt(p, hightSensibility) * 16;
    gfx->drawString(Point(p), ", ");
    p.x += 16;
    p.x += printInt(p, lowSensibility) * 16;
    gfx->drawString(Point(p), ")   ");
    p.x += 64;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    p.x += 16;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    p.x += 16;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    p.x += 16;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    p.x += 16;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    p.x += 16;
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET) {
        gfx->drawString(Point(p), "1");
    } else {
        gfx->drawString(Point(p), "0");
    }
    return actual;
}

static uint16_t printInt(Point point, uint32_t value) {
    char buffer[10];
    itoa(value, buffer, 10);
    return gfx->drawString(Point(point), buffer);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
