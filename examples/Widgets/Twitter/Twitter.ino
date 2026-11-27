/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  Simple tweet example

  App project setup:
    Twitter widget (connect it to your Twitter account!)

  Connect a button to pin 2 and GND...
  Pressing this button will also tweet a message! ;)
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

void tweetUptime()
{
  long uptime = millis() / 60000L;
  Serial.println("Tweeting every 10 minutes ;)");

  // Actually send the message.
  // Note:
  //   We allow 1 tweet per 5 seconds for now.
  //   Twitter doesn't allow identical subsequent messages.
  Blynk.tweet(String("Running for ") + uptime + " minutes.");
}

void tweetOnButtonPress()
{
  // Invert state, since button is "Active LOW"
  int isButtonPressed = !digitalRead(2);
  if (isButtonPressed) {
    Serial.println("Button is pressed.");

    Blynk.tweet("Yaaay... button is pressed! :)\n #arduino #IoT #tinkerblink");
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Tweet immediately on startup
  Blynk.tweet("My Arduino project is tweeting using TinkerBlink and it’s awesome!\n #arduino #IoT #blynk");

  // Setup a function to be called every 10 minutes
  timer.setInterval(10L * 60000L, tweetUptime);

  // Setup twitter button on pin 2
  pinMode(2, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler
  attachInterrupt(digitalPinToInterrupt(2), tweetOnButtonPress, CHANGE);
}

void loop()
{
  Blynk.run();
  timer.run();
}

