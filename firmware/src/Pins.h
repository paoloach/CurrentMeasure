/*
 * Pins.h
 *
 *  Created on: 16 gen 2017
 *      Author: paolo
 */

#ifndef PINS_H_
#define PINS_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"


/**

 * D0 --> E0
 * D1 --> E1
 * D2 --> E2
 * D3 --> E3
 * D4 --> E4
 * D5 --> E5
 * D6 --> E6
 * D7 --> E7
 * D8 --> E8
 * D9 --> E9
 * D10 -->E10
 * D11 -->E11
 * D12 -->E12
 * D13 -->E13
 * D14 -->E14
 * D15 -->DE15
 * Reset -->C2
 * CS -->C3
 * RS -->C4
 * WR -->C5
 *
 * AD_SERIAL --> A5
 * AD_CS     --> B8
 * AD_IN     --> A7
 * AD_OUT    --> A6
 * AD_BUSY   --> B6
 *
 */

constexpr auto AD_SERIAL_PORT=GPIOA;
#define AD_SERIAL_PIN   GPIO_PIN_5
constexpr auto AD_CS_PORT=GPIOB;
#define AD_CS_PIN   GPIO_PIN_8
constexpr auto AD_IN_PORT=GPIOA;
#define AD_IN_PIN   GPIO_PIN_7
constexpr auto AD_OUT_PORT=GPIOA;
#define AD_OUT_PIN   GPIO_PIN_6
constexpr auto AD_BUSY_PORT=GPIOB;
#define AD_BUSY_PIN   GPIO_PIN_6

constexpr auto RESET_PORT=GPIOC;
#define RESET_PIN  GPIO_PIN_3
constexpr auto CS_PORT=GPIOC;
#define CS_PIN  GPIO_PIN_2
constexpr auto RS_PORT=GPIOC;
#define RS_PIN  GPIO_PIN_4
constexpr auto WR_PORT=GPIOC;
#define WR_PIN  GPIO_PIN_5
constexpr auto D0_PORT=GPIOE;
#define D0_PIN  GPIO_PIN_0
constexpr auto D1_PORT=GPIOE;
#define D1_PIN  GPIO_PIN_1
constexpr auto D2_PORT=GPIOE;
#define D2_PIN  GPIO_PIN_2
constexpr auto D3_PORT=GPIOE;
#define D3_PIN  GPIO_PIN_3
constexpr auto D4_PORT=GPIOE;
#define D4_PIN  GPIO_PIN_4
constexpr auto D5_PORT=GPIOE;
#define D5_PIN  GPIO_PIN_5
constexpr auto D6_PORT=GPIOE;
#define D6_PIN  GPIO_PIN_6
constexpr auto D7_PORT=GPIOE;
#define D7_PIN  GPIO_PIN_7
constexpr auto D8_PORT=GPIOE;
#define D8_PIN  GPIO_PIN_8
constexpr auto D9_PORT=GPIOE;
#define D9_PIN  GPIO_PIN_9
constexpr auto D10_PORT=GPIOE;
#define D10_PIN  GPIO_PIN_10
constexpr auto D11_PORT=GPIOE;
#define D11_PIN  GPIO_PIN_11
constexpr auto D12_PORT=GPIOE;
#define D12_PIN  GPIO_PIN_12
constexpr auto D13_PORT=GPIOE;
#define D13_PIN  GPIO_PIN_13
constexpr auto D14_PORT=GPIOE;
#define D14_PIN  GPIO_PIN_14
constexpr auto D15_PORT=GPIOE;
#define D15_PIN  GPIO_PIN_15




#endif /* PINS_H_ */
