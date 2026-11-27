#!/usr/bin/env python3
"""
Builds the examples with arduino-cli, each one for the board in METADATA
(Arduino Uno when not listed).

  python3 extras/build-examples.py                 all the examples
  python3 extras/build-examples.py <name>.ino      one example

Needs arduino-cli in the PATH, with the board packages and the libraries
used by the examples installed. The build log goes to ./build.log.
"""

import fnmatch
import os
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

ESP8266 = { "fqbn": "esp8266:esp8266:nodemcuv2" }
ESP32 = { "fqbn": "esp32:esp32:esp32" }

metadata = {
  "Arduino_Yun.ino"             : { "fqbn": "arduino:avr:yun" },
  "Arduino_Zero_M0_Serial.ino"  : { "fqbn": "arduino:samd:arduino_zero_native" },
  "ENC28J60.ino"                : { "fqbn": "arduino:avr:nano:cpu=atmega328" },
  "ESP8266_Shield.ino"          : { "fqbn": "arduino:avr:mega:cpu=atmega2560" },
  "RN_XV_WiFly.ino"             : { "fqbn": "arduino:avr:leonardo" },
  "Serial_HM10_HC08.ino"        : { "fqbn": "arduino:avr:leonardo" },
  "Adafruit_Feather_32u4_BLE.ino": { "fqbn": "adafruit:avr:feather32u4" },
  "Seeed_EthernetV2_0.ino"      : { "fqbn": "arduino:avr:uno" },

  # arduino.org
  "Arduino_Ethernet2.ino"       : { "skip": True },
  "Arduino_org_UNO_WiFi.ino"    : { "fqbn": "arduino:avr:unowifi" },

  # ESP8266
  "Sparkfun_Blynk_Board.ino"    : ESP8266,
  "NodeMCU.ino"                 : ESP8266,
  "ESP8266_DirectConnect.ino"   : ESP8266,
  "ESP8266_Standalone.ino"      : ESP8266,
  "ESP8266_Standalone_Manual_IP.ino"  : ESP8266,
  "ESP8266_Standalone_SmartConfig.ino"  : ESP8266,
  "ESP8266_Standalone_SSL.ino"  : ESP8266,

  "ESP8266_Station.ino"         : ESP8266,
  "ESP8266_AccessPoint.ino"     : ESP8266,
  "ESP8266_WiFi.ino"            : ESP8266,

  "ESP32_WiFi.ino"              : ESP32,
  "ESP32_WiFi_SSL.ino"          : ESP32,

  "myPlant_ESP8266.ino"         : ESP8266,
  "Template_ESP8266.ino"        : ESP8266,
  "Template_MKR1000.ino"        : { "fqbn": "arduino:samd:mkr1000" },

  # TheAirBoard
  "TheAirBoard_WiFly.ino"       : { "fqbn": "arduino:avr:fio" },
  "TheAirBoard_BLE_Link.ino"    : { "fqbn": "arduino:avr:fio" },

  # Digistump
  "Digistump_Digispark.ino"     : { "fqbn": "digistump:avr:digispark-pro" },
  "Digistump_Oak.ino"           : { "fqbn": "digistump:oak:oak1:CpuFrequency=80,UploadTool=oak_ota,FlashSize=OAK,RomConfig=Full" },

  # Intel
  "Intel_Edison_WiFi.ino"       : { "fqbn": "Intel:i686:izmir_ec" },
  "Intel_Galileo.ino"           : { "fqbn": "Intel:i586:izmir_fg" },
  "Arduino_101_BLE.ino"         : { "fqbn": "Intel:arc32:arduino_101" },

  # RBL
  "RedBear_Duo_WiFi.ino"        : { "fqbn": "RedBear:STM32F2:RedBear_Duo" },
  "RedBear_Duo_BLE.ino"         : { "fqbn": "RedBear:STM32F2:RedBear_Duo" },
  "RedBearLab_BLE_Nano.ino"     : { "fqbn": "RedBear:nRF51822:nRF51822_NANO_32KB" },
  "RedBearLab_BlendMicro.ino"   : { "fqbn": "RedBear:avr:blendmicro8" },
  "RedBearLab_BLE_Mini.ino"     : { "fqbn": "arduino:avr:leonardo" },

  # GSM
  "SIM800_SIM900.ino"           : { "fqbn": "arduino:avr:leonardo" },

  #Other
  "Blue_Pill_STM32F103C.ino"    : { "fqbn": "stm32duino:STM32F1:genericSTM32F103C:device_variant=STM32F103C8" },
  "Fishino.ino"                 : { "skip": True },  # requires extra lib
  "BBC_MicroBit.ino"            : { "fqbn": "sandeepmistry:nRF5:BBCmicrobit:softdevice=s130" },
  "Simblee_BLE.ino"             : { "fqbn": "Simblee:Simblee:Simblee" },
  "RFDuino_BLE.ino"             : { "fqbn": "RFduino:RFduino:RFduino" },
  "DFRobot_Bluno_BLE_Link.ino"  : { "fqbn": "arduino:avr:leonardo" },
  "Microduino_BLE.ino"          : { "fqbn": "arduino:avr:leonardo" },
  "TinyDuino_WiFi.ino"          : { "fqbn": "arduino:avr:pro:cpu=8MHzatmega328" },
  "WildFire_V3.ino"             : { "fqbn": "WildFire- 4.0.0:avr:wildfireo3" },
  "WildFire_V4.ino"             : { "fqbn": "WildFire- 4.0.0:avr:wildfireo3" },
  "chipKIT_Uno32.ino"           : { "fqbn": "chipKIT:pic32:uno_pic32" },
  "LightBlue_Bean.ino"          : { "fqbn": "LightBlue-Bean:avr:beanplus" },
  "Teensy3.ino"                 : { "fqbn": "teensy:avr:teensy31:usb=serial,speed=96,opt=o2std,keys=en-us" },
  "ATtiny85.ino"                : { "fqbn": "attiny:avr:ATtinyX5:cpu=attiny85,clock=internal8" },

  # Special examples
  "ESP8266.ino"                 : ESP8266,
  "HandleDisconnect.ino"        : ESP8266,

  # No linux support
  "LinkItONE.ino"               : { "skip": True },

  # Energia: not supported by arduino-cli
  "Energia_WiFi.ino"            : { "skip": True },
  "Energia_BLE.ino"             : { "skip": True },
  "Energia_Ethernet.ino"        : { "skip": True },
  "Energia_Serial_USB.ino"      : { "skip": True },
}


