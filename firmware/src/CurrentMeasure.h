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
    void init() {ads7841.init();}
    // Max current:     86 mA
    uint16_t getHighSensibility(){
        uint16_t adc = ads7841.get(0);
        constexpr double rINA122 = 470;
        constexpr double gain = 5+200000/rINA122;
        constexpr double rShunt = 0.11;
        double current = adc/(gain*rShunt);
        return current;
    }
    // Max current: 1489 mA
    uint16_t getLowSensibility(){
        uint16_t adc = ads7841.get(1);
        constexpr double rINA122 = 10000;
        constexpr double gain = 5+200000/rINA122;
        constexpr double rShunt = 0.11;
        double current = adc/(gain*rShunt);
        return current;
    }
private:
    ADS7841 ads7841;
};

#endif /* CURRENTMEASURE_H_ */
