/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

// Modified by Neal Horman 7/14/2012 for use in mbed
// Modified by Maxime Lavigne 2017/07/28

#include "ssd1306.h"

#include <vector>

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_WIDTH 128

Adafruit_SSD1306::Adafruit_SSD1306(I2C& com, PinName rstPin, uint8_t height)
    : _com(com), _reset(rstPin), _image(SSD1306_WIDTH, height, kBlack) {
}

void Adafruit_SSD1306::init(uint8_t addr) {
  _addr = addr;
  _reset = 1;
  // VDD (3.3V) goes high at start, lets just chill for a ms
  wait_ms(1);
  // bring reset low
  _reset = 0;
  // wait 10ms
  wait_ms(10);
  // bring out of reset
  _reset = 1;
  // turn on VCC (9V?)

  cmd(SSD1306_DISPLAYOFF);
  cmd(SSD1306_SETDISPLAYCLOCKDIV);
  cmd(0x80);  // the suggested ratio 0x80

  cmd(SSD1306_SETMULTIPLEX);
  cmd(_image.height - 1);

  cmd(SSD1306_SETDISPLAYOFFSET);
  cmd(0x0);  // no offset

  cmd(SSD1306_SETSTARTLINE | 0x0);  // line #0

  cmd(SSD1306_CHARGEPUMP);
  cmd(0x14);

  cmd(SSD1306_MEMORYMODE);
  cmd(0x00);  // 0x0 act like ks0108

  cmd(SSD1306_SEGREMAP | 0x1);

  cmd(SSD1306_COMSCANDEC);

  cmd(SSD1306_SETCOMPINS);
  cmd(_image.height == 32 ? 0x02 : 0x12);  // TODO - calculate based on _rawHieght ?

  cmd(SSD1306_SETCONTRAST);
  cmd(_image.height == 32 ? 0x8F :  0xCF );

  cmd(SSD1306_SETPRECHARGE);
  cmd(0x22);

  cmd(SSD1306_SETVCOMDETECT);
  cmd(0x40);

  cmd(SSD1306_DISPLAYALLON_RESUME);

  cmd(SSD1306_NORMALDISPLAY);

  cmd(SSD1306_DISPLAYON);
}

Adafruit_SSD1306::~Adafruit_SSD1306() {}

void Adafruit_SSD1306::draw(int16_t x, int16_t y, ImageMono& image) {
  // Copy to internal buffer
  for (int16_t i = 0; i < _image.width; i++) {
    for (int16_t j = 0; j < _image.height; j++) {
      if (i < x || i >= image.width) continue;
      if (j < y || j >= image.height) continue;
      if (image.at(i, j) == kTrans) continue;
      _image.set(i, j, image.at(i, j));
    }
  }

  // Write Data to Buffer;
  cmd(SSD1306_SETLOWCOLUMN | 0x0 /* Low Column Start Address  0 */);
  cmd(SSD1306_SETHIGHCOLUMN | 0x0 /* High Column at 0 */);
  cmd(SSD1306_SETSTARTLINE | 0x0 /* Start line at 0 */);

  char buff1[17];
  buff1[0] = 0x40;

  for (int t = 0; t < 4; t++) { /* Per Page Row */
    for (int tx = 0; tx < SSD1306_WIDTH; tx++) { /* The Whole Line */
      for (int ty = 0; ty < 8; ty++) { /* 8 pix deep from top to bottom */
        if (_image.at(tx, ty + t * 8) == kWhite)
          buff1[tx % 16 + 1] |= 1 << (ty % 8);
        else
          buff1[tx % 16 + 1] &= ~(1 << (ty % 8));
      }
      if (tx % 16 == 15) /* Send Chunk Every 16 byte */
        _com.write(_addr, buff1, sizeof(buff1));
    }
  }
}

void Adafruit_SSD1306::invert(bool i) {
  cmd(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

void Adafruit_SSD1306::clear() {
  ImageFixed black(_image.width, _image.height, kBlack);
  draw(0, 0, black);
}