#include "driver.h"

#include <Arduino.h>

uint8_t ButtonDriver::numButtons_;
int8_t* ButtonDriver::pinEnabled_;
bool* ButtonDriver::pinValues_;
bool ButtonDriver::isDirty_ = false;

ButtonDriver::ButtonDriver(uint8_t numButtons) {
  numButtons_ = numButtons;
  pinEnabled_ = new int8_t[numButtons_];
  pinValues_ = new bool[numButtons_]();
  for (uint8_t i = 0; i < numButtons; i++) 
    pinEnabled_[i] = -1; 
}

bool ButtonDriver::registerPinForUse(uint8_t pin) {
  for (uint8_t i = 0; i < numButtons_; i++) {
    if (pinEnabled_[i] != -1) 
      continue;
    pinEnabled_[i] = pin;
    pinValues_[i] = false;
    return false;
  }
  return true;
}

Button ButtonDriver::bind(uint8_t pin) {

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
      isDirty_ = true;
    }
  };
  
  attachInterrupt(digitalPinToInterrupt(pin), onAnyChanged, CHANGE);

  Button button(pin);
  return button;
}

void ButtonDriver::update() {
  if (isDirty_)
    Serial.println("foobar\n");
}

Button::Button(uint8_t pin) : pin_(pin) {}

Button::~Button() {}

bool Button::isPressed() {
  for (uint8_t i = 0; i < ButtonDriver::numButtons_; i++) {
    if (ButtonDriver::pinEnabled_[i] == pin_)
      return ButtonDriver::pinValues_[i];
  }
  return false;
}

int8_t Button::onPressed(std::function<void (void)> callback) {
  return 0;
}
int8_t Button::onReleased(std::function<void (void)> callback) {
  return 0;
}