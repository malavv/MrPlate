#ifndef IMG_MONO_h
#define IMG_MONO_h

#include <stdint.h>

// Available colours for the Monocrome + Transparent palette
enum MonoColour {
  kBlack = 0,
  kWhite,
  kTrans,
  kOutside
};

/**
 * This is the base class with which you will interface.
 *
 * It defines how an image using Monochrome (black+white) and transparent
 * should behave.
 */
class ImageMono {
public:
  // Creates an Monochrome image with transparent pixels.
  ImageMono(uint16_t width, uint16_t height) : width(width), height(height) {}
  virtual ~ImageMono() {};

  // Gets the colour of a pixel
  virtual MonoColour at(uint16_t x, uint16_t y) const = 0;
  /**
   * Sets a specific pixel (does not print transparent or outside)
   *
   * (Not implemented in all sub-classes)
   */
  virtual void set(uint16_t x, uint16_t y, MonoColour colour) = 0;
  // Is a pixel in the image
  bool contains(uint16_t x, uint16_t y) const { return x < width && y < height; }
  // Width and height of the image.
  const uint16_t width, height;
};

/** An implementation of ImageMono that always returns the same colour */
class ImageFixed : public ImageMono {
public:
  ImageFixed(uint16_t width, uint16_t height, MonoColour colour)
    : ImageMono(width, height), _colour(colour) {}

  // Always the same colour except outside.
  virtual MonoColour at(uint16_t x, uint16_t y) const {
    return contains(x, y) ? _colour : kOutside;
  }
  // Unimplemented
  virtual void set(uint16_t x, uint16_t y, MonoColour colour) { return; }

private:
  MonoColour _colour;
};

/** An implementation of a modifiable monochromatic image */
class ImageMonoImpl : public ImageMono {
public:
  ImageMonoImpl(uint16_t width, uint16_t height, MonoColour colour = kTrans);
  virtual ~ImageMonoImpl();

  // Gets the colour of a pixel
  virtual MonoColour at(uint16_t x, uint16_t y) const;
  // Sets a specific pixel (does not print transparent or outside)
  virtual void set(uint16_t x, uint16_t y, MonoColour colour);

private:
  uint8_t* _buffer;
  const uint16_t _numBytes;
  static const uint8_t pixelPerByte = 4;
  static const uint8_t numBitsPerPixel = 2; // white, black, and trans so 2 bits
  static const uint8_t rightmostPixelMask = 0x3; // bitmask for a 2 bit pixel

  MonoColour _getPixelInByte(uint16_t byteOffset, uint8_t pixelIndex) const;
  void _setPixelInByte(uint16_t byteOffset, uint8_t pixelIndex, MonoColour colour);
};

#endif // IMG_MONO
