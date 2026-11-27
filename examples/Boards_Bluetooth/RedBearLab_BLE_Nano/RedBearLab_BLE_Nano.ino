/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use RedBearLab BLE Nano
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleRedBearLab_BLE_Nano.h>
#include <BLE_API.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

uint8_t device_name[] = "Blynk";

void setup() {
  Serial.begin(9600);
  delay(100);

  ble.init();

  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED);
  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME,
                                         device_name, sizeof(device_name) - 1);

  ble.gap().setDeviceName(device_name);
  ble.gap().setTxPower(4);

  Serial.println("Waiting for connections...");

  // Add Blynk service...
  Blynk.begin(auth);

  ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
  ble.gap().setAdvertisingInterval(Gap::MSEC_TO_GAP_DURATION_UNITS(1000));
  ble.gap().setAdvertisingTimeout(0);
  ble.startAdvertising();
}

void loop() {
  Blynk.run();
}