def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                yield os.path.join(root, basename)


def main():
    only = sys.argv[1] if len(sys.argv) > 1 else None
    failed, built, skipped = [], [], []

    with open('build.log', 'w') as log:
        for fn in sorted(find_files(os.path.join(ROOT, 'examples'), '*.ino')):
            path, ino = os.path.split(fn)
            m = metadata.get(ino, {})
            if ino in metadata:
                m['sketch_found'] = True
            if only and ino != only:
                continue
            if m.get('skip'):
                skipped.append(ino)
                continue
            fqbn = m.get('fqbn', 'arduino:avr:uno')

            log.write('\n\n=================\nBuilding: %s (%s)\n=================\n\n' % (ino, fqbn))
            log.flush()
            print('Building:', ino, '...', end=' ', flush=True)

            rc = subprocess.call(['arduino-cli', 'compile', '--fqbn', fqbn, '--library', ROOT, path],
                                 stdout=log, stderr=subprocess.STDOUT)
            if rc:
                print('Failed')
                failed.append(ino)
            else:
                print('OK')
                built.append(ino)

    print('==================')
    if skipped:
        print(' Skipped:', len(skipped))
    if not only:
        for ino, m in metadata.items():
            if 'sketch_found' not in m:
                print(' Sketch not found:', ino)
    if failed:
        print(' Failed:', failed)
        sys.exit(1)
    print(' All', len(built), 'examples OK')


if __name__ == '__main__':
    main()
