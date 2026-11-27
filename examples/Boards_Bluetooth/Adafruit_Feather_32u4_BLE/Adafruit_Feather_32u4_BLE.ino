/*************************************************************
  TinkerBlink Library: https://github.com/gablau/tinkerblink-library
  Connects your board to a self-hosted Blynk Legacy server
  (blynk-server 0.41.x and 0.50.x).

  This example code is in public domain.

 *************************************************************

  This example shows how to use Adafruit Feather 32u4 BLE
  to connect your project to Blynk.

  Note: This requires BluefruitLE nRF51 library
    from http://librarymanager/all#Adafruit_BluefruitLE_nRF51
    or https://github.com/adafruit/Adafruit_BluefruitLE_nRF51

  NOTE: BLE support is in beta!

 *************************************************************/

#define BLYNK_USE_DIRECT_CONNECT

#define BLYNK_PRINT Serial

#include <BlynkSimpleSerialBLE.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>
#include <SPI.h>

// You should get Auth Token in the app.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// SHARED SPI SETTINGS (see adafruit webpages for details)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
#define BLUEFRUIT_VERBOSE_MODE         true

// Create ble instance, see pinouts above
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void setup() {
  Serial.begin(9600);

  ble.begin(BLUEFRUIT_VERBOSE_MODE);
  ble.factoryReset(); // Optional
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println("Waiting for connections...");

  Blynk.begin(auth, ble);
}

void loop() {
  Blynk.run();
}

