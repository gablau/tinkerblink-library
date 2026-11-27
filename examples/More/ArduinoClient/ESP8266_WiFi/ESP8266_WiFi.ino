/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This sketch shows how to access WiFiClient directly in Blynk

  1. This gives you full control of the connection process.
  2. Shows a sensible way of integrating other connectivity hardware,
     that was not supported by Blynk out-of-the-box.

  NOTE: This requires ESP8266 support package:
       https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
    in the Tools -> Board menu!

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleStream.h>

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

WiFiClient wifiClient;

// This function tries to connect to the cloud using TCP
bool connectBlynk()
{
  wifiClient.stop();
  return wifiClient.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
}

// This function tries to connect to your WiFi network
void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  connectWiFi();

  connectBlynk();

  Blynk.begin(wifiClient, auth);
}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // Reconnect to Blynk Cloud
  if (!wifiClient.connected()) {
    connectBlynk();
    return;
  }

  Blynk.run();
}

