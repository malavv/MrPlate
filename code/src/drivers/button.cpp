#include "button.h"

#include "Arduino.h"
#include "board.h"

ButtonEvent::ButtonEvent(uint8_t pin, ButtonAction action)
  : Event(EventType::kBtn), pin(pin), action(action) {}

ButtonDriver::ButtonDriver(uint8_t pinNumber) : pin(pinNumber), ignoreUntil(0) {}
ButtonDriver::~ButtonDriver() {}

void ButtonDriver::registerInterrupt(void (*interrupt)(), const uint8_t state) {
  pinMode(pin, INPUT);
  attachInterrupt(pin, interrupt, state);
}

void ButtonDriver::onChange(EventBus& bus) {
  const uint32_t currentTimeMs = millis();
  if (currentTimeMs < ignoreUntil)
    return;

  ignoreUntil = currentTimeMs + debounceMs;
  delay(settleTimeMs);
  bus.post(new ButtonEvent(pin, digitalRead(pin) == 1 ? PRESSED : RELEASED));
}
