#ifndef ADVANCED_STATE_H
#define ADVANCED_STATE_H

#include "state.h"
#include "bus.h"
#include "board.h"

class AdvancedState : public State {

public:
  AdvancedState(EventBus& bus);
  virtual ~AdvancedState();

  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
  bool isBtn1Pressed, isBtn2Pressed;

};

#endif // ADVANCED_STATE_H
