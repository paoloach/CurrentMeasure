/*
 * CurrentMeasure.h
 *
 *  Created on: 05 giu 2017
 *      Author: paolo
 */

#ifndef CURRENTMEASURE_H_
#define CURRENTMEASURE_H_

#include <string.h>
#include "ADS7841.h"
#include "Current.h"
extern "C" {

void TIM2_IRQHandler(void);
}



class CurrentMeasure {
public:
    void init();
    static void saveData();
    // Return from 0 to 860 100uA
    static Current  getHighSensibility(){
           auto adc = ADS7841::resultHigh;
           constexpr double rINA122 = 470;
           constexpr double gain = 5+200000/rINA122;
           constexpr double rShunt = 0.11;
           return Current(adc/(gain*rShunt));
       }
    // Return from 8 to  1489 mA
    static Current getLowSensibility(){
            uint16_t adc = ADS7841::resultLow;
            constexpr double rINA122 = 10000;
            constexpr double gain = 5+200000/rINA122;
            constexpr double rShunt = 0.11;
            return Current(8+ adc/(gain*rShunt));
        }
    static void start() ;
    static void restartAcquire() ;

    static Current * readingSamples;
    static uint32_t mean;
    static bool     trigger;
    static uint16_t triggerStart;
    static uint16_t triggerLevel;
    static uint16_t count;
private:
    static uint32_t meanUsed;
    static Current samples1[480];
    static Current samples2[480];
    static Current * writingSamples;
    static uint16_t meanPos;
    static uint16_t samplePos;
    static TIM_HandleTypeDef handle;

    friend void TIM2_IRQHandler();
};

#endif /* CURRENTMEASURE_H_ */
