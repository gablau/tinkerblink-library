/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  Output any data on LCD widget!

  App project setup:
    LCD widget, SIMPLE mode, in widget settings :
    - Select pin V0 for zero pin
    - Select pin V1 for first pin
    - Leave "Reading Frequency" on "1 sec" interval
    - Type into first edit field "/pin0/ seconds"
    - Type into second edit field "/pin1/ millis"
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, millis() / 1000);
}

BLYNK_READ(V1) {
  Blynk.virtualWrite(V1, millis());
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}

