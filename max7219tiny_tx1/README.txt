Max7219Tiny - Tinusaur MAX7219 Library for LED 8x8 Matrix

This is a testing project.

This program shows some characters on the display by turning some LEDs on.

To build the project, type:
  $ make

To program the binary into the microcontroller, type:
  $ make program

Alternatively, to program the binary, type:
  $ avrdude -c usbasp -p t85 -U flash:w:"main.hex":a

To clean up files left out from previous builds, type:
  $ make clean

