#!/bin/bash
# Installs what the build needs (Debian, Raspberry Pi OS, Ubuntu) and builds.

case "$1" in
linux)
    sudo apt-get update
    sudo apt-get install -y build-essential
    make clean all
    exit 0
    ;;
gpiod)
    sudo apt-get update
    sudo apt-get install -y build-essential pkg-config libgpiod-dev
    make clean all target=gpiod
    exit 0
    ;;
raspberry)
    # WiringPi (legacy): https://github.com/WiringPi/WiringPi
    sudo apt-get update
    sudo apt-get install -y build-essential git
    if ! hash gpio 2>/dev/null; then
        git clone https://github.com/WiringPi/WiringPi.git
        (cd WiringPi && ./build)
    fi
    gpio -v
    make clean all target=raspberry
    exit 0
    ;;
esac

echo "Please specify the build: linux, gpiod (GPIO with libgpiod), raspberry (GPIO with WiringPi)"
exit 1
