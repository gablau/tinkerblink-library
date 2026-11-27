/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This sketch shows how to connect to your ESP8266 directly from the app

  NOTE: This requires ESP8266 support package:
       https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
    in the Tools -> Board menu!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleStream.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "YourNetworkName";
const char* pass = "YourPassword";

WiFiServer wifiServer(80);
WiFiClient wifiClient;

// This function tries to reconnect to WiFi network
void connectWiFi() {
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

  wifiServer.stop();

  wifiServer.begin();

  Serial.print("Server started: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);

  connectWiFi();
}

void loop() {
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }

  // If thereis some client
  wifiClient = wifiServer.available();
  if (wifiClient)
  {
    Serial.println("client available");
    Blynk.begin(wifiClient, auth);
    while (wifiClient.connected())
    {
      Blynk.run();
    }
  }
}

