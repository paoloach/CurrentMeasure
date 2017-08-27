/*
 * MessageQueue.h
 *
 *  Created on: 20 ago 2017
 *      Author: paolo
 */

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include <queue>
#include "stm32f4xx_hal.h"

enum MessageType {
    ButtonUp,
    Button2,
    ButtonDown,
    Button4,
    Button5,
    Button6,
    RemoveTrigger,
    NewAdcSample,
    RequestNewSample,
    MeanAvailable,
    SampleEnd
};

class Message {
public:
    constexpr Message(MessageType type):type(type){}
    MessageType type;
};

class MessageQueue {
public:
    static void addMessage(Message && message){
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        NVIC_DisableIRQ(DMA2_Stream0_IRQn);
        NVIC_DisableIRQ(TIM2_IRQn);
        HAL_SuspendTick();
        queue.push(message);
        HAL_ResumeTick();
        NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        NVIC_EnableIRQ(TIM2_IRQn);
    }


    static bool empty()  {
        NVIC_DisableIRQ(TIM2_IRQn);
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        NVIC_DisableIRQ(DMA2_Stream0_IRQn);
        HAL_SuspendTick();
        bool result = queue.empty();
        HAL_ResumeTick();
        NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        NVIC_EnableIRQ(TIM2_IRQn);
        return result;
    }

    static void pop() {
        NVIC_DisableIRQ(TIM2_IRQn);
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        NVIC_DisableIRQ(DMA2_Stream0_IRQn);
        HAL_SuspendTick();
        queue.pop();
        HAL_ResumeTick();
        NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        NVIC_EnableIRQ(TIM2_IRQn);
    }

    static Message & get() {
        NVIC_DisableIRQ(TIM2_IRQn);
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        NVIC_DisableIRQ(DMA2_Stream0_IRQn);
        HAL_SuspendTick();
        Message & message = queue.front();
        HAL_ResumeTick();
        NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        NVIC_EnableIRQ(TIM2_IRQn);
        return message;
        }
private:
    static std::queue<Message> queue;
};

#endif /* MESSAGEQUEUE_H_ */
