/*
 * SampleHour.h
 *
 *  Created on: 28 mar 2017
 *      Author: paolo
 */

#ifndef SAMPLEHOUR_H_
#define SAMPLEHOUR_H_

#include <stdint.h>
#include "Current.h"

class SampleHour {
public:
    constexpr static uint16_t SAMPLE_SIZE=3600;
    SampleHour():sampleWrite(0){};
    void addSample(Current value);
    std::tuple<Current,Current, uint32_t> getStats();
    uint16_t getNextTrigger(uint16_t trigPos, Current triggerValue);
    Current get(uint16_t index) const {return data[index];}
private:
    Current data[SAMPLE_SIZE];
    uint16_t sampleWrite;
};

#endif /* SAMPLEHOUR_H_ */
