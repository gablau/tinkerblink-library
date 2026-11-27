/**
 * @file       BlynkSimplePicoW_SSL.h
 * @author     Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2026 Gabriele Lauricella
 * @brief      Raspberry Pi Pico W and Pico 2 W (arduino-pico core), TLS
 *
 * How the server is verified: see Adapters/BlynkArduinoClientBearSSL.h
 * (same options as ESP8266: own CA, SHA-1 fingerprint, insecure).
 */

#ifndef BlynkSimplePicoW_SSL_h
#define BlynkSimplePicoW_SSL_h

#if !defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_MBED) || !defined(PICO_CYW43_SUPPORTED)
#error This code is intended for Raspberry Pi Pico W / Pico 2 W with the arduino-pico core (Earle Philhower)! Please check your Tools->Board setting.
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "CYW43"
#endif

#define BLYNK_SEND_ATOMIC

#include <WiFi.h>
#include <Adapters/BlynkArduinoClientBearSSL.h>
#include <Adapters/BlynkWiFiStation.h>

typedef BlynkArduinoClientSecure<BearSSL::WiFiClientSecure> BlynkPicoWTransport;

static BearSSL::WiFiClientSecure _blynkWifiClient;
static BlynkPicoWTransport _blynkTransport(_blynkWifiClient);
BlynkWiFiStation<BlynkPicoWTransport, BLYNK_DEFAULT_PORT_SSL> Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
