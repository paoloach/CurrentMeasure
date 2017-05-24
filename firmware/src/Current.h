/*
 * Current.h
 *
 *  Created on: 29 mar 2017
 *      Author: paolo
 */

#ifndef CURRENT_H_
#define CURRENT_H_

#include "GFX.h"

class Current {
public:
    Current():value(0){};
    Current(uint16_t value):value(value){}
    bool operator >(const Current & other){return value > other.value;}
    void printMilliAmpere(Point p);
    void static printMilliAmpere(Point p, uint32_t value);
    uint16_t getValue() const {return value;}
    uint16_t scale(Current max, uint16_t scaling){
        return ((uint32_t)value*scaling)/max.value;
    }
private:
    static uint16_t CONV_NUM;
    static uint16_t CONV_DEN;
    uint16_t value;
};

#endif /* CURRENT_H_ */
