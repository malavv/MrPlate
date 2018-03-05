#include "ui.h"

#include <thread>
#include <chrono>
#include <mutex>
#include "Adafruit_SSD1306.h"

std::mutex screenMutex;

// function call from Arduino
void setup();
void loop();

// UI main code, launching Arduino on a separate thread.
int main(int, char**) {
	UI& ui = UI::get();
	
	ui.setup();

	std::thread th([] {
		setup();
		loop();
	});
	
	ui.evtloop();

	th.join();
	ui.destroy();

	return 0;
}

void delay(unsigned long ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
long random(long min, long max) { 
	return rand() % (max - min) + min; 
}
long random(long max) {	
	return random(0, max); 
}
