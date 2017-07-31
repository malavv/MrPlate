#include "imageMono/image.h"

ImageMonoImpl::ImageMonoImpl(uint16_t width, uint16_t height, MonoColour colour)
    : ImageMono(width, height) {
  _buffer = new uint8_t[width * height / 4 + 1]; /* 2 bits per pixel */
  for (uint16_t x = 0; x < width; x++)
    for (uint16_t y = 0; y < height; y++)
      set(x, y, colour);
}

ImageMonoImpl::~ImageMonoImpl() {
  delete _buffer;
}

MonoColour ImageMonoImpl::at(uint16_t x, uint16_t y) const {
  if (x >= width || y >= height)
    return kOutside;
  const uint16_t offset = x + y * width;
  return static_cast<MonoColour>(((_buffer[offset / 4]) >> (offset % 4 * 2)) & 0x3);
}

void ImageMonoImpl::set(uint16_t x, uint16_t y, MonoColour colour) {
  if (x >= width || y >= height)
    return;
  uint16_t offset = x + y * width;
  uint8_t bitOff = offset % 4;
  _buffer[offset / 4] = (_buffer[offset / 4] & ~(0x3 << (bitOff * 2))) + ((uint8_t)colour << (bitOff * 2));
}
