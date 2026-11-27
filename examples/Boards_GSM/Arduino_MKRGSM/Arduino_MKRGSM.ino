/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino MKRGSM 1400
  to connect your project to Blynk.

  Note: This requires MKRGSM library
    from https://www.arduino.cc/en/Reference/MKRGSM

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <MKRGSM.h>
#include <BlynkSimpleMKRGSM.h>

GSMClient client;
GPRS gprs;
GSM gsmAccess;


// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your SIM and GPRS credentials
// Leave empty, if missing pin, user or pass
char pin[]  = "";
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, gsmAccess, gprs, client, pin, apn, user, pass);
}

void loop()
{
  Blynk.run();
}
