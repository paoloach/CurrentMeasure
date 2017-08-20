/*
 * ADS7841.cpp
 *
 *  Created on: 29 mag 2017
 *      Author: paolo
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "ADS7841.h"
#include "Pins.h"
#include "diag/Trace.h"

SPI_HandleTypeDef ADS7841::handle;
uint8_t ADS7841::TxBuffer[6];
uint8_t ADS7841::RxBuffer[6];
DMA_HandleTypeDef ADS7841::txDMA;
DMA_HandleTypeDef ADS7841::rxDMA;
uint16_t ADS7841::resultHigh;
uint16_t ADS7841::resultLow;
bool ADS7841::resultPresent;

int line;

void debugDMA() {
    uint32_t dma2_stream0_ndtr = DMA2_Stream0->NDTR;
    uint32_t dma2_stream0_cr = DMA2_Stream0->CR;
    uint32_t dma2_stream3_ndtr = DMA2_Stream3->NDTR;
    uint32_t dma2_stream3_cr = DMA2_Stream3->CR;

    trace_printf("DMA2_Stram0_CR==%08X\n", dma2_stream0_cr);
    trace_printf("DMA2_Stram0_FCR==%08X\n", DMA2_Stream0->FCR);
    trace_printf("DMA2_Stram0_M0AR==%08X\n", DMA2_Stream0->M0AR);
    trace_printf("DMA2_Stram0_M1AR==%08X\n", DMA2_Stream0->M1AR);
    trace_printf("DMA2_Stram0_NDTR==%08X\n", dma2_stream0_ndtr);
    trace_printf("DMA2_Stram0_PAR==%08X\n", DMA2_Stream0->PAR);

    trace_printf("DMA2_Stram3_CR==%08X\n", dma2_stream3_cr);
    trace_printf("DMA2_Stram3_FCR==%08X\n", DMA2_Stream3->FCR);
    trace_printf("DMA2_Stram3_M0AR==%08X\n", DMA2_Stream3->M0AR);
    trace_printf("DMA2_Stram3_M1AR==%08X\n", DMA2_Stream3->M1AR);
    trace_printf("DMA2_Stram3_NDTR==%08X\n", dma2_stream3_ndtr);
    trace_printf("DMA2_Stram3_PAR==%08X\n", DMA2_Stream3->PAR);
}

ADS7841::ADS7841() {
    init();
}

void ADS7841::init() {
    __HAL_RCC_SPI1_CLK_ENABLE()
    ;
    __HAL_RCC_GPIOB_CLK_ENABLE()
    ;
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = AD_SERIAL_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(AD_SERIAL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = AD_IN_PIN;
    HAL_GPIO_Init(AD_IN_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = AD_OUT_PIN;
    HAL_GPIO_Init(AD_OUT_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = AD_CS_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(AD_CS_PORT, &GPIO_InitStructure);

    handle.Instance = SPI1;
    handle.Init.Mode = SPI_MODE_MASTER;
    handle.Init.Direction = SPI_DIRECTION_2LINES;
    handle.Init.DataSize = SPI_DATASIZE_8BIT;
    handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    handle.Init.NSS = SPI_NSS_SOFT;
    handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    handle.Init.TIMode = SPI_TIMODE_DISABLE;
    handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    __HAL_RCC_DMA2_CLK_ENABLE()
    ;

    handle.hdmatx = &txDMA;
    handle.hdmarx = &rxDMA;
    txDMA.Init.Channel = DMA_CHANNEL_3;
    txDMA.Init.Direction = DMA_MEMORY_TO_PERIPH;
    txDMA.Init.PeriphInc = DMA_PINC_DISABLE;
    txDMA.Init.MemInc = DMA_MINC_ENABLE;
    txDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    txDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    txDMA.Init.Mode = DMA_NORMAL;
    txDMA.Init.Priority = DMA_PRIORITY_MEDIUM;
    txDMA.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    txDMA.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    txDMA.Init.MemBurst = DMA_MBURST_SINGLE;
    txDMA.Init.PeriphBurst = DMA_PBURST_SINGLE;
    txDMA.Instance = DMA2_Stream3;

    rxDMA.Init.Channel = DMA_CHANNEL_3;
    rxDMA.Init.Direction = DMA_PERIPH_TO_MEMORY;
    rxDMA.Init.PeriphInc = DMA_PINC_DISABLE;
    rxDMA.Init.MemInc = DMA_MINC_ENABLE;
    rxDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    rxDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    rxDMA.Init.Mode = DMA_NORMAL;
    rxDMA.Init.Priority = DMA_PRIORITY_MEDIUM;
    rxDMA.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    rxDMA.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    rxDMA.Init.MemBurst = DMA_MBURST_SINGLE;
    rxDMA.Init.PeriphBurst = DMA_PBURST_SINGLE;
    rxDMA.Instance = DMA2_Stream0;

    TxBuffer[0] = 0x96;
    TxBuffer[1] = 0;
    TxBuffer[2] = 0x00;
    TxBuffer[3] = 0xD4;
    TxBuffer[4] = 0;
    TxBuffer[5] = 0x00;

    HAL_DMA_Init(&rxDMA);
    HAL_DMA_Init(&txDMA);

    rxDMA.Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
    txDMA.Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    HAL_SPI_Init(&handle);
    SPI1->CR1 |= SPI_CR1_SPE;
    AD_CS_PORT->BSRR = AD_CS_PIN;
}

void ADS7841::get() {
    AD_CS_PORT->BSRR = AD_CS_PIN << 16;
    rxDMA.Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
    txDMA.Instance->CR |= DMA_IT_TC | DMA_IT_TE | DMA_IT_DME;
    rxDMA.Instance->NDTR = 6;
    txDMA.Instance->NDTR = 6;

    rxDMA.Instance->PAR = (uint32_t) (&handle.Instance->DR);
    txDMA.Instance->PAR = (uint32_t) (&handle.Instance->DR);
    rxDMA.Instance->M0AR = (uint32_t) RxBuffer;
    txDMA.Instance->M0AR = (uint32_t) TxBuffer;
    rxDMA.Instance->CR |= DMA_SxCR_EN;
    txDMA.Instance->CR |= DMA_SxCR_EN;

    /* Enable Tx DMA Request */
    SET_BIT(handle.Instance->CR2, SPI_CR2_TXDMAEN);
    SET_BIT(handle.Instance->CR2, SPI_CR2_RXDMAEN);
}

