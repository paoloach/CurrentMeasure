/*
 * Pins.h
 *
 *  Created on: 16 gen 2017
 *      Author: paolo
 */

#ifndef PINS_H_
#define PINS_H_

#include "stm32f10x.h"


/**

 * D0 --> B0
 * D1 --> B1
 * D2 --> B5
 * D3 --> B6
 * D4 --> B7
 * D5 --> B8
 * D6 --> B9
 * D7 --> B10
 * D8 --> B11
 * D9 --> B12
 * D10 -->B13
 * D11 -->B14
 * D12 -->B15
 * D13 -->A8
 * D14 -->A9
 * D15 -->A10
 * Reset -->A12
 * CS -->A5
 * RS -->A6
 * WR -->A7
 *
 * AD_SERIAL --> A0
 * AD_CS     --> A1
 * AD_IN     --> A2
 * AD_OUT    --> A3
 * AD_BUSY   --> A4
 *
 */

constexpr auto AD_SERIAL_PORT=GPIOA;
#define AD_SERIAL_PIN   GPIO_Pin_0
constexpr auto AD_CS_PORT=GPIOA;
#define AD_CS_PIN   GPIO_Pin_1
constexpr auto AD_IN_PORT=GPIOA;
#define AD_IN_PIN   GPIO_Pin_2
constexpr auto AD_OUT_PORT=GPIOA;
#define AD_OUT_PIN   GPIO_Pin_4
constexpr auto AD_BUSY_PORT=GPIOA;
#define AD_BUSY_PIN   GPIO_Pin_3

constexpr auto RESET_PORT=GPIOA;
#define RESET_PIN  GPIO_Pin_12
constexpr auto CS_PORT=GPIOA;
#define CS_PIN  GPIO_Pin_5
constexpr auto RS_PORT=GPIOA;
#define RS_PIN  GPIO_Pin_6
constexpr auto WR_PORT=GPIOA;
#define WR_PIN  GPIO_Pin_7
constexpr auto D0_PORT=GPIOB;
#define D0_PIN  GPIO_Pin_0
constexpr auto D1_PORT=GPIOB;
#define D1_PIN  GPIO_Pin_1
constexpr auto D2_PORT=GPIOB;
#define D2_PIN  GPIO_Pin_5
constexpr auto D3_PORT=GPIOB;
#define D3_PIN  GPIO_Pin_6
constexpr auto D4_PORT=GPIOB;
#define D4_PIN  GPIO_Pin_7
constexpr auto D5_PORT=GPIOB;
#define D5_PIN  GPIO_Pin_8
constexpr auto D6_PORT=GPIOB;
#define D6_PIN  GPIO_Pin_9
constexpr auto D7_PORT=GPIOB;
#define D7_PIN  GPIO_Pin_10
constexpr auto D8_PORT=GPIOB;
#define D8_PIN  GPIO_Pin_11
constexpr auto D9_PORT=GPIOB;
#define D9_PIN  GPIO_Pin_12
constexpr auto D10_PORT=GPIOB;
#define D10_PIN  GPIO_Pin_13
constexpr auto D11_PORT=GPIOB;
#define D11_PIN  GPIO_Pin_14
constexpr auto D12_PORT=GPIOB;
#define D12_PIN  GPIO_Pin_15
constexpr auto D13_PORT=GPIOA;
#define D13_PIN  GPIO_Pin_8
constexpr auto D14_PORT=GPIOA;
#define D14_PIN  GPIO_Pin_9
constexpr auto D15_PORT=GPIOA;
#define D15_PIN  GPIO_Pin_10




#endif /* PINS_H_ */
