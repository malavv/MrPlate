#include "digitalOut.h"

#include <Arduino.h>

DigitalOut::DigitalOut(uint8_t pin) : pin_(pin), isOn_(false) {};
DigitalOut::DigitalOut(uint8_t pin, bool isOn) : pin_(pin), isOn_(isOn) {};
DigitalOut::~DigitalOut() {};
  
void DigitalOut::write(bool isOn) {
  isOn_ = isOn;
  digitalWrite(pin_, isOn_);
};
bool DigitalOut::read() { return isOn_; };
void DigitalOut::setup() { pinMode(pin_, OUTPUT); }
DigitalOut& DigitalOut::operator=(bool isOn) { 
    write(isOn);
    return *this; 
};
DigitalOut& DigitalOut::operator=(DigitalOut& rhs) { 
    write(rhs.isOn_);
    return *this; 
};
DigitalOut::operator bool() { return isOn_; };