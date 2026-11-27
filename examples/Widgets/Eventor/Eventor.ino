/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  You can use predefined rules on application side.

  Project setup in the app:
    Eventor widget with next rules :
      a) When V0 is equal to 1, set V1 to 255;
      b) When V0 is equal to 0, set V1 to 0;
    Led widget on V1 pin
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
boolean flag = true;

void sendFlagToServer() {
  if (flag) {
    Blynk.virtualWrite(V0, 1);
  } else {
    Blynk.virtualWrite(V0, 0);
  }
  flag = !flag;
}

BLYNK_WRITE(V1) {
  //here you'll get 0 or 255
  int ledValue = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendFlagToServer);
}

void loop()
{
  Blynk.run();
  timer.run();
}

