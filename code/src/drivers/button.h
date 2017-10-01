#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "bus.h"

enum ButtonAction : uint8_t { PRESSED = 0, RELEASED = 1 };

struct ButtonEvent : public Event {
public:
  ButtonEvent();
  ButtonEvent(uint8_t pin, ButtonAction action);
  const uint8_t pin;
  const ButtonAction action;
};

class ButtonDriver {

public:
  // Button Pin Number
  const uint8_t pin;
  // Time to wait after an input to eliminate duplicates
  const uint16_t debounceMs = 20;
  // Time to wait after an interrupt before reading the pin value.
  const uint16_t settleTimeMs = 5;

  // Time used for debouncing, in MS since program start.
  volatile uint32_t ignoreUntil;

  ButtonDriver();
  ButtonDriver(uint8_t pinNumber);
  ~ButtonDriver();

  bool isPressed() const;
  uint16_t heldForMs() const;

  void registerInterrupt(void (*interrupt)(), const uint8_t state);
  void onChange(EventBus& bus);

private:
  bool _isPressed;
  uint32_t _whenPressed;
};

#endif //BUTTON_DRIVER_H
