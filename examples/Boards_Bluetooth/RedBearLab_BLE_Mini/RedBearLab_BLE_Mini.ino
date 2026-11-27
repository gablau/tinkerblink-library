/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use Arduino + RedBearLab BLE Mini
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define SerialBLE  Serial1      // Set Serial object

void setup()
{
  // This is for debug prints
  Serial.begin(9600);

  Serial.println("Waiting for connections...");

  SerialBLE.begin(57600);       // BLE Mini uses baud 57600
  Blynk.begin(SerialBLE, auth);
}

void loop()
{
  Blynk.run();
}

