/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use Energia BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#define BLYNK_RUN_YIELD() { ble.handleEvents(); }

#include <BLE.h>
#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(115200);

  ble.setLogLevel(BLE_LOG_ERRORS);
  ble.begin();
  ble.serial();
  ble.setAdvertName("Blynk BLE");
  ble.startAdvert();

  Serial.println("Waiting for connections...");

  Blynk.begin(ble, auth);
}

void loop() {
  Blynk.run();
}

