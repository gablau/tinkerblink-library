# Linux client (Raspberry Pi, other boards, PC)

A small program that connects a Linux machine to a Blynk Legacy server, as a board does. `main.cpp`
is the sketch: it sends the uptime on V0 and prints what the app writes on V1. Change it for your
project and build again.

## Build

```bash
$ git clone https://github.com/gablau/tinkerblink-library.git
$ cd tinkerblink-library/linux
```

Then one of:

| Build | GPIO pins from the app | Needs |
|-------|------------------------|-------|
| `make` | no (virtual pins only) | `build-essential` |
| `make target=gpiod` | digital read/write through libgpiod | `build-essential pkg-config libgpiod-dev` |
| `make target=raspberry` | digital read/write and PWM through WiringPi (legacy) | [WiringPi](https://github.com/WiringPi/WiringPi) |

`./build.sh linux`, `./build.sh gpiod` and `./build.sh raspberry` install what is needed
(Debian, Raspberry Pi OS, Ubuntu) and build.

`target=gpiod` is the one to use on a current Raspberry Pi OS: WiringPi is no longer shipped. It
works with libgpiod 1.x (Raspberry Pi OS / Debian 12, Ubuntu 22.04 and 24.04) and 2.x (Debian
13): the Makefile picks the API from `pkg-config`.

## Run

```bash
$ ./blynk --token=YourAuthToken --server=myserver.example.com --port=8080
```

The default server and port are `myserver.example.com` and 8080.

With `target=gpiod`:

- the pins are the GPIO numbers of the chip: on a Raspberry Pi the BCM numbers, as in the app
  ("gp17" is GPIO17);
- the chip is `gpiochip0`; set `BLYNK_GPIOCHIP` for another one, e.g.
  `BLYNK_GPIOCHIP=gpiochip4 ./blynk ...` on a Raspberry Pi 5 with an older kernel;
- the user needs access to `/dev/gpiochip*` (the `gpio` group on Raspberry Pi OS), not root;
- analog write (PWM) is not available and is ignored.

`target=raspberry` needs `sudo` to reach the GPIO.

## Board name

The client reports "Linux" (`target=raspberry`: "Raspberry") as board name to the server. To send
another one, e.g. the name of the board in the app:

```bash
$ make target=gpiod DEVICE="Raspberry Pi 4 B/400"
```
