#include "ui.h"

#include "Adafruit_SSD1306.h"

#include "SDL.h"
#include <iostream>

// function call from Arduino
void setup();
void loop();

extern bool isRunning;

// UI main code, launching Arduino on a separate thread.
int main(int, char**) {

	UI ui;

	ui.setup();
	Adafruit_SSD1306::setUI(&ui);

	std::thread th([] {
		setup();
		loop();
	});
	
	ui.evtloop();

  isRunning = false;
	th.join();

	return 0;
}