#ifndef BTN_DRIVER_H
#define BTN_DRIVER_H

#include <stdint.h>
#include <functional>

#include "../bus.h"

class Button {
public:
  Button(uint8_t pin);
  ~Button();
  bool isPressed();
  int8_t onPressed(std::function<void (void)> callback);
  int8_t onReleased(std::function<void (void)> callback);

private:
  uint8_t pin_;
};

class ButtonDriver {
public:
  ButtonDriver(uint8_t numButtons);
  Button bind(const uint8_t pin);
  void update();
  
private:
  friend class Button;
  static uint8_t numButtons_;
  static int8_t* pinEnabled_;
  static bool* pinValues_;
  static bool isDirty_;

  bool registerPinForUse(uint8_t pin);
};

enum ButtonAction : uint8_t { PRESSED = 0, RELEASED = 1 };

struct ButtonEvent : public Event {
public:
  ButtonEvent();
  ButtonEvent(uint8_t pin, ButtonAction action);
  const uint8_t pin;
  const ButtonAction action;
};

#endif // BTN_DRIVER_H
