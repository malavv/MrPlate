#include "buttons.h"

#include <Arduino.h>

extern EventBus bus;

uint8_t ButtonsDriver::numButtons_;
int8_t* ButtonsDriver::pinEnabled_;
bool* ButtonsDriver::pinValues_;

ButtonsDriver::ButtonsDriver(uint8_t numButtons) {
  numButtons_ = numButtons;
  pinEnabled_ = new int8_t[numButtons_];
  pinValues_ = new bool[numButtons_]();
  for (uint8_t i = 0; i < numButtons; i++) 
    pinEnabled_[i] = -1; 
}

bool ButtonsDriver::registerPinForUse(uint8_t pin) {
  for (uint8_t i = 0; i < numButtons_; i++) {
    if (pinEnabled_[i] != -1) 
      continue;
    pinEnabled_[i] = pin;
    pinValues_[i] = false;
    return false;
  }
  return true;
}

Button ButtonsDriver::bind(uint8_t pin) {

  registerPinForUse(pin);

  auto onAnyChanged = []() {
    for (uint8_t i = 0; i < numButtons_; i++) {
      auto pinNumber = pinEnabled_[i];
      if (pinNumber == -1)  
        continue;

      auto current = digitalRead(pinNumber);
      auto last = pinValues_[i];
      if (current == last)
        continue;
      
      pinValues_[i] = current;
      bus.post(new ButtonsEvent(pinNumber, current)); 
    }
  };
  
  attachInterrupt(digitalPinToInterrupt(pin), onAnyChanged, CHANGE);

  Button button(pin);
  return button;
}

bool Button::isPressed() {
  for (uint8_t i = 0; i < ButtonsDriver::numButtons_; i++) {
    if (ButtonsDriver::pinEnabled_[i] == pin_)
      return ButtonsDriver::pinValues_[i];
  }
  return false;
}