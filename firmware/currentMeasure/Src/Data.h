//
// Created by paolo on 05/03/18.
//

#ifndef CURRENTMEASURE_DATA_H
#define CURRENTMEASURE_DATA_H

const constexpr uint DATA_SIZE=480;

class Data {
private:
    uint16_t * iter;
public:
    static uint32_t totalTime;
    static uint32_t total;
    static uint32_t totalMinute;
    static uint32_t  totalMinuteTmp;
    static uint16_t ms;
    static bool  newTotalMinute;
    uint16_t data[DATA_SIZE];
    static uint16_t RxBuffer[3];
    static uint8_t rxPos;
    static bool triggerOn;
    static uint16_t triggerLevel;
    bool working;
    static uint32_t count;
    static uint32_t tmpData[10];
    static uint32_t countTempData;

    static void rxISR(struct __SPI_HandleTypeDef *hspi);
    static uint16_t getResultHigh() {
        auto *buffer = reinterpret_cast<uint8_t *>(RxBuffer);
        return (((uint16_t) buffer[0]) << 4) + (((uint16_t) buffer[3]) >> 4);
    }

    static uint16_t getResultLow() {
        auto *buffer = reinterpret_cast<uint8_t *>(RxBuffer);
        return (((uint16_t) buffer[5]) << 4) + (((uint16_t) buffer[4]) >> 4);
    }

    static uint16_t getResult() {
        auto *buffer = reinterpret_cast<uint8_t *>(RxBuffer);
        return (((uint16_t) buffer[5]) << 4) + (((uint16_t) buffer[4]) >> 4);
    }
    void init();
    void resetTotal();
    void start();
    void stop();
    void insertData(uint16_t value){
        if (working && iter < data + DATA_SIZE){
            *iter = value;
            iter++;
            if (iter == data + DATA_SIZE)
                working=false;
        }
    }

    friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
private:
    static void startAcquire();
};

extern Data dataInstance;

#endif //CURRENTMEASURE_DATA_H
