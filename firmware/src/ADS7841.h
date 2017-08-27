/*
 * ADS7841.h
 *
 *  Created on: 29 mag 2017
 *      Author: paolo
 */

#ifndef ADS7841_H_
#define ADS7841_H_

#include <stdint.h>
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_spi.h"

extern "C" {
void DMA2_Stream0_IRQHandler(void);
}

class ADS7841 {
public:
    ADS7841();
    static void init();
    static void get();
    static uint16_t resultHigh;
    static uint16_t resultLow;
private:
    static SPI_HandleTypeDef handle;
    static DMA_HandleTypeDef txDMA;
    static DMA_HandleTypeDef rxDMA;
    static uint8_t TxBuffer[6];
    static uint8_t RxBuffer[6];
    friend void DMA2_Stream0_IRQHandler(void) ;
};

#endif /* ADS7841_H_ */
