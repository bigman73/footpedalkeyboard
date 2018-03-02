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
from pyautogui import press #, typewrite, hotkey
import re
from window_services import get_active_window

DRIVER_VERSION = "0.0.1"

# Teensy USB serial microcontroller program id data:
VENDOR_ID = "16C0"
PRODUCT_ID = "0483"
TEENSY_SIGNATURE = "USB VID:PID={0}:{1}".format(VENDOR_ID, PRODUCT_ID)

event_regex = re.compile('Pedal\s([A-E])\s(pressed|released)')

practicesharp_title_regex = re.compile('Practice# (\d*.\d*.\d*.\d*)')


def getTeensyPort():
    print('Searching for a teensy COM port:')
    for port in list(list_ports.comports()):
        print(port, "=>", port[2])
        if port[2].startswith(TEENSY_SIGNATURE):
            return port[0]


def listen_to_teensy_serial(teensy_port):
    ser = serial.Serial(port=teensy_port, baudrate=9600, timeout=0.1)
    #print('Serial port = ' + ser.name) #print the port used
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
        action = m.group(2)
        # print("[DEBUG] Action =", action)

        active_window_title = get_active_window()
        print("Active window:", active_window_title)

        if practicesharp_title_regex.match(active_window_title):
            print(">> Practice# key mapping")
            # TODO: Change with dynamic json map
            if action == 'released':
                if pedal == 'A':
                    press('l')
                elif pedal == 'B':
                    press(',')
                elif pedal == 'C':
                    press('space')
                elif pedal == 'D':
                    press('s')
                elif pedal == 'E':
                    press('f')


if __name__ == "__main__":
    print('--- Foot Pedal Keyboard :: Driver, Version {0} ---'.format(DRIVER_VERSION))
    teensy_port = getTeensyPort()
    if teensy_port:
        print("GREAT SUCCESS: Teensy found on port {0}".format(teensy_port))
        print()

        # TODO: Read keyboard JSON mapping file

        listen_to_teensy_serial(teensy_port)
    else:
        print("No compatible Teensy found. Aborting.")

    sys.exit(1)
