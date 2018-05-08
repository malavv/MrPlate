#ifndef _STATE_INIT_H_
#define _STATE_INIT_H_

#include "Adafruit_SSD1306.h"
#include "state/init.h"
#include "state/menu.h"

extern Adafruit_SSD1306 display;

class Initializing : public State {
public:
	Initializing() {};
	~Initializing() {};

	virtual State* onEnter() { return this; }
	virtual int8_t id() { return INIT; }

	virtual States process() {
		display.begin(SSD1306_SWITCHCAPVCC /* Provide External 3.3V */, 0x3C /* I2C Address of the 128x32 */);
		// Begin Initializing
		display.clearDisplay();
		display.setTextSize(1);
		display.setTextColor(WHITE);
		display.setCursor(0, 12);
		display.println("  Initializing");
		display.display();

		delay(500);

		display.drawPixel(87, 18, WHITE);
		display.display();

		delay(500);

		display.drawPixel(90, 18, WHITE);
		display.display();

		delay(500);

		display.drawPixel(93, 18, WHITE);
		display.display();

		delay(500);
		return MENU;
	};

	virtual void backPressed() {}
	virtual void selectPressed() {}
	virtual void navTurned(int8_t delta) {}
};

#endif // _STATE_INIT_H_