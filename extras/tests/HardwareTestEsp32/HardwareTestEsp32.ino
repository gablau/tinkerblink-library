/*
  TinkerBlink Library - hardware test for ESP32 boards
  (classic ESP32, ESP32-S2, S3, C3, C6, Arduino Nano ESP32)

  Checks against a blynk-server 0.41.x / 0.50.x:
    1. connection, plain (8080) or TLS (9443) with the chosen verification;
    2. the board name sent to the server ("dev"), see the log and the
       device info in the app;
    3. from the app:
       - Button on a digital pin (e.g. the LED pin): digitalWrite;
       - Slider 0-255 on the same pin: analogWrite (PWM), core 2.0.1+;
       - Value Display on V1: uptime in seconds, pushed every 2 s;
       - Value Display on V2 with reading frequency 1 s: BLYNK_READ;
       - Slider on V3: the value is echoed to the Terminal;
       - Terminal on V0: type "info", "ping" or any text (echoed back);
       - zeRGBa on V4 (MERGE, 0-255): the RGB LED of the board, if
         TEST_RGB_PIN is set.
    4. disconnect/reconnect: switch off the server or the WiFi router for a
       while, the board must reconnect by itself.

  Fill in the settings below (or in test_settings.h), select the board in
  the IDE and upload. Serial monitor at 115200 baud; on boards with native
  USB (SuperMini, LOLIN S2 Mini) enable "USB CDC On Boot".
  Uncomment BLYNK_DEBUG to log every message exchanged with the server.

  Digital and analog write can also be sent without the app, through the
  HTTP API of the server (same port as the boards):
    http://<server>:8080/<token>/update/D13?value=1     digitalWrite(13, 1)
    http://<server>:8080/<token>/update/A13?value=128   analogWrite(13, 128)
    http://<server>:8080/<token>/update/V0?value=info   Terminal command
*/

// ---- Settings ---------------------------------------------------------

// Put your own settings in test_settings.h next to this sketch (ignored by
// git, so the token and the WiFi password stay out of the repository),
// or edit the defaults below.
#if __has_include("test_settings.h")
  #include "test_settings.h"
#endif

#ifndef TEST_AUTH
#define TEST_AUTH     "YourAuthToken"
#define TEST_SSID     "YourNetworkName"
#define TEST_PASS     "YourPassword"
#endif

#ifndef TEST_SERVER
#define TEST_SERVER   "myserver.example.com"   // host name or IP, without port
#endif

// 0: plain TCP, 1: TLS
#ifndef TEST_USE_TLS
#define TEST_USE_TLS  0
#endif

// Port: leave undefined for the defaults, 8080 plain or 9443 TLS
//#define TEST_PORT     8080

// Addressable RGB LED (WS2812) on the board, driven by a zeRGBa on V4 in
// MERGE mode, 0-255: e.g. 48 on ESP32-S3 SuperMini and DevKitC boards.
// It cannot be driven by the app with digitalWrite or analogWrite.
//#define TEST_RGB_PIN  48

// TLS verification, only one (none of them: ISRG Root X1 / Let's Encrypt):
//#define BLYNK_SSL_INSECURE
//#define BLYNK_SSL_FINGERPRINT "AA:BB:..."   // SHA-256 of the server certificate
//#define BLYNK_SSL_ROOT_CA     server_ca_pem // and fill server_ca_pem below

// Uncomment to force a board name, e.g. for the S3 SuperMini:
//#define BLYNK_INFO_DEVICE "ESP32-S3 SuperMini"

// ----------------------------------------------------------------------

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#if defined(BLYNK_SSL_ROOT_CA)
static const char server_ca_pem[] = R"PEM(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)PEM";
#endif

#include <WiFi.h>
#if TEST_USE_TLS
  #include <WiFiClientSecure.h>
  #include <BlynkSimpleEsp32_SSL.h>
  #ifndef TEST_PORT
  #define TEST_PORT BLYNK_DEFAULT_PORT_SSL
  #endif
