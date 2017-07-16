#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "board.h"

class PwmDriver {

public:
  PwmDriver(Pin pin);
  ~PwmDriver();

  /** Maximum Duty Cycle Value */
  uint32_t getMaxDutyCyle();

  /** Allows for more resolution in duty cycle */
  void setResolution(uint8_t resolution /* in bits */);
  /** Sets the duty cycle value from 0 to getMaxDutyCyle() */
  void setDutyCycle(uint32_t duty);

private:
  const Pin pin;
  const uint8_t defaultResolutionBits = 8; /* range: 0-255 */

  uint8_t resolution; /* in bits */
  uint16_t frequency; /* in Hertz */

  uint32_t idealFreqByResolution(uint8_t resolution /* in bits */);
};

#endif //PWM_DRIVER_H
