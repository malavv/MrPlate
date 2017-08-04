#include "imageMono/image.h"

#include <cstring>

ImageMonoImpl::ImageMonoImpl(uint16_t width, uint16_t height, MonoColour colour)
    : ImageMono(width, height), _numBytes(width * height / pixelPerByte + 1) {
  // Image Mono only has 3 states to care about White, Black, and Trans. Having
  // 3 states means 2 bits per pixel. This class manages the packaging.
  _buffer = new uint8_t[_numBytes];

  const uint8_t setByte =
      colour == kBlack   ? 0x00 :
      colour == kWhite   ? 0b01010101 :
      colour == kTrans   ? 0b10101010 : 0xff;

  std::memset(_buffer, setByte, _numBytes);
}

ImageMonoImpl::~ImageMonoImpl() {
  delete _buffer;
}

MonoColour ImageMonoImpl::at(uint16_t x, uint16_t y) const {
  if (!contains(x, y))
    return kOutside;
  const uint16_t offset = x + y * width;
  const uint8_t byte = _buffer[offset / pixelPerByte];
  const uint8_t pixOffsetInByte = offset % pixelPerByte * bitPerPixel;
  const uint8_t bits = (byte >> pixOffsetInByte) & rightmostPixelMask;
  return static_cast<MonoColour>(bits);
}

void ImageMonoImpl::set(uint16_t x, uint16_t y, MonoColour colour) {
  if (!contains(x, y))  return;

  const uint16_t offset = x + y * width;
  const uint8_t byteOffset = offset / pixelPerByte;
  // Left to Right Associative, if you change this, put parantheses.
  uint8_t bitOffset = offset % pixelPerByte * bitPerPixel;
  _buffer[byteOffset] &= ~(rightmostPixelMask << bitOffset); // Clear color
  _buffer[byteOffset] |= (uint8_t)colour << bitOffset; // Set color
}
