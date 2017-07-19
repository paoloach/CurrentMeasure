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
    return CS_PORT == port || RS_PORT == port || WR_PORT == port ||  D0_PORT == port || D1_PORT == port || D2_PORT == port || D3_PORT == port || D4_PORT == port || D5_PORT == port
            || D6_PORT == port || D7_PORT == port || D8_PORT == port || D9_PORT == port || D10_PORT == port || D11_PORT == port || D12_PORT == port || D13_PORT == port || D14_PORT == port
            || D15_PORT == port;
}

static constexpr int countDataOnPort(GPIO_TypeDef * port) {
    return (D0_PORT == port ? 1 : 0) + (D1_PORT == port ? 1 : 0) + (D2_PORT == port ? 1 : 0) + (D3_PORT == port ? 1 : 0) + (D4_PORT == port ? 1 : 0) + (D5_PORT == port ? 1 : 0)
            + (D6_PORT == port ? 1 : 0) + (D7_PORT == port ? 1 : 0);

}

inline void checkPorts() {
    if (checkPort(GPIOA)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    if (checkPort(GPIOB)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    if (checkPort(GPIOC)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    if (checkPort(GPIOD)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    if (checkPort(GPIOE)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
    if (checkPort(GPIOF)) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    }
}

inline void initIOPort() {
    checkPorts();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = CS_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = RESET_PIN;
    HAL_GPIO_Init(RESET_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = RS_PIN;
    HAL_GPIO_Init(RS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = WR_PIN;
    HAL_GPIO_Init(WR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = D0_PIN;
    HAL_GPIO_Init(D0_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D1_PIN;
    HAL_GPIO_Init(D1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D2_PIN;
    HAL_GPIO_Init(D2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D3_PIN;
    HAL_GPIO_Init(D3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D4_PIN;
    HAL_GPIO_Init(D4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D5_PIN;
    HAL_GPIO_Init(D5_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D6_PIN;
    HAL_GPIO_Init(D6_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D7_PIN;
    HAL_GPIO_Init(D7_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D8_PIN;
    HAL_GPIO_Init(D8_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D9_PIN;
    HAL_GPIO_Init(D9_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D10_PIN;
    HAL_GPIO_Init(D10_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D11_PIN;
    HAL_GPIO_Init(D11_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D12_PIN;
    HAL_GPIO_Init(D12_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D13_PIN;
    HAL_GPIO_Init(D13_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D14_PIN;
    HAL_GPIO_Init(D14_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D15_PIN;
    HAL_GPIO_Init(D15_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);

    HAL_GPIO_WritePin(RESET_PORT, RESET_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RS_PORT, RS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(WR_PORT, WR_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(D0_PORT, D0_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D1_PORT, D1_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D2_PORT, D2_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D3_PORT, D3_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D4_PORT, D4_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D5_PORT, D5_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D6_PORT, D6_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D7_PORT, D7_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D8_PORT, D8_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D9_PORT, D9_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D10_PORT, D10_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D11_PORT, D11_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D12_PORT, D12_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D13_PORT, D13_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D14_PORT, D14_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(D15_PORT, D15_PIN,GPIO_PIN_RESET);
}

inline void dataPortToWrite() {
    checkPorts();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin = D0_PIN;
    HAL_GPIO_Init(D0_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D1_PIN;
    HAL_GPIO_Init(D1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D2_PIN;
    HAL_GPIO_Init(D2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D3_PIN;
    HAL_GPIO_Init(D3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D4_PIN;
    HAL_GPIO_Init(D4_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D5_PIN;
    HAL_GPIO_Init(D5_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D6_PIN;
    HAL_GPIO_Init(D6_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D7_PIN;
    HAL_GPIO_Init(D7_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D8_PIN;
    HAL_GPIO_Init(D8_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D9_PIN;
    HAL_GPIO_Init(D9_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D10_PIN;
    HAL_GPIO_Init(D10_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D11_PIN;
    HAL_GPIO_Init(D11_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D12_PIN;
    HAL_GPIO_Init(D12_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D13_PIN;
    HAL_GPIO_Init(D13_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D14_PIN;
    HAL_GPIO_Init(D14_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = D15_PIN;
    HAL_GPIO_Init(D15_PORT, &GPIO_InitStructure);
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
static constexpr bool isData8OnPort(GPIO_TypeDef * port) {
    return D8_PORT == port;
}
static constexpr bool isData9OnPort(GPIO_TypeDef * port) {
    return D9_PORT == port;
}
static constexpr bool isData10OnPort(GPIO_TypeDef * port) {
    return D10_PORT == port;
}
static constexpr bool isData11OnPort(GPIO_TypeDef * port) {
    return D11_PORT == port;
}
static constexpr bool isData12OnPort(GPIO_TypeDef * port) {
    return D12_PORT == port;
}
static constexpr bool isData13OnPort(GPIO_TypeDef * port) {
    return D13_PORT == port;
}
static constexpr bool isData14OnPort(GPIO_TypeDef * port) {
    return D14_PORT == port;
}
static constexpr bool isData15OnPort(GPIO_TypeDef * port) {
    return D15_PORT == port;
}

// Empty function if any data are on the given port
inline void setDataOnPort(uint16_t data, GPIO_TypeDef * port) {
    if (countDataOnPort(port) > 0) {
        if (isData0OnPort(port)) {
            if (data & 1) {
                HAL_GPIO_WritePin(port, D0_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D0_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData1OnPort(port)) {
            if (data & 2) {
                HAL_GPIO_WritePin(port, D1_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D1_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData2OnPort(port)) {
            if (data & 4) {
                HAL_GPIO_WritePin(port, D2_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D2_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData3OnPort(port)) {
            if (data & 8) {
                HAL_GPIO_WritePin(port, D3_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D3_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData4OnPort(port)) {
            if (data & 16) {
                HAL_GPIO_WritePin(port, D4_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D4_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData5OnPort(port)) {
            if (data & 32) {
                HAL_GPIO_WritePin(port, D5_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D5_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData6OnPort(port)) {
            if (data & 64) {
                HAL_GPIO_WritePin(port, D6_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D6_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData7OnPort(port)) {
            if (data & 128) {
                HAL_GPIO_WritePin(port, D7_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D7_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData8OnPort(port)) {
            if (data & 0x100) {
                HAL_GPIO_WritePin(port, D8_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D8_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData9OnPort(port)) {
            if (data & 0x200) {
                HAL_GPIO_WritePin(port, D9_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D9_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData10OnPort(port)) {
            if (data & 0x400) {
                HAL_GPIO_WritePin(port, D10_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D10_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData11OnPort(port)) {
            if (data & 0x800) {
                HAL_GPIO_WritePin(port, D11_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D11_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData12OnPort(port)) {
            if (data & 0x1000) {
                HAL_GPIO_WritePin(port, D12_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D12_PIN, GPIO_PIN_RESET);
            }
        }
        if (isData13OnPort(port)) {
            if (data & 0x2000) {
                HAL_GPIO_WritePin(port, D13_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D13_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData14OnPort(port)) {
            if (data & 0x4000) {
                HAL_GPIO_WritePin(port, D14_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D14_PIN,GPIO_PIN_RESET);
            }
        }
        if (isData15OnPort(port)) {
            if (data & 0x8000) {
                HAL_GPIO_WritePin(port, D15_PIN, GPIO_PIN_SET);
            } else {
                HAL_GPIO_WritePin(port, D15_PIN,GPIO_PIN_RESET);
            }
        }
    }
}

inline void setData(uint16_t data) {
    setDataOnPort(data, GPIOA);
    setDataOnPort(data, GPIOB);
    setDataOnPort(data, GPIOC);
}

inline uint16_t readDataOnPort(uint16_t io, GPIO_TypeDef * port) {
    uint16_t data = 0;
    if (isData0OnPort(port)) {
        data |= (io & D0_PIN) ? 1 : 0;
    }
    if (isData1OnPort(port)) {
        data |= (io & D1_PIN) ? 0x02 : 0;
    }
    if (isData2OnPort(port)) {
        data |= (io & D2_PIN) ? 0x04 : 0;
    }
    if (isData3OnPort(port)) {
        data |= (io & D3_PIN) ? 0x08 : 0;
    }
    if (isData4OnPort(port)) {
        data |= (io & D4_PIN) ? 0x10 : 0;
    }
    if (isData5OnPort(port)) {
        data |= (io & D5_PIN) ? 0x20 : 0;
    }
    if (isData6OnPort(port)) {
        data |= (io & D6_PIN) ? 0x40 : 0;
    }
    if (isData7OnPort(port)) {
        data |= (io & D7_PIN) ? 0x80 : 0;
    }
    if (isData8OnPort(port)) {
        data |= (io & D8_PIN) ? 0x100 : 0;
    }
    if (isData9OnPort(port)) {
        data |= (io & D9_PIN) ? 0x200 : 0;
    }
    if (isData10OnPort(port)) {
        data |= (io & D10_PIN) ? 0x400 : 0;
    }
    if (isData11OnPort(port)) {
        data |= (io & D11_PIN) ? 0x800 : 0;
    }
    if (isData12OnPort(port)) {
        data |= (io & D12_PIN) ? 0x1000 : 0;
    }
    if (isData13OnPort(port)) {
        data |= (io & D13_PIN) ? 0x2000 : 0;
    }
    if (isData14OnPort(port)) {
        data |= (io & D14_PIN) ? 0x4000 : 0;
    }
    if (isData15OnPort(port)) {
        data |= (io & D15_PIN) ? 0x8000 : 0;
    }
    return data;
}

inline uint16_t readData() {
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
    RESET_PORT->BSRR = RESET_PIN << 16;
}

inline void resetOff() {
    RESET_PORT->BSRR = RESET_PIN;
}

inline void activeWR() {
    WR_PORT->BSRR = WR_PIN << 16;
}

inline void idleWR() {
    WR_PORT->BSRR = WR_PIN;
}


inline void activeCS() {
    CS_PORT->BSRR = CS_PIN << 16;
}

inline void idleCS() {
    CS_PORT->BSRR = CS_PIN;
}

inline void command() {
    RS_PORT->BSRR = RS_PIN << 16;
}

inline void data() {
    RS_PORT->BSRR = RS_PIN;
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
