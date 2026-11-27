/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  Note: This requires CurieBLE library
    from http://librarymanager/all#CurieBLE

  Warning: Bluetooth support is in beta!
 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <BlynkSimpleCurieBLE.h>
#include <CurieBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BLEPeripheral  blePeripheral;

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(1000);

  blePeripheral.setLocalName("Blynk");
  blePeripheral.setDeviceName("Blynk");
  blePeripheral.setAppearance(384);

  Blynk.begin(blePeripheral, auth);

  blePeripheral.begin();

  Serial.println("Waiting for connections...");
}

void loop()
{
  blePeripheral.poll();
  Blynk.run();
}

