/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use LinkItONE to connect your project to Blynk.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <LWiFi.h>
#include <LWiFiClient.h>
#include <BlynkSimpleLinkItONE.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Choose wifi_sec from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int wifi_sec = LWIFI_WPA;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, wifi_sec);
}

void loop()
{
  Blynk.run();
}
