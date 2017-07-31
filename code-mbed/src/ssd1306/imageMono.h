#ifndef IMG_MONO_h
#define IMG_MONO_h

#include "mbed.h"

enum MonoColour : uint8_t {
  kBlack,
  kWhite,
  kTrans,
  kOutside
};

class ImageMono {
  public:
  ImageMono(uint8_t width, uint8_t height) : width(width), height(height) {}
  virtual ~ImageMono() {};

  virtual MonoColour at(uint8_t x, uint8_t y) const = 0;
  virtual void set(uint8_t x, uint8_t y, MonoColour colour) = 0;

  const uint8_t width, height;
};

class ImageFixed : public ImageMono {
  public:
  ImageFixed(uint8_t width, uint8_t height, MonoColour colour)
    : ImageMono(width, height), _colour(colour) {}

  virtual MonoColour at(uint8_t x, uint8_t y) const { return _colour; }
  virtual void set(uint8_t x, uint8_t y, MonoColour colour) { return; }

  private:
  MonoColour _colour;
};

class ImageMonoImpl : public ImageMono {
  public:
  ImageMonoImpl(uint8_t width, uint8_t height, MonoColour colour = kTrans);
  virtual ~ImageMonoImpl();

  virtual MonoColour at(uint8_t x, uint8_t y) const;
  virtual void set(uint8_t x, uint8_t y, MonoColour colour);

  private:
  uint8_t* _buffer;
};

#endif // IMG_MONO
