/*
 * BaseAcccess.h
 *
 *  Created on: 18 gen 2017
 *      Author: paolo
 */

#ifndef BASEACCCESS_H_
#define BASEACCCESS_H_

#include "Pins.h"
#include "diag/Trace.h"

static constexpr bool checkPort(GPIO_TypeDef * port) {
    return CS_PORT == port || RS_PORT == port || WR_PORT == port || RD_PORT == port || D0_PORT == port || D1_PORT == port || D2_PORT == port || D3_PORT == port || D4_PORT == port || D5_PORT == port
            || D6_PORT == port || D7_PORT == port;
}

static constexpr int countDataOnPort(GPIO_TypeDef * port) {
    return (D0_PORT == port ? 1 : 0) + (D1_PORT == port ? 1 : 0) + (D2_PORT == port ? 1 : 0) + (D3_PORT == port ? 1 : 0) + (D4_PORT == port ? 1 : 0) + (D5_PORT == port ? 1 : 0)
            + (D6_PORT == port ? 1 : 0) + (D7_PORT == port ? 1 : 0);

}

inline void checkPorts() {
    if (checkPort(GPIOA)) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    if (checkPort(GPIOB)) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
    if (checkPort(GPIOC)) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    }
}

inline void initIOPort() {
    checkPorts();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Pin = CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RESET_PIN;
    GPIO_Init(RESET_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RS_PIN;
    GPIO_Init(RS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = WR_PIN;
    GPIO_Init(WR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RD_PIN;
    GPIO_Init(RD_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = D0_PIN;
    GPIO_Init(D0_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D1_PIN;
    GPIO_Init(D1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D2_PIN;
    GPIO_Init(D2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D3_PIN;
    GPIO_Init(D3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D4_PIN;
    GPIO_Init(D4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D5_PIN;
    GPIO_Init(D5_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D6_PIN;
    GPIO_Init(D6_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D7_PIN;
    GPIO_Init(D7_PORT, &GPIO_InitStructure);

    GPIO_SetBits(CS_PORT, CS_PIN);
    GPIO_SetBits(RESET_PORT, RESET_PIN);
    GPIO_SetBits(RS_PORT, RS_PIN);
    GPIO_SetBits(WR_PORT, WR_PIN);
    GPIO_SetBits(RD_PORT, RD_PIN);
    GPIO_ResetBits(D0_PORT, D0_PIN);
    GPIO_ResetBits(D1_PORT, D1_PIN);
    GPIO_ResetBits(D2_PORT, D2_PIN);
    GPIO_ResetBits(D3_PORT, D3_PIN);
    GPIO_ResetBits(D4_PORT, D4_PIN);
    GPIO_ResetBits(D5_PORT, D5_PIN);
    GPIO_ResetBits(D6_PORT, D6_PIN);
    GPIO_ResetBits(D7_PORT, D7_PIN);
}

inline void dataPortToWrite() {
    checkPorts();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = D0_PIN;
    GPIO_Init(D0_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D1_PIN;
    GPIO_Init(D1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D2_PIN;
    GPIO_Init(D2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D3_PIN;
    GPIO_Init(D3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D4_PIN;
    GPIO_Init(D4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D5_PIN;
    GPIO_Init(D5_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D6_PIN;
    GPIO_Init(D6_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D7_PIN;
    GPIO_Init(D7_PORT, &GPIO_InitStructure);
}

inline void dataPortToRead() {
    checkPorts();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = D0_PIN;
    GPIO_Init(D0_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D1_PIN;
    GPIO_Init(D1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D2_PIN;
    GPIO_Init(D2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D3_PIN;
    GPIO_Init(D3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D4_PIN;
    GPIO_Init(D4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D5_PIN;
    GPIO_Init(D5_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D6_PIN;
    GPIO_Init(D6_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = D7_PIN;
    GPIO_Init(D7_PORT, &GPIO_InitStructure);
}

static constexpr bool isData0OnPort(GPIO_TypeDef * port) {
    return D0_PORT == port;
}
static constexpr bool isData1OnPort(GPIO_TypeDef * port) {
    return D1_PORT == port;
}
static constexpr bool isData2OnPort(GPIO_TypeDef * port) {
    return D2_PORT == port;
}
static constexpr bool isData3OnPort(GPIO_TypeDef * port) {
    return D3_PORT == port;
}
static constexpr bool isData4OnPort(GPIO_TypeDef * port) {
    return D4_PORT == port;
}
static constexpr bool isData5OnPort(GPIO_TypeDef * port) {
    return D5_PORT == port;
}
static constexpr bool isData6OnPort(GPIO_TypeDef * port) {
    return D6_PORT == port;
}
static constexpr bool isData7OnPort(GPIO_TypeDef * port) {
    return D7_PORT == port;
}

// Empty function if any data are on the given port
inline void setDataOnPort(uint8_t data, GPIO_TypeDef * port) {
    if (countDataOnPort(port) > 0) {
        if (isData0OnPort(port)) {
            if (data & 1) {
                GPIO_SetBits(port, D0_PIN);
            } else {
                GPIO_ResetBits(port, D0_PIN);
            }
        }
        if (isData1OnPort(port)) {
            if (data & 2) {
                GPIO_SetBits(port, D1_PIN);
            } else {
                GPIO_ResetBits(port, D1_PIN);
            }
        }
        if (isData2OnPort(port)) {
            if (data & 4) {
                GPIO_SetBits(port, D2_PIN);
            } else {
                GPIO_ResetBits(port, D2_PIN);
            }
        }
        if (isData3OnPort(port)) {
            if (data & 8) {
                GPIO_SetBits(port, D3_PIN);
            } else {
                GPIO_ResetBits(port, D3_PIN);
            }
        }
        if (isData4OnPort(port)) {
            if (data & 16) {
                GPIO_SetBits(port, D4_PIN);
            } else {
                GPIO_ResetBits(port, D4_PIN);
            }
        }
        if (isData5OnPort(port)) {
            if (data & 32) {
                GPIO_SetBits(port, D5_PIN);
            } else {
                GPIO_ResetBits(port, D5_PIN);
            }
        }
        if (isData6OnPort(port)) {
            if (data & 64) {
                GPIO_SetBits(port, D6_PIN);
            } else {
                GPIO_ResetBits(port, D6_PIN);
            }
        }
        if (isData7OnPort(port)) {
            if (data & 128) {
                GPIO_SetBits(port, D7_PIN);
            } else {
                GPIO_ResetBits(port, D7_PIN);
            }
        }
    }
}

inline void setData(uint8_t data) {
    setDataOnPort(data, GPIOA);
    setDataOnPort(data, GPIOB);
    setDataOnPort(data, GPIOC);
}

inline uint8_t readDataOnPort(uint16_t io, GPIO_TypeDef * port ) {
    uint16_t data = 0;
    if (isData0OnPort (port)) {
        data |= (io & D0_PIN) ? 1 : 0;
    }
    if (isData1OnPort (port)) {
        data |= (io & D1_PIN) ? 0x02 : 0;
    }
    if (isData2OnPort (port)) {
        data |= (io & D2_PIN) ? 0x04 : 0;
    }
    if (isData3OnPort (port)) {
        data |= (io & D3_PIN) ? 0x08 : 0;
    }
    if (isData4OnPort (port)) {
        data |= (io & D4_PIN) ? 0x10 : 0;
    }
    if (isData5OnPort (port)) {
        data |= (io & D5_PIN) ? 0x20 : 0;
    }
    if (isData6OnPort (port)) {
        data |= (io & D6_PIN) ? 0x40 : 0;
    }
    if (isData7OnPort (port)) {
        data |= (io & D7_PIN) ? 0x80 : 0;
    }
    return data;
}

inline uint8_t readData() {
    uint16_t data = 0;
    uint16_t portA = 0;
    uint16_t portB = 0;
    uint16_t portC = 0;
    if (countDataOnPort(GPIOA)) {
        portA = GPIOA->IDR;
    }
    if (countDataOnPort(GPIOB)) {
        portB = GPIOB->IDR;
    }
    if (countDataOnPort(GPIOC)) {
        portC = GPIOC->IDR;
    }
    if (countDataOnPort(GPIOA)) {
        data |= readDataOnPort(portA, GPIOA);
    }
    if (countDataOnPort(GPIOB)) {
        data |= readDataOnPort(portB, GPIOB);
    }
    if (countDataOnPort(GPIOC)) {
        data |= readDataOnPort(portC, GPIOC);
    }
    return data;
}

inline void resetOn() {
    RESET_PORT->BRR=RESET_PIN;
}

inline void resetOff() {
    RESET_PORT->BSRR=RESET_PIN;
}

inline void activeWR() {
    WR_PORT->BRR=WR_PIN;
}

inline void idleWR() {
    WR_PORT->BSRR=WR_PIN;
}

inline void activeRD() {
    RD_PORT->BRR=RD_PIN;
}

inline void idleRD() {
    RD_PORT->BSRR=RD_PIN;
}

inline void activeCS() {
    CS_PORT->BRR=CS_PIN;
}

inline void idleCS() {
    CS_PORT->BSRR=CS_PIN;
}

inline void command() {
    RS_PORT->BRR=RS_PIN;
}

inline void data() {
    RS_PORT->BSRR=RS_PIN;
}

inline void strobeWR() {
    activeWR();
// Wait for 70 ns
    __asm(
            "nop\n" // 14 ns at 70 Mhz
            "nop\n"// 14 ns at 70 Mhz
            "nop\n"// 14 ns at 70 Mhz
            "nop\n"// 14 ns at 70 Mhz
            "nop\n"// 14 ns at 70 Mhz
    );
    idleWR();
}

inline void write(uint8_t value) {
    setData(value);
    strobeWR();
}




#endif /* BASEACCCESS_H_ */