extern "C" {
// RX
void DMA2_Stream0_IRQHandler(void) {
    if (DMA2->LISR & DMA_FLAG_DMEIF0_4) {
        DMA2->LIFCR = DMA_FLAG_DMEIF0_4;
    } else if (DMA2->LISR & DMA_FLAG_FEIF0_4) {
        DMA2->LIFCR = DMA_FLAG_FEIF0_4;
    } else if (DMA2->LISR & DMA_FLAG_TEIF0_4) {
        DMA2->LIFCR = DMA_FLAG_TEIF0_4;
    } else if (DMA2->LISR & DMA_FLAG_HTIF0_4) {
        DMA2->LIFCR = DMA_FLAG_HTIF0_4;
    } else if (DMA2->LISR & DMA_FLAG_TCIF0_4) {
        DMA2->LIFCR = DMA_FLAG_TCIF0_4;
        AD_CS_PORT->BSRR = AD_CS_PIN;
        ADS7841::resultHigh = (((uint16_t) ADS7841::RxBuffer[1]) << 4) + (((uint16_t) ADS7841::RxBuffer[2]) >> 4);
        ADS7841::resultLow = (((uint16_t) ADS7841::RxBuffer[4]) << 4) + (((uint16_t) ADS7841::RxBuffer[5]) >> 4);
        ADS7841::resultPresent = true;
    }
}
// TX
void DMA2_Stream3_IRQHandler(void) {
    if (DMA2->LISR & DMA_FLAG_DMEIF3_7) {
        DMA2->LIFCR = DMA_FLAG_DMEIF3_7;
    } else if (DMA2->LISR & DMA_FLAG_FEIF3_7) {
        DMA2->LIFCR = DMA_FLAG_FEIF3_7;
    } else if (DMA2->LISR & DMA_FLAG_TEIF3_7) {
        DMA2->LIFCR = DMA_FLAG_TEIF3_7;
    } else if (DMA2->LISR & DMA_FLAG_HTIF3_7) {
        DMA2->LIFCR = DMA_FLAG_HTIF3_7;
    } else if (DMA2->LISR & DMA_FLAG_TCIF3_7) {
        DMA2->LIFCR = DMA_FLAG_TCIF3_7;
    }

}
}

