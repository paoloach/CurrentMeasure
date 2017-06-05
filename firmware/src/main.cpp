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
#include "ADS7841.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


GFX<Color16Bit> * gfx;

constexpr Point INSTANTANEUS_VALUE(128, 8);
constexpr Point MAX_VALUE(128, 24);
constexpr Point MEAN_VALUE(128, 40);
constexpr Point TOT(128, 56);

int main(int argc, char* argv[]) {
    trace_puts("Current measure!");
    Timer timer;
    ADS7841 ads7841;
    trace_printf("System clock: %u Hz\n", SystemCoreClock);

    timer.start();


    GFX<Color16Bit>::initIO();

    gfx = new HX8357::HX8357 {LcdID::ID_HX8357B};


    gfx->setForeground(WHITE);
    gfx->setBackground(BLACK);
    gfx->setFont(&bigFont);


    gfx->drawString(Point(0, INSTANTANEUS_VALUE.y), "Actual: ");
    gfx->drawString(Point(0, MAX_VALUE.y), "Max: ");
    gfx->drawString(Point(0, MEAN_VALUE.y), "Mean: ");
    gfx->drawString(Point(0, TOT.y), "mA/h: ");

    char buffer[10];
    while (true){
        auto value = ads7841.get(0);
        itoa(value, buffer, 10);
        strcat(buffer, "    ");
        gfx->drawString(Point(INSTANTANEUS_VALUE), buffer );
         value = ads7841.get(1);
        itoa(value, buffer, 10);
        strcat(buffer, "    ");
        gfx->drawString(Point(MAX_VALUE), buffer );
    }
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
