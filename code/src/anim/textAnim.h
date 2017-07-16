#ifndef SHOW_TEXT_ANIM_H
#define SHOW_TEXT_ANIM_H

#include <stdint.h>

#include "anim.h"

class TextAnimation : public Animation {

public:
  TextAnimation(const char* msg, uint32_t duration);

  void start();
  void step();
  bool hasEnded();

private:
  uint32_t startTime;
  uint32_t duration;
  const char* msg;
};

#endif // SHOW_TEXT_ANIM_H
