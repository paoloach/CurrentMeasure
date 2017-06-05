/*
 * Current.cpp
 *
 *  Created on: 29 mar 2017
 *      Author: paolo
 */

#include <stdio.h>
#include "Current.h"

extern GFX<Color16Bit> * gfx;

uint16_t Current::CONV_NUM=1;
uint16_t Current::CONV_DEN=1;

void Current::printMilliAmpere(Point p){
    char buffer[10];
    uint16_t mA = ((uint32_t)CONV_NUM*value)/CONV_DEN;

    sprintf(buffer,"%d mA", mA);
    gfx->drawString(std::move(p), buffer);
}


void Current::printMilliAmpere(Point p, uint32_t value){
    char buffer[10];
    uint32_t mA = ((uint32_t)CONV_NUM*value)/CONV_DEN;

    sprintf(buffer,"%d mA", mA);
    gfx->drawString(std::move(p), buffer);
}
