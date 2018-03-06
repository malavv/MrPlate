#include <Arduino.h>
#include <iostream>
#include "Adafruit_SSD1306.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void onBackButtonReleased() {
	std::cout << "back button released" << std::endl;
}

void onWheelReleased() {
	std::cout << "wheel released" << std::endl;
}

void onWheelScrolled(int8_t delta /* + or - */) {
	std::cout << "wheel scroll " << (int)delta << std::endl;
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.display();
	delay(2000);

	// Clear the buffer.
	display.clearDisplay();
}

void loop() {
	while (true) {
		delay(20);
	}
}