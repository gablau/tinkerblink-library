/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  For this example you need UIPEthernet library:
    https://github.com/UIPEthernet/UIPEthernet

  Typical wiring would be:
   VCC -- 5V
   GND -- GND
   CS  -- D10
   SI  -- D11
   SCK -- D13
   SO  -- D12
   INT -- D2

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";


void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "myserver.example.com", 8080);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
}

