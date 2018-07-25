#include "Arduino.h"

#include "Adafruit_SSD1306.h"

#include "state/state.h"
#include "state/init.h"
#include "state/basic.h"
#include "state/adv.h"
#include "state/info.h"
#include "pins.h"

Adafruit_SSD1306 display(PIN_OLED_RESET);
State *lastState, *state, *states[5];

// Set of callback to be used by the Hardware Abstraction Layer (Teensy or Emulator)
void onBackButtonReleased() { state->backPressed(); }
void onWheelReleased() { state->selectPressed(); }
void onWheelScrolled(int8_t delta /* + or - */) { state->navTurned(delta); }
void onReadSpeedInDeciRPM(short rpm) {}

// Initialization of the Code
void setup() {
  Serial.begin(9600);

  pinMode(PIN_BACK_BTN,INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);

  states[0] = new Initializing();
  states[1] = new Menu();
  states[2] = new Basic();
  states[3] = new Advanced();
  states[4] = new Info();

  state = states[0];
  lastState = state;

  digitalWrite(PIN_LED, HIGH);
  attachInterrupt(PIN_BACK_BTN, onBackButtonReleased, RISING);
}

// Main loop dealing with states and events
void loop() {
  while (true) {
    state = states[state->process()];
    if (!state)
      return;
    if (state->id() != lastState->id())
      state->onEnter();
    lastState = state;
  }
}
