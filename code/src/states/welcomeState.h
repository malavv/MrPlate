#ifndef WELCOME_STATE_H
#define WELCOME_STATE_H

#include "state.h"
#include "bus.h"

class WelcomeState : public State {

public:
  WelcomeState(EventBus& bus);
  virtual ~WelcomeState();
  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
    const char* WelcomeMsg = "Mr. Plate welcomes you to some yeast extravaganza v1.0";

};

#endif // WELCOME_STATE_H
