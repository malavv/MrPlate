#include "imageMono/image.h"

#include <cstring>

ImageMonoImpl::ImageMonoImpl(uint16_t width, uint16_t height, MonoColour colour)
    : ImageMono(width, height), _numBytes(width * height / pixelPerByte + 1) {
  // Image Mono only has 3 states to care about White, Black, and Trans. Having
  // 3 states means 2 bits per pixel. This class manages the packaging.
  _buffer = new uint8_t[_numBytes];
  std::memset(_buffer, (colour << 6) | (colour << 4) | (colour << 2) | colour, _numBytes);
}

ImageMonoImpl::~ImageMonoImpl() {
  delete _buffer;
}

MonoColour ImageMonoImpl::at(uint16_t x, uint16_t y) const {
  if (!contains(x, y))
    return kOutside;

  const uint16_t offset = x + y * width;
  return _getPixelInByte(offset / pixelPerByte, offset % pixelPerByte);
}

void ImageMonoImpl::set(uint16_t x, uint16_t y, MonoColour colour) {
  if (colour == kTrans || colour == kOutside) return;
  if (!contains(x, y)) return;

  const uint16_t offset = x + y * width;
  _setPixelInByte(offset / pixelPerByte, offset % pixelPerByte, colour);
}

void ImageMonoImpl::_setPixelInByte(uint16_t byteOffset, uint8_t pixelIndex, MonoColour colour) {
  // Convert and then safety mask it.
  const uint8_t data = static_cast<uint8_t>(colour) & rightmostPixelMask;
  // First Clears the Space then sets the right data.
  _buffer[byteOffset] &= ~(rightmostPixelMask << (pixelIndex * numBitsPerPixel));
  _buffer[byteOffset] |= data << (pixelIndex * numBitsPerPixel);
}

MonoColour ImageMonoImpl::_getPixelInByte(uint16_t byteOffset, uint8_t pixelIndex) const {
  return static_cast<MonoColour>((_buffer[byteOffset] >> (pixelIndex * numBitsPerPixel)) & rightmostPixelMask);
}
