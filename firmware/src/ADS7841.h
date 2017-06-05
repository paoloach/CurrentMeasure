/*
 * ADS7841.h
 *
 *  Created on: 29 mag 2017
 *      Author: paolo
 */

#ifndef ADS7841_H_
#define ADS7841_H_

#include <stdint.h>

class ADS7841 {
public:
    ADS7841();
    void init();
    uint16_t get(uint8_t channel);
};

#endif /* ADS7841_H_ */
