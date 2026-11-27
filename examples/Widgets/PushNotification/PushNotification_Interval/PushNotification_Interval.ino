/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  Simple push notification example

  App project setup:
    Push widget

  Connect a button to pin 2 and GND...
  Pressing this button will also push a message! ;)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BlynkTimer timer;

void notifyUptime()
{
  long uptime = millis() / 60000L;

  // Actually send the message.
  // Note:
  //   We allow 1 notification per 5 seconds for now.
  Blynk.notify(String("Running for ") + uptime + " minutes.");

  // You can also use {DEVICE_NAME} placeholder for device name,
  // that will be replaced by your device name on the server side.
  // Blynk.notify(String("{DEVICE_NAME} running for ") + uptime + " minutes.");
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Notify immediately on startup
  Blynk.notify("Device started");

  // Setup a function to be called every minute
  timer.setInterval(60000L, notifyUptime);
}

void loop()
{
  Blynk.run();
  timer.run();
}

