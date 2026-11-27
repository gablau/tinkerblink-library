/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use Simblee BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSimbleeBLE.h>
#include <SimbleeBLE.h>

char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600);

  SimbleeBLE.deviceName = "Simblee";
  SimbleeBLE.advertisementInterval = MILLISECONDS(300);
  SimbleeBLE.txPowerLevel = -20;  // (-20dbM to +4 dBm)

  // start the BLE stack
  SimbleeBLE.begin();

  Serial.println("Waiting for connections...");

  Blynk.begin(auth);
}


void loop()
{
  Blynk.run();
}

