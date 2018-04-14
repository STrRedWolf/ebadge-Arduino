/* Ebadge v0.3 by Kelly "STrRedWolf" Price
 *
 * Based on Adafruit's Bitmap example code.
 *
 * This is licensed via the MIT license per below.
 */

/* v0.3: TGA support */

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
// On the Feather M0:
#define TFT_DC 10
#define TFT_CS 9
#define TFT_RST -1
#define SD_CS 5
#define LITEPIN 0

#else
// Uno/Duemilanove/Pro Mini multifunction pins.
#define TFT_DC 2
#define TFT_CS 10
#define TFT_RST -1
#define SD_CS 7
#define LITEPIN 0

#endif

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);

// We need this set here.
int fileindex = 1;
uint8_t opmode=0;

void setup(void) {
  Serial.begin(9600);

  tft.begin(HX8357D);
  tft.setRotation(2); // upsidedown
  tft.fillScreen(HX8357_BLUE);

  if(LITEPIN != 0) {
    pinMode(LITEPIN,OUTPUT);
    analogWrite(LITEPIN,191);
  }
  
  tft.print(F("Initializing SD card... "));
  if (!SD.begin(SD_CS)) {
    tft.fillScreen(HX8357_RED);
    tft.println(F("failed!"));
    opmode++;
  }
  tft.println(F("OK!"));
}

void loop(void) {
  tft.println(F("In the loop..."));
  showpic();
}

void showpic() {
  char fi;
  char fname[16] = "0000.TGA";
  
  fi=(char)(fileindex & 255);

  //tft.println(fname);

  if( (fi/100) > 0) {
    fname[1]=(char)(fi/100)+'0';
    fi=fi%100;
  }
  //tft.println(fname);

  if((fi/10) > 0) {
    fname[2]=(char)(fi/10)+'0';
    fi=fi%10;
  }
  //tft.println(fname);
  //tft.print(F("bug"));
  fname[3]=((char)(fi & 15)+'0');

  tft.println(fname);

  if(tgaDraw(fname) == 0)
  {
    delay(30000);
    fileindex++;
  } else {
    fileindex=1;
  }
  //opmode++;
}

// This function opens a NetPBM Pixmap (ppm) file and
// displays it at 0,0.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

// With the M0's, do whole lines.
#define BUFFPIXEL 80

uint8_t ppmDraw(char *filename) {

  File     ppmFile;
  int      maxVal;
  uint32_t ppmImageoffset = 0;
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  int      w, h, row, col, berr;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  tft.print(F("Loading image "));
  tft.println(filename);

  // Open requested file on SD card
  if ((ppmFile = SD.open(filename)) == NULL) {
    //Serial.print(F("File not found: "));
    //Serial.println(filename);
    tft.print(F("File not found: "));
    tft.println(filename);
    return 0;
  }

  tft.println(F("Opened..."));

  // Parse the PPM header.
  // The format is roughly "P6 X Y Z" followed by a hard LF and raw RGB
  // X is width, Y is height, Z is a max value.
  // Spaces above are whitespace (what isspace() says it is)
  // A "# comment" can occur before the hard LF and raw RGB.

  // Is it a P6?
  if(ppmFile.read() != 0x50 && ppmFile.read() != 0x36)
  {
      // Nope.
      tft.print(F("File not a P6 PPM: "));
      tft.println(filename);
    return 1;
  }
  w=readNum(ppmFile);
  
  w=readNum(ppmFile);
  h=readNum(ppmFile);
  maxVal=readNum(ppmFile);
  ppmImageoffset=ppmFile.position();  

//  tft.println(F("Ready to plot."));
  
  //tft.setAddrWindow(0, 0, 319, 479);

  berr=-1;
  for (row=0; row<h; row++) { // For each scanline...
    for (col=0; col<w; col++) { // For each pixel...
      // Time to read more pixel data?
      if (buffidx >= sizeof(sdbuffer)) { // Indeed
        berr=ppmFile.read(sdbuffer, sizeof(sdbuffer));
        if(berr==-1) break;
        buffidx = 0; // Set index to beginning
      }
      // Convert pixel from ppm to TFT format, push to display
      r = sdbuffer[buffidx++];
      g = sdbuffer[buffidx++];
      b = sdbuffer[buffidx++];
      
      //tft.pushColor(tft.color565(r,g,b));
      tft.drawPixel(col,row,tft.color565(r,g,b));
    } // end pixel
    // delay(10);
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

uint8_t tgaDraw(char *filename) {
    File f;
    uint8_t  idlen,cmaptype,imgtype,cmapentrysize,depth,info;
    uint16_t  cmapfirst,cmaplen,xorg,yorg,w,h,row,col,r,g,b;
    uint8_t sdbuffer[256]; // generic buffer.
    int sx,sy,x,y,dx,dy;
    int isok=0;
    
    //tft.setCursor(0,0);
    tft.println(filename);
    
    // Open the file...
    if((f = SD.open(filename)) == NULL) {
        tft.println(F("File not found"));
        return 1;
    }
    
    // Pull the header, and parse through it a bit.
    idlen=f.read();
    if((cmaptype=f.read()) > 1) {
        tft.println(F("File has an invalid format."));
        return 2;
    }
    
    imgtype=f.read();
    cmapfirst=f.read()+(f.read() << 8);
    cmaplen=f.read()+(f.read() << 8);
    cmapentrysize=f.read();
    xorg=f.read()+(f.read() << 8);
    yorg=f.read()+(f.read() << 8);
    w=f.read()+(f.read() << 8);
    h=f.read()+(f.read() << 8);
    depth=f.read();
    info=f.read();
    
    // Before we go futher, lets save ourselves
    // and limit what we *can* read.
    // 24-bit true-color, 240x320, top-left corner is 0,0
    // direction left to right, then top to bottom.
    if(depth != 24 || (imgtype & 15) != 2) {
        tft.println(F("File is an unsupported format."));
        return 3;
    }
    
    if(w != 320 || h != 480) {
        tft.print(F("File is not 320 by 480: "));
        tft.print(w);
        tft.print("x");
        tft.println(h);
        return 4;
    }

    if(xorg == 0 && yorg == 0 && (info & 0x30)==0) {
      // image is flipped
      sx=0;
      sy=479;
      dx=1;
      dy=-1;
      isok=1;
    }

    if(xorg == 0 && yorg == 479 && (info&0x30)==0x20) {
      sx=sy=0;
      dx=dy=1;
      isok=1;
    }
    
    if(isok ==0) { // 479
        tft.print(F("File is stored weird. Org/info:"));
        tft.print(xorg);
        tft.print(",");
        tft.print(yorg);
        tft.print(" ");
        tft.println(info);
        return 5;
    }
        
    // Skip the info...
    if(idlen > 0) {
        f.read(sdbuffer,idlen);
    }
    
    // There shouldn't be any cmap data...
    if(cmaplen > 0) {
        tft.println(F("File has invalid format."));
        return 2;
    }
    
    // Now we can get the image data.
    y=sy;
    for(row=0;row<480;row++) {
      x=sx;
        for(col=0;col<320;col++) {
            f.read(sdbuffer,3);

            // TGA uses BGR.
            b=sdbuffer[0];
            g=sdbuffer[1];
            r=sdbuffer[2];
            tft.drawPixel(x,y,tft.color565(r,g,b));
            x+=dx;
        }
        delay(10); //because the screen can't cope.
        y+=dy;
    }
    
    // we're done
    f.close();
    return 0;
}
       
