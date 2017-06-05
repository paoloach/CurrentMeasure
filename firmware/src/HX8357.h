/*
 * HX8357.h
 *
 *  Created on: 24 mag 2017
 *      Author: paolo
 */

#ifndef HX8357_H_
#define HX8357_H_

#include <functional>
#include <tuple>
#include <array>
#include "GFX.h"
#include "BaseAcccess.h"

namespace HX8357 {

enum class REG {
    NOP = 0x00,
    RESET = 0x01,
    RDDIDIF = 0x04,
    SLEEPIN = 0x10,
    SLEEPOUT = 0x11,
    NORMAL_DISPLAY = 0x14,
    DISPLAY_INVERSION_OFF = 0x20,
    DISPLAY_INVERSION_ON = 0x21,
    ALL_PIXEL_OFF = 0x22,
    ALL_PIXEL_ON = 0x23,
    DISPLAY_OFF = 0x28,
    DISPLAY_ON = 0x29,
    CASET = 0x2A,
    PASET = 0x2B,
    RAMWR = 0x2C,
    TEARING_ON = 0x35,
    MADCTL = 0x36,
    ExitIdleMode=0x38,
    COLMOD = 0x3A,
    SETEXTC_B = 0xB0,
    SET_OSC_D = 0xB0,
    SET_POWER_D = 0xB1,
    SET_DISPLAY_MODE=0xB4,
    SET_VCOM_C = 0xB6,
    SET_VCOM_D = 0xB6,
    SETEXTC_D = 0xB9,
    GET_DEVICE_ID_B = 0xBF,
    SET_PANEL_B = 0xC0,
    SET_NOR_TIME_B = 0xC1,
    SET_PAR_TIME_B = 0xC2,
    SET_ILD_TIME_B = 0xC3,
    SET_OSC_B = 0xC5,
    SET_RGB_B = 0xC6,
    SET_GAMMA_B = 0xC8,
    SET_POWER_B = 0xD0,
    GET_IC_ID_D = 0xD0,
    SET_VCOM_B = 0xD1,
    SET_NOR_POW_B = 0xD2,
    SET_PART_POW_B=0xD3,
    SET_IDLE_POW_B=0xD4,
    SET_GAMMA_D=0xE0,
    SET_PANEL_RELATED=0xE9,
    SET_DIR_TIM=0xED,
    SET_EQ=0xEE
};

class RegisterBase {
protected:
    static void writeIndex(uint16_t index) {
        RS_PORT->BRR = RS_PIN;
        WR_PORT->BRR = WR_PIN;
        setData(index);
        WR_PORT->BSRR = WR_PIN;
    }

    static void writeDataByte(uint16_t word) {
        RS_PORT->BSRR = RS_PIN;
        WR_PORT->BRR = WR_PIN;
        setData(word);
        WR_PORT->BSRR = WR_PIN;
    }
};

template<REG r>
class Register0: public RegisterBase {
public:
    static void apply() {
        RegisterBase::writeIndex(static_cast<uint16_t>(r));
    }
};

template<REG r>
class Register1Write: public RegisterBase {
public:
    static void writeData(uint16_t word) {
        CS_PORT->BRR = CS_PIN;
        RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(r)));
        RS_PORT->BSRR = RS_PIN;
        writeDataByte(word);
    }
    static void writeData(uint16_t word, int len) {
        CS_PORT->BRR = CS_PIN;
        RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(r)));
        RS_PORT->BSRR = RS_PIN;
        for (int i=0; i< len; i++)
            writeDataByte(word);
    }
};

template<REG r, std::size_t _Nm>
class RegisterWrite: public RegisterBase {
public:
    static void writeData(std::array<uint16_t, _Nm> && data) {
        CS_PORT->BRR = CS_PIN;
        RegisterBase::writeIndex(static_cast<uint16_t>(static_cast<uint16_t>(r)));
        RS_PORT->BSRR = RS_PIN;
        for (uint16_t & word : data) {
            writeDataByte(word);
        }
    }
};

class CaSet: public RegisterBase {
public:
    static void writeData(uint16_t start, uint16_t end);
};

