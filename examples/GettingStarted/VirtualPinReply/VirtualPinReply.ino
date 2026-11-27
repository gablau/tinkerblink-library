/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to send requested values to the app

  Project setup in the app:
    Value Display widget attached to V5. Set any reading
    frequency (i.e. 1 second)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// This function is called when there is a Widget
// which is requesting data from Virtual Pin (5)
BLYNK_READ(V5)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(V5, millis() / 1000);
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

