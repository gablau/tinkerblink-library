/**
 * @file       BlynkApiGpiod.h
 * @author     Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2026 Gabriele Lauricella
 * @brief      Linux client with the GPIO pins driven through libgpiod
 *             (Raspberry Pi and other boards with a GPIO character device).
 *
 * Pins are the line offsets of the GPIO chip: on a Raspberry Pi the BCM
 * numbers, as in the app ("gp17" = GPIO17). The chip is gpiochip0, or the
 * one in the BLYNK_GPIOCHIP environment variable (a name like "gpiochip4"
 * or a path like "/dev/gpiochip4").
 *
 * libgpiod 1.x (Debian 12, Ubuntu 22.04) and 2.x (Debian 13, Ubuntu 24.04):
 * the Makefile defines BLYNK_GPIOD_V2 for 2.x. There is no PWM in libgpiod:
 * analog write is ignored.
 */

#ifndef BlynkApiGpiod_h
#define BlynkApiGpiod_h

#include <Blynk/BlynkApi.h>
#include <gpiod.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef BLYNK_INFO_DEVICE
    #define BLYNK_INFO_DEVICE  "Linux"
#endif

#ifndef BLYNK_GPIOD_CONSUMER
    #define BLYNK_GPIOD_CONSUMER "blynk"
#endif

namespace BlynkGpiod {

enum Mode { NONE, INPUT, INPUT_PULLUP, INPUT_PULLDOWN, OUTPUT };

#if defined(BLYNK_GPIOD_V2)
    typedef struct gpiod_line_request* Handle;
#else
    typedef struct gpiod_line* Handle;
#endif

static struct gpiod_chip* chip = NULL;
static bool chipFailed = false;
static Handle lines[256];
static Mode modes[256];

static bool openChip()
{
    if (chip) return true;
    if (chipFailed) return false;
    const char* name = getenv("BLYNK_GPIOCHIP");
    if (!name || !*name) name = "gpiochip0";
#if defined(BLYNK_GPIOD_V2)
    char path[64];
    if (name[0] == '/') {
        snprintf(path, sizeof(path), "%s", name);
    } else {
        snprintf(path, sizeof(path), "/dev/%s", name);
    }
    chip = gpiod_chip_open(path);
#else
    chip = gpiod_chip_open_lookup(name);
#endif
    if (!chip) {
        chipFailed = true;
        BLYNK_LOG2(BLYNK_F("Cannot open GPIO chip "), name);
        return false;
    }
    return true;
}

static void release(uint8_t pin)
{
    if (!lines[pin]) return;
#if defined(BLYNK_GPIOD_V2)
    gpiod_line_request_release(lines[pin]);
#else
    gpiod_line_release(lines[pin]);
#endif
    lines[pin] = NULL;
    modes[pin] = NONE;
}

/** Requests the line in the given mode (output starts low). */
static bool setMode(uint8_t pin, Mode mode)
{
    if (lines[pin] && modes[pin] == mode) return true;
    if (!openChip()) return false;
    release(pin);

#if defined(BLYNK_GPIOD_V2)
    struct gpiod_line_settings* settings = gpiod_line_settings_new();
    struct gpiod_line_config* lineCfg = gpiod_line_config_new();
    struct gpiod_request_config* reqCfg = gpiod_request_config_new();
    Handle h = NULL;
    if (settings && lineCfg && reqCfg) {
        if (mode == OUTPUT) {
            gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
            gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
        } else {
            gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
            gpiod_line_settings_set_bias(settings,
                mode == INPUT_PULLUP   ? GPIOD_LINE_BIAS_PULL_UP :
                mode == INPUT_PULLDOWN ? GPIOD_LINE_BIAS_PULL_DOWN :
                                         GPIOD_LINE_BIAS_DISABLED);
        }
        unsigned int offset = pin;
        gpiod_request_config_set_consumer(reqCfg, BLYNK_GPIOD_CONSUMER);
        if (gpiod_line_config_add_line_settings(lineCfg, &offset, 1, settings) == 0) {
            h = gpiod_chip_request_lines(chip, reqCfg, lineCfg);
        }
    }
    if (reqCfg) gpiod_request_config_free(reqCfg);
    if (lineCfg) gpiod_line_config_free(lineCfg);
    if (settings) gpiod_line_settings_free(settings);
#else
    Handle h = gpiod_chip_get_line(chip, pin);
    if (h) {
        int rc;
        if (mode == OUTPUT) {
            rc = gpiod_line_request_output(h, BLYNK_GPIOD_CONSUMER, 0);
        } else {
            int flags =
                mode == INPUT_PULLUP   ? GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP :
                mode == INPUT_PULLDOWN ? GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN :
                                         GPIOD_LINE_REQUEST_FLAG_BIAS_DISABLE;
            rc = gpiod_line_request_input_flags(h, BLYNK_GPIOD_CONSUMER, flags);
        }
        if (rc != 0) h = NULL;
    }
#endif

    if (!h) {
        BLYNK_LOG2(BLYNK_F("Cannot request GPIO line "), pin);
        return false;
    }
    lines[pin] = h;
    modes[pin] = mode;
    return true;
}

static void write(uint8_t pin, int value)
{
    if (!setMode(pin, OUTPUT)) return;
#if defined(BLYNK_GPIOD_V2)
    gpiod_line_request_set_value(lines[pin], pin,
        value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
#else
    gpiod_line_set_value(lines[pin], value ? 1 : 0);
#endif
}

static int read(uint8_t pin)
{
    // An output pin is read back; any other pin becomes an input.
    if (!lines[pin] && !setMode(pin, INPUT)) return 0;
#if defined(BLYNK_GPIOD_V2)
    int v = gpiod_line_request_get_value(lines[pin], pin);
#else
    int v = gpiod_line_get_value(lines[pin]);
#endif
    return v > 0 ? 1 : 0;
}

} // namespace BlynkGpiod

#ifdef BLYNK_NO_INFO

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo() {}

#else

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::sendInfo()
{
    static const char profile[] BLYNK_PROGMEM = "blnkinf\0"
        BLYNK_PARAM_KV("ver"    , BLYNK_VERSION)
        BLYNK_PARAM_KV("h-beat" , BLYNK_TOSTRING(BLYNK_HEARTBEAT))
        BLYNK_PARAM_KV("buff-in", BLYNK_TOSTRING(BLYNK_MAX_READBYTES))
#ifdef BLYNK_INFO_DEVICE
        BLYNK_PARAM_KV("dev"    , BLYNK_INFO_DEVICE)
#endif
#ifdef BLYNK_INFO_CPU
        BLYNK_PARAM_KV("cpu"    , BLYNK_INFO_CPU)
#endif
#ifdef BLYNK_INFO_CONNECTION
        BLYNK_PARAM_KV("con"    , BLYNK_INFO_CONNECTION)
#endif
#ifdef BOARD_FIRMWARE_VERSION
        BLYNK_PARAM_KV("fw"     , BOARD_FIRMWARE_VERSION)
#endif
        BLYNK_PARAM_KV("build"  , __DATE__ " " __TIME__)
        "\0"
    ;
    const size_t profile_len = sizeof(profile)-8-2;

    char mem_dyn[64];
    BlynkParam profile_dyn(mem_dyn, 0, sizeof(mem_dyn));
    profile_dyn.add_key("conn", "Socket");
#ifdef BOARD_TEMPLATE_ID
    profile_dyn.add_key("tmpl", BOARD_TEMPLATE_ID);
#endif

    static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_INTERNAL, 0, profile+8, profile_len, profile_dyn.getBuffer(), profile_dyn.getLength());
    return;
}

