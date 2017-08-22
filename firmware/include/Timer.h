//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef TIMER_H_
#define TIMER_H_

#include <vector>
#include <tuple>
#include "cmsis_device.h"
#include "../src/MessageQueue.h"
#include "diag/Trace.h"

// ----------------------------------------------------------------------------

class Timer {
public:
    typedef uint32_t ticks_t;
    static constexpr ticks_t FREQUENCY_HZ = 1000u;

private:
    static volatile ticks_t ms_delayCount;
    using Messages = std::vector<std::tuple<uint32_t, Message>>;
    static std::vector<std::tuple<uint32_t, Message>> messages;
public:
    // Default constructor
    Timer() = default;

    inline void start(void) {
        // Use SysTick as reference for the delay loops.
        SysTick_Config(SystemCoreClock / FREQUENCY_HZ);
    }

    static void sleep(uint32_t ticks);

    void delayMessage(Message message, uint32_t ms) {
        HAL_SuspendTick();
        auto tick = ms + HAL_GetTick();
        messages.emplace_back(tick, message);
        HAL_ResumeTick();
    }

    inline static void tick(void) {
        auto tick = HAL_GetTick();
        for (Messages::iterator it = messages.begin(); it != messages.end(); it++) {
            if (std::get<0>(*it) <= tick) {
                MessageQueue::addMessage(std::get<1>(*it));
                messages.erase(it);
                break;
            }
        }
        // Decrement to zero the counter used by the delay routine.
        if (ms_delayCount != 0u) {
            --ms_delayCount;
        }
    }
};

// ----------------------------------------------------------------------------

#endif // TIMER_H_
