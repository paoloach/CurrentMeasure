/*
 * HX8357.cpp
 *
 *  Created on: 24 mag 2017
 *      Author: paolo
 */

#include "HX8357.h"
#include "Timer.h"

namespace HX8357 {

HX8357::HX8357(LcdID lcd) {
    driver = lcd;
    init();
}

void HX8357::init() {
    reset();
    if (driver == LcdID::ID_HX8357B) {
//         SleepOut::apply();
//         Timer::sleep(20);
//         SetPower_B::writeData( { 0x07, 0x42, 0x18 });
//         SetVCOM_B::writeData( { 0x00, 0x07, 0x10 });
//         SetNorPow::writeData( { 0x01, 0x02 });
//         SetPanel::writeData( { 0x10, 0x3B, 0x00, 0x02, 0x11 });
//         SetOsc::writeData(0x08);
//         SetGamma::writeData( { 0x00, 0x32, 0x36, 0x45, 0x06, 0x16, 0x37, 0x75, 0x77, 0x54, 0x0C, 00 });
//         MadCtl::writeData(0x0A);
//         ColMod::writeData(0x55);
//
//         Timer::sleep(120);
//         DisplayOn::apply();
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
        DisplayOff::apply();
        ColMod::writeData(0x55);

        SetOsc_D::writeData( { 0x66, 0x01 });
        Timer::sleep(50);

        SleepOut::apply();
        Timer::sleep(200);
    }

    DisplayOn::apply();
    Timer::sleep(200);
    CaSet::writeData(0x00, DEFAULT_WIDTH - 1);
    PaSet::writeData(0x00, DEFAULT_HEIGTH - 1);
    Timer::sleep(120);


    flood(Color16Bit(127, 0, 0), DEFAULT_HEIGTH*DEFAULT_WIDTH );

    width = DEFAULT_WIDTH;
    height = DEFAULT_HEIGTH;
}

void HX8357::drawPixel(Point point, Color16Bit color) {
    if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
        return;
    CaSet::writeData(point.x, point.x);
    PaSet::writeData(point.y, point.y);
    RamWR::writeData(color.color);

}
void HX8357::drawPixelInternal(Point && point, Color16Bit && color) {
    CaSet::writeData(point.x, point.x);
    PaSet::writeData(point.y, point.y);
    RamWR::writeData(color.color);
}
void HX8357::setDriverQuality(DriverQuality driverQuality) {

}
void HX8357::reset() {
    CS_PORT->BRR = CS_PIN;
    RESET_PORT->BRR = RESET_PIN;
    Timer::sleep(20);
    RESET_PORT->BSRR = RESET_PIN;
    Timer::sleep(20);
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
    CS_PORT->BRR = CS_PIN;
    RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(REG::CASET)));
    RS_PORT->BSRR = RS_PIN;
    writeDataByte(start >> 8);
    writeDataByte(start & 0xFF);
    writeDataByte(end >> 8);
    writeDataByte(end & 0xFF);

}

void PaSet::writeData(uint16_t start, uint16_t end) {
    CS_PORT->BRR = CS_PIN;
    RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(REG::PASET)));
    RS_PORT->BSRR = RS_PIN;
    writeDataByte(start >> 8);
    writeDataByte(start & 0xFF);
    writeDataByte(end >> 8);
    writeDataByte(end & 0xFF);

}

}

