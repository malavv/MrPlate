#include "welcomeState.h"

#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 Display;

WelcomeState::WelcomeState(EventBus& bus) : State(StateType::Welcome, bus) {}
WelcomeState::~WelcomeState() {}

void WelcomeState::onEntering() {}

void WelcomeState::loop(const std::shared_ptr<const Event>& evt) {
  // Prints welcome Message.
  Display.setTextSize(2);
  Display.setTextWrap(false);
  Display.setTextColor(WHITE);
  Display.setCursor(0, 0);

  Display.clearDisplay();
  Display.println(WelcomeMsg);
  Display.display();
  delay(1000);

  for (int i = 0; i < 180; i++) {
    Display.setCursor(-i * 4, 0);
    Display.clearDisplay();
    Display.println(WelcomeMsg);
    Display.display();
  }

  bus.post(new StateChangeEvent(StateType::MainMenu));
}

void WelcomeState::onLeaving() {}
