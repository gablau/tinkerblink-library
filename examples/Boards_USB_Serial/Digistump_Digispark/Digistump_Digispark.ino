/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  There is an USB issue on Linux:
    https://github.com/digistump/DigistumpArduino/issues/16

 *************************************************************/

// Minimize size for non-Pro version
#if !defined(ARDUINO_AVR_DIGISPARKPRO)
  #define BLYNK_NO_INFO
  #define BLYNK_NO_BUILTIN
#endif

#include <BlynkSimpleStream.h>
#include <DigiCDC.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Blynk will work through SerialUSB
  // Do not read or write this serial manually in your sketch
  SerialUSB.begin();
  Blynk.begin(SerialUSB, auth);
}

void loop()
{
  Blynk.run();
}

