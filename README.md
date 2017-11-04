# ebadge-Arduino
An electronic badge using Adafruit parts!  It's a 3.5" screen that rotates through PPM images stored on a 1GB MicroSD card.

Around November 1st, 2017, Adafruit released a 3.5" TFT Feather wing, which simplifies things, and makes it all accessible!

# New version (mark 2)

## Bill of Materials

* 3.5" TFT Feather wing
* Feather M0 Basic Proto
* LithIon battery, 3.7v 2200mAh barrel shape
* A small capacity (8 GB is fine) MicroSD card, FAT16 formatted.
* A 3D printer (of course)
* Breadboard friendly SPDT slide switch
* Wire, of course. (the silicon coated wire may be best. I used the 22awg wire I had.)
* Heat-shrink tubing

## Building it!

Print out the parts in the STL folder.  The TFT goes right in and is pressure fit with the back half.

You will need to solder two wires to the switch (middle and one of the sides) first.  Then solder one wire to the Enable Pad, the other to the Ground pad. 

Solder up the M0 with the shipped headers, plug in the battery, and plug it in the back of the TFT.  I route the cable of the battery under the M0 Feather.  Make sure the Enable switch on the TFT is switched ON!

Click in the switch to the back half of the case (hopefully your wires were long enough).  Now close up the two halves of the case.  

Plug it in, turn it on, and upload the sketch.  

Now prep your MicroSD card!

# Old version (mark 1)

## Bill of Materials (old version)

* Adafruit Feather 32u4 Basic Proto
* 3.5" TFT 320x480 + Touchscreen Breakout Board w/MicroSD socket (HXD8357D)
* LithIon battery, 3.7v 2000mAh
* Breadboard friendly SPDT slide switch.
* Female/female jumper wires, 3"
* 22awg wire (I used the hook-up wire set)
* Hot glue gun w/glue
* Male right-angle break-away header, 0.1" pitch
* A 1 GB MicroSD card, FAT16 formatted.

## Soldering the Feather's headers and the switch

The first photo in the images directory gives an idea on how to solder things.  The switch is soldered on the back and centered with the Feather.  A six-pin header is soldered starting at pin 13 and going to pin 6 (which in the end will leave pin 9 "floating").  Two individual headers are covering all but a hole for the ground and 3.3V that stretches along the sides of the Feather.  A wire goes from a ground to the switch and then to the Enable pin.  A three-pin header is soldered on "backwards" on the SPI pins to keep it compact.

# Prepping the MicroSD card:  Portable PixMaps (PPM)

The eBadge as it's coded now will **NOT** take BMPs.  I've elected to use a much simplier format called Portable PixMap, or PPM.  This is a Unix-borne format, but most software (including Gimp) will export to "binary PPM".  PPM has a simple header and then data right after it, making the code much simpler.  For details, go to NetPBM's site.

That said, if you got Gimp, save RGB formtted binary PPMs.  Those work.  Start with 0001.PPM and go all the way up to 0255.PPM.

# Ideas for expansion

* Hidden 80x30 column VT52 terminal?