/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to monitor a button state
  using interrupts mechanism.

  App project setup:
    LED widget on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

WidgetLED led1(V1);

// We make these values volatile, as they are used in interrupt context
volatile bool pinChanged = false;
volatile int  pinValue   = 0;

// Most boards won't send data to WiFi out of interrupt handler.
// We just store the value and process it in the main loop.
void checkPin()
{
  // Invert state, since button is "Active LOW"
  pinValue = !digitalRead(2);

  // Mark pin value changed
  pinChanged = true;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Make pin 2 HIGH by default
  pinMode(2, INPUT_PULLUP);
  // Attach INT to our handler
  attachInterrupt(digitalPinToInterrupt(2), checkPin, CHANGE);
}

void loop()
{
  Blynk.run();

  if (pinChanged) {

    // Process the value
    if (pinValue) {
      led1.on();
    } else {
      led1.off();
    }

    // Clear the mark, as we have processed the value
    pinChanged = false;
  }
}

