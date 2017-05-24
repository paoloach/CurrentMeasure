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

 * CS --> B0
 * RS --> A7
 * WR --> A6
 * RD --> B6
 * D0 --> B7
 * D1 --> B8
 * D2 --> B9
 * D3 --> A12
 * D4 --> B15
 * D5 --> B12
 * D6 --> B13
 * D7 --> B14
 */


constexpr auto RESET_PORT=GPIOA;
#define RESET_PIN  GPIO_Pin_8
constexpr auto CS_PORT=GPIOB;
#define CS_PIN  GPIO_Pin_0
constexpr auto RS_PORT=GPIOA;
#define RS_PIN  GPIO_Pin_7
constexpr auto WR_PORT=GPIOA;
#define WR_PIN  GPIO_Pin_6
constexpr auto RD_PORT=GPIOB;
#define RD_PIN  GPIO_Pin_6
constexpr auto D0_PORT=GPIOB;
#define D0_PIN  GPIO_Pin_7
constexpr auto D1_PORT=GPIOB;
#define D1_PIN  GPIO_Pin_8
constexpr auto D2_PORT=GPIOB;
#define D2_PIN  GPIO_Pin_9
constexpr auto D3_PORT=GPIOA;
#define D3_PIN  GPIO_Pin_11
constexpr auto D4_PORT=GPIOB;
#define D4_PIN  GPIO_Pin_15
constexpr auto D5_PORT=GPIOB;
#define D5_PIN  GPIO_Pin_12
constexpr auto D6_PORT=GPIOB;
#define D6_PIN  GPIO_Pin_13
constexpr auto D7_PORT=GPIOB;
#define D7_PIN  GPIO_Pin_14

constexpr auto XP_PORT=D6_PORT;
constexpr auto XP_PIN=GPIO_Pin_13;
constexpr auto XM_PORT=RS_PORT;
constexpr auto XM_PIN=RS_PIN;
constexpr auto XM_ADC_PORT= ADC_Channel_1;
constexpr auto YP_PORT=WR_PORT;
constexpr auto YP_PIN=WR_PIN;
constexpr auto YP_ADC_PORT = ADC_Channel_0;
constexpr auto YM_PORT=D7_PORT;
constexpr auto YM_PIN=D7_PIN;




#endif /* PINS_H_ */
