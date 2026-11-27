/**
 * @file       BlynkSimpleMbedWiFi.h
 * @author     Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2026 Gabriele Lauricella
 * @brief      Arduino mbed boards with the WiFi library of their core:
 *             GIGA R1 WiFi, Portenta H7, Nicla Vision, Opta WiFi. Plain TCP
 *
 */

#ifndef BlynkSimpleMbedWiFi_h
#define BlynkSimpleMbedWiFi_h

#if !defined(ARDUINO_ARCH_MBED)
#error This code is intended for Arduino mbed boards with WiFi (GIGA R1 WiFi, Portenta H7...)! Please check your Tools->Board setting.
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "Murata 1DX"
#endif

#define BLYNK_SEND_ATOMIC

#include <WiFi.h>
#include <Adapters/BlynkArduinoClient.h>
#include <Adapters/BlynkWiFiStation.h>

static WiFiClient _blynkWifiClient;
static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWiFiStation<BlynkArduinoClient, BLYNK_DEFAULT_PORT> Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
