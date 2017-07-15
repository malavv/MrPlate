#include "simpleState.h"

#include <Adafruit_SSD1306.h>

#include <algorithm>
#include "board.h"

extern Adafruit_SSD1306 Display;

SimpleState::SimpleState(EventBus& bus) : State(StateType::Simple, bus) {
  isBtn1Pressed = false;
  isBtn2Pressed = false;
  powerLevel = 0;
}
SimpleState::~SimpleState() {}
void SimpleState::onEntering() {
  analogWriteResolution(numDigits); // Setting Highest Possible
  analogWriteFrequency(Pin::kPwm, 	freq);
  isBtn1Pressed = false;
  isBtn2Pressed = false;
  powerLevel = 0;
  startTime = now();
  setTime(0);
}

void SimpleState::handleButton(const std::shared_ptr<const ButtonEvent>& evt) {

  if (evt->pin == Pin::kBtn1) {
    if (evt->action == ButtonAction::PRESSED) {
      isBtn1Pressed = true;
      btn1SetTimeMs = millis();
    } else {
      isBtn1Pressed = false;
    }
  }

  if (evt->pin == Pin::kBtn2) {
    if (evt->action == ButtonAction::PRESSED) {
      isBtn2Pressed = true;
      btn2SetTimeMs = millis();
    } else {
      isBtn2Pressed = false;
    }
  }

  if (isBtn1Pressed && isBtn2Pressed)
    bus.post(new StateChangeEvent(StateType::MainMenu));

  // Check Single Button Tap.
  // The reason it is on release and not on pressed is for the ability to get both
  // the hold functionality and going back on two button pressed.
  if (evt->action == ButtonAction::RELEASED) {
    if (evt->pin == Pin::kBtn1) inc(maxPowerLvl / 40);
    if (evt->pin == Pin::kBtn2) dec(maxPowerLvl / 40);
  }
}

void SimpleState::inc(uint16_t deltaPower) {
  powerLevel = std::min((uint16_t)(powerLevel + deltaPower), maxPowerLvl);
}
void SimpleState::dec(uint16_t deltaPower) {
  powerLevel = powerLevel < (minPowerLvl + deltaPower) ? minPowerLvl : powerLevel - deltaPower;
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
  Display.printf("%04d/%d", powerLevel, maxPowerLvl);

  Display.display();

  analogWrite(Pin::kPwm, powerLevel);
}

void SimpleState::onLeaving() {
  Display.clearDisplay();
  powerLevel = 0;
  analogWriteResolution(8); // Reverting to default
  analogWriteFrequency(Pin::kPwm, 488); // Reverting to default
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
