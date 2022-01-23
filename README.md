# Foot Pedal Keyboard
A foot pedal keyboard, using Korg EC5, Arduino and Python
Converts pedal button events into operating system virtual keyboard sequences.

## Sample applications

### Media editing 📸
Control media editing software, such as Adobe Photosop and Premiere, with your foot

### Program code 👨‍💻
Use the foot keyboard pedal to send macros to a code IDE, such as Visual Studio Code

### Practicing musical instruments 🎸
Control playback with your foot, without stopping the practice. The hands stay on the instrument (e.g, Bass guitar) and hitting a foot pedal rewinds the playback to the start.

### Using with Practice#
For example: If Pedal A is clicked (pressed and released) then the python driver will generate a Media Prev keyboard stroke on the operating system. A musician can then control music playback by using his/her feet, a very natural way for musicians that use foot pedals for effects and volume control, when playing instruments such as guitar, bass guitar and keyboards.

Originally, I wanted to be able to able to control [Practice#](https://github.com/bigman73/practicesharp), my open source practice application, with my feet - instead of getting out of practice position and having to use my hands in order control the application 
With Practice# the setup is as followes:
+ Pedal A -> Look back (key = l)
+ Pedal B -> Rewind a few seconds back (key = ,)
+ Pedal C -> Toggle Play / Pause (key = space)
+ Pedal D -> Slow down (key = s)
+ Pedal E -> Speed up (key = f)

But I decided to make a more generic solution. The python driver supoorts dynamic supports any desktop application through an easy to configure json file (key-mappings.json). The default configuration file contains Chrome mappings - a tab can be closed with Pedal E!  

### Video games
A foot keyboard can provide extra keys to use in games such as first person shooters (FPS).
For example: Pedal A could be assigned to a `crouch` action, or to a series of keyboard actions that would be executed in sequence

## Korg EC5 Foot Pedal Board
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/ec5-b.jpg "Korg EC5 Foot Pedal Board")

## The final result
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/pedal-final.jpg "Used in 'production' :)")
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/breadboard.jpg  "The breadboard solution")
![alt text](https://raw.githubusercontent.com/bigman73/footpedalkeyboard/master/images/python-driver-windows.png  "The FPK python driver running on Windows")

## More information - In the wiki
Bill Of Materials, wiring schematics and other details: 
https://github.com/bigman73/footpedalkeyboard/wiki

## Credits
I took some direction and ideas from a similar project done by Sylvain Wallez:
https://bluxte.net/musings/2016/07/29/usb-foot-keyboard/

