#include "Arduino.h"

#include "Adafruit_SSD1306.h"
#include <iostream>

#include "state/state.h"
#include "state/init.h"
#include "state/basic.h"
#include "state/adv.h"
#include "state/info.h"

Adafruit_SSD1306 display(4 /* OLED RESET */);
State* state = new Initializing();
State* lastState = state;

State* states[] = {
	new Initializing(),
	new Menu(),
	new Basic(),
	new Advanced(),
	new Info()
};

void setup() {
    Serial.begin(9600);
}

void loop() { 
	while(state = states[state->process()]) {
		if (state->id() != lastState->id())
			state->onEnter();
		lastState = state;
	} 
}

void onBackButtonReleased() {
	state->backPressed();
}

void onWheelReleased() {
	state->selectPressed();
}

void onWheelScrolled(int8_t delta /* + or - */) {
	state->navTurned(delta);
}

void onReadSpeedInDeciRPM(short rpm) {}