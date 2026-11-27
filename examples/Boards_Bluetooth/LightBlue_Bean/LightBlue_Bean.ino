/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use LightBlue Bean / Bean+
  to connect your project to Blynk.

  Note: For this example you need to install Bean Loader:
    https://punchthrough.com/bean/guides/getting-started/intro/

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

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

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin();
  Blynk.begin(Serial, auth);

  timer.setInterval(1000L, []() {
    Blynk.virtualWrite(V1, millis());
  });
}

void loop()
{
  Blynk.run();
  timer.run();
}

// Attach a ZeRGBa widget to the Virtual pin 1
// to control the built-in RGB led!
BLYNK_WRITE(V0) {
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  Bean.setLed(r, g, b);
}

