#ifndef SIMPLE_STATE_H
#define SIMPLE_STATE_H

#include <Time.h>

#include "state.h"
#include "bus.h"
#include "../button/driver.h"

class SimpleState : public State {

public:
  SimpleState(EventBus& bus);
  virtual ~SimpleState();
  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
  // Each press inc/dec by 1/_dutyFrac of max duty.
  const uint32_t _dutyFrac = 40;

  uint32_t duty;
  uint32_t maxDuty;

    const uint16_t minPowerLvl = 0, maxPowerLvl = 2048, numDigits = 11;

    void printDigits(int digits);
    void digitalClockDisplay();
    void handleButton(const std::shared_ptr<const ButtonEvent>& evt);

    void inc(uint16_t deltaPower);
    void dec(uint16_t deltaPower);
};

#endif // SIMPLE_STATE_H
