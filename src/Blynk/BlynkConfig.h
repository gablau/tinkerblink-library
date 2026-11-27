/**
 * @file       BlynkConfig.h
 * @author     Volodymyr Shymanskyy, Gabriele Lauricella
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 *             Copyright (c) 2026 Gabriele Lauricella
 * @date       Jan 2015
 * @brief      Configuration of different aspects of library
 *
 */

#ifndef BlynkConfig_h
#define BlynkConfig_h

#include <Blynk/BlynkDetectDevice.h>

/***************************************************
 * Change these settings to match your need
 ***************************************************/

// Server used when begin()/config() get no server.
// Define any of these before including the Blynk header to override them.
// The ports are the hardware ports of blynk-server 0.41.x and 0.50.x:
// 8080 plain TCP (http.port), 9443 TLS (https.port).
#ifndef BLYNK_DEFAULT_DOMAIN
#define BLYNK_DEFAULT_DOMAIN     "myserver.example.com"
#endif
#ifndef BLYNK_DEFAULT_PORT
#define BLYNK_DEFAULT_PORT       8080
#endif
#ifndef BLYNK_DEFAULT_PORT_SSL
#define BLYNK_DEFAULT_PORT_SSL   9443
#endif

/***************************************************
 * Professional settings
 ***************************************************/
// Library version.
#define BLYNK_VERSION        "0.7.0"

// Heartbeat period in seconds.
#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT      10
#endif

// Network timeout in milliseconds.
#ifndef BLYNK_TIMEOUT_MS
#define BLYNK_TIMEOUT_MS     3000UL
#endif

// Limit the amount of outgoing commands per second.
#ifndef BLYNK_MSG_LIMIT
#define BLYNK_MSG_LIMIT      15
#endif

// Limit the incoming command length.
#ifndef BLYNK_MAX_READBYTES
#define BLYNK_MAX_READBYTES  256
#endif

// Limit the outgoing command length.
#ifndef BLYNK_MAX_SENDBYTES
#define BLYNK_MAX_SENDBYTES  128
#endif

// TLS server verification (BlynkSimpleEsp8266_SSL.h, BlynkSimpleEsp32_SSL.h,
// BlynkSimplePicoW_SSL.h).
// By default the server certificate must chain to ISRG Root X1
// (Let's Encrypt). Define one of these before including the header
// (or call Blynk.setRootCA/setFingerprint/setInsecure before begin()):
//
// Own CA, or the server's self-signed certificate, as a PEM string
// declared before the include:
//#define BLYNK_SSL_ROOT_CA my_root_ca_pem
//
// Pin the server certificate (ESP8266, Pico W: SHA-1; ESP32: SHA-256; hex):
//#define BLYNK_SSL_FINGERPRINT "AA:BB:..."
//
// Encrypt without verifying the server (testing only):
//#define BLYNK_SSL_INSECURE

// Uncomment to disable built-in analog and digital operations.
//#define BLYNK_NO_BUILTIN

// Uncomment to disable providing info about device to the server.
//#define BLYNK_NO_INFO

// Uncomment to enable debug prints.
//#define BLYNK_DEBUG

// Uncomment to force-enable 128 virtual pins
//#define BLYNK_USE_128_VPINS

// Uncomment to disable fancy logo
//#define BLYNK_NO_FANCY_LOGO

// Uncomment to enable 3D fancy logo
//#define BLYNK_FANCY_LOGO_3D

// Uncomment to enable experimental functions.
//#define BLYNK_EXPERIMENTAL

// Uncomment to disable all float/double usage
//#define BLYNK_NO_FLOAT

// Uncomment to switch to direct-connect mode
//#define BLYNK_USE_DIRECT_CONNECT


// Uncomment to append command body to header (uses more RAM)
//#define BLYNK_SEND_ATOMIC

// Split whole command into chunks (in bytes)
//#define BLYNK_SEND_CHUNK 64

// Wait after sending each chunk (in milliseconds)
//#define BLYNK_SEND_THROTTLE 10

#endif
