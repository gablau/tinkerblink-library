/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to configure static IP with Ethernet.
  Be sure to check ordinary Ethernet example first!!!

  NOTE: Pins 10, 11, 12 and 13 are reserved for Ethernet module.
        DON'T use them in your sketch directly!

  WARNING: If you have an SD card, you may need to disable it
        by setting pin 4 to HIGH. Read more here:
        https://www.arduino.cc/en/Main/ArduinoEthernetShield

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

IPAddress server_ip (10, 0, 0, 10);

// Mac address should be different for each device in your LAN
byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress arduino_ip ( 10,   0,   0,  20);
IPAddress dns_ip     (  8,   8,   8,   8);
IPAddress gateway_ip ( 10,   0,   0,   1);
IPAddress subnet_mask(255, 255, 255,   0);

#define W5100_CS  10
#define SDCARD_CS 4

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card

  Blynk.begin(auth, server_ip, 8080, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
  // Or like this:
  //Blynk.begin(auth, "myserver.example.com", 8080, arduino_ip, dns_ip, gateway_ip, subnet_mask, arduino_mac);
}

void loop()
{
  Blynk.run();
}

