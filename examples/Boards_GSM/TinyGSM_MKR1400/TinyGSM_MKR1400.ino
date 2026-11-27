/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  Attention! Please check out TinyGSM guide:
    https://tiny.cc/tinygsm-readme

  Change GPRS apm, user, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Arduino MKR GSM 1400 uses U-blox modem
#define TINY_GSM_MODEM_UBLOX

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

TinyGsm modem(SerialGSM);

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  // Set GSM module baud rate
  SerialGSM.begin(115200);

  pinMode(GSM_DTR, OUTPUT);
  digitalWrite(GSM_DTR, LOW);
  delay(5);

  // Turn on the GSM module by triggering GSM_RESETN pin
  pinMode(GSM_RESETN, OUTPUT);
  digitalWrite(GSM_RESETN, HIGH);
  delay(100);
  digitalWrite(GSM_RESETN, LOW);

  delay(1000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");

  Blynk.begin(auth, modem, apn, user, pass);
}

void loop()
{
  Blynk.run();
}

