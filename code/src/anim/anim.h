#ifndef ANIM_H
#define ANIM_H

#include "../bus.h"

class Animation {

public:
  virtual void start() = 0;
  virtual void step() = 0;
  virtual bool hasEnded() = 0;
};

#endif // ANIM_H
