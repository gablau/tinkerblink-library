/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to monitor a button state
  using polling mechanism.

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

int prevState = -1;
int currState = -1;
long lastChangeTime = 0;

void checkPin()
{
  // Invert state, since button is "Active LOW"
  int state = !digitalRead(2);

  // Debounce mechanism
  long t = millis();
  if (state != prevState) {
    lastChangeTime = t;
  }
  if (t - lastChangeTime > 50) {
    if (state != currState) {
      currState = state;
      Blynk.virtualWrite(V1, state);
    }
  }
  prevState = state;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Make pin 2 default HIGH, and attach INT to our handler
  pinMode(2, INPUT_PULLUP);
}

void loop()
{
  Blynk.run();
  checkPin();
}

