const String FIRMWARE_VERSION = "1.0.0";

// Teensy 3.x / Teensy LC have the LED on pin 13
const int MAIN_LED_PEN = 13;
const int STATUS_LED_BLINK_TIME = 400; // msec
const int SERIAL_DATA_RATE = 9600; // baud
const int STARTUP_TEST_DELAY = 150; // msec

const int LED_PEDAL_PIN_A = 8;
const int LED_PEDAL_PIN_B = 9;
const int LED_PEDAL_PIN_C = 10;
const int LED_PEDAL_PIN_D = 11;
const int LED_PEDAL_PIN_E = 12;

const int NUM_PEDALS = 5;
const int ledPedalPins[] = {LED_PEDAL_PIN_A, LED_PEDAL_PIN_B, LED_PEDAL_PIN_C, LED_PEDAL_PIN_D, LED_PEDAL_PIN_E};

// See README.md of firmware for Korg EC-5 cable pins mapping
const int INPUT_PEDAL_PIN_A = 23; // Purple
const int INPUT_PEDAL_PIN_B = 22; // Orange
const int INPUT_PEDAL_PIN_C = 21; // Grey
const int INPUT_PEDAL_PIN_D = 20; // Green
const int INPUT_PEDAL_PIN_E = 19; // Blue

const int inputPedalPins[] = {INPUT_PEDAL_PIN_A, INPUT_PEDAL_PIN_B, INPUT_PEDAL_PIN_C, INPUT_PEDAL_PIN_D, INPUT_PEDAL_PIN_E};
