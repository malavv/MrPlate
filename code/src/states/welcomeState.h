#ifndef WELCOME_STATE_H
#define WELCOME_STATE_H

#include "state.h"
#include "bus.h"
#include "anim/textAnim.h"
#include "anim/markeeAnim.h"

class WelcomeState : public State {

public:
  WelcomeState(EventBus& bus);
  virtual ~WelcomeState();
  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
  const char* WelcomeMsg = "MrPlate welcomes you to a yeast extravaganza v1.0";
  const StateType next = StateType::MainMenu;
  TextAnimation textAnim;
  MarkeeAnimation markeeAnim;
  uint8_t state;
};

#endif // WELCOME_STATE_H
