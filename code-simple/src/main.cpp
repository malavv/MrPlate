
#include <Arduino.h>
#include "Adafruit_SSD1306.h"

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

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