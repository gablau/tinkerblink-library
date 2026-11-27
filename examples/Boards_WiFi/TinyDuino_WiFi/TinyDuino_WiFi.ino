/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use TinyCircuits CC3000 shield
  to connect your project to Blynk.

  Note: Firmware version 1.14 or later is preferred.
        Tools->Board     should be "Arduino Pro or Pro Mini"
        Tools->Processor should be "ATmega 328 (3.3V, 8MHz)"

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Adafruit_CC3000.h>
#include <BlynkSimpleTinyDuino.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Choose wifi_sec from WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int wifi_sec = WLAN_SEC_WPA2;

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

