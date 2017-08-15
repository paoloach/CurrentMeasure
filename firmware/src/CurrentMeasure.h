/*
 * CurrentMeasure.h
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#ifndef CURRENTMEASURE_H_
#define CURRENTMEASURE_H_

#include "ADS7841.h"

class CurrentMeasure {
public:
    void init();
    // Max current:     86 mA
    static uint16_t  getHighSensibility(){
        uint16_t adc = ADS7841::get(0);
        constexpr double rINA122 = 470;
        constexpr double gain = 5+200000/rINA122;
        constexpr double rShunt = 0.11;
        double current =adc/(gain*rShunt);
        return current;
    }
    // Max current: 1489 mA
    static uint16_t getLowSensibility(){
        uint16_t adc = ADS7841::get(1);
        constexpr double rINA122 = 10000;
        constexpr double gain = 5+200000/rINA122;
        constexpr double rShunt = 0.11;
        double current =8+ adc/(gain*rShunt);
        return current;
    }

    static void start() ;
    static uint16_t currents[480];
    static uint16_t pos;
    static bool     sampleEnd;
    static bool     trigger;
    static uint16_t triggerStart;
    static uint16_t triggerLevel;
};

#endif /* CURRENTMEASURE_H_ */
