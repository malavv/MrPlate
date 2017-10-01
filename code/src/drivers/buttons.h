#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#include "bus.h"

struct ButtonsEvent : public Event {
public:
  ButtonsEvent(uint8_t pin, bool isOn) : Event(EventType::kBtn), pin_(pin), isOn_(isOn) {}
  const uint8_t pin_;
  const bool isOn_;
};

class Button {
public:
  Button(uint8_t pin) : pin_(pin) {};
  bool isPressed();
private:
  uint8_t pin_;
};

class ButtonsDriver {
public:
  ButtonsDriver(uint8_t numButtons);
  Button bind(const uint8_t pin);

  static uint8_t numButtons_;
  static int8_t* pinEnabled_;
  static bool* pinValues_;
  
private:    
  bool registerPinForUse(uint8_t pin);
};

#endif // BUTTONS_H
