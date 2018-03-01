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

// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;

const int ledPedalAPin = 18;
const int ledPedalBPin = 17;
const int ledPedalCPin = 16;
const int ledPedalDPin = 15;
const int ledPedalEPin = 14;

const int buttonPin = 17;

const int pedalAPin = 23; // Purple
const int pedalBPin = 22; // Orange
const int pedalCPin = 21;
const int pedalDPin = 20;
const int pedalEPin = 19;

// Instantiate a Bounce object
Bounce debouncerPedalA = Bounce();
Bounce debouncerPedalB = Bounce();
Bounce debouncerPedalC = Bounce();
Bounce debouncerPedalD = Bounce();
Bounce debouncerPedalE = Bounce();

int ledState = 0;
int buttonState = 0;         // variable for reading the pushbutton status
int pedalState = 0;         // variable for reading the pedal wire status
unsigned long startTime;
unsigned long elapsedTime;


// the setup() method runs once, when the sketch starts

void setup() {
  Serial.begin(9600);  
  Serial.println("-- FootPedalKeyboard, Version: 0.01 ---");

  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPedalAPin, OUTPUT);
  pinMode(ledPedalBPin, OUTPUT);
  pinMode(ledPedalCPin, OUTPUT);
  pinMode(ledPedalDPin, OUTPUT);
  pinMode(ledPedalEPin, OUTPUT);

  pinMode(buttonPin, INPUT);
  pinMode(pedalAPin, INPUT_PULLUP);
  pinMode(pedalBPin, INPUT_PULLUP);
  pinMode(pedalCPin, INPUT_PULLUP);
  pinMode(pedalDPin, INPUT_PULLUP);
  pinMode(pedalEPin, INPUT_PULLUP);

  // After setting up the button, setup the Bounce instance:
  debouncerPedalA.attach(pedalAPin);
  debouncerPedalA.interval(5); // interval in ms
  debouncerPedalB.attach(pedalBPin);
  debouncerPedalB.interval(5); // interval in ms
  debouncerPedalC.attach(pedalCPin);
  debouncerPedalC.interval(5); // interval in ms
  debouncerPedalD.attach(pedalDPin);
  debouncerPedalD.interval(5); // interval in ms
  debouncerPedalE.attach(pedalEPin);
  debouncerPedalE.interval(5); // interval in ms

  digitalWrite(ledPedalAPin, LOW);
  digitalWrite(ledPedalBPin, LOW);
  digitalWrite(ledPedalCPin, LOW);
  digitalWrite(ledPedalDPin, LOW);
  digitalWrite(ledPedalEPin, LOW);
  
  startTime = millis();
}

// the loop() methor runs over and over again,
// as long as the board has power

void loop() {
  // Update the Bounce instances
  debouncerPedalA.update();
  debouncerPedalB.update();
  debouncerPedalC.update();
  debouncerPedalD.update();
  debouncerPedalE.update();
  
  elapsedTime = millis() - startTime;

  if (elapsedTime > 400) {
    ledState = 1 - ledState;
    if (ledState) {
      digitalWrite(ledPin, HIGH);   // set the LED on    
    } else {
      digitalWrite(ledPin, LOW);    // set the LED off
    }
    startTime = millis();
  }
    
 // buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
 /* if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPedalAPin, HIGH); // TODO: Use another LED output 
  } else {
    // turn LED off:
    digitalWrite(ledPedalAPin, LOW);
  }*/

  handleButtonChange(debouncerPedalA, ledPedalAPin, "A");
  handleButtonChange(debouncerPedalB, ledPedalAPin, "B"); // TODO: use the final 
  handleButtonChange(debouncerPedalC, ledPedalAPin, "C");
  handleButtonChange(debouncerPedalD, ledPedalAPin, "D");
  handleButtonChange(debouncerPedalE, ledPedalAPin, "E");
  
  delay(25);
}

void handleButtonChange(Bounce debouncerPedal, int ledPedalPin, String pedalName) {
   if ( debouncerPedal.fell() ) {
     Serial.println("Pedal " + pedalName + " pressed");
     digitalWrite(ledPedalPin, HIGH);
   } else if ( debouncerPedal.rose() ) {
     Serial.println("Pedal " + pedalName + " released");
     digitalWrite(ledPedalPin, LOW);
   }  
}
