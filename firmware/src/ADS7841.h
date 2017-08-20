/*
 * ADS7841.h
 *
 *  Created on: 29 mag 2017
 *      Author: paolo
 */

#ifndef ADS7841_H_
#define ADS7841_H_

#include <stdint.h>

extern "C" {
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_spi.h"
}

class ADS7841 {
public:
    ADS7841();
    static void init();
    static void get();
    static SPI_HandleTypeDef handle;
    static DMA_HandleTypeDef txDMA;
    static DMA_HandleTypeDef rxDMA;
    static uint8_t TxBuffer[6];
    static uint8_t RxBuffer[6];
    static uint16_t resultHigh;
    static uint16_t resultLow;
    static bool resultPresent;
};

#endif /* ADS7841_H_ */
