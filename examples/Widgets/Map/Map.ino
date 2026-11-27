/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  Output any data on Map widget!

  App project setup:
    Map widget on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

WidgetMap myMap(V1);

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // If you want to remove all points:
  //myMap.clear();

  int index = 0;
  float lat = 51.5074;
  float lon = 0.1278;
  myMap.location(index, lat, lon, "value");
}

void loop()
{
  Blynk.run();
}

