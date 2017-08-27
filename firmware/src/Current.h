/*
 * Current.h
 *
 *  Created on: 29 mar 2017
 *      Author: paolo
 */

#ifndef CURRENT_H_
#define CURRENT_H_

class Current {
public:
    Current():value(0){};
    constexpr Current(uint16_t value):value(value){}
    bool operator >(const Current & other){return value > other.value;}
    bool operator <(const Current & other){return value < other.value;}
public:
    uint16_t value;
};


#endif /* CURRENT_H_ */
