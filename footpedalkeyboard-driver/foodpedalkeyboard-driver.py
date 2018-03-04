""" =============================================================================
MIT License

Copyright (c) 2018 Yuval Naveh (bigman73)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
=============================================================================== """

import sys
import serial
from serial.tools import list_ports
from pyautogui import press, keyDown, keyUp
import re
from window_services import get_active_window
import json
from pprint import pprint

DRIVER_VERSION = "0.0.2"

# Teensy USB serial microcontroller program id data:
VENDOR_ID = "16C0"
PRODUCT_ID = "0483"
TEENSY_SIGNATURE = "USB VID:PID={0}:{1}".format(VENDOR_ID, PRODUCT_ID)

MAX_WINDOW_TITLE_CACHE_SIZE = 50

event_regex = re.compile('Pedal\s([A-E])\s(pressed|released)')

# Key: Mapping application name, Value: application mapping data
applications = {}

# Key: Window title, Value: Mapping application name
title_application_mapping_cache = {}


def find_teensy_port():
    print('Searching for a teensy COM port:')
    for port in list(list_ports.comports()):
        print(port, "=>", port[2])
        if port[2].startswith(TEENSY_SIGNATURE):
            return port[0]

    return None


def listen_to_teensy_serial(teensy_port):
    ser = serial.Serial(port=teensy_port, baudrate=9600, timeout=0.1)
    print('Listening to incoming serial messages...')
    print('-' * 80)

    while True:
        arduino_messsage = ser.readline()
        if arduino_messsage and len(arduino_messsage) > 0:
            event = arduino_messsage.decode("utf-8")
            event = event.replace('\r\n', '')
            process_footpedalkeyboard_event(event)
            print()


def process_footpedalkeyboard_event(event):
    print("RECEIVED: " + event)

    m = event_regex.match(event)
    if m:
        pedal = m.group(1)
        # print("[DEBUG] Pedal =", pedal)
        pedal_action = m.group(2)
        # print("[DEBUG] Action =", action)

        active_window_title = get_active_window()
        print("Active window:", active_window_title)

        matching_application = None

        cached_application_name = title_application_mapping_cache.get(active_window_title)

        if cached_application_name:
            print("Using a cached application =>", cached_application_name)
            matching_application = applications[cached_application_name]
        else:
            for application_name, application in applications.items():
                window_title_compiled_regex = application['compiled_regex']
                if window_title_compiled_regex.match(active_window_title):
                    print("Found a matching application =>", application_name)
                    matching_application = application
                    # Add to cache for next time
                    if title_application_mapping_cache.__len__() > MAX_WINDOW_TITLE_CACHE_SIZE:
                        title_application_mapping_cache.clear()
                    title_application_mapping_cache[active_window_title] = application['application_name']
                    break

        if matching_application:
            pedal_action_mappings = matching_application['mappings']['pedal' + pedal]
            if pedal_action_mappings:
                action_instructions = pedal_action_mappings.get(pedal_action)
                if action_instructions:
                    instruction_type = action_instructions['type']
                    if instruction_type == 'press':
                        keys = action_instructions['keys']
                        print("PRESS keys:", keys)
                        press(keys)
                    elif instruction_type == 'hotkey':
                        keys = action_instructions['keys']
                        for key in keys:
                            keyDown(key)
                        for key in keys:
                            keyUp(key)


def read_mapping_config_file():
    config_data = json.load(open('key-mappings.json'))
    pprint(config_data)

    for application_config in config_data['applications']:
        print(application_config)
        application_name = application_config['application_name']
        applications[application_name] = application_config
        window_title_regex = application_config['window_title_regex']
        application_config['compiled_regex'] = re.compile(window_title_regex)


if __name__ == "__main__":
    print('--- Foot Pedal Keyboard :: Driver, Version {0} ---'.format(DRIVER_VERSION))
    _teensy_port = find_teensy_port()
    if _teensy_port:
        print("GREAT SUCCESS: Teensy found on port {0}".format(_teensy_port))
        print()

        read_mapping_config_file()

        listen_to_teensy_serial(_teensy_port)
    else:
        print("No compatible Teensy found. Aborting.")

    sys.exit(1)
