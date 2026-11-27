/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  You can set predefined properties of any widget. Like color, label

  Project setup in the app:
    Gauge widget (0...100) on V0 in PUSH mode
    Slider widget (0...100) on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

String gaugeColor;

BLYNK_WRITE(V1) {
  int gaugeValue = param.asInt();

  String newColor;
  if (gaugeValue > 80) {
    newColor = BLYNK_RED;
  } else if (gaugeValue > 50) {
    newColor = BLYNK_YELLOW;
  } else {
    newColor = BLYNK_GREEN;
  }

  // Send only if changed
  if (newColor != gaugeColor) {
    gaugeColor = newColor;
    Blynk.setProperty(V0, "color", gaugeColor);
  }

  Blynk.virtualWrite(V0, gaugeValue);
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

