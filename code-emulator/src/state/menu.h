#ifndef _STATE_MENU_H_
#define _STATE_MENU_H_

#include "Adafruit_SSD1306.h"
#include "state/init.h"

extern Adafruit_SSD1306 display;

class Menu : public State {
public:
	Menu() {
		index = 0;
		wantsOut = false;
	};
	~Menu() {};

	virtual States process() {
		if (toDetails)
			return INFO;
		if (wantsOut)
			return index <= 0 ? BASIC : ADVAN;

		display.clearDisplay();
		display.setCursor(0, 12);
		display.println("Basic");

		display.drawFastHLine(index <= 0 ? 0 : 60, 20, 35, WHITE);

		display.setCursor(60, 12);
		display.println("Advan");

		display.display();

		delay(30);

		return MENU;
	};

	virtual void backPressed() {
		toDetails = true;
	}
	virtual void selectPressed() { 
		wantsOut = true; 
	}
	virtual State* onEnter() {
		index = 0;
		wantsOut = false;
		toDetails = false;
		return this; 
	}
	virtual int8_t id() { return MENU; }
	virtual void navTurned(int8_t delta) {
		index -= delta;
		if (index < -5) index = -5;
		if (index > 5) index = 5;
	}

private:
	int8_t index;
	bool wantsOut;
	bool toDetails;
};

#endif // _STATE_MENU_H_