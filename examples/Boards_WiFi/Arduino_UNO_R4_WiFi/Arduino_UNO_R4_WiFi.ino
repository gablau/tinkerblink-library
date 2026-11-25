/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example runs directly on Arduino UNO R4 WiFi.

  Note: This requires "Arduino UNO R4 Boards" (Arduino Renesas)
  in the Boards Manager.

  For TLS (port 9443) uncomment BLYNK_USE_SSL: the WiFi module checks
  the server with its own root CA list (Let's Encrypt included).

  Change WiFi ssid, pass, and the auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#define BLYNK_USE_SSL

#include <WiFiS3.h>
#include <BlynkSimpleWiFiS3.h>

// Auth token of the device: copy it from the app.
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "myserver.example.com", 8080);   // 9443 with BLYNK_USE_SSL
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}
