#include "driver.h"

#include <Arduino.h>

extern EventBus bus;

void ButtonDriver::onAnyBtnTrigger() {
  for (ButtonData* dat : buttonsData_) {
    bool last = dat->isPressed;
    bool current = digitalRead(dat->pin);

    dat->isPressed = current;
    dat->isDirty = last != current;
  }
}

ButtonData* ButtonDriver::addBtnData(const uint8_t pin) {
  ButtonData* dat = new ButtonData(pin);
  buttonsData_.push_back(dat);
  return dat;
}

ButtonData* ButtonDriver::bind(const uint8_t pin) {

  ButtonData* dat = getBtnById(pin);
  if (dat != 0)
    return dat;

  // Register any of its trigger to our class.
  attachInterrupt(
    digitalPinToInterrupt(pin),
    []() { ButtonDriver::getInstance().onAnyBtnTrigger(); },
    CHANGE);

  // Reserve some memory for the button
  return ButtonDriver::getInstance().addBtnData(pin);
}

ButtonData* ButtonDriver::getBtnById(uint8_t pin) const {
  for (ButtonData* dat : buttonsData_) {
    if (dat->pin == pin)
      return dat;
  }
  return 0;
}

void ButtonDriver::broadcast(const bool isPressed, const uint8_t pin) {
  bus.post(new ButtonEvent(pin, isPressed ? PRESSED : RELEASED));
}

bool ButtonDriver::isDirty() const {
  for (ButtonData* dat : buttonsData_) {
    if (dat->isDirty)
      return true;
  }
  return false;
}

void ButtonDriver::update() {
  if (!isDirty())
    return;

  for (ButtonData* dat : buttonsData_) {
    if (!dat->isDirty)
      continue;
    dat->isDirty = false;
    broadcast(dat->isPressed, dat->pin);
  }
}

Button::Button(uint8_t pin) { data_ = ButtonDriver::getInstance().bind(pin); }
