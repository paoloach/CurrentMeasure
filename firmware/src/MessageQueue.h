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
    Button1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    RemoveTrigger
};

class Message {
public:
    Message(MessageType type):type(type){}
    MessageType type;
};

class MessageQueue {
public:
    static void addMessage(Message  message){
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        HAL_SuspendTick();
        queue.push(message);
        HAL_ResumeTick();
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

    static bool empty()  {
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        HAL_SuspendTick();
        bool result = queue.empty();
        HAL_ResumeTick();
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        return result;
    }

    static void pop() {
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        HAL_SuspendTick();
        queue.pop();
        HAL_ResumeTick();
        NVIC_EnableIRQ(EXTI15_10_IRQn);
    }

    static Message & get() {
        NVIC_DisableIRQ(EXTI15_10_IRQn);
        HAL_SuspendTick();
        Message & message = queue.front();
        HAL_ResumeTick();
        NVIC_EnableIRQ(EXTI15_10_IRQn);
        return message;
        }
private:
    static std::queue<Message> queue;
};

#endif /* MESSAGEQUEUE_H_ */
