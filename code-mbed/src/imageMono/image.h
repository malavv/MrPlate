#ifndef IMG_MONO_h
#define IMG_MONO_h

#include <stdint.h>

enum MonoColour {
  kBlack = 0,
  kWhite,
  kTrans,
  kOutside
};

class ImageMono {
public:
  ImageMono(uint16_t width, uint16_t height) : width(width), height(height) {}
  virtual ~ImageMono() {};

  virtual MonoColour at(uint16_t x, uint16_t y) const = 0;
  virtual void set(uint16_t x, uint16_t y, MonoColour colour) = 0;

  const uint16_t width, height;
};

class ImageFixed : public ImageMono {
public:
  ImageFixed(uint16_t width, uint16_t height, MonoColour colour)
    : ImageMono(width, height), _colour(colour) {}

  virtual MonoColour at(uint16_t x, uint16_t y) const { return _colour; }
  virtual void set(uint16_t x, uint16_t y, MonoColour colour) { return; }

private:
  MonoColour _colour;
};

class ImageMonoImpl : public ImageMono {
public:
  ImageMonoImpl(uint16_t width, uint16_t height, MonoColour colour = kTrans);
  virtual ~ImageMonoImpl();

  virtual MonoColour at(uint16_t x, uint16_t y) const;
  virtual void set(uint16_t x, uint16_t y, MonoColour colour);

private:
  uint8_t* _buffer;
};

#endif // IMG_MONO
