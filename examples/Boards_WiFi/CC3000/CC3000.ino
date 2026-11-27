/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use Adafruit CC3000 breakout
  to connect your project to Blynk.

  For this example you need Adafruit_CC3000_Library library:
    https://github.com/adafruit/Adafruit_CC3000_Library

  Note: Firmware version 1.14 or later is preferred.

  1. Update pin definitions according to your setup.
  2. Change WiFi ssid, pass, and Blynk auth token
  3. Run :)

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


// These are the interrupt and control pins for СС3000
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

#include <SPI.h>
#include <Adafruit_CC3000.h>
#include <BlynkSimpleCC3000.h>

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

