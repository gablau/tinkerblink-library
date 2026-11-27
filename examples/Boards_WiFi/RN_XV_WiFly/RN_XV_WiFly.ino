/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example shows how to use WiFly RN-XV
  to connect your project to Blynk.

  For this example you need WiFlyHQ library:
    https://github.com/harlequin-tech/WiFlyHQ

  Note: Ensure a stable serial connection!
        Hardware serial is preferred.
        Firmware version 4.41 or later is preferred.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFlyHQ.h>
#include <BlynkSimpleWiFly.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define WiFlySerial Serial1
// This can be a SoftwareSerial object:
//#include <SoftwareSerial.h>
//SoftwareSerial WiFlySerial(2, 3); // RX, TX

WiFly wifly;

void setup()
{
  // Debug console
  Serial.begin(9600);

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

