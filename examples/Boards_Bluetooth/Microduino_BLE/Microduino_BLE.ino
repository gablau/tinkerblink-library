/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use Microduino/mCookie Bluetooth LE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

// For CoreUSB, use Serial for debug output
#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Serial.println("Waiting for connections...");

  // Blynk will work through Serial1
  // Do not read or write this serial manually in your sketch
  Serial1.begin(9600);
  Blynk.begin(auth, Serial1);
}

void loop()
{
  Blynk.run();
}

