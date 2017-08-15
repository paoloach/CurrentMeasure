/*
 * BaseAcccess.h
 *
 *  Created on: 18 gen 2017
 *      Author: paolo
 */

#ifndef BASEACCCESS_H_
#define BASEACCCESS_H_

#include "Pins.h"
#include "stm32f4xx_hal_sram.h"
#include "diag/Trace.h"

static constexpr bool checkPort(GPIO_TypeDef * port) {
    return CS_PORT == port || RS_PORT == port || WR_PORT == port || D0_PORT == port || D1_PORT == port || D2_PORT == port || D3_PORT == port || D4_PORT == port || D5_PORT == port || D6_PORT == port
            || D7_PORT == port || D8_PORT == port || D9_PORT == port || D10_PORT == port || D11_PORT == port || D12_PORT == port || D13_PORT == port || D14_PORT == port || D15_PORT == port;
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

#ifdef USE_FSMC
inline void initFSMC() {
    SRAM_HandleTypeDef sram_init_struct;
    FSMC_NORSRAM_TimingTypeDef readWriteTiming;
    FSMC_NORSRAM_TimingTypeDef writeTiming;

    readWriteTiming.AddressSetupTime = 1;     //HCLK 1/168M=6ns*16=96ns
    readWriteTiming.AddressHoldTime = 0x1;
    readWriteTiming.DataSetupTime = 1;         //HCLK  =6*60=360ns
    readWriteTiming.BusTurnAroundDuration = 0x00;
    readWriteTiming.CLKDivision = 16;
    readWriteTiming.DataLatency = 0x02;
    readWriteTiming.AccessMode = FSMC_ACCESS_MODE_A;

    writeTiming.AddressSetupTime = 1;     //12ns
    writeTiming.AddressHoldTime = 0x01;
    writeTiming.DataSetupTime = 100;       //HCLK=102ns
    writeTiming.BusTurnAroundDuration = 0x00;
    writeTiming.CLKDivision = 0x01;
    writeTiming.DataLatency = 0x02;
    writeTiming.AccessMode = FSMC_ACCESS_MODE_A;


    sram_init_struct.Instance = FSMC_NORSRAM_DEVICE;
    sram_init_struct.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    sram_init_struct.Init.NSBank             = FSMC_NORSRAM_BANK1;
    sram_init_struct.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    sram_init_struct.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    sram_init_struct.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    sram_init_struct.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    sram_init_struct.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    sram_init_struct.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    sram_init_struct.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    sram_init_struct.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    sram_init_struct.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    sram_init_struct.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE; // maybe enable?
    sram_init_struct.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    sram_init_struct.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
    //sram_init_struct.Init.PageSize           = FSMC_PAGE_SIZE_128;
    sram_init_struct.Init.PageSize           = FSMC_PAGE_SIZE_NONE;

    __HAL_RCC_FSMC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_SRAM_Init(&sram_init_struct, &readWriteTiming, &writeTiming);

    FSMC_Bank1->BTCR[FSMC_NORSRAM_BANK1] |= 1;
}

inline void initIOPort() {
    checkPorts();
    initFSMC();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = CS_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStructure.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = RS_PIN;
    HAL_GPIO_Init(RS_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

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


}
# else
inline void initIOPort() {
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
// Configure pin in output push/pull mode
    GPIO_InitStructure.Pin = CS_PIN | RESET_PIN | RS_PIN | WR_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull=GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = D0_PIN | D1_PIN | D2_PIN |D3_PIN |D4_PIN |D5_PIN |D6_PIN |D7_PIN | D8_PIN | D9_PIN | D10_PIN |D11_PIN |D12_PIN |D13_PIN |D14_PIN |D15_PIN ;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

}

inline void setData(uint16_t data) {
    GPIOE->ODR=data;
}
#endif



#endif /* BASEACCCESS_H_ */
