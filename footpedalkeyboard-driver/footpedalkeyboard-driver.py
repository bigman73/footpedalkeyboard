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
from pyautogui import press, keyDown, keyUp, typewrite
import re
from window_services import get_active_window
import json
import psutil
import win32process
import win32gui
import time

DRIVER_VERSION = "1.1.1"

# Teensy USB serial microcontroller program id data:
VENDOR_ID = "16C0"
PRODUCT_ID = "0483"
TEENSY_SIGNATURE = "USB VID:PID={0}:{1}".format(VENDOR_ID, PRODUCT_ID)

MAX_CACHE_SIZE = 50

event_regex = re.compile('Pedal\\s([A-E])\\s(pressed|released)')

# Key: Mapping application name, Value: application mapping data
applications = {}

# Key: Window title, Value: Mapping application name
title_application_mapping_cache = {}

# Key: Process Name, Value: Mapping application name
process_application_mapping_cache = {}


def get_active_window_process_name():
    """
    Gets the process name for the currently active window
    """

    # This produces a list of PIDs active window relates to
    pid = win32process.GetWindowThreadProcessId(win32gui.GetForegroundWindow())
    # pid[-1] is the most likely to survive last longer
    process_name = psutil.Process(pid[-1]).name()

    return process_name


def find_teensy_port():
    """
    Finds the teensy COM port
    """

    print('Searching for a teensy COM port:')
    for port in list(list_ports.comports()):
        print(port, "=>", port[2])
        if port[2].startswith(TEENSY_SIGNATURE):
            return port[0]

    return None


def listen_to_teensy_serial(teensy_port):
    """
    Listens to teensy serial messages
    """

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
    """
    Processes an event coming from the foot pedal keyboard
    """

    print("RECEIVED KEYBOARD EVENT: " + event)

    matchingEvent = event_regex.match(event)
    if matchingEvent:
        pedal = matchingEvent.group(1)
        pedal_action = matchingEvent.group(2)

        active_window_title = get_active_window()
        print("Active window:", active_window_title)

        active_window_process_name = get_active_window_process_name()
        print("Active process:", active_window_process_name)

        matching_application = None

        cached_application_name = title_application_mapping_cache.get(
            active_window_title)

        # Lookup cache by active window title
        if cached_application_name:
            print("Using a cached application (by window title) =>",
                  cached_application_name)
            matching_application = applications[cached_application_name]

        # Lookup cache by process name of active window
        if not matching_application:
            cached_application_name = process_application_mapping_cache.get(
                active_window_process_name)
            if cached_application_name:
                print("Using a cached application (by process name) =>",
                      cached_application_name)
                matching_application = applications[cached_application_name]

        # Match application by active window's title
        if not matching_application:
            for application_name, application in applications.items():
                if not 'window_title_compiled_regex' in application:
                    continue

                window_title_compiled_regex = application['window_title_compiled_regex']
                if window_title_compiled_regex.match(active_window_title):
                    print("Found a matching application by window title =>",
                          application_name)
                    matching_application = application
                    # Add to cache for next time
                    if title_application_mapping_cache.__len__() > MAX_CACHE_SIZE:
                        title_application_mapping_cache.clear()
                    title_application_mapping_cache[active_window_title] = application['application_name']
                    break

        # Match application by active window's process name
        if not matching_application:
            for application_name, application in applications.items():
                if not 'process_name_compiled_regex' in application:
                    continue

                process_name_compiled_regex = application['process_name_compiled_regex']
                if process_name_compiled_regex.match(active_window_process_name):
                    print("Found a matching application by process name =>",
                          application_name)
                    matching_application = application
                    # Add to cache for next time
                    if process_application_mapping_cache.__len__() > MAX_CACHE_SIZE:
                        process_application_mapping_cache.clear()
                    process_application_mapping_cache[active_window_process_name] = application['application_name']
                    break

        if matching_application:
            apply_application_keys(matching_application, pedal, pedal_action)


def apply_application_keys(matching_application, pedal, pedal_action):
    """
    Applies the matching applications keys to the operating system
    """

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
                print("Hot key:", keys)
                for key in keys:
                    keyDown(key)
                for key in keys:
                    keyUp(key)
            elif instruction_type == 'typewrite':
                text = action_instructions['text']
                interval = action_instructions['interval']
                typewrite(text, interval)


def read_mapping_config_file():
    """
    Reads the key mappings JSON configuration file
    """

    config_data = json.load(open('key-mappings.json'))

    all_applications_config = config_data['applications']
    print('Found {0} configured applications'.format(
        len(all_applications_config)))

    for application_config in all_applications_config:
        application_name = application_config['application_name']
        applications[application_name] = application_config

        if 'window_title_regex' in application_config:
            window_title_regex = application_config['window_title_regex']
            application_config['window_title_compiled_regex'] = re.compile(
                window_title_regex)

        if 'process_name_regex' in application_config:
            process_name_regex = application_config['process_name_regex']
            application_config['process_name_compiled_regex'] = re.compile(
                process_name_regex)


# Main program
if __name__ == "__main__":
    print(
        '--- Foot Pedal Keyboard :: Driver, Version {0} ---'.format(DRIVER_VERSION))
    _teensy_port = find_teensy_port()
    if _teensy_port:
        print("GREAT SUCCESS: Teensy found on port {0}".format(_teensy_port))
        print()

        read_mapping_config_file()

        listen_to_teensy_serial(_teensy_port)
    else:
        print("TOTAL FAIL NO WORKY: No compatible Teensy found. Aborting.")

    sys.exit(1)
