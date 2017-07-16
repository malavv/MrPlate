#include "welcomeState.h"

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 Display;

WelcomeState::WelcomeState(EventBus& bus)
    : State(StateType::Welcome, bus),
     textAnim(WelcomeMsg, 1000 /*ms*/),
     markeeAnim(WelcomeMsg, 5000 /*ms*/) {
  state = 0;
}

WelcomeState::~WelcomeState() {}

void WelcomeState::onEntering() {
  if (kIsDebug)
    bus.post(new StateChangeEvent(next));

  textAnim.start();
}

void WelcomeState::loop(const std::shared_ptr<const Event>& evt) {
  switch (state) {
    case 0: {
      if (textAnim.hasEnded()) { state = 1; markeeAnim.start(); }
      else textAnim.step(); break;
    }
    case 1: {
      if (markeeAnim.hasEnded()) state = 2;
      else markeeAnim.step(); break;
    }
    case 2:
      return bus.post(new StateChangeEvent(next));
  }
}

void WelcomeState::onLeaving() {}
