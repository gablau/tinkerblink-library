/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use WildFire V4
  to connect your project to Blynk.

  Note: You need to install Arduino IDE WildFire support:
    http://shop.wickeddevice.com/resources/wildfire/

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

// Digital the pin that is used to reset/enable the ESP8266 module
const int EspSwitch =  23;

ESP8266 wifi(&Serial1);

void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(10);

  // Set ESP8266 baud rate
  Serial1.begin(115200);
  delay(10);

  // Reset ESP
  pinMode(EspSwitch, OUTPUT);
  digitalWrite(EspSwitch, LOW);
  delay(50);
  digitalWrite(EspSwitch, HIGH);

  // Configure Blynk connection
  Blynk.begin(auth, wifi, ssid, pass);
}

void loop()
{
  Blynk.run();
}

