Hardware modding instructions:

In this directory you will find two images: [front.png](front.png) and [back.png](back.png). These correspond with the front and back of the binary blaster, and show the mods required on each side. It is easiest to modify the PCB before constructing the binary blaster, but one can also modify an already constructed binary blaster.

0. If modding an existing binary blaster, desolder and remove the 7-segment displays, the buzzer, the sound switch, and (optionally) the battery connectors. The 7-segments and the buzzer won't be needed, but try not to break the sound switch or the battery connectors

1. Use an abrasive to remove the silkscreen and expose the copper where marked in lavender on the diagram. Sandpaper works for this, although I've found the best tool for the job is the rounded end of a pointy knife/fishing hook sharpener like the one seen [here](https://a.co/d/bYEm2wv). It's best to try not to expose any other traces, but it's not the end of the world if you do.

2. Use something hard and sharp to cut through the traces along the red lines in the diagram. An exacto knife or box cutter works well for this, although a thumbtack will work in a pinch. Be careful not to cut any traces not marked in red, and make sure the trace is completely cut, potentially using a continuity tester to check.

3. Solder jumper wire where marked in peach on the back of the board. Any thin hookup wire will do for this, although solid core wire works best. try to make the jumpers no longer than necessary, as wires with slack in them can get caught on things, get moved around, and eventually fail from fatigue.

4. If modding a new binary blaster kit that has not been constructed yet, insert the ATmega328 and bend a pin on either corner to keep it in place, but do not solder it down

5. Solder jumper wire where marked in peach on the front of the board. Any thin hookup wire will do for this, although solid core wire works best. try to make the jumpers no longer than necessary, as wires with slack in them can get caught on things, get moved around, and eventually fail from fatigue.

	5a. Solder the rest of the pins on the atmega328

	5b. Bend the pins of the alphanumeric display outward to match the green part of [front.png](front.png).

	5c. Solder the alphanumeric display to the pins as shown in [front.png](front.png).

	5d. (Optional) Solder the battery holders on the opposite side of the board as is intended. This is optional but makes holding and playing the game MUCH more comfortable.

	5e. If modding a new Binary Blaster kit, solder the remaining components to the board as usual, excluding the 7-segment displays, buzzer, and buzzer switch.

6. Pop some AA batteries in that bad boy.

7. Flash new firmware using the FTDI Basic or pin-compatible usb-to-uart adapter.

8. Turn the toy on, and ensure that the display shows "full"

DO NOT CONTINUE UNLESS THE DISPLAY SHOWS "full" ON BOOT. IGNORING THIS WARNING WILL BREAK SHIT.
	
9. Pop the batteries out again.

10. Solder the sound switch where the buzzer used to go, bending the pins so that the middle pin gets soldered to the upper pad of the buzzer and one of the other pins gets soldered to the lower pad.
