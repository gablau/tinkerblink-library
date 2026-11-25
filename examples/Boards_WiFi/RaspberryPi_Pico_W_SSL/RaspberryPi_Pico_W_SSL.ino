/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************
  This example runs directly on Raspberry Pi Pico W and Pico 2 W, over TLS.

  Note: This requires "Raspberry Pi Pico/RP2040/RP2350" by Earle Philhower
  in the Boards Manager.

  Change WiFi ssid, pass, and the auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimplePicoW_SSL.h>

// Auth token of the device: copy it from the app.
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup()
{
  // Debug console
  Serial.begin(115200);

  // The server certificate must chain to ISRG Root X1 (Let's Encrypt).
  // For a server with its own CA or a self-signed certificate, call one
  // of these before Blynk.begin():
  //Blynk.setRootCA(server_ca_pem);      // the CA (or the certificate) as PEM text
  //Blynk.setFingerprint("AA:BB:...");   // SHA-1 of the server certificate:
  //                                     // openssl x509 -in server.crt -noout -fingerprint -sha1
  //Blynk.setInsecure();                 // no verification: testing only

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "myserver.example.com", 9443);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 9443);
}

void loop()
{
  Blynk.run();
}
