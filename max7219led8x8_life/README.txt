MAX7219LED8x8 - Tinusaur MAX7219 Library for LED 8x8 Matrix

This is a testing project.

This program plays the Conway's Game of Life.

To build the project, type:
   $ make

To program the binary into the microcontroller, type:
   $ make program

Alternatively, to program the binary, type:
	$ avrdude -c usbasp -p t85 -U flash:w:"main.hex":a

To clean up files left out from previous builds, type:
   $ make clean


