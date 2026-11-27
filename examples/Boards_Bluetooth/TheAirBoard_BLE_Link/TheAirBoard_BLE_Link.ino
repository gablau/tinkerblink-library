/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use AirBoard + BLE-Link
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************
             USB HOWTO: http://tiny.cc/BlynkUSB
 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(10, 11); // RX, TX

#define BLYNK_PRINT DebugSerial

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BlynkTimer timer;

void setup()
{
  DebugSerial.begin(9600);

  DebugSerial.println("Waiting for connections...");

  // Blynk works through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(115200);
  Blynk.begin(Serial, auth);

  setLED(0x04, 0x16, 0x10);

  timer.setInterval(1000L, []() {
    Blynk.virtualWrite(V1, millis());
  });
}

void loop()
{
  Blynk.run();
  timer.run();
}

#define LED_RED   9
#define LED_GREEN 5
#define LED_BLUE  6

void setLED(byte r, byte g, byte b) {
  analogWrite(LED_RED,   r);
  analogWrite(LED_GREEN, g);
  analogWrite(LED_BLUE,  b);
}

BLYNK_CONNECTED() {
  setLED(0x00, 0x00, 0x20);
}

BLYNK_WRITE(V0) {
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  setLED(r, g, b);
}

