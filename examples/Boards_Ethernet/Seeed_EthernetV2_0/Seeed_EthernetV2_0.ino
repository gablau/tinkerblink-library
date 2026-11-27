/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use Seeed Ethernet Shield V2.0 (W5200)
  to connect your project to Blynk.

  More about the shield (and library download):
    http://www.seeedstudio.com/wiki/Ethernet_Shield_V2.0
  (It looks like this library for W5200 works only on Arduino IDE 1.0.*)

  Note: This Hardware is not tested.

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <EthernetV2_0.h>
#include <BlynkSimpleEthernetV2_0.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define W5200_CS  10
#define SDCARD_CS 4

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "myserver.example.com", 8080);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}

