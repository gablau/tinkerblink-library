/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use RedBear Duo BLE
  to connect your project to Blynk.

  NOTE: BLE support is in beta!

 *************************************************************/

#if defined(ARDUINO)
  SYSTEM_MODE(MANUAL);    // If Arduino - do not connect to Particle cloud
#else
  SYSTEM_MODE(AUTOMATIC); // Otherwise, connect to Particle cloud
#endif

//#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleRedBear_Duo_BLE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup() {
  Serial.begin(9600);
  delay(5000);

  Serial.println("Waiting for connections...");

  Blynk.begin(auth);
}

void loop() {
  Blynk.run();
}

// Next functions are optional.
// Add ZeRGBa Widget on V0 to control onboard LED.
BLYNK_CONNECTED() {
  RGB.control(true);
}

BLYNK_WRITE(V0) {
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  RGB.color(r, g, b);
}

