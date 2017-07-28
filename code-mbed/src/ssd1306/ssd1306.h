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

#ifndef SSD1306_h
#define SSD1306_h

#include "mbed.h"

/**
 * Adafruit SSD1306 Driver class
 *
 * This aims to provide the most basic interface allowing you to communicate
 * with a SSD1306 board. All of the advanced functionalities are to be pushed
 * to a higher level class.
 */
class Adafruit_SSD1306_ {
public:
  #define DEFAULT_I2C_ADDR 0x78
  #define COLOUR_BLACK 0
  #define COLOUR_WHITE 1
  #define COLOUR_TRANS 2

  /**
   * Creates an I2C driver to the SSD1306 module.
   *
   * Note there are no size parameter on the constructor. The caller will
   * therefore have the choice to print outside of the drawspace. Doing this
   * will just result in no pixel being shown.
   *
   * @param com I2C communication BUS
   * @param rstPin The Pin used for reset
   * @param width [128] Width of the drawspace
   * @param height [32] Height of the drawspace
   * @param addr [Default I2C address] The address for the module
   */
  Adafruit_SSD1306_(I2C& com, PinName rstPin, uint8_t width = 128, uint8_t height = 32, uint8_t addr = DEFAULT_I2C_ADDR);
  ~Adafruit_SSD1306_();

  /**
   * Draws a colour buffer on the display.
   *
   * Drawing outside the drawspace is allowed but obviously won't show. The
   * colour space allows for transparent pixels.
   */
  void draw(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t* data);
  void invert(bool i);
  void init();
  void clear();

private:
  uint8_t _addr;
  I2C& _com;
  uint8_t _height;
  DigitalOut _reset;
  uint8_t _width;
  uint8_t* _buffer;

  void cmd(uint8_t cmd);
};

inline void Adafruit_SSD1306_::cmd(uint8_t cmd) {
  const uint8_t msg[2] = {0x00 /* Command Mode */, cmd};
  _com.write(_addr, (const char*)msg, 2 /* sizeof(msg) */);
}



#endif // SSD1306_h
