# Ascii Annihilator #

The Ascii Annihilator (or 'Asc Blaster') is a modification to sparkfun's popular "Binary Blaster" PTH soldering kit [TODO: put link]. The asc blaster replaces the 2-digit 7-segment display with a 4-digit 14-segment alphanumeric display, and allows the player to quiz themself on decimal- hexidecimal- and ascii-to-binary conversions.

## IMPORTANT WARNING ##

DO NOT INSTALL THE NIBBLE SWITCH BEFORE UPLOADING THE NEW FIRMWARE

IGNORING THIS STEP WILL RESULT IN FRYING PIN D6, WHICH WILL REQUIRE FURTHER MODIFICATION

warning explanation: The binary blaster code is configured to use D6 as the buzzer output, and on boot with original code it will output a PWM signal. with the nibble switch connected and switched on, this PWM signal will get shorted to ground, and the pin will be killed

solution: build all but the nibble switch, upload the new firmware and ensure that it displays the new startup sequence [TODO], and only THEN solder on the nibble switch

Note: at this time, sound is not available, as removing the buzzer is necessary to simplify the hardware modifications

## Parts: ##
-Binary Blaster kit [TODO: link]
-14-segment alphanumeric display [TODO: link]
-wire for jumpers
-FTDI Basic for programming

## Hardware Mods: ##

Within the "Hardware" directory, you will find images of the top and bottom of the binary blaster's board, with traces to cut and jumpers to add marked, as well as instructions and [TODO: write walkthrough] a step-by-step walkthrough of the mods necessary.

## Software Mods: ##

Open Ascii_Annihilator.ino in the arduino IDE, and follow the instructions silkscreened on the back of the binary blaster to upload.

## How to Play: ##

Upon boot, the Ascii Annihilator will display the mode selected, and begin quizzing the player on the corresponding set of numbers and characters.

the first digit of the display is used to show what type of character the player must convert to binary as follows:
a - ascii
d - decimal
x - hexadecimal

The Annihilator is capable of quizzing the player on hex and decimal from 0-255, and all of standard ascii, including non-printing characters. As the Binary Blaster (and thus the Ascii Annihilator) only has 4 buttons, the use of the 'nibble switch' to the bottom right of the display is used. an example follows:

display shows:
x Ab

corresponding binary:
10101011

player must input:
with nibble switch high: 1010
with nibble switch low: 1011

The Ascii Annihilator allows the user to select between several modes by holding certain buttons on boot. the buttons are assigned as follows:

-bit 3: ascii mode
-bit 2: decimal mode
-bit 1: hexadecimal mode
-bit 0: wimp mode

-all bits: expert mode

Ascii mode: the player gets quizzed on all of ascii, including non-printing characters, symbols, numbers, and letters

Hex mode: the player gets quizzed on hexadecimal from 0-FF

Dec mode: the player gets quizzed on decimal from 0-255

Wimp mode: ascii mode will only show letters and numbers, hex only 0-F, and dec only 0-15

Expert mode: the player will only get quizzed on symbols and non-printing ascii characters

ascii, decimal, hexadecimal, and wimp modes can be combined in any way to be quizzed on all of the selected sets.