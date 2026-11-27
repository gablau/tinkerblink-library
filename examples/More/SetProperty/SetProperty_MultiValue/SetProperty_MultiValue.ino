/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  You can set predefined properties of any widget. Like color, label

  Project setup in the app:
    Menu Widget on V1 with 2 items
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BLYNK_WRITE(V1) {
  int value = param.asInt();
  if (value == 1) {
    Serial.println("Item 1 selected");
  } else if (value == 2) {
    // If item 2 is selected, change menu items...
    BlynkParamAllocated items(128); // list length, in bytes
    items.add("New item 1");
    items.add("New item 2");
    items.add("New item 3");
    Blynk.setProperty(V1, "labels", items);

    // You can also use it like this:
    //Blynk.setProperty(V1, "labels", "item 1", "item 2", "item 3");

  } else {
    Serial.println("Unknown item selected");
  }
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

