# Changelog

## 0.7.0 (unreleased)

First release of TinkerBlink Library, based on the Blynk library 0.6.1 (Legacy).

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
- Examples use `myserver.example.com` instead of `blynk-cloud.com`.
- Library name "TinkerBlink Library", version 0.7.0 (also reported to the server as `ver`). The
  banner printed on the serial log at start shows the TinkerBlink logo.
- Builds checked by GitHub Actions on AVR, ESP8266 (cores 2.7.4 and 3.x) and ESP32 (cores 2.0 and
  3.x).

### Upgrading from 0.6.1

- Uninstall the "Blynk" library: both use the same header names.
- If a sketch calls `Blynk.begin()` without a server, add it or define `BLYNK_DEFAULT_DOMAIN`.
- If a sketch relied on the default ports 80/443 (Blynk cloud), pass the port explicitly.
- TLS with a self-signed or private-CA server: add `Blynk.setRootCA()` or
  `Blynk.setFingerprint()`. ESP8266 fingerprints keep the 0.6.1 format (SHA-1 hex).
