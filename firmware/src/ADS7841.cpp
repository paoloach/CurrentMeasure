/*
 * ADS7841.cpp
 *
 *  Created on: 29 mag 2017
 *      Author: paolo
 */


#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "ADS7841.h"
#include "Pins.h"
#include "diag/Trace.h"

ADS7841::ADS7841() {
    init();
}

void ADS7841::init() {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = AD_SERIAL_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(AD_SERIAL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = AD_CS_PIN;
    HAL_GPIO_Init(AD_CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = AD_IN_PIN;
    HAL_GPIO_Init(AD_IN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pin = AD_OUT_PIN;
    HAL_GPIO_Init(AD_OUT_PORT, &GPIO_InitStructure);

}

#define WAIT_200 __asm(\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
);

#define WAIT_UNTIL_200A __asm(\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
);

#define WAIT_UNTIL_200B __asm(\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
                    "nop\n nop\n nop\n nop\n nop\n"\
);

uint16_t ADS7841::get(uint8_t channel) {
    uint16_t result = 0;

    uint8_t toSend;
    switch(channel){
    case 0:
        toSend = 0x94;
        break;
    case 1:
        toSend = 0xD4;
        break;
    case 2:
        toSend = 0xA4;
        break;
    case 3:
        toSend = 0xE4;
        break;
    default:
        toSend = 0x090;
    }

    AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN << 16;
    AD_CS_PORT->BSRR = AD_CS_PIN << 16;
    for (uint8_t i = 0; i < 8; i++) {
        if (toSend & 0x80)
            AD_IN_PORT->BSRR = AD_IN_PIN;
        else
            AD_IN_PORT->BSRR = AD_IN_PIN << 16;
        toSend = toSend << 1;
        __asm(
                "nop\n nop\n nop\n nop\n  nop\n"
                "nop\n nop\n nop\n nop\n  nop\n"
                );

        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN;
        WAIT_200
        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN << 16;
    }
    // BUSY
    WAIT_200
    AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN;
    WAIT_200
    for (uint8_t i = 0; i < 12; i++) {
        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN << 16;
        WAIT_200
        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN;
        if (AD_OUT_PORT->IDR & AD_OUT_PIN) {
            result |= 1;
        }
        result = result << 1;
        WAIT_UNTIL_200B
    }

    for (uint8_t i = 0; i < 4; i++) {
        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN << 16;
        WAIT_200
        AD_SERIAL_PORT->BSRR = AD_SERIAL_PIN;
        WAIT_200
    }

    AD_CS_PORT->BSRR = AD_CS_PIN;

    result = result >> 1;

    return result;
}

