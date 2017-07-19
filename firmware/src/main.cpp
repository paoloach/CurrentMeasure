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

GFX<Color16Bit> * gfx;

extern Sampling sampling;

constexpr Point INSTANTANEUS_VALUE(128, 8);
constexpr Point MAX_VALUE(128, 24);
constexpr Point MEAN_VALUE(128, 40);
constexpr Point TOT(128, 56);


int main(int argc, char* argv[]) {
    trace_puts("Current measure!");
    Timer timer;
    CurrentMeasure currentMeasure;
    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    timer.start();

    currentMeasure.init();
    GFX<Color16Bit>::initIO();

    gfx = new HX8357::HX8357 { LcdID::ID_HX8357B };

    gfx->setForeground(WHITE);
    gfx->setBackground(BLACK);
    gfx->setFont(&bigFont);

    gfx->drawString(Point(0, INSTANTANEUS_VALUE.y), "Actual: ");
//    sampling.startSampling();
    while (true) {
        auto high = currentMeasure.getHighSensibility();
        auto low = currentMeasure.getLowSensibility();
        printActual(high, low);
        sampling.isr();
    }


}

static uint16_t printActual(uint16_t hightSensibility, uint16_t lowSensibility) {
    uint16_t actual;
    if (lowSensibility < 50) {
        actual = hightSensibility;
    } else {
        actual = lowSensibility;
    }
    Point p( printInt(INSTANTANEUS_VALUE, actual)*16+INSTANTANEUS_VALUE.x + 16, INSTANTANEUS_VALUE.y);
    gfx->drawString(Point(p), "(");
    p.x += 16;
    p.x += printInt(p, hightSensibility)*16;
    gfx->drawString(Point(p), ", ");
    p.x += 32;
    p.x += printInt(p, lowSensibility)*16;
    gfx->drawString(Point(p), ")   ");
    return actual;
}

static uint16_t printInt(Point point, uint16_t value) {
    char buffer[10];
    itoa(value, buffer, 10);
    return gfx->drawString(Point(point), buffer);
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
