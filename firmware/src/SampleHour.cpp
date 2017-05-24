/*
 * SampleHour.cpp
 *
 *  Created on: 28 mar 2017
 *      Author: paolo
 */

#include <limits>
#include "SampleHour.h"

void SampleHour::addSample(Current value){
    data[sampleWrite]=value;
    sampleWrite++;
    if (sampleWrite > 3600){
        sampleWrite=0;
    }
}


std::tuple<Current,Current,uint32_t> SampleHour::getStats() {
    uint32_t total=0;
    std::tuple<Current,Current, uint32_t> stats;
    for (auto current: data){
        if (current > std::get<0>(stats)){
            std::get<0>(stats) = current;
        }
        total += current.getValue();
    }
    std::get<1>(stats) = Current(total/3600);
    std::get<2>(stats) = total;
    return stats;
}

uint16_t SampleHour::getNextTrigger(uint16_t triggerStart, Current triggerValue) {
    for (;triggerStart < 3600; triggerStart++){
        if (data[triggerStart] > triggerValue){
            return triggerStart;
        }
    }
    return std::numeric_limits<uint16_t>::max();
}
