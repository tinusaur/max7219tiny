MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix

This is a testing project.

This program fills out the display i.e. turns all the LEDs on - with some paterns.

To build the project, type:
   $ make

To program the binary into the microcontroller, type:
   $ make program

Alternatively, to program the binary, type:
	$ avrdude -c usbasp -p t85 -U flash:w:"main.hex":a

To clean up files left out from previous builds, type:
   $ make clean


