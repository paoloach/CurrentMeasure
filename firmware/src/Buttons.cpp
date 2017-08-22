/*
 * Buttons.cpp
 *
 *  Created on: 20 ago 2017
 *      Author: paolo
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx.h"
#include "Buttons.h"
#include "MessageQueue.h"

#define GPIO_MODE             ((uint32_t)0x00000003U)
#define EXTI_MODE             ((uint32_t)0x10000000U)
#define GPIO_MODE_IT          ((uint32_t)0x00010000U)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000U)
#define RISING_EDGE           ((uint32_t)0x00100000U)
#define FALLING_EDGE          ((uint32_t)0x00200000U)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010U)

void Buttons::init() {
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.Mode = EXTI_MODE | RISING_EDGE | GPIO_MODE_IT;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 1);
     HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

extern "C" {
void EXTI15_10_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_10)) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_10);
        MessageQueue::addMessage(Message(MessageType::Button1));
    }
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_11)) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_11);
        MessageQueue::addMessage(Message(MessageType::Button2));
    }
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_12)) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_12);
        MessageQueue::addMessage(Message(MessageType::Button3));
    }
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_13)) {
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_13);
        MessageQueue::addMessage(Message(MessageType::Button4));
    }
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_14)){
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);
        MessageQueue::addMessage(Message(MessageType::Button5));
    }
    if (__HAL_GPIO_EXTI_GET_FLAG(GPIO_PIN_15)){
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_15);
        MessageQueue::addMessage(Message(MessageType::Button6));
    }
}
}

