#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include "bus.h"

enum ButtonAction : uint8_t { PRESSED = 0, RELEASED = 1 };

struct ButtonEvent : public Event {
public:
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

    ButtonDriver(uint8_t pinNumber);
    ~ButtonDriver();

    void registerInterrupt(void (*interrupt)(), const uint8_t state);
    void onChange(EventBus& bus);
};

#endif //BUTTON_DRIVER_H
