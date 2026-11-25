# TinkerBlink Library

[![Build](https://github.com/gablau/tinkerblink-library/actions/workflows/build.yml/badge.svg)](https://github.com/gablau/tinkerblink-library/actions/workflows/build.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Arduino/C++ library that connects ESP8266, ESP32, Arduino and many other boards to a **self-hosted
Blynk Legacy server**: [blynk-server](https://github.com/blynkkk/blynk-server) 0.41.x and the
maintained [0.50.x fork](https://github.com/gablau/blynk-server).

It is the maintained continuation of the Blynk library **0.6.1**, the last version for the legacy
platform. Blynk moved to Blynk IoT, whose library (1.x) speaks to `blynk.cloud` and does not work
with legacy servers; 0.6.1 stayed frozen with known bugs and TLS code that no longer builds with
current ESP8266 cores. This library fixes that and keeps the legacy protocol unchanged.

It is the companion of [TinkerBlink](https://github.com/gablau/tinkerblink), a mobile app for
self-hosted Blynk Legacy servers, whose widget docs link to the examples here.

> TinkerBlink Library is an independent project, not affiliated with or endorsed by Blynk Inc.
> "Blynk" is a trademark of its respective owners and is used here only to describe compatibility.

## Contents

- [Compatibility](#compatibility)
- [Installation](#installation)
- [Quick start](#quick-start)
- [Server address and ports](#server-address-and-ports)
- [TLS on ESP8266 and ESP32](#tls-on-esp8266-and-esp32)
- [Examples](#examples)
- [Supported hardware](#supported-hardware)
- [Related projects](#related-projects)
- [Contributing](#contributing)
- [License](#license)

## Compatibility

- **Servers**: blynk-server 0.41.x (last original release 0.41.17) and the 0.50.x fork.
- **Protocol**: the same as 0.6.1. Sketches written for 0.6.1 build unchanged in most cases.
- **Differences from 0.6.1** that can affect an existing sketch (details in
  [CHANGELOG.md](CHANGELOG.md)):
  - the default server is no longer `blynk-cloud.com`: pass your server to `Blynk.begin()` (most
    sketches already do) or define it once, see [below](#server-address-and-ports);
  - the default ports are the ones of blynk-server, 8080 and 9443, instead of 80 and 443;
  - with TLS on ESP8266 and ESP32 the default trusted CA is ISRG Root X1 (Let's Encrypt); a server
    with its own or self-signed certificate needs one line, see [TLS](#tls-on-esp8266-and-esp32).

## Installation

The library uses the same header names as the official Blynk library (`BlynkSimpleEsp8266.h` and so
on). **Uninstall the "Blynk" library** from the Library Manager first, or the IDE may pick the wrong
one.

**Arduino IDE**: download the ZIP of the latest release from
[Releases](https://github.com/gablau/tinkerblink-library/releases) and add it with
*Sketch → Include Library → Add .ZIP Library...*. You can also clone the repository into the
`libraries` folder of your sketchbook.

**PlatformIO**, in `platformio.ini`:

```ini
lib_deps = https://github.com/gablau/tinkerblink-library.git
```

**arduino-cli** (needs `library.enable_unsafe_install: true` in its configuration):

```sh
arduino-cli lib install --git-url https://github.com/gablau/tinkerblink-library.git
```

## Quick start

1. In your app (TinkerBlink or the original Blynk Legacy app, connected to your server), create a
   project and copy the **auth token** of its device.
2. In the IDE open *File → Examples → TinkerBlink Library → Boards_WiFi → ESP8266_Standalone* (or
   the example for your board).
3. Set the token, your WiFi network and your server:

```cpp
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "myserver.example.com", 8080);
}

void loop()
{
  Blynk.run();
}
```

4. Upload it and open the serial monitor: `Ready` means the board is online.

## Server address and ports

blynk-server listens for boards on two ports:

| Port | Server setting | Use |
|------|----------------|-----|
| 8080 | `http.port`    | plain TCP: the token travels in clear, use it on your LAN |
| 9443 | `https.port`   | TLS: use it when the board reaches the server over the Internet |

You can pass the server to every `Blynk.begin()`, or define it once before including the library;
then `Blynk.begin(auth, ssid, pass)` uses it:

```cpp
#define BLYNK_DEFAULT_DOMAIN   "myserver.example.com"
#define BLYNK_DEFAULT_PORT     8080   // plain TCP
#define BLYNK_DEFAULT_PORT_SSL 9443   // TLS

#include <BlynkSimpleEsp8266.h>
```

An IP address works too: `Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080)`.

## TLS on ESP8266 and ESP32

Use `BlynkSimpleEsp8266_SSL.h` or `BlynkSimpleEsp32_SSL.h` and port 9443. By default the server
certificate must chain to **ISRG Root X1**, so a server with a Let's Encrypt certificate works with
no extra code. For other servers pick one mode, either with a call before `Blynk.begin()` or with a
define before the include:

| Server certificate | Call before `Blynk.begin()` | Or define |
|--------------------|-----------------------------|-----------|
| Let's Encrypt | nothing | nothing |
| Own CA, or self-signed | `Blynk.setRootCA(ca_pem)` | `BLYNK_SSL_ROOT_CA ca_pem` |
| Any, pinned by fingerprint | `Blynk.setFingerprint("AB:CD:...")` | `BLYNK_SSL_FINGERPRINT "AB:CD:..."` |
| Any, not verified (testing only) | `Blynk.setInsecure()` | `BLYNK_SSL_INSECURE` |

- `ca_pem` is the CA certificate (or the self-signed server certificate) as PEM text, declared
  before the include:

  ```cpp
  static const char ca_pem[] PROGMEM = R"EOF(
  -----BEGIN CERTIFICATE-----
  ...
  -----END CERTIFICATE-----
  )EOF";
  ```

- The fingerprint is **SHA-1 on ESP8266** and **SHA-256 on ESP32**, in hex, with or without spaces
  or colons. To read it from the server:

  ```sh
  openssl s_client -connect myserver.example.com:9443 </dev/null 2>/dev/null \
    | openssl x509 -noout -fingerprint -sha1      # -sha256 for ESP32
  ```

  A fingerprint changes every time the certificate is renewed (every 90 days with Let's Encrypt).

- With a CA, the certificate must be valid for the server name used in `Blynk.begin()`.
- On ESP8266 the CA mode first sets the clock via NTP (`pool.ntp.org`, `time.nist.gov`), to check
  the certificate dates. `BLYNK_SSL_NTP_SERVER1`, `BLYNK_SSL_NTP_SERVER2` and
  `BLYNK_SSL_NTP_TIMEOUT_MS` change servers and timeout.
- TLS on ESP8266 needs ESP8266 Arduino core 2.5.0 or newer (BearSSL) and about 25 KB of free heap.
- The certificate built into blynk-server is meant for testing only: give the server a real one
  (see its README), or at least pin its fingerprint.

Other boards with TLS (WiFiNINA, MKR1000, Ethernet SSL) verify the server with the certificates
stored in their WiFi module or library, as in 0.6.1.

## Examples

The [examples](examples) folder has a sketch for every connection type (`Boards_*`), the basics
(`GettingStarted`), the widgets (`Widgets`) and more (`More`). Any board example can be combined with
any other example: replace the connection part.

The widget docs of the TinkerBlink app link to the sketches in `GettingStarted`, `Widgets` and
`More/Sync`; their paths stay stable.

## Supported hardware

The boards and connections of 0.6.1: ESP8266 and ESP32 (WiFi, BLE, Bluetooth), Arduino boards with
Ethernet, WiFi, WiFiNINA, WiFi101 and GSM shields or modules (TinyGSM), Serial and USB links, BLE
modules, Particle, Energia, Raspberry Pi and Linux (see [linux](linux)).

Support for newer boards (Raspberry Pi Pico W, Arduino UNO R4 WiFi, ESP32-S2/S3/C3/C6, Nano ESP32,
GIGA R1 WiFi) is planned.

## Related projects

- [TinkerBlink](https://github.com/gablau/tinkerblink): the mobile app.
- [blynk-server](https://github.com/gablau/blynk-server): maintained fork of the Blynk Legacy
  server (0.50.x).
- [tinkerblink-boards](https://github.com/gablau/tinkerblink-boards): the board list and pins
  used by the app.
- [blynkkk/blynk-library](https://github.com/blynkkk/blynk-library): the original library. Its
  current version is for Blynk IoT; fixes that also apply to the legacy protocol are brought here.

## Contributing

Bug reports and pull requests are welcome. The library must keep working with blynk-server 0.41.x
and 0.50.x: a change to what the board sends needs a check against the server sources. Every push
is built by [GitHub Actions](.github/workflows/build.yml) on AVR, ESP8266 and ESP32.

## License

MIT, see [LICENSE](LICENSE). Based on the Blynk library by Volodymyr Shymanskyy.
