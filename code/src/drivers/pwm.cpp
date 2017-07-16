#include "pwm.h"

#include <Arduino.h>

PwmDriver::PwmDriver(Pin pin) : pin(pin) {}
PwmDriver::~PwmDriver() {}

void PwmDriver::setResolution(uint8_t resolution /* in bits */) {
  this->resolution = resolution;
  analogWriteResolution(resolution);
  analogWriteFrequency(pin, idealFreqByResolution(resolution));
  Serial.printf("[INFO] Pwm Resolution changed to %d bits, range is now [0, %d]\n", resolution, getMaxDutyCyle());
}

uint32_t PwmDriver::getMaxDutyCyle() {
  return (1 << resolution) - 1;
}

void PwmDriver::setDutyCycle(uint32_t duty) {
  analogWrite(pin, duty);
}

uint32_t PwmDriver::idealFreqByResolution(uint8_t resolution /* in bits */) {
  switch (resolution) {
    case 8: return 187500; /* Hertz */
    case 9: return 93750;
    case 10: return 46875;
    case 11: return 23438;
    case 12: return 11719;
    case 13: return 5859;
    case 14: return 2930;
    case 15: return 1465;
    case 16: return 732;
    default:
      Serial.printf("[WARN] Unsupported pwm resolution %d\n", resolution);
      return 488;
  }
}
