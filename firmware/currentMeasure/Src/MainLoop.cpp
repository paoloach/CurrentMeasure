//
// Created by paolo on 25/02/18.
//

#include <cstring>
#include <stm32f407xx.h>
#include "stm32f4xx_hal.h"
#include "MainLoop.h"
#include "HX8357.h"
#include "Data.h"
#include "Graph.h"
#include "Buttons.h"
#include "Events.h"
#include "Colors.h"

HX8357::HX8357 *gfx;
constexpr Point INSTANTANEUS_VALUE_POS(128, 0);
constexpr Point HVALUE(192, 20);
constexpr Point LVALUE(192, 40);
constexpr int64_t msToHour = 1000 * 3600;
char buffer[20];

extern TIM_HandleTypeDef htim2;

void mainLoopInit(void) {
    gfx = new HX8357::HX8357{LcdID::ID_HX8357D};

    gfx->setForeground(BLACK);
    gfx->setBackground(WHITE);
    gfx->setFont(&bigFont);


    dataInstance.init();
    graphInstance.clean();
    dataInstance.start();
    Buttons::init();

    gfx->setForeground(TEXT_COLOR);
    gfx->drawString(Point(0, 20), "total mA/h: ");
    gfx->drawString(Point(0, 40), "minute: ");

    Events::addTimedRecurrent(1000, [] {
                                  uint32_t consume = (Data::total * msToHour) / Data::totalTime;
                                  gfx->drawInt(Point(176, 20), consume);
                                  gfx->drawInt(Point(176, 60), Data::total);
                                  gfx->drawInt(Point(176, 80), Data::totalTime);
                              }
    );
}

void mainLoopRun(void) {

    Events::loop();
    if (!dataInstance.working) {
        graphInstance.displayGraph();
        dataInstance.start();
    }

    if (Data::newTotalMinute) {
        gfx->setForeground(TEXT_COLOR);
        gfx->drawInt(Point(128, 40), Data::totalMinute);
        Data::newTotalMinute = false;
    }

    if (Events::isButtonUp()) {
        Events::resetButtonUp();
        if (Data::triggerOn) {
            Data::triggerLevel++;
        }
    }

    if (Events::isButtonDown()) {
        Events::resetButtonDown();
        if (Data::triggerOn) {
            Data::triggerLevel--;
        }
    }

    if (Events::isButton6() && !Events::isButton5()) {
        Events::resetButton6();
        gfx->setForeground(NOTIFY_COLOR);
        if (Data::triggerOn) {
            gfx->drawString(Point(304, 0), "trigger off");
            Data::triggerOn = false;
        } else {
            gfx->drawString(Point(304, 0), "trigger on ");
            Data::triggerOn = true;
        }
        Events::addTimedEventFromNow(5000, [] {
            gfx->fillRect(Point(304, 0), 176, 16, WHITE);
        });
    }


}
