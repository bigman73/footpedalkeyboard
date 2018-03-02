# footpedalkeyboard
A foot pedal keyboard, using Korg EC5, Arduino and Python
Converts pedal button events into operating system virtual keyboard sequences.
For example: If Pedal A is clicked (pressed and released) then the python driver will generate a Media Prev keyboard stroke on the operating system. A musician can then control music playback by using his/her feet, a very natural way for musicians that use foot pedals for effects and volume control, when playing instruments such as guitar, bass guitar and keyboards.

Origianlly, I wanted to be able to able to control [Practice#](https://github.com/bigman73/practicesharp), my open source practice application, with my feet - instead of getting out of practice position and having to use my hands in order control the application (especially I needed to pause/play and lookback).

## Korg EC5 Foot Pedal Board
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/fritzing/ec5-b.jpg "Korg EC5 Foot Pedal Board")

## The final result
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/pedal-final.jpg "Used in 'production' :)")
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/breadboard.jpg  "The breadboard solution")
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/python-driver-windows.png  "The FPK python driver running on Windows")


TODO - Youtube Video

## More information - In the wiki
Bill Of Materials, wiring schematics and other details: 
https://github.com/bigman73/footpedalkeyboard/wiki

## Credits
I took some direction and ideas from a similar project done by Sylvain Wallez:
https://bluxte.net/musings/2016/07/29/usb-foot-keyboard/

