/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SoftwareSerial SwSerial(2, 3); // RX, TX

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Blynk will work through SoftwareSerial
  // Do not read or write this serial manually in your sketch
  SwSerial.begin(9600);
  Blynk.begin(SwSerial, auth);
}

void loop()
{
  Blynk.run();
}