class PaSet: public RegisterBase {
public:
    static void writeData(uint16_t start, uint16_t end);
};

using Reset= Register0<REG::RESET>;
using SleepIn = Register0<REG::SLEEPIN>;
using SleepOut = Register0<REG::SLEEPOUT>;
using NormalMode = Register0<REG::NORMAL_DISPLAY>;
using DisplayInversionOff = Register0<REG::DISPLAY_INVERSION_OFF>;
using DisplayInversionOn = Register0<REG::DISPLAY_INVERSION_ON>;
using AllDisplayOff = Register0<REG::ALL_PIXEL_OFF>;
using AllDisplayOn = Register0<REG::ALL_PIXEL_ON>;
using DisplayOff = Register0<REG::DISPLAY_OFF>;
using DisplayOn = Register0<REG::DISPLAY_ON>;
using RamWR = Register1Write<REG::RAMWR>;
using TearingOn = Register0<REG::TEARING_ON>;
using MadCtl = Register1Write<REG::MADCTL>;
using ExitIdleMode = Register0<REG::ExitIdleMode>;
using ColMod = Register1Write<REG::COLMOD>;

using SetExtCmd_B = Register1Write<REG::SETEXTC_B>;
using SetExtCmd_D = RegisterWrite<REG::SETEXTC_D, 3>;

using SetPanel_C = RegisterWrite<REG::SET_PANEL_B, 6>;
using SetPanel = RegisterWrite<REG::SET_PANEL_B, 5>;
using SetNorTime = RegisterWrite<REG::SET_NOR_TIME_B, 1>;
using SetParTime = RegisterWrite<REG::SET_PAR_TIME_B, 3>;
using SetIdlTime = RegisterWrite<REG::SET_ILD_TIME_B, 3>;
using SetOsc = Register1Write<REG::SET_OSC_B>;
using SetRGB = Register1Write<REG::SET_RGB_B>;
using SetGamma = RegisterWrite<REG::SET_GAMMA_B, 12>;
using SetGamma_D = RegisterWrite<REG::SET_GAMMA_D, 34>;

using SetPower_B =RegisterWrite<REG::SET_POWER_B, 3>;
using SetPower_C =RegisterWrite<REG::SET_POWER_D, 6>;
using SetPower_D =RegisterWrite<REG::SET_POWER_D, 7>;
using SetVCOM_B =RegisterWrite<REG::SET_VCOM_B, 3>;
using SetVCOM_C =Register1Write<REG::SET_VCOM_C>;
using SetVCOM_D =RegisterWrite<REG::SET_VCOM_D, 2>;
using SetNorPow =RegisterWrite<REG::SET_NOR_POW_B, 2>;
using SetParPow =RegisterWrite<REG::SET_PART_POW_B, 2>;
using SetIdlePow =RegisterWrite<REG::SET_IDLE_POW_B, 2>;
using SetPanelRelated =Register1Write<REG::SET_PANEL_RELATED>;
using SetOsc_D =RegisterWrite<REG::SET_OSC_D, 2>;

using SetDirTIM =RegisterWrite<REG::SET_DIR_TIM, 15>;
using SetEq =RegisterWrite<REG::SET_EQ, 4>;
using Set_Eq =Register1Write<REG::SET_EQ>;

using SetDisplayMode_B = Register1Write<REG::SET_DISPLAY_MODE>;
using SetDisplayMode_C = RegisterWrite<REG::SET_DISPLAY_MODE, 7>;

class HX8357: public GFX<Color16Bit> {
public:
    HX8357(LcdID driver);
    static bool checkPresence();
    void drawPixel(Point point, Color16Bit color);
    void drawPixelInternal(Point && point, Color16Bit && color) override;
    void setDriverQuality(DriverQuality driverQuality);
    void static reset();
protected:
    void setAddrWindow(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) override;
    void flood(Color16Bit color, uint32_t len);
private:
    static const uint32_t DEFAULT_WIDTH = 320;
    static const uint32_t DEFAULT_HEIGTH = 480;

    void init();

};
}

#endif /* HX8357_H_ */
