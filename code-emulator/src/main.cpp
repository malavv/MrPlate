#include "Arduino.h"

#include "Adafruit_SSD1306.h"
#include <iostream>

#include "state/state.h"
#include "state/init.h"
#include "state/basic.h"
#include "state/adv.h"
#include "state/info.h"

Adafruit_SSD1306 display(4 /* OLED RESET */);
State *lastState, *state, *states[5];

// Initialization of the Code
void setup() {
  Serial.begin(9600);

  states[0] = new Initializing();
  states[1] = new Menu();
  states[2] = new Basic();
  states[3] = new Advanced();
  states[4] = new Info();

  state = states[0];
  lastState = state;
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

// Set of callback to be used by the Hardware Abstraction Layer (Teensy or Emulator)
void onBackButtonReleased() { state->backPressed(); }
void onWheelReleased() { state->selectPressed(); }
void onWheelScrolled(int8_t delta /* + or - */) { state->navTurned(delta); }
void onReadSpeedInDeciRPM(short rpm) {}