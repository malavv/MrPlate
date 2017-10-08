#include "button.h"

#include "Arduino.h"
#include "board.h"


ButtonEvent::ButtonEvent(uint8_t pin, ButtonAction action)
  : Event(EventType::kBtn), pin(pin), action(action) {}

  ButtonDriverOld::ButtonDriverOld()
  : pin(0), ignoreUntil(0), _isPressed(false), _whenPressed(0) {}
  ButtonDriverOld::ButtonDriverOld(uint8_t pinNumber)
  : pin(pinNumber), ignoreUntil(0), _isPressed(false), _whenPressed(0) {}
  ButtonDriverOld::~ButtonDriverOld() {}

void ButtonDriverOld::registerInterrupt(void (*interrupt)(), const uint8_t state) {
  pinMode(pin, INPUT);
  attachInterrupt(pin, interrupt, state);
}

void ButtonDriverOld::onChange(EventBus& bus) {
  const uint32_t currentTimeMs = millis();
  if (currentTimeMs < ignoreUntil)
    return;

  ignoreUntil = currentTimeMs + debounceMs;
  delay(settleTimeMs);
  ButtonAction action = digitalRead(pin) == 1 ? PRESSED : RELEASED;

  _isPressed = action == PRESSED;
  _whenPressed = millis();

  bus.post(new ButtonEvent(pin, action));
}

uint16_t ButtonDriverOld::heldForMs() const {
  return _isPressed ? millis() - _whenPressed : 0;
}

bool ButtonDriverOld::isPressed() const {
  return _isPressed;
}
