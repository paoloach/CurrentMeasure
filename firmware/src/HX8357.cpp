/*
 * HX8357.cpp
 *
 *  Created on: 24 mag 2017
 *      Author: paolo
 */

#include "HX8357.h"
#include "Timer.h"

namespace HX8357 {

#ifdef USR_FSMC
volatile uint16_t * LCD_REG = (uint16_t *) 0x60000000;
volatile uint16_t * LCD_DATA = (uint16_t *) 0x60080000;
#endif


HX8357::HX8357(LcdID lcd) {
    driver = lcd;
    init();
}

void HX8357::init() {
    CS_PORT->BSRR=CS_PIN;
    RESET_PORT->BSRR = RESET_PIN;
    Timer::sleep(2);
    RESET_PORT->BSRR = RESET_PIN << 16;
    Timer::sleep(200);
    RESET_PORT->BSRR = RESET_PIN;
//    trace_printf("FSMC:\n");
//    trace_printf(" GPIOB->MODER = %08X\n", GPIOB->MODER);
//    trace_printf(" GPIOB->OTYPER = %08X\n", GPIOB->OTYPER);
//    trace_printf(" GPIOB->OSPEEDR = %08X\n", GPIOB->OSPEEDR);
//    trace_printf(" GPIOB->PUPDR = %08X\n", GPIOB->PUPDR);
//    trace_printf(" GPIOB->LCKR = %08X\n", GPIOB->LCKR);
//    trace_printf(" GPIOB->AFR[0] = %08X\n", GPIOB->AFR[0]);
//    trace_printf(" GPIOB->AFR[1] = %08X\n", GPIOB->AFR[1]);


//    trace_printf ("FSMC:\n");
//    trace_printf (" GPIOD->MODER = %08X\n",GPIOD->MODER);
//    trace_printf (" GPIOD->OTYPER = %08X\n",GPIOD->OTYPER);
//    trace_printf (" GPIOD->OSPEEDR = %08X\n",GPIOD->OSPEEDR);
//    trace_printf (" GPIOD->PUPDR = %08X\n",GPIOD->PUPDR);
//    trace_printf (" GPIOD->LCKR = %08X\n",GPIOD->LCKR);
//    trace_printf (" GPIOD->AFR[0] = %08X\n",GPIOD->AFR[0]);
//    trace_printf (" GPIOD->AFR[1] = %08X\n",GPIOD->AFR[1]);
//
//    trace_printf (" GPIOE->MODER = %08X\n",GPIOE->MODER);
//    trace_printf (" GPIOE->OTYPER = %08X\n",GPIOE->OTYPER);
//    trace_printf (" GPIOE->OSPEEDR = %08X\n",GPIOE->OSPEEDR);
//    trace_printf (" GPIOE->PUPDR = %08X\n",GPIOE->PUPDR);
//    trace_printf (" GPIOE->LCKR = %08X\n",GPIOE->LCKR);
//    trace_printf (" GPIOE->AFR[0] = %08X\n",GPIOE->AFR[0]);
//    trace_printf (" GPIOE->AFR[1] = %08X\n",GPIOE->AFR[1]);

//    while (true) {
//        for (int i = 0; i < 256; i++) {
//            *LCD_REG = i;
//            for (int j = 0; j < 5; j++)
//                __asm(
//                        "nop\n"
//
//                );
//            *LCD_DATA = 0xAAAA;
//            for (int j = 0; j < 15; j++)
//                __asm(
//                        "nop\n"
//
//                );
//            *LCD_DATA = 0xAAAA;
//            for (int j = 0; j < 15; j++)
//                __asm(
//                        "nop\n"
//
//                );
//
//        }
//    }


    if (driver == LcdID::ID_HX8357B) {

        SleepOut::apply();
        Timer::sleep(120);
        SetEq::writeData( { 0x02, 0x01, 0x02, 0x01 });
        SetDirTIM::writeData( { 0x00, 0x00, 0x9A, 0x9A, 0x9B, 0x9B, 0x00, 0x00, 0x00, 0x00, 0xAE, 0xAE, 0x01, 0xA2, 0x00 });
        SetDisplayMode_B::writeData(0x0);
        SetPanel::writeData( { 0x00, 0x3B, 0x00, 0x02, 0x11 });
        SetNorTime::writeData( { 0x10 });
        SetGamma::writeData( { 0x00, 0x46, 0x12, 0x20, 0x0C, 0x00, 0x56, 0x12, 0x67, 0x02, 0x00, 0x0C });
        SetPower_B::writeData( { 0x44, 0x42, 0x06 });
        SetVCOM_B::writeData( { 0x43, 0x16 });
        SetNorPow::writeData( { 0x04, 0x22 });
        SetParPow::writeData( { 0x04, 0x12 });
        SetIdlePow::writeData( { 0x07, 0x12 });
        SetPanelRelated::writeData(0x00);
        MadCtl::writeData(0x0A);
        ColMod::writeData(0x55);
        NormalMode::apply();
        ExitIdleMode::apply();
        DisplayInversionOn::apply();
    }

    if (driver == LcdID::ID_HX8357C) {
        SetExtCmd_D::writeData( { 0xFF, 0x83, 0x57 });
        Timer::sleep(50);
        SetVCOM_C::writeData(0x20);
        SleepOut::apply();
        Timer::sleep(200);
        TearingOn::apply();
        ColMod::writeData(0X0055);
//        SetPower_C::writeData( { 0x00, 0x15, 0x0D, 0x0D, 0x83, 0x48 });
//        SetPanel_C::writeData( { 0x24, 0x24, 0x01, 0x3C, 0xC8, 0x08 });
        SetDisplayMode_C::writeData( { 0x02, 0x40, 0x00, 0x2A, 0x2A, 0x0D, 0x4F });
        SetGamma_D::writeData( { 0x00, 0x15, 0x1D, 0x2A, 0x31, 0x42, 0x4C, 0x53, 0x45, 0x40, 0x3B, 0x32, 0x2E, 0x28, 0x24, 0x03, 0x00, 0x15, 0x1D, 0x2A, 0x31, 0x42, 0x4C, 0x53, 0x45, 0x40, 0x3B, 0x32,
                0x2E, 0x28, 0x24, 0x03, 0x00, 0x01 });
        MadCtl::writeData(0x48);
        Timer::sleep(20);
        DisplayInversionOn::apply();
        Timer::sleep(20);
    }

    if (driver == LcdID::ID_HX8357D) {
        SetExtCmd_D::writeData( { 0xFF, 0x83, 0x57 });
        Timer::sleep(50);
        SetVCOM_D::writeData(0x25);
        DisplayOff::apply();
        ColMod::writeData(0x55);


        SetOsc_D::writeData( { 0x68, 0x01 });
        SetPanel_D::writeData(5);
        SetPower_D::writeData({0,0x15,0x1C,0x1C, 0x83,0xAA,0x29});
 //       SetSTBA::writeData({0x50,0x50,0x01,0x3C, 0x1E,0x08});
 //       SetCYC::writeData({0x02,0x40,0x00,0x2A,0x2A, 0x0D, 0x78});
        SetGamma_D::writeData( { 0x02, 0x0A, 0x11, 0x1D, 0x23,
                                 0x35, 0x41, 0x4b, 0x4B, 0x42,
                                 0x3A, 0x27, 0x1B, 0x08, 0x09,
                                 0x03, 0x02, 0x0A, 0x11, 0x1D,
                                 0x23, 0x35, 0x41, 0x4B, 0x4B,
                                 0x42, 0x3A,  0x27, 0x1B, 0x08,
                                 0x09, 0x03, 0x00, 0x01});
        SetTearLine::writeData({0x0,0x02});
        Timer::sleep(50);

        SleepOut::apply();
        Timer::sleep(200);
    }

    DisplayOn::apply();
    Timer::sleep(200);
    CaSet::writeData(0x00, DEFAULT_WIDTH - 1);
    PaSet::writeData(0x00, DEFAULT_HEIGTH - 1);
    flood(Color16Bit(255, 255, 255), DEFAULT_HEIGTH * DEFAULT_WIDTH);

    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGTH;
}

void HX8357::drawPixel(Point point, Color16Bit color) {
    if (point.x < 0 || point.x >= height|| point.y < 0 || point.y >= width)
        return;
    point.y = height-point.y;

    CaSet::writeData(point.y, point.y);
    PaSet::writeData(point.x, point.x);
    RamWR::writeData(color.color);

}
void HX8357::drawPixelInternal(Point && point, Color16Bit && color) {
    point.y = height-point.y;
    CaSet::writeData(point.x, point.y);
    PaSet::writeData(point.y, point.x);
    RamWR::writeData(color.color);
}

void HX8357::drawPixelInternal(uint16_t y, uint16_t x, Color16Bit color) {
    y=height-y;
    CaSet::writeData(x, x);
    PaSet::writeData(y, y);
    RamWR::writeData(color.color);
}


void HX8357::drawFastSample(uint16_t x, uint16_t value) {

    x = DEFAULT_HEIGTH-x;
    CaSet::writeData(20, DEFAULT_WIDTH-1);
    PaSet::writeData(x, x);

    CS_PORT->BSRR = CS_PIN << 16;
    RegisterBase::writeIndex(static_cast<uint16_t>(REG::RAMWR));
    RS_PORT->BSRR = RS_PIN;
    auto len=300-value;
    for (int i = 0; i < len; i++)
        RegisterBase::writeDataByte(WHITE.color);
    for (int i = 0; i < value; i++)
        RegisterBase::writeDataByte(BLACK.color);
}


void HX8357::setDriverQuality(DriverQuality driverQuality) {

}
void HX8357::setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) {
    if (left >= width || top >= height || right >= width || bottom >= height)
        return;
    CaSet::writeData(left, right);
    PaSet::writeData(top, bottom);
}
void HX8357::flood(Color16Bit color, uint32_t len) {
    RamWR::writeData(color.color, len);
}

void CaSet::writeData(uint16_t start, uint16_t end) {
    CS_PORT->BSRR = CS_PIN << 16;
    RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(REG::CASET)));
    RS_PORT->BSRR = RS_PIN;
    writeDataByte(start >> 8);
    writeDataByte(start & 0xFF);
    writeDataByte(end >> 8);
    writeDataByte(end & 0xFF);

}

void PaSet::writeData(uint16_t start, uint16_t end) {
    CS_PORT->BSRR = CS_PIN << 16;
    RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(REG::PASET)));
    RS_PORT->BSRR = RS_PIN;
    writeDataByte(start >> 8);
    writeDataByte(start & 0xFF);
    writeDataByte(end >> 8);
    writeDataByte(end & 0xFF);

}

}

