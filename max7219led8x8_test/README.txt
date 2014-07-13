MAX7219Led8x8 - Test


This program fills out the display i.e. turns all the LEDs on - one by one, and then clears the display i.e. turns all the LEDs off.

To make, execute:
	$ make

To upload, execute:
	$ avrdude -c usbasp -p t85 -U flash:w:"main.hex":a


