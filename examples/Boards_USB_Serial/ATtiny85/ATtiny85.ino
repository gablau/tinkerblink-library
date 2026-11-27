/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  Note: This requires ATtiny support package:
    https://github.com/damellis/attiny

  Be sure to check ordinary Serial example first!!!

                   +-\/-+
  A0|6 (D 5) PB5  1|    |8  Vcc
  A3|9 (D 3) PB3  2|    |7  PB2 (D 2) A1|7
  A2|8 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
             GND  4|    |5  PB0 (D 0) pwm0
                   +----+

 *************************************************************/

#include <SoftwareSerial.h>
SoftwareSerial DebugSerial(1, 2); // RX, TX
SoftwareSerial SwSerial(3, 4);

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT DebugSerial


#include <BlynkSimpleStream.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

//#include <SoftwareServo.h>
//SoftwareServo servo1;

void setup()
{
  // Debug console
  DebugSerial.begin(9600);

  // Blynk will work through SoftwareSerial
  // Do not read or write this serial manually in your sketch
  SwSerial.begin(9600);
  Blynk.begin(SwSerial, auth);

  // Sometimes you need to calibrate your ATtiny timer
  //OSCCAL = 175;
}

void loop()
{
  Blynk.run();
}

