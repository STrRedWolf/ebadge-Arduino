# ebadge-Arduino
An electronic badge using Adafruit parts!  It's a 3.5" screen that rotates through PPM images stored on a 1GB MicroSD card.

Around November 1st, 2017, Adafruit released a 3.5" TFT Feather wing, which simplifies things, and makes it all accessible!

# New version (mark 3 and 4)

## Bill of Materials

Get all of these from Adafruit!

* 3.5" TFT Feather wing
* Feather M0 Basic Proto
* LithIon battery.  The mark 3 version will fit a PKCELL ICR18650 2200mAh 3.7V cell (barrel shape).  The mark 4 uses a PKCELL flat-pack 2500mAh 3.7V cell.
* A small capacity (8 GB is fine) MicroSD card, FAT16 formatted.
* A 3D printer (of course -- I used a Printrbot Simple Metal)
* Breadboard friendly SPDT slide switch
* Wire, of course. (the silicon coated wire may be best. I used the 22awg wire I had.)
* Heat-shrink tubing

You'll also need some Scotch tape.

## Building it!

Print out the parts first.  If you have the round battery, you'll have to print the mark 3 version (EBadge3-*.stl).  Flat battery is the mark 4 design (EBadge4-*.stl).  The TFT goes right in and is pressure fit with the back half.  Mark 3's will need to use the divot to keep the switch in place.

You will need to solder two wires to the switch (middle pin and one of the side pins) first.  Then solder one wire to the Enable Pad, the other to the Ground pad. Snip the unused pin on the switch.  Make sure the Enable switch on the TFT is switched ON, and the switch itself is switched to connect the two wired pins.  Afterwards, bend the metal that forms the short sides of the switch out so they're flush with the switch (we'll use these to keep the switch from moving).

Solder up the M0 with the shipped headers.  

From here, it depends on which version you have.

### Mark 3 (barrel battery)

Plug in the battery, and plug it in the back of the TFT.  I route the cable of the battery under the M0 Feather. 

Slide the switch into it's place and follow it up with the "divot" you printed.  You did print the divot, right?

Now close up the two halves and you should be ready for programming it.

### Mark 4 (flat battery)

Take the Feather M0 you soldered up, and trim all the pins under the board to be flush with the plastic spacer the pins were shipped with.  When they're all done, remove the spacer.  The length of the pins fits on top of the TFT's connector, making it rather flush.

Connect the battery and tape it to the back shell you printed, all the way up top. The raised part at the bottom corner (where the USB port is) lays on top of the switch, so you should place the battery catty-cornered from it.

Place the switch in the top shell.  You may find it easier to tape the switch in place.

Now close up the two halves and you should be ready for programming it.

## Program it!

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
* Micro BBS?
