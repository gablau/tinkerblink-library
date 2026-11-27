/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use AirBoard + RN-XV WiFly module
  to connect your project to Blynk.

  For this example you need WiFlyHQ library:
    https://github.com/harlequin-tech/WiFlyHQ

  Note: Be sure to read this: http://www.theairboard.cc/quick-start/
        WiFly firmware version 4.41 or later is preferred.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(10, 11); // RX, TX

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT DebugSerial


#include <TheAirBoard.h>
#include <WiFlyHQ.h>
#include <BlynkSimpleWiFly.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define WiFlySerial Serial
WiFly wifly;

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  delay(10);
  WiFlySerial.begin(9600);  // Set your RN-XV baud rate
  delay(10);

  // Bind WiFly driver to the serial
  if (!wifly.begin(&WiFlySerial)) {
    BLYNK_FATAL("Failed to start wifly");
  }

  // You can try increasing baud rate:
  //wifly.setBaud(115200);
  //WiFlySerial.begin(115200);

  Blynk.begin(auth, wifly, ssid, pass);
}

void loop()
{
  Blynk.run();
}

