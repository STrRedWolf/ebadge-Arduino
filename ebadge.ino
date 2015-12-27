/* Ebadge v0.1 by Kelly "STrRedWolf" Price
 *
 * Based on Adafruit's Bitmap example code.
 *
 * This is licensed via the MIT license per below.
 */


/***************************************************
  This is our Bitmap drawing example for the Adafruit HX8357 Breakout
  ----> http://www.adafruit.com/products/2050

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_HX8357.h"
#include <SPI.h>
#include <SD.h>

// TFT display and SD card will share the hardware SPI interface.
// Hardware SPI pins are specific to the Arduino board type and
// cannot be remapped to alternate pins.  For Arduino Uno,
// Duemilanove, etc., pin 11 = MOSI, pin 12 = MISO, pin 13 = SCK.

#define HARDSPI 1

#ifdef HARDSPI
// Hardware SPI that's not aliased anywhere (like the Feather)
#define TFT_DC 12
#define TFT_CS 13
#define TFT_RST 11
#define SD_CS 6
#define LITEPIN 10

#else
// Uno/Duemilanove/Pro Mini multifunction pins.
#define TFT_DC 2
#define TFT_CS 10
#define TFT_RST 4
#define SD_CS 7
#define LITEPIN 6

#endif

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// We need this set here.
int fileindex = 1;
uint8_t opmode=0;

void setup(void) {
  Serial.begin(9600);

  tft.begin(HX8357D);
  tft.fillScreen(HX8357_BLUE);

  pinMode(LITEPIN,OUTPUT);
  analogWrite(LITEPIN,191);
  
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("failed!");
    opmode++;
  }
  Serial.println("OK!");

  //ppmDraw("0001.PPM");
}

void loop() {
  if(opmode)
    bbs();
  else
    showpic();
}

void showpic() {
  int fi;
  char *fname = "0000.PPM";
  
  fi=fileindex;
  if( (fi/1000) > 0) {
    fname[0]=(char)(fi/1000)+'0';
    fi=fi%1000;
  }
  if( (fi/100) > 0) {
    fname[1]=(char)(fi/100)+'0';
    fi=fi%100;
  }
  if((fi/10) > 0) {
    fname[2]=(char)(fi/10)+'0';
    fi=fi%10;
  }
  fname[3]=(char)fi + '0';

  if(ppmDraw(fname) > 0)
  {
    delay(30000);
    fileindex++;
  } else {
    fileindex=1;
  }
  opmode++;
}

// This function opens a NetPBM Pixmap (ppm) file and
// displays it at 0,0.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

#define BUFFPIXEL 20

uint8_t ppmDraw(char *filename) {

  File     ppmFile;
  int      maxVal;
  uint32_t ppmImageoffset = 0;
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  //Serial.println(F("Loading image '"));
  //Serial.println(filename);
  

  // Open requested file on SD card
  if ((ppmFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found: "));
    Serial.println(filename);
    tft.print(F("File not found: "));
    tft.println(filename);
    return 0;
  }

  // Parse the PPM header.
  // The format is roughly "P6 X Y Z" followed by a hard LF and raw RGB
  // X is width, Y is height, Z is a max value.
  // Spaces above are whitespace (what isspace() says it is)
  // A "# comment" can occur before the hard LF and raw RGB.

  // Is it a P6?
  if(ppmFile.read() != 0x50 && ppmFile.read() != 0x36)
  {
      // Nope.
      Serial.print(F("File not a P6 PPM:"));
      Serial.println(filename);
    return 1;
  }
  w=readNum(ppmFile);
  
  w=readNum(ppmFile);
  h=readNum(ppmFile);
  maxVal=readNum(ppmFile);
  ppmImageoffset=ppmFile.position();  
  
  tft.setAddrWindow(0, 0, 319, 479);

  for (row=0; row<h; row++) { // For each scanline...
    for (col=0; col<w; col++) { // For each pixel...
      // Time to read more pixel data?
      if (buffidx >= sizeof(sdbuffer)) { // Indeed
        ppmFile.read(sdbuffer, sizeof(sdbuffer));
        buffidx = 0; // Set index to beginning
      }
      // Convert pixel from ppm to TFT format, push to display
      r = sdbuffer[buffidx++];
      g = sdbuffer[buffidx++];
      b = sdbuffer[buffidx++];
      
      tft.pushColor(tft.color565(r,g,b));
    } // end pixel
    delay(10);
  } // end scanline
  ppmFile.close();
  return 1;
}

int readNum(File &f) {
  boolean junk=true;
  int num = 0;

  // Clean up beforehand.
  while(junk) {
    // Chew up any whitespace preceding.
    while(isWhitespace(f.peek())) {
        f.read();
    }
    
    // Comment?  Skip that as well
    if(f.peek() == '#') {
       while(f.peek() != 10) {
          f.read();
       }
       f.read(); // Chew the linefeed as well.
    } else {
      junk=false;
    }
  }
  
  // Now read the number.
  while(isDigit(f.peek())) {
      f.peek();
    num=(num*10)+(f.read() - '0');
  }

  // Be kind, and chomp any whitespace up to a next LF.
  while(isWhitespace(f.peek()) && f.peek() != 10) {
	f.read();
  }
  if(f.peek() == 10)
	f.read();

  return num;
}

// Mini-BBS mode.
void bbs() {
  ;
}
