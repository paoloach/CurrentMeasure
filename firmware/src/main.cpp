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

static uint16_t printInt(Point point, uint16_t value);
static uint16_t printActual(uint16_t highSensibility, uint16_t lowSensibility);
static void printGraph();

HX8357::HX8357 * gfx;

extern Sampling sampling;
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

//    trace_printf("HSI_ON: --> %d\n", RCC->CR & 0x1);
//    trace_printf("HSI_RDY: --> %d\n", (RCC->CR >> 1) & 0x1);
//    trace_printf("HSI_TRIM: --> %d\n", (RCC->CR >> 3) & 0x1F);
//    trace_printf("HSI_CAL: --> %d\n", (RCC->CR >> 8) & 0xFF);
//    trace_printf("HSE_ON: --> %d\n", (RCC->CR >> 16) & 0x1);
//    trace_printf("HSE_RDY: --> %d\n", (RCC->CR >> 17) & 0x1);
//    trace_printf("HSE_BYP: --> %d\n", (RCC->CR >> 18) & 0x1);
//    trace_printf("CSS_ON: --> %d\n", (RCC->CR >> 19) & 0x1);
//    trace_printf("PLL_ON: --> %d\n", (RCC->CR >> 24) & 0x1);
//    trace_printf("PLL_RDY: --> %d\n", (RCC->CR >> 25) & 0x1);
//    trace_printf("PLLIS2_ON: --> %d\n", (RCC->CR >> 26) & 0x1);
//    trace_printf("PLLIS2_RDY: --> %d\n", (RCC->CR >> 27) & 0x1);
//
//    trace_printf("PLLM: --> %d\n", RCC->PLLCFGR & 0x3F);
//    trace_printf("PLLN: --> %d\n", (RCC->PLLCFGR >> 6) & 0x1FF);
//    trace_printf("PLLP: --> %d\n", (RCC->PLLCFGR >> 16) & 0x3);
//    trace_printf("PLLSRC: --> %d\n", (RCC->PLLCFGR >> 22) & 0x1);
//    trace_printf("PLLQ: --> %d\n", (RCC->PLLCFGR >> 24) & 0xF);
//
//    trace_printf("SW: --> %d\n", RCC->CFGR & 3);
//    trace_printf("SWS: --> %d\n", (RCC->CFGR >> 2) & 3);
//    trace_printf("HPRE: --> %d\n", (RCC->CFGR >> 4) & 0xF);
//    trace_printf("PPRE: --> %d\n", (RCC->CFGR >> 10) & 7);
//    trace_printf("PPRE2: --> %d\n", (RCC->CFGR >> 13) & 7);
//    trace_printf("RTCPRE: --> %d\n", (RCC->CFGR >> 16) & 0x1F);
//    trace_printf("MC01: --> %d\n", (RCC->CFGR >> 21) & 3);
//    trace_printf("I2_SSCR: --> %d\n", (RCC->CFGR >> 23) & 1);
//    trace_printf("MC01_PRE: --> %d\n", (RCC->CFGR >> 24) & 0x7);
//    trace_printf("MC02_PRE: --> %d\n", (RCC->CFGR >> 27) & 0x1F);
//    trace_printf("MC02: --> %d\n", (RCC->CFGR >> 30) & 3);
//
//    trace_printf("CR: -->  %08X\n", RCC->CR);
//    trace_printf("PLLCFGR: -->  %08X\n", RCC->PLLCFGR);
//    trace_printf("CFGR: -->  %08X\n", RCC->CFGR);
//    trace_printf("CSR: -->  %08X\n", RCC->CSR);
//    trace_printf("BDCR: -->  %08X\n", RCC->BDCR);

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

    gfx->setForeground(WHITE);
    gfx->setBackground(BLACK);
    gfx->setFont(&bigFont);

    gfx->drawString(Point(0, INSTANTANEUS_VALUE.y), "Actual: ");
    CurrentMeasure::start();
    printGraph();
    while (true) {
        while (!CurrentMeasure::sampleEnd)
            ;
        trace_printf("counted: %d\n", count);
        printGraph();
        CurrentMeasure::start();
        auto high = currentMeasure.getHighSensibility();
        auto low = currentMeasure.getLowSensibility();
        printActual(high, low);
    }
}

static void printGraph() {
    for (uint16_t x = 0; x < 480; x++) {
        uint16_t val = CurrentMeasure::currents[x];
        gfx->drawFastSample(x, val);
    }
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

static uint16_t printInt(Point point, uint16_t value) {
    char buffer[10];
    itoa(value, buffer, 10);
    return gfx->drawString(Point(point), buffer);
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
