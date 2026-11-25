/**
 * @file       WidgetBridge.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Apr 2015
 * @brief
 *
 */

#ifndef WidgetBridge_h
#define WidgetBridge_h

#include <Blynk/BlynkWidgetBase.h>

// Arduino Nano ESP32 (and other boards with BOARD_HAS_PIN_REMAP) turn
// digitalWrite/analogWrite into function-like macros: keep them out of
// the method names below. Calls in a sketch still go through the macro,
// which remaps the pin number with the local board table: there write
// (bridge.digitalWrite)(pin, value) to send the pin unchanged.
#pragma push_macro("digitalWrite")
#pragma push_macro("analogWrite")
#undef digitalWrite
#undef analogWrite

class WidgetBridge
    : private BlynkWidgetBase
{
public:
    WidgetBridge(uint8_t vPin)
        : BlynkWidgetBase(vPin)
    {}

    void setAuthToken(const char* token) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("i");
        cmd.add(token);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void digitalWrite(const T& pin, int val) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("dw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename T>
    void analogWrite(const T& pin, int val) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("aw");
        cmd.add(pin);
        cmd.add(val);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    template <typename... Args>
    void virtualWrite(int pin, Args... values) {
        char mem[BLYNK_MAX_SENDBYTES];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        cmd.add_multi(values...);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    void virtualWriteBinary(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(mPin);
        cmd.add("vw");
        cmd.add(pin);
        Blynk.sendCmd(BLYNK_CMD_BRIDGE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWriteBinary(pin, param.getBuffer(), param.getLength()-1);
    }

};

#pragma pop_macro("analogWrite")
#pragma pop_macro("digitalWrite")

#endif
