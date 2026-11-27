/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows you how you can use server as storage for
  your data like EEPROM

  Project setup in the app (not necessary):
    Value display on V0 in PUSH mode.
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BlynkTimer timer;
int uptimeCounter;

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  //get data stored in virtual pin V0 from server
  Blynk.syncVirtual(V0);
}

// restoring counter from server
BLYNK_WRITE(V0)
{
  //restoring int value
  uptimeCounter = param.asInt();
}

void increment() {
  uptimeCounter++;

  //storing int in V0 pin on server
  Blynk.virtualWrite(V0, uptimeCounter);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  timer.setInterval(1000L, increment);
}

void loop()
{
  Blynk.run();
  timer.run();
}

