#!/usr/bin/env python3
"""
Builds extras/tests/BlynkBuildTest on every board installed in arduino-cli
and keeps the results in extras/all-boards.json.

  python3 extras/build-boards.py              all the installed boards
  python3 extras/build-boards.py <fqbn>       one board

Needs arduino-cli in the PATH, with the board packages to test installed
(arduino-cli core install ...). The build log goes to ./build.log.

 Author:   Volodymyr Shymanskyy, Gabriele Lauricella
 License:  The MIT license
"""

import json
import os
import re
import subprocess
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
DATAFILE = os.path.join(ROOT, 'extras', 'all-boards.json')
SKETCH = os.path.join(ROOT, 'extras', 'tests', 'BlynkBuildTest')

# Board options that differ from the defaults
ADVANCED_BOARDS = {
    "attiny:avr:ATtinyX5": ":cpu=attiny85,clock=internal16",
    "attiny:avr:ATtinyX4": ":cpu=attiny84,clock=internal8",
}

SKIP_BOARDS = [
    # Hello world fails:
    "chipKIT:pic32:OpenScope",
    "chipKIT:pic32:openbci",
    "chipKIT:pic32:uno_pmod",
    "stm32duino:STM32F1:genericGD32F103C",
    "stm32duino:STM32F1:genericSTM32F103T",
    # __FlashString problem:
    "digistump:avr:digispark-tiny.*",
    # No c++0x:
    "stm32duino:STM32F3:discovery_f3",
]


def installed_boards():
    out = subprocess.run(['arduino-cli', 'board', 'listall', '--format', 'json'],
                         check=True, capture_output=True, text=True).stdout
    boards = json.loads(out).get('boards', [])
    return [{'name': b['name'], 'fqbn': b['fqbn']} for b in boards if b.get('fqbn')]


def main():
    try:
        with open(DATAFILE) as fp:
            json_data = json.load(fp)
    except (OSError, ValueError):
        json_data = {}
    data = {
        'failed': set(json_data.get('failed', [])),
        'built': set(json_data.get('built', [])),
    }

    only = sys.argv[1] if len(sys.argv) > 1 else None
    skip = re.compile('(' + ')|('.join(SKIP_BOARDS) + ')')

    boards = installed_boards()
    boards.sort(key=lambda b: b['fqbn'] in data['built'])

    with open('build.log', 'w') as log:
        for b in boards:
            if only:
                if b['fqbn'] != only:
                    continue
            elif skip.match(b['fqbn']):
                continue

            print('Building:', b['name'], '(' + b['fqbn'] + ') ...', end=' ', flush=True)
            log.write('\n\n=================\nBuilding: %s (%s)\n=================\n\n' % (b['name'], b['fqbn']))
            log.flush()

            fqbn = b['fqbn'] + ADVANCED_BOARDS.get(b['fqbn'], '')
            rc = subprocess.call(['arduino-cli', 'compile', '--fqbn', fqbn, '--library', ROOT, SKETCH],
                                 stdout=log, stderr=subprocess.STDOUT)
            if rc:
                print('Failed')
                data['failed'].add(b['fqbn'])
            else:
                print('OK')
                data['built'].add(b['fqbn'])
                data['failed'].discard(b['fqbn'])

            with open(DATAFILE, 'w') as fp:
                json.dump({
                    '_stats': {
                        'failed': len(data['failed']),
                        'built': len(data['built']),
                    },
                    'failed': sorted(data['failed']),
                    'built': sorted(data['built']),
                    'broken': sorted(data['built'] & data['failed']),
                }, fp, sort_keys=True, indent=4)

    print('==================')
    if data['built']:
        print(' OK:  ', len(data['built']))
    if data['failed']:
        print(' Fail:', len(data['failed']))


if __name__ == '__main__':
    main()
