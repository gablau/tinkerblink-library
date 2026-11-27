# Changelog

## 0.7.0 (2026-11-27)

First release of TinkerBlink Library, based on the Blynk library 0.6.1 (Legacy).

### New boards

- Raspberry Pi Pico W and Pico 2 W (arduino-pico core): `BlynkSimplePicoW.h`, and
  `BlynkSimplePicoW_SSL.h` with the same TLS options as ESP8266.
- Arduino UNO R4 WiFi: `BlynkSimpleWiFiS3.h` (`BLYNK_USE_SSL` for TLS through the WiFi module).
- Arduino GIGA R1 WiFi and the other Arduino mbed boards with WiFi (Portenta H7, Nicla Vision,
  Opta WiFi): `BlynkSimpleMbedWiFi.h`. The Arduino mbed cores are no longer taken for plain mbed OS.
- Arduino Nano RP2040 Connect: example with WiFiNINA.
- ESP32-S2, S3, C3, C6 and Arduino Nano ESP32 with `BlynkSimpleEsp32.h`; on the Nano ESP32 the
  Bridge widget and analog pin names build with the core pin remapping.
- New examples in `Boards_WiFi` for each of them.
- The board name sent to the server (`dev`) recognizes these boards and the LOLIN ESP32 boards,
  with the names of tinkerblink-boards (e.g. "Raspberry Pi Pico W", "LOLIN S2 Mini",
  "ESP32-C3 SuperMini"); other ESP32 chips report "ESP32-S2", "ESP32-S3", "ESP32-C3", "ESP32-C6".
- Analog write (PWM) from the app works on ESP32 with Arduino core 2.0.1 or newer; 0.6.1 ignored it
  on every ESP32.
- Checked on hardware with blynk-server 0.50: ESP32-S3 SuperMini, LOLIN S2 Mini, ESP32-C3
  SuperMini.
- Linux client with GPIO through libgpiod (`make target=gpiod`, libgpiod 1.x and 2.x), for the
  Raspberry Pi OS releases without WiringPi; `make DEVICE="..."` sets the board name.

### Security

- CVE-2019-5065 (TALOS-2019-0854): the host name of a server redirect is always terminated; its
  buffer grows from 32 to 64 bytes (`BLYNK_REDIRECT_HOST_SIZE`) and out-of-range ports are ignored.
- Internal server commands (`rtc`, `ota`, `acon`, `adis`) no longer read past the input buffer when
  a message carries the command name alone.

### Fixes

- TLS on ESP8266 builds again with ESP8266 Arduino core 3.x: it now uses BearSSL (core 2.5.0 or
  newer); the axTLS calls of 0.6.1 no longer exist.
- No "variable `myip` set but not used" warning when logging is off (blynkkk/blynk-library#504).
- `param.asLongLong()` is available on every platform, AVR and MSP430 included
  (blynkkk/blynk-library#505).
- On ESP8266 the wait for the NTP time before a TLS connection has a timeout
  (`BLYNK_SSL_NTP_TIMEOUT_MS`, 20 s) instead of looping forever.

### Changes

- Default server `myserver.example.com` instead of the retired `blynk-cloud.com`, and default
  ports 8080 (plain) and 9443 (TLS) of blynk-server instead of 80 and 443. All three can be
  defined before including the library: `BLYNK_DEFAULT_DOMAIN`, `BLYNK_DEFAULT_PORT`,
  `BLYNK_DEFAULT_PORT_SSL`.
- TLS on ESP8266 and ESP32 trusts ISRG Root X1 (Let's Encrypt) by default. The expired DST Root
  CA X3, Let's Encrypt Authority X3 and blynk-cloud.com certificates are removed;
  `BLYNK_SSL_USE_LETSENCRYPT` is no longer needed.
- TLS on ESP8266 and ESP32 can verify the server with an own CA or self-signed certificate
  (`Blynk.setRootCA()`, `BLYNK_SSL_ROOT_CA`), a certificate fingerprint (`Blynk.setFingerprint()`,
  `BLYNK_SSL_FINGERPRINT`; SHA-1 on ESP8266, SHA-256 on ESP32) or not at all, for testing
  (`Blynk.setInsecure()`, `BLYNK_SSL_INSECURE`).
- Examples use `myserver.example.com` instead of `blynk-cloud.com`, and start with a TinkerBlink
  header instead of the Blynk one, whose links point to closed sites. Their paths are unchanged.
- Library name "TinkerBlink Library", version 0.7.0 (also reported to the server as `ver`). The
  banner printed on the serial log at start shows the TinkerBlink logo.
- Hardware test sketch for ESP32 boards: `extras/tests/HardwareTestEsp32` (connection, virtual
  pins, Terminal, digital and analog write, RGB LED; own settings in an untracked
  `test_settings.h`).
- Serial gateway (`scripts/blynk-ser.sh`, `blynk-ser.bat`) and `scripts/blynk_ctrl.py` default to
  `myserver.example.com`, port 8080. `blynk-ser.sh` uses plain TCP by default; with `-t SSL` it
  verifies the server with `--cert` or the system CA certificates. The expired blynk-cloud.com
  certificate in `scripts/certs` is removed.
- Scripts in `extras` and `scripts` run on Python 3; `extras/build-boards.py` and
  `extras/build-examples.py` build with arduino-cli. The pseudo-servers in `extras/tests` accept
  the login and the board info of the current library.
- Removed: the Travis CI configuration and the Java tools for the Arduino IDE 1.x
  (`extras/ide-tools`).
- The README lists the discontinued boards whose headers and examples are kept unchecked.
- Builds checked by GitHub Actions on AVR, ESP8266 (cores 2.7.4 and 3.x), ESP32 (cores 2.0 and 3.x;
  ESP32-S2, S3, C3, C6, Nano ESP32), Raspberry Pi Pico W and Pico 2 W, UNO R4 WiFi, GIGA R1 WiFi,
  Nano RP2040 Connect and the Linux client (plain and with libgpiod 1.x and 2.x).

### Upgrading from 0.6.1

- Uninstall the "Blynk" library: both use the same header names.
- If a sketch calls `Blynk.begin()` without a server, add it or define `BLYNK_DEFAULT_DOMAIN`.
- If a sketch relied on the default ports 80/443 (Blynk cloud), pass the port explicitly.
- TLS with a self-signed or private-CA server: add `Blynk.setRootCA()` or
  `Blynk.setFingerprint()`. ESP8266 fingerprints keep the 0.6.1 format (SHA-1 hex).
