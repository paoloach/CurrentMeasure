/*
 * Sampling.h
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#ifndef SAMPLING_H_
#define SAMPLING_H_

#include <array>
#include "CurrentMeasure.h"

class Sampling {
    using Array = std::array<uint16_t, 1000>;
#define TIMER  TIM1
public:
    Sampling();
    void enableISR() {runIsr=true;}
    void isr();
    uint16_t meanSecond();
    void startSampling(){
        TIMER->DIER |= TIM_DIER_UIE;
    }
    void stopSampling(){
        TIMER->DIER &= ~TIM_DIER_UIE;
    }
private:
    CurrentMeasure currentMeasure;
    bool runIsr;
    Array seconds1;
    Array seconds2;
    Array::iterator currentIter;
    Array &current;
    Array &next;
};

#endif /* SAMPLING_H_ */
