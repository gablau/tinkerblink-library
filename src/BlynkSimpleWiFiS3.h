/**
 * @file       BlynkSimpleWiFiS3.h
 * @author     Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2026 Gabriele Lauricella
 * @brief      Arduino UNO R4 WiFi (WiFiS3 library)
 *
 * Define BLYNK_USE_SSL before the include to connect over TLS (port 9443):
 * the WiFi module checks the server against its own root CA list, which
 * includes ISRG Root X1 (Let's Encrypt).
 */

#ifndef BlynkSimpleWiFiS3_h
#define BlynkSimpleWiFiS3_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "WiFiS3"
#endif

#define BLYNK_SEND_ATOMIC

#include <WiFiS3.h>
#include <Adapters/BlynkWiFiCommon.h>

#ifdef BLYNK_USE_SSL
static WiFiSSLClient _blynkWifiClient;
#else
static WiFiClient _blynkWifiClient;
#endif

static BlynkArduinoClient _blynkTransport(_blynkWifiClient);
BlynkWifiCommon Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
