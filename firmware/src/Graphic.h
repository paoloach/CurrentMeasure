/*
 * Graphic.h
 *
 *  Created on: 29 mar 2017
 *      Author: paolo
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#include "SampleHour.h"

class Graphic {
public:
    Graphic(SampleHour * sampleHour): sampleHour(sampleHour), triggerPos(0){};
    void setTriggerValue(Current value){trigger= value;}
    Current getTriggerValue() const {return trigger;}
    uint16_t getTriggerPos() const {return triggerPos;}
    bool goNextTrigger();
    void display();
private:
    void printLabel(Point && p, uint16_t num);
    SampleHour * sampleHour;
    Current trigger;
    uint16_t triggerPos;
};

#endif /* GRAPHIC_H_ */
