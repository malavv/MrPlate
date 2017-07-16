#include "simpleState.h"

#include <Adafruit_SSD1306.h>

#include <algorithm>
#include "board.h"
#include "drivers/button.h"
#include "drivers/pwm.h"

extern Adafruit_SSD1306 Display;
extern PwmDriver pwm;
extern ButtonDriver btn1;
extern ButtonDriver btn2;

SimpleState::SimpleState(EventBus& bus) : State(StateType::Simple, bus) {}
SimpleState::~SimpleState() {}

void SimpleState::onEntering() {
  pwm.setResolution(numDigits);
  maxDuty = pwm.getMaxDutyCyle();
  duty = 0;
  setTime(0); /* Reset Board Internal Clock (to then use hours(), ...) */
}

void SimpleState::handleButton(const std::shared_ptr<const ButtonEvent>& evt) {
  if (btn1.isPressed() && btn2.isPressed())
    bus.post(new StateChangeEvent(StateType::MainMenu));

  // Check Single Button Tap.
  // The reason it is on release and not on pressed is for the ability to get both
  // the hold functionality and going back on two button pressed.
  if (evt->action == ButtonAction::RELEASED) {
    if (evt->pin == Pin::kBtn2) dec(maxPowerLvl / _dutyFrac);
    if (evt->pin == Pin::kBtn1) inc(maxPowerLvl / _dutyFrac);
  }
}

void SimpleState::inc(uint16_t deltaPower) {
  duty = duty + deltaPower > maxPowerLvl ? maxPowerLvl : duty + deltaPower;
}

void SimpleState::dec(uint16_t deltaPower) {
  duty = duty < deltaPower ? 0 : duty - deltaPower;
}

void SimpleState::loop(const std::shared_ptr<const Event>& evt) {
  if (evt != nil && evt->type == EventType::kBtn)
    handleButton(std::static_pointer_cast<const ButtonEvent>(evt));

  Display.clearDisplay();

  // Top Left Clock
  digitalClockDisplay();

  // Power Sign
  Display.setTextSize(1);
  Display.setCursor(SSD1306_LCDWIDTH - 40, 0);
  Display.print("Power");

  // Power Level
  Display.setTextSize(2);
  Display.setCursor(5, SSD1306_LCDHEIGHT / 2);
  uint32_t displayp = ((float) duty / (float)maxPowerLvl) * 10000;
  uint32_t top = displayp / 100;
  Display.printf("%3d.%02d%%\n", top, displayp - (top * 100));

  Display.display();
  pwm.setDutyCycle(duty);
}

void SimpleState::onLeaving() {
  Display.clearDisplay();
  duty = 0;
  pwm.setResolution(8); // Reverting to default
}

void SimpleState::digitalClockDisplay() {
  Display.setTextSize(1);
  Display.setCursor(0, 0);
  Display.print(hour());
  printDigits(minute());
  printDigits(second());
}

void SimpleState::printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Display.print(":");
  if(digits < 10)
    Display.print('0');
  Display.print(digits);
}
