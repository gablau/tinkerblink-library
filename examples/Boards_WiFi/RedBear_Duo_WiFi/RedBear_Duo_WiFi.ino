/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use RedBear Duo
  to connect your project to Blynk.

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <BlynkSimpleRedBear_Duo.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Choose wifi_sec from WPA2, WPA, or WEP
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int wifi_sec = WPA2;

//SYSTEM_MODE(AUTOMATIC);
SYSTEM_MODE(MANUAL);

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(5000);

  WiFi.on();
  WiFi.setCredentials(ssid, pass, wifi_sec);
  WiFi.connect();

  Blynk.begin(auth);
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "myserver.example.com", 8080);
  //Blynk.begin(auth, server_ip, port);
}

void loop()
{
  Blynk.run();
}

