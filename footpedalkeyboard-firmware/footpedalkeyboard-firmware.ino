/* =============================================================================
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
=============================================================================== */

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

const String FIRMWARE_VERSION = "0.0.3";

// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;
const int STATUS_LED_BLINK_TIME = 400; // msec

const int ledPedalAPin = 8;
const int ledPedalBPin = 9;
const int ledPedalCPin = 10;
const int ledPedalDPin = 11;
const int ledPedalEPin = 12;

const int NUM_PEDALS = 5;
const int ledPedalPins[] = {ledPedalAPin, ledPedalBPin, ledPedalCPin, ledPedalDPin, ledPedalEPin};

const int pedalAPin = 23; // Purple
const int pedalBPin = 22; // Orange
const int pedalCPin = 21; // Grey
const int pedalDPin = 20; // Green
const int pedalEPin = 19; // Blue

const int inputPedalPins[] = {pedalAPin, pedalBPin, pedalCPin, pedalDPin, pedalEPin};

// Instantiate a Bounce object
Bounce debouncerPedalA = Bounce();
Bounce debouncerPedalB = Bounce();
Bounce debouncerPedalC = Bounce();
Bounce debouncerPedalD = Bounce();
Bounce debouncerPedalE = Bounce();

Bounce * pedalDebouncers[] = {&debouncerPedalA, &debouncerPedalB, &debouncerPedalC, &debouncerPedalD, &debouncerPedalE};

int ledState = 0;
int buttonState = 0;         // variable for reading the pushbutton status
int pedalState = 0;         // variable for reading the pedal wire status
unsigned long startTime;
unsigned long elapsedTime;


// the setup() method runs once, when the sketch starts

void setup() {
  Serial.begin(9600);  
  delay(25);
  Serial.println("-- FootPedalKeyboard, Version: " + FIRMWARE_VERSION + " ---");

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);

  setupLedPedalPins();

  setupPedalPins();

  startupTestRoutine();

  Serial.println("-- Status: Ready to roll!");

  startTime = millis();
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  updateDebouncers();
  
  elapsedTime = millis() - startTime;

  if (elapsedTime > STATUS_LED_BLINK_TIME) {
    ledState = 1 - ledState;
    if (ledState) {
      digitalWrite(ledPin, HIGH);   // set the LED on    
    } else {
      digitalWrite(ledPin, LOW);    // set the LED off
    }
    startTime = millis();
  }

  handleButtonsChanges();
  
  delay(25);
}

void setupLedPedalPins() {
  for (int i = 0; i < (int) (sizeof(ledPedalPins) / sizeof(int)); i++) {
    int ledPedalPin = ledPedalPins[i];
    pinMode(ledPedalPin, OUTPUT);
    digitalWrite(ledPedalPin, LOW);
  }
}

void setupPedalPins() {
  for (int i = 0; i < NUM_PEDALS; i++) {
    int inputPedalPin = inputPedalPins[i];
    pinMode(inputPedalPin, INPUT_PULLUP);

    Bounce * pPedalDobouncer = pedalDebouncers[i];
    pPedalDobouncer->attach(inputPedalPin);
    pPedalDobouncer->interval(5); // interval in ms
  }
}

void startupTestRoutine() {
  int i;
  for (i = 0; i < NUM_PEDALS; i++) {
    int ledPedalPin = ledPedalPins[i];
    digitalWrite(ledPedalPin, HIGH);
    delay(150);
  }

  delay(100);

  for (i = NUM_PEDALS; i >= 0; i--) {
    int ledPedalPin = ledPedalPins[i];
    digitalWrite(ledPedalPin, LOW);
    delay(150);
  }
}

void updateDebouncers() {
   for (int i = 0; i < NUM_PEDALS; i++) {
    Bounce * pPedalDebouncer = pedalDebouncers[i];
    pPedalDebouncer->update();
  }
}

void handleButtonsChanges() {
  for (int i = 0; i < NUM_PEDALS; i++) {
    Bounce * pPedalDebouncer = pedalDebouncers[i];
    int ledPedalPin = ledPedalPins[i];
    String pedal = (char)((int) 'A' + i);
    handleButtonChange(pPedalDebouncer, ledPedalPin, pedal);
  }
}

void handleButtonChange(Bounce * pDebouncerPedal, int ledPedalPin, String pedalName) {
   if ( pDebouncerPedal->fell() ) {
     Serial.println("Pedal " + pedalName + " pressed");
     digitalWrite(ledPedalPin, HIGH);
   } else if ( pDebouncerPedal->rose() ) {
     Serial.println("Pedal " + pedalName + " released");
     digitalWrite(ledPedalPin, LOW);
   }  
}
