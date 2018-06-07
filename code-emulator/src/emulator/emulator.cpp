#define SDL_MAIN_NEEDED
#define SDL_MAIN_HANDLED

#include "ui.h"

#include "Adafruit_SSD1306.h"

// function call from Arduino
void setup();
void loop();

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

	th.join();

	return 0;
}