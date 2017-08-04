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
#include "imageMono/image.h"

/**
 * Adafruit SSD1306 Driver class
 *
 * This aims to provide the most basic interface allowing you to communicate
 * with a SSD1306 board. All of the advanced functionalities are to be pushed
 * to a higher level class.
 */
class Adafruit_SSD1306 {
public:
  #define SSD1306_DFLT_I2C_ADDR 0x78
  #define SSD1306_DFLT_HEIGHT 32

  /**
   * Creates an I2C driver to the SSD1306 module.
   *
   * The particularity of this connector is that it only draws "images" in the
   * drawspace. Therefore, any graphic processing must be done on an image,
   * and then the image with transparency is pushed to the connector.
   *
   * @param com I2C communication BUS
   * @param rstPin The Pin used for reset
   * @param height [32] Height of the drawspace
   */
  Adafruit_SSD1306(I2C& com, PinName rstPin, uint8_t height = SSD1306_DFLT_HEIGHT);
  /** Deconstruct the connector */
  ~Adafruit_SSD1306();

  /**
   * Draws an image in the drawspace.
   *
   * Drawing outside the drawspace is allowed but obviously won't show. The
   * colour space allows for transparent pixels.
   */
  void draw(int16_t x, int16_t y, ImageMono& image);
  /** Inverts the pixel of the display */
  void invert(bool i);
  /** Initialization routine for the connector. */
  void init(uint8_t addr = SSD1306_DFLT_I2C_ADDR);
  /** Clears (with kBlack) the background of the drawspace. */
  void clear();

private:
  uint8_t _addr;
  I2C& _com;
  ImageMonoImpl _image;
  DigitalOut _reset;
  // Sends a command on the I2C bus.
  void cmd(uint8_t cmd);
};

inline void Adafruit_SSD1306::cmd(uint8_t cmd) {
  const uint8_t msg[2] = {0x00 /* Command Mode */, cmd};
  _com.write(_addr, (const char*)msg, 2 /* sizeof(msg) */);
}



#endif // SSD1306_h
