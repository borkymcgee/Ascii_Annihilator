![glamor_shot_small](https://user-images.githubusercontent.com/78439591/190876986-dc66f5b8-4b21-41d2-8fd3-9e7d38d229f8.jpg)
# Ascii Annihilator #

The Ascii Annihilator (or 'Asc Blaster') is a modification to [sparkfun's popular "Binary Blaster" PTH soldering kit](https://www.sparkfun.com/products/12037). The asc blaster replaces the 2-digit 7-segment display with a 4-digit 14-segment alphanumeric display, and allows the player to quiz themself on decimal- hexidecimal- and ascii-to-binary conversions.

## IMPORTANT WARNING ##

DO NOT INSTALL THE NIBBLE SWITCH BEFORE UPLOADING THE NEW FIRMWARE

IGNORING THIS STEP WILL RESULT IN FRYING PIN D6, WHICH WILL REQUIRE FURTHER MODIFICATION

warning explanation: The binary blaster code is configured to use D6 as the buzzer output, and on boot with original code it will output a PWM signal. with the nibble switch connected and switched on, this PWM signal will get shorted to ground, and the pin will be killed

solution: build all but the nibble switch, upload the new firmware and ensure that it displays 'full', and only THEN solder on the nibble switch

Note: at this time, sound is not available, as removing the buzzer is necessary to simplify the hardware modifications

## Parts: ##
-[Binary Blaster kit](https://www.sparkfun.com/products/12037)

-[14-segment alphanumeric display](https://www.sparkfun.com/products/21217)

-wire for jumpers

-FTDI Basic for programming

## Hardware Mods: ##

Within the "Hardware" directory, you will find images of the top and bottom of the binary blaster's board, with traces to cut and jumpers to add marked, as well as instructions and a step-by-step walkthrough of the mods necessary.

## Software Mods: ##

Open Ascii_Annihilator.ino in the arduino IDE, and follow the instructions silkscreened on the back of the binary blaster to upload.

## How to Play: ##

Upon boot, the Ascii Annihilator will display the mode selected, and begin quizzing the player on the corresponding set of numbers and characters.

the first digit of the display is used to show what type of character the player must convert to binary as follows:

a - ascii

d - decimal

x - hexadecimal

The Annihilator is capable of quizzing the player on hex and decimal from 0-255, and all of standard ascii, including non-printing characters. As the Binary Blaster (and thus the Ascii Annihilator) only has 4 buttons, the use of the 'nibble switch' to the bottom right of the display is used.

### Example: ###

display shows:
x 7D

(hexadecimal 7D)

corresponding binary:
01111101

player must input:
with nibble switch high:  0111
with nibble switch low:   1101

### Selecting game mode: ###

The Ascii Annihilator allows the user to select between several modes by holding certain buttons on boot. the buttons are assigned as follows:

- bit 3: ascii mode

- bit 2: decimal mode

- bit 1: hexadecimal mode

- bit 0: wimp mode

- all bits: expert mode

### Ascii mode ###
the player gets quizzed on all of ascii, including non-printing characters, symbols, numbers, and letters

### Hex mode ### 
the player gets quizzed on hexadecimal from 0-FF

### Dec mode ###
the player gets quizzed on decimal from 0-255

### Wimp mode ###
ascii mode will only show letters and numbers, hex only 0-F, and dec only 0-15

### Expert mode ###
the player will only get quizzed on symbols and non-printing ascii characters

ascii, decimal, hexadecimal, and wimp modes can be combined in any way to be quizzed on all of the selected sets at once.

## About this mod ##

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;I've worked at sparkfun for around a year now (and been a fan much longer), and one day as I was packaging up some binary blaster kits I became enchanted by the idea of them: a kit to not only teach a person basic soldering, to not only provide them with a fun game afterward, but to make that game itself a teaching tool for learning binary and hexidecimal! I bought one, put it together that day on my 15 minute break, and spent the next day or two just quizzing myself on my binary and ascii. 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;After a little while, I started to get bored of just being able to quiz myself on 0-15 and 0-F, and started to think about ways to increase the toy's replayability. Sparkfun had just started selling a 4 character alphanumeric display with a qwiic connector and interface, so after buying one and extracting the display from  it, I immediately set about integrating it into the binary blaster.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;I decided from the get-go that there were a few design restrictions I'd put on myself: I wanted the code to all be under the GNU GPL, I wanted to make the modification as low-profile and robust as possible, and I wanted the new display itself to be the only new component needed.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;There were quite a few hurdles to overcome, most significantly that to control the display, the lit buttons, and the nibble switch I needed to add, required a few more I/O pins than I had at my disposal. I ended up having to charlieplex the display and button LEDs together, as well as get rid of the buzzer to free enough pins to address everything. I also had to figure out how to use the existing traces on the pcb as much as possible to cut down on the complexity and messiness of the mod.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The next hurdle was programming. The original code for the binary blaster was all released under CC-BY-SA, which is incompatible with the GLPV3+ I wanted to use, so that meant completely rewriting the code to run the toy. Additionally, since I'd charlieplexed the display and buttons together, I couldn't use any preexisting code to run the display and had to implement it myself, taking inspiration from other typefaces I'd seen on 14 segment displays and tweaking them to enable me to display all of the standard ASCII charset. There were some other interesting challenges, like how using the buttons on boot vastly reduced the amount of randomness available to me, or the fun knuth-fisher-yates algorithm for shuffling the guess order in-place.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Eventually I had it all done and working, and I've been having fun playing with the result ever since! 
  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Another amusing note: apart from actually soldering the thing together, all the designing and programming for this project was performed on the hour-long bus commutes to-and-from work, balancing my laptop, the annihilator, and a multimeter on my lap. the development took around 2ish weeks, over which time I attracted quite a few curious or conerned glances from fellow bus partons! A discussion over lunch with [Pete Lewis](https://github.com/lewispg228), the designer of the original binary blaster, revealed that development on public transportation is a common theme with this project, with the original binary blaster having been largely coded on a flight to his honeymoon!

P.S. I've recently been eyeing Pete's other game-based soldering kit, the Simon Says, and thinking about how to mod that one for increased replayability. If you're interested in that, keep an eye on my github!
