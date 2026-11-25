/**
 * @file       BlynkSimplePicoW.h
 * @license    This project is released under the MIT License (MIT)
 * @brief      Raspberry Pi Pico W and Pico 2 W (arduino-pico core), plain TCP
 *
 */

#ifndef BlynkSimplePicoW_h
#define BlynkSimplePicoW_h

#if !defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED) || !defined(PICO_CYW43_SUPPORTED)
#error This code is intended for Raspberry Pi Pico W / Pico 2 W with the arduino-pico core (Earle Philhower)! Please check your Tools->Board setting.
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "CYW43"
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
