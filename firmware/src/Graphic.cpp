/*
 * Graphic.cpp
 *
 *  Created on: 29 mar 2017
 *      Author: paolo
 */

#include <limits>
#include <stdlib.h>
#include "Graphic.h"

#include "GFX.h"

extern GFX<Color16Bit> * gfx;

constexpr uint16_t TOP=80;
constexpr uint16_t BOTTOM=309;

bool Graphic::goNextTrigger() {
    auto nextTrig = sampleHour->getNextTrigger(triggerPos, trigger);
    if (nextTrig != std::numeric_limits<uint16_t>::max()){
        triggerPos = nextTrig;
        return true;
    }
    return false;
}

void Graphic::display() {
    uint16_t half = gfx->width/2;
    gfx->fillRect(Point(0,BOTTOM),239,10, WHITE);
    gfx->fillRect(Point(0,TOP),239,BOTTOM-TOP, BLACK);
    uint16_t start = triggerPos < half ? 0 : triggerPos-half;
    uint16_t end = start + gfx->width;
    if (end > SampleHour::SAMPLE_SIZE){
        end = SampleHour::SAMPLE_SIZE;
        start = end - gfx->width;
    }
    for (int i=0; i < 5; i++){
        printLabel(Point(i*50,319), start);
    }

    auto stats = sampleHour->getStats();
    for (uint16_t i = start; i < end; i++){
        uint16_t y = sampleHour->get(i).scale(std::get<0>(stats),BOTTOM-TOP );
        y+=BOTTOM-y;
        gfx->drawPixel(Point(i, y), WHITE);
    }
}

void Graphic::printLabel(Point && p, uint16_t num) {
    gfx->setFont(&smallFont);
    char buffer[10];
    itoa(num, buffer, 10);
    gfx->drawFastVLine(Point(p.x, 309), 15, Color16Bit(128,128,128));
    gfx->drawString(std::move(p), buffer);

}
