//
// Created by paolo on 05/03/18.
//

#include "stm32f4xx_hal.h"
#include "Data.h"

constexpr auto AD_CS_PIN = GPIO_PIN_8;

Data dataInstance;
uint16_t Data::RxBuffer[3];
uint8_t Data::rxPos;
uint32_t Data::totalTime;
uint32_t Data::total;
uint32_t Data::totalMinute;
uint32_t Data::totalMinuteTmp;
uint16_t Data::ms;
bool Data::newTotalMinute;
bool Data::triggerOn=false;
uint16_t Data::triggerLevel=15;
uint32_t Data::count;
uint32_t Data::tmpData[10];
uint32_t Data::countTempData=0;


extern TIM_HandleTypeDef htim2;
extern SPI_HandleTypeDef hspi1;

void Data::start() {
    iter=data;
    working = true;
    HAL_TIM_Base_Start_IT(&htim2);
}

void Data::stop() {
    HAL_TIM_Base_Stop_IT(&htim2);
}

void Data::resetTotal() {
    total=0;
}

void Data::init() {
    hspi1.RxISR = rxISR;
}

void Data::rxISR(struct __SPI_HandleTypeDef *hspi) {
    volatile uint16_t dr = SPI1->DR;
    RxBuffer[rxPos] = dr;
    if (rxPos == 0) {
        SPI1->DR = 0x00D4;
        rxPos++;
    } else if (rxPos == 1) {
        SPI1->DR = 0x0000;
        rxPos++;
    } else if (rxPos == 2) {
        GPIOB->BSRR = AD_CS_PIN;
        uint16_t result = getResult();
        tmpData[countTempData] = result;
        countTempData++;
        if (countTempData == 10) {
            uint32_t mean=0;
            for(int i=0; i < 10; i++)
                 mean += tmpData[i];
            mean = mean/10;
            total += mean;
            totalMinuteTmp += mean;
            dataInstance.insertData(mean);
            count = htim2.Instance->CNT;
        } else{
            startAcquire();
        }
    }
}

void Data::startAcquire() {
    SPI1->CR1 |= SPI_CR1_SPE;
    GPIOB->BSRR = AD_CS_PIN << 16;
    rxPos = 0;
    SPI1->DR = 0x9400;
    SPI1->CR2 = SPI_CR2_RXNEIE;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (dataInstance.working) {
        Data::countTempData=0;
        Data::startAcquire();
    }
    Data::ms++;
    Data::totalTime++;
    if (Data::ms >= 10000){
        Data::totalMinute=Data::totalMinuteTmp;
        Data::totalMinuteTmp=0;
        Data::newTotalMinute=true;
        Data::ms = 0;
    }
}