/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use DFRobot Bluno or BLE Link module
  to connect your project to Blynk.

  Please be sure to update your DFRobot firmware to at least V1.97:
    https://github.com/DFRobot/BLE_firmware_V1.9

  For Bluno, read instructions here:
    https://www.dfrobot.com/wiki/index.php/Bluno_SKU:DFR0267

  For BLE-Link, read instructions here:
    https://www.dfrobot.com/wiki/index.php/BLE-Link_(SKU:TEL0073)

  NOTE: BLE support is in beta!

 *************************************************************/

//#define BLYNK_USE_DIRECT_CONNECT

// You could use a spare Hardware Serial on boards that have it (like Mega)
#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(2, 3); // RX, TX

#define BLYNK_PRINT DebugSerial

#include <BlynkSimpleSerialBLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  DebugSerial.println("Waiting for connections...");

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(115200);
  Blynk.begin(Serial, auth);
}

void loop()
{
  Blynk.run();
}

