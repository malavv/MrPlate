#include "imageMono.h"

ImageMonoImpl::ImageMonoImpl(uint8_t width, uint8_t height, MonoColour colour)
    : ImageMono(width, height) {
  _buffer = new uint8_t[width * height / 4 + 1]; /* 2 bits per pixel */
  for (uint8_t x = 0; x < width; x++)
    for (uint8_t y = 0; y < height; y++)
      set(x, y, colour);
}

ImageMonoImpl::~ImageMonoImpl() {
  delete _buffer;
}

MonoColour ImageMonoImpl::at(uint8_t x, uint8_t y) const {
  if (x >= width || y >= height)
    return kOutside;
  uint16_t offset = x + (uint16_t)y * width;
  uint8_t b = _buffer[offset / 4];
  uint8_t t = offset % 4;
  return static_cast<MonoColour>((b >> (t * 2)) & 0x3);
}

void ImageMonoImpl::set(uint8_t x, uint8_t y, MonoColour colour) {
  if (x >= width || y >= height)
    return;
  uint16_t offset = x + (uint16_t)y * width;
  uint8_t t = offset % 4;
  uint8_t mask = ~(0x3 << (t * 2));
  _buffer[offset / 4] = (_buffer[offset / 4] & mask) + ((uint8_t)colour << (t * 2));
}