#endif

template<class Proto>
BLYNK_FORCE_INLINE
void BlynkApi<Proto>::processCmd(const void* buff, size_t len)
{
    BlynkParam param((void*)buff, len);
    BlynkParam::iterator it = param.begin();
    if (it >= param.end())
        return;
    const char* cmd = it.asStr();
    uint16_t cmd16;
    memcpy(&cmd16, cmd, sizeof(cmd16));
    if (++it >= param.end())
        return;

    uint8_t pin = it.asInt();

    switch(cmd16) {

#ifndef BLYNK_NO_BUILTIN

    case BLYNK_HW_PM: {
        while (it < param.end()) {
            pin = it.asInt();
            ++it;
            if (!strcmp(it.asStr(), "in")) {
                BlynkGpiod::setMode(pin, BlynkGpiod::INPUT);
            } else if (!strcmp(it.asStr(), "out") || !strcmp(it.asStr(), "pwm")) {
                BlynkGpiod::setMode(pin, BlynkGpiod::OUTPUT);
            } else if (!strcmp(it.asStr(), "pu")) {
                BlynkGpiod::setMode(pin, BlynkGpiod::INPUT_PULLUP);
            } else if (!strcmp(it.asStr(), "pd")) {
                BlynkGpiod::setMode(pin, BlynkGpiod::INPUT_PULLDOWN);
            } else {
#ifdef BLYNK_DEBUG
                BLYNK_LOG4(BLYNK_F("Invalid pin "), pin, BLYNK_F(" mode "), it.asStr());
#endif
            }
            ++it;
        }
    } break;
    case BLYNK_HW_DR: {
        char mem[16];
        BlynkParam rsp(mem, 0, sizeof(mem));
        rsp.add("dw");
        rsp.add(pin);
        rsp.add(BlynkGpiod::read(pin));
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, rsp.getBuffer(), rsp.getLength()-1);
    } break;
    case BLYNK_HW_DW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        BlynkGpiod::write(pin, it.asInt());
    } break;
    case BLYNK_HW_AW: {
        // Should be 1 parameter (value)
        if (++it >= param.end())
            return;

        // No PWM in libgpiod
#ifdef BLYNK_DEBUG
        BLYNK_LOG3(BLYNK_F("Analog write not supported, pin "), pin, "");
#endif
    } break;

#endif

    case BLYNK_HW_VR: {
        BlynkReq req = { pin };
        WidgetReadHandler handler = GetReadHandler(pin);
        if (handler && (handler != BlynkWidgetRead)) {
            handler(req);
        } else {
            BlynkWidgetReadDefault(req);
        }
    } break;
    case BLYNK_HW_VW: {
        ++it;
        char* start = (char*)it.asStr();
        BlynkParam param2(start, len - (start - (char*)buff));
        BlynkReq req = { pin };
        WidgetWriteHandler handler = GetWriteHandler(pin);
        if (handler && (handler != BlynkWidgetWrite)) {
            handler(req, param2);
        } else {
            BlynkWidgetWriteDefault(req, param2);
        }
    } break;
    default:
        BLYNK_LOG2(BLYNK_F("Invalid HW cmd: "), cmd);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_RESPONSE, static_cast<Proto*>(this)->msgIdOutOverride, NULL, BLYNK_ILLEGAL_COMMAND);
    }
}

#endif
