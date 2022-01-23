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

#include <Bounce2.h>
#include "constants.h"

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

/*
 * Initial setup - the setup() method runs once, when the sketch starts
 */
void setup() {
  Serial.begin(SERIAL_DATA_RATE);  
  delay(25);
  Serial.println("-- FootPedalKeyboard by bigman73, Version: " + FIRMWARE_VERSION + " ---");

  // initialize the digital pin as an output.
  pinMode(MAIN_LED_PEN, OUTPUT);

  setupLedPedalOutputPins();
  setupPedalInputPins();

  startupTestRoutine();

  Serial.println("-- Status: ready!");

  startTime = millis();
}

/*
 * Main loop - the loop() methor runs over and over again, as long as the board has power
 */
void loop() {
  updateDebouncers();
  
  handleStatusLed();

  handlePedalChanges();
  
  delay(25);
}

/*
 * Setup the pedals LED pins as output
 */
void setupLedPedalOutputPins() {
  for (int i = 0; i < (int) (sizeof(ledPedalPins) / sizeof(int)); i++) {
    int ledPedalPin = ledPedalPins[i];
    pinMode(ledPedalPin, OUTPUT);
    digitalWrite(ledPedalPin, LOW);
  }
}

/*
 * Setup the pedal pins as input with a debouncer
 */
void setupPedalInputPins() {
  for (int i = 0; i < NUM_PEDALS; i++) {
    int inputPedalPin = inputPedalPins[i];
    pinMode(inputPedalPin, INPUT_PULLUP);

    Bounce * pPedalDobouncer = pedalDebouncers[i];
    pPedalDobouncer->attach(inputPedalPin);
    pPedalDobouncer->interval(5); // interval in ms
  }
}

/*
 * Perform the initial startup self test routine that flashes the LEDs
 */
void startupTestRoutine() {
  int i;
  for (i = 0; i < NUM_PEDALS; i++) {
    int ledPedalPin = ledPedalPins[i];
    digitalWrite(ledPedalPin, HIGH);
    delay(STARTUP_TEST_DELAY);
  }

  delay(100);

  for (i = NUM_PEDALS; i >= 0; i--) {
    int ledPedalPin = ledPedalPins[i];
    digitalWrite(ledPedalPin, LOW);
    delay(STARTUP_TEST_DELAY);
  }
}

/*
 * Update and refresh the debouncers internal state
 */
void updateDebouncers() {
   for (int i = 0; i < NUM_PEDALS; i++) {
    Bounce * pPedalDebouncer = pedalDebouncers[i];
    pPedalDebouncer->update();
  }
}

/*
 * Handles state change for all pedals
 */
void handlePedalChanges() {
  for (int i = 0; i < NUM_PEDALS; i++) {
    Bounce * pPedalDebouncer = pedalDebouncers[i];
    int ledPedalPin = ledPedalPins[i];
    String pedal = String((char)((int) 'A' + i));
    handleButtonChange(pPedalDebouncer, ledPedalPin, pedal);
  }
}

/*
 * Handles state changes for a given pedal
 */
void handleButtonChange(Bounce * pDebouncerPedal, int ledPedalPin, String pedalName) {
   if ( pDebouncerPedal->fell() ) {
     Serial.println("Pedal " + pedalName + " pressed");
     digitalWrite(ledPedalPin, HIGH);
   } else if ( pDebouncerPedal->rose() ) {
     Serial.println("Pedal " + pedalName + " released");
     digitalWrite(ledPedalPin, LOW);
   }  
}


/*
 * Handles the status LED - turns it off and on in cycles
 */
void handleStatusLed() {
  elapsedTime = millis() - startTime;

  if (elapsedTime > STATUS_LED_BLINK_TIME) {
    ledState = 1 - ledState;
    if (ledState) {
      digitalWrite(MAIN_LED_PEN, HIGH);   // set the LED on    
    } else {
      digitalWrite(MAIN_LED_PEN, LOW);    // set the LED off
    }
    startTime = millis();
  }
}
