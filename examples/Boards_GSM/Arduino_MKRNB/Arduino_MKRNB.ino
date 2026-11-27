/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino MKRNB 1500
  to connect your project to Blynk.
  Note: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRNB
  Feel free to apply it to any other example. It's simple!
 *************************************************************/
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <MKRNB.h>
#include <BlynkSimpleMKRNB.h>

NBClient client;
GPRS gprs;
NB nbAccess;
// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";
// Your SIM credential
// Leave empty, if missing pin
char pin[]  = "";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, nbAccess, gprs, client , pin);
}
void loop()
{
  Blynk.run();
}
