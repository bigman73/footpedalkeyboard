import sys
import serial
from serial.tools import list_ports
from pyautogui import press #, typewrite, hotkey
import re

DRIVER_VERSION = "0.0.1"

# Teensy USB serial microcontroller program id data:
VENDOR_ID = "16C0"
PRODUCT_ID = "0483"
TEENSY_SIGNATURE = "USB VID:PID={0}:{1}".format(VENDOR_ID, PRODUCT_ID)

event_regex = re.compile('Pedal\s([A-E])\s(pressed|released)')


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
        print("Pedal =", pedal)
        action = m.group(2)
        print("Action =", action)

        # TODO: Change with dynamic json map
        if action == 'released':
            if pedal == 'A':
                press('l')
            elif pedal == 'B':
                press('space')
            elif pedal == 'C':
                press('s')
            elif pedal == 'D':
                press('f')
            else:
                press(pedal)


if __name__ == "__main__":
    print('--- Foot Pedal Keyboard :: Driver, Version {0} ---'.format(DRIVER_VERSION))
    teensy_port = getTeensyPort()
    if teensy_port:
        print("GREAT SUCCESS: Teensy found on port {0}".format(teensy_port))
        print()
        
        listen_to_teensy_serial(teensy_port)
    else:
        print("No compatible Teensy found. Aborting.")

    sys.exit(1)
