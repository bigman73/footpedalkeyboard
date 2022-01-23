# Requirements
Prior to compiling and upload the firmware some preparation steps are needed

## 1. Install Arduino IDE
1. Download and install Arduino IDE

https://www.arduino.cc/en/Main/Software

>    As of January 2022-> version 1.8.19

## 2. Install Teensyduino
1. Download Teensyduino installer from
https://www.pjrc.com/teensy/td_download.html

> As of January 2022 -> version 1.56

2. Select the Arudino IDE folder (from step 1) as target folder
3. Unselect all libraries
4. Install

## 3. Select Teensy LC Board
1. Main menu / Tools / Board "..." / Teensyduino
2. Select `Teensy LC`

## 4. Install bounce2 library
1. Main menu / Tools / Manage Libraries...
2. Select the library `bounce2`
>    As of January 2022 -> version 2.60.0

# Hardware Configuration
## Korg EC-5
[Korg EC-5 External Controller - Product page](https://www.korg.com/us/products/accessories/ec_5/)

### Top View

![Korg EC-5, top](https://cdn.korg.com/us/products/upload/f9cf3880d4089be89e24affb28709f1e_sp.png)
### Rear View
The modified MIDI cable connects to the port in the rear
![Korg EC-5, rear](https://cdn.korg.com/us/products/upload/fbc20c8fcf343468972121e170d7142b_sp.png)


## Wiring Korg EC-5 Pins
| KORG CABLE WIRE | PEDAL         | SOLDERED WIRE |
|-----------------|:-------------:|--------------:|
| Yellow       🟨 |  [GND]       | Black    ⬛   |
| Black        ⬛ |  A           | Purple   🟪   |
| Red          🟥 |  B           | Orange   🟧   |
| Grey         🗄  |  C           | Gray     🗄   |
| Green        🟩 |  D           | Green    🟩   |
| Blue         🟦 |  E           | Blue     🟦   |
