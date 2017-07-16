#include "welcomeState.h"

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 Display;

WelcomeState::WelcomeState(EventBus& bus)
    : State(StateType::Welcome, bus),
     animation1(WelcomeMsg, 1000 /*ms*/),
     animation2(WelcomeMsg, 5000 /*ms*/) {
  state = 0;
}

WelcomeState::~WelcomeState() {}

void WelcomeState::onEntering() {
  if (kIsDebug)
    bus.post(new StateChangeEvent(next));

  Serial.println("void WelcomeState::onEntering()");

  animation1.start();
}

void WelcomeState::loop(const std::shared_ptr<const Event>& evt) {
  switch (state) {
    case 0: { if (animation1.hasEnded()) { state = 1; animation2.start(); } else animation1.step(); break; }
    case 1: { if (animation2.hasEnded()) state = 2; else animation2.step(); break; }
    case 2: return bus.post(new StateChangeEvent(next));
  }
}

void WelcomeState::onLeaving() {}
