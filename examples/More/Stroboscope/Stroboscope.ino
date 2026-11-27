/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This is a simple stroboscope.
  You can turn it on and of using a button,
  and control frequency with a slider.

  App project setup:
    Button widget (Switch) on V1
    Slider widget (100...1000) on V2
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

#define LED_PIN 9

BlynkTimer timer;
int t1;

// Toggle LED
void ledBlynk()
{
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

// Enable/disable blinking using virtual pin 1
BLYNK_WRITE(V1)
{
  if (param.asInt()) {
    timer.enable(t1);
  } else {
    timer.disable(t1);
    digitalWrite(LED_PIN, LOW);
  }
}

// Change blink interval using virtual pin 2
BLYNK_WRITE(V2)
{
  long interval = param.asLong();
  boolean wasEnabled = timer.isEnabled(t1);
  timer.deleteTimer(t1);
  t1 = timer.setInterval(interval, ledBlynk);
  if (!wasEnabled) {
    timer.disable(t1);
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Configure LED and timer
  pinMode(LED_PIN, OUTPUT);
  t1 = timer.setInterval(500L, ledBlynk);
  timer.disable(t1);
}

void loop()
{
  Blynk.run();
  timer.run();
}

