#ifndef MARKEE_ANIM_H
#define MARKEE_ANIM_H

#include <stdint.h>
#include "anim.h"

class MarkeeAnimation : public Animation {

public:
  MarkeeAnimation(const char* msg, uint32_t duration);

  virtual void start();
  virtual void step();
  virtual bool hasEnded();

private:
  uint16_t cursor;
  uint32_t startTime;
  uint32_t duration;
  const char* msg;
  const uint8_t deltaPx = 2;
};

#endif // MARKEE_ANIM_H
