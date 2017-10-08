#include "advancedState.h"

#include <Adafruit_SSD1306.h>

#include "../button/driver.h"

extern Adafruit_SSD1306 Display;

AdvancedState::AdvancedState(EventBus& bus) : State(StateType::Advanced, bus) {}
AdvancedState::~AdvancedState() {}

void AdvancedState::onEntering() {
  isBtn1Pressed = false;
  isBtn2Pressed = false;
}

void AdvancedState::loop(const std::shared_ptr<const Event>& evt) {
  if (evt != nil && evt->type == EventType::kBtn) {
    std::shared_ptr<const ButtonEvent> btnEvt = std::static_pointer_cast<const ButtonEvent>(evt);
    (btnEvt->pin == Pin::kBtn1 ? isBtn1Pressed : isBtn2Pressed) = btnEvt->action == ButtonAction::PRESSED;

    if (isBtn1Pressed && isBtn2Pressed)
      bus.post(new StateChangeEvent(StateType::MainMenu));
  }

  Display.clearDisplay();
  Display.setTextSize(2);
  Display.setCursor(0, 0);
  Display.println("Advanced");
  Display.display();
}

void AdvancedState::onLeaving() {}
