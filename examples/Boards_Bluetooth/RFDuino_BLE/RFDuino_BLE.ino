/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use RFduino BLE
  to connect your project to Blynk.

  Note: This requires RFduino support package:
    https://github.com/RFduino/RFduino

  And BLEPeripheral library
    from http://librarymanager/all#BLEPeripheral
    or https://github.com/sandeepmistry/arduino-BLEPeripheral

  NOTE: BLE support is in beta!

 *************************************************************/

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleBLEPeripheral.h>
#include <BLEPeripheral.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Create ble serial instance, parameters are ignored for MicroBit
BLESerial SerialBLE(0, 0, 0);

void setup() {
  Serial.begin(9600);

  SerialBLE.setLocalName("Blynk");
  SerialBLE.setDeviceName("Blynk");
  SerialBLE.setAppearance(0x0080);
  SerialBLE.begin();

  Serial.println("Waiting for connections...");

  Blynk.begin(SerialBLE, auth);
}

void loop() {
  SerialBLE.poll();

  if (SerialBLE) {    // If BLE is connected...
    Blynk.run();
  }
}

