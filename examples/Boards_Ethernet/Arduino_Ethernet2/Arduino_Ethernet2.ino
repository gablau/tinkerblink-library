/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino Ethernet Shield 2 (W5500)
  to connect your project to Blynk.

  Note: This requires Ethernet2 library
    from http://librarymanager/all#Ethernet2

        Pins 10, 11, 12 and 13 are reserved for Ethernet module.
        DON'T use them in your sketch directly!

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet2.h>
#include <BlynkSimpleEthernet2.h>

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
  // For more options, see Boards_Ethernet/Arduino_Ethernet_Manual example
}

void loop()
{
  Blynk.run();
}

