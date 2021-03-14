# Simon Says
##### A 'Simon' clone written for Engineering Design II at Eastern Mennonite University

This is a clone of the common 'Simon' game, ported to Arduino. The game 
will display a series of blinking lights, and the player must repeat 
those lights back to the Arduino. If they fail, they lose the game.

### Construction
Pins 13, 12, 11, and 10 are each connected to ground through a momentary pushbutton
Pins 9, 8, 7, and 6 are connected to 330 ohm resistors, then LEDs connected to ground.
Pin 5 is connected to a loudspeaker through a passive low-pass filter, then a common-emitter amplifier (see circuit diagram to be added)


