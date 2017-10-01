#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

#include <stdint.h>

class DigitalOut {

public:
  DigitalOut(uint8_t pin);
  DigitalOut(uint8_t pin, bool value);
  ~DigitalOut();
  
  void write(bool isOn);
  void setup();
  bool read();

  DigitalOut& operator=(bool isOn);
  DigitalOut& operator=(DigitalOut& rhs);
  operator bool();

private:
  const uint8_t pin_;
  bool isOn_;
};

#endif // DIGITAL_OUT_H
