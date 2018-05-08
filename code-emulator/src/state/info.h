#ifndef _STATE_INFO_H_
#define _STATE_INFO_H_

#include "Adafruit_SSD1306.h"
#include "state/init.h"

extern Adafruit_SSD1306 display;

class Info : public State {
public:
	Info() { wantsOut = false; };
	~Info() {};

	virtual States process() {
		if (wantsOut)
			return MENU;

		display.clearDisplay();
		display.setCursor(15, 12);
		display.println("v0.1");
		display.display();

		delay(30);

		return INFO;
	};
	virtual void backPressed() {
		wantsOut = true;
	}
	virtual void selectPressed() {}
	virtual State* onEnter() {
		wantsOut = false;
		return this;
	}
	virtual int8_t id() { return INFO; }
	virtual void navTurned(int8_t delta) { }

private:
	bool wantsOut;
};

#endif // _STATE_INFO_H_