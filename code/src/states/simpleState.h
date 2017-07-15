#ifndef SIMPLE_STATE_H
#define SIMPLE_STATE_H

#include <Time.h>

#include "state.h"
#include "bus.h"
#include "../drivers/button.h"

class SimpleState : public State {

public:
  SimpleState(EventBus& bus);
  virtual ~SimpleState();
  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
    bool isBtn1Pressed, isBtn2Pressed;
    uint16_t powerLevel;
    const uint16_t minPowerLvl = 0, maxPowerLvl = 2048, numDigits = 11, freq = 23438;

    uint32_t btn1SetTimeMs, btn2SetTimeMs;
    time_t startTime;

    void printDigits(int digits);
    void digitalClockDisplay();
    void handleButton(const std::shared_ptr<const ButtonEvent>& evt);

    void inc(uint16_t deltaPower);
    void dec(uint16_t deltaPower);
};

#endif // SIMPLE_STATE_H
