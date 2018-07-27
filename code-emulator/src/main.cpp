#include "Arduino.h"
#include "limits.h"

#include "Adafruit_SSD1306.h"
#include "Encoder.h"

#include "state/state.h"
#include "state/init.h"
#include "state/basic.h"
#include "state/adv.h"
#include "state/info.h"
#include "pins.h"

Adafruit_SSD1306 display(PIN_OLED_RESET);
State *lastState, *state, *states[5];
Encoder wheel(PIN_WHEEL_1, PIN_WHEEL_2);
int32_t wheel_pos = LONG_MIN;
int8_t wheel_delta = 0;

// Create it for the purpose of terminating the emulation.
volatile bool isRunning = true;

// Set of callback to be used by the Hardware Abstraction Layer (Teensy or Emulator)
void onBackButtonReleased() { state->backPressed(); }
void onWheelReleased() { state->selectPressed(); }
void onWheelScrolled(int8_t delta /* + or - */) { state->navTurned(delta); }
void onReadSpeedInDeciRPM(short rpm) {}

// Initialization of the Code
void setup() {
  Serial.begin(9600);

  pinMode(PIN_BACK_BTN, INPUT_PULLUP);
  pinMode(PIN_WHEEL_BTN, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  states[0] = new Initializing();
  states[1] = new Menu();
  states[2] = new Basic();
  states[3] = new Advanced();
  states[4] = new Info();

  state = states[0];
  lastState = state;

  attachInterrupt(PIN_BACK_BTN, onBackButtonReleased, RISING);
  attachInterrupt(PIN_WHEEL_BTN, onWheelReleased, RISING);

  analogWriteResolution(12); // analogWrite value 0 to 4095, or 4096 for high
  analogWriteFrequency(PIN_PWM_FTM0, 11718.75); // Teensy 3.0 pin 3 also changes to 375 kHz
}

// Reading Rotary Encoder
bool pool_wheel(int8_t &delta) {
  const int32_t pos = wheel.read();
  if (pos == wheel_pos)
    return false;
  if (abs(wheel_pos - pos) > 1000)
    return false;
  delta = (int8_t)constrain(wheel_pos - pos, -1000, 1000);
  wheel_pos = pos;
  return true;
}

// Main loop dealing with states and events
void loop() {
  while (isRunning /* Always True on Arduino */) {
    state = states[state->process()];
    if (!state)
      return;
    if (state->id() != lastState->id())
      state->onEnter();
    if (pool_wheel(wheel_delta))
      onWheelScrolled(wheel_delta);

    lastState = state;
  }
}
