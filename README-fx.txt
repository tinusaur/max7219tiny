README for the FX Library


---- Tools ----

Bitmap converter for mono and color LCD displays
http://en.radzio.dxp.pl/bitmap_converter/

Fotografix --  free image editor for Windows
https://lmadhavan.com/fotografix/


---- Fons Files ----

Download a bitmap font.

Sources:
- Font Bitmaps: http://uzebox.org/wiki/Font_Bitmaps


---- Requirements and Suggestions ----

All characters should be left-aligned.

Image file should be in BMP format, 1 bit per pixel.


---- Convert Fonts ----

Make sure the file is:
- BMP file format
- Black&White, i.e 1-bit per pixel

Load the image in the LCD Assistant application.

Check options:
- Byte orientation: Vertical
- Size: Width/Height should match font bitmap size
- Include size: no
- Pixel per byte: 8
- Table name: the name of the variable in the output file - ex.: font8x6_data

Save the output:
- Choose a name for the file - ex: "font8x6_data.h"


---- Edit the Source Code File ----

Open the source code file - ex.: "font8x6_data.h"

Add the "PROGMEM" directive to the variable definition.
Ex.:
	const unsigned char font8x6_data [] PROGMEM = {


---- Bitmap Images Processing ----

The same as the fonts.