#else
  #include <BlynkSimpleEsp32.h>
  #ifndef TEST_PORT
  #define TEST_PORT BLYNK_DEFAULT_PORT
  #endif
#endif

char auth[] = TEST_AUTH;
char ssid[] = TEST_SSID;
char pass[] = TEST_PASS;

WidgetTerminal terminal(V0);
BlynkTimer timer;

static void printInfo(Print& out)
{
  out.print(F("TinkerBlink Library ")); out.println(BLYNK_VERSION);
  out.print(F("dev: ")); out.println(BLYNK_INFO_DEVICE);
  out.print(F("chip: ")); out.print(ESP.getChipModel());
  out.print(F(" rev ")); out.println(ESP.getChipRevision());
#if defined(ESP_ARDUINO_VERSION_MAJOR)
  out.print(F("core: ")); out.print(ESP_ARDUINO_VERSION_MAJOR); out.print('.');
  out.print(ESP_ARDUINO_VERSION_MINOR); out.print('.'); out.println(ESP_ARDUINO_VERSION_PATCH);
#endif
#if defined(BLYNK_HAS_ANALOG_WRITE)
  out.println(F("analogWrite: yes"));
#else
  out.println(F("analogWrite: no (core older than 2.0.1)"));
#endif
  out.print(F("server: ")); out.print(TEST_SERVER); out.print(':'); out.print(TEST_PORT);
#if TEST_USE_TLS
  #if defined(BLYNK_SSL_INSECURE)
    out.println(F(" TLS, insecure"));
  #elif defined(BLYNK_SSL_FINGERPRINT)
    out.println(F(" TLS, fingerprint"));
  #elif defined(BLYNK_SSL_ROOT_CA)
    out.println(F(" TLS, own CA"));
  #else
    out.println(F(" TLS, ISRG Root X1"));
  #endif
#else
  out.println(F(" plain"));
#endif
  out.print(F("IP: ")); out.print(WiFi.localIP());
  out.print(F("  RSSI: ")); out.println(WiFi.RSSI());
  out.print(F("free heap: ")); out.println(ESP.getFreeHeap());
}

BLYNK_CONNECTED()
{
  Serial.println(F("[test] connected"));
  terminal.println(F("--- connected ---"));
  printInfo(terminal);
  terminal.flush();
}

BLYNK_DISCONNECTED()
{
  Serial.println(F("[test] disconnected"));
}

BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2, WiFi.RSSI());
}

BLYNK_WRITE(V3)
{
  terminal.print(F("V3 = "));
  terminal.println(param.asStr());
  terminal.flush();
}

#if defined(TEST_RGB_PIN)
BLYNK_WRITE(V4)
{
  // neopixelWrite(): ESP32 core 2.0.x and 3.x (in 3.x also as rgbLedWrite)
  neopixelWrite(TEST_RGB_PIN, param[0].asInt(), param[1].asInt(), param[2].asInt());
  terminal.print(F("RGB = "));
  terminal.print(param[0].asInt()); terminal.print(' ');
  terminal.print(param[1].asInt()); terminal.print(' ');
  terminal.println(param[2].asInt());
  terminal.flush();
}
#endif

BLYNK_WRITE(V0)
{
  String cmd = param.asStr();
  cmd.trim();
  if (cmd == "info") {
    printInfo(terminal);
  } else if (cmd == "ping") {
    terminal.println(F("pong"));
  } else {
    terminal.print(F("echo: "));
    terminal.println(cmd);
  }
  terminal.flush();
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  printInfo(Serial);

  Blynk.begin(auth, ssid, pass, TEST_SERVER, TEST_PORT);

  timer.setInterval(2000L, []() {
    Blynk.virtualWrite(V1, millis() / 1000);
  });
}

void loop()
{
  Blynk.run();
  timer.run();
}
