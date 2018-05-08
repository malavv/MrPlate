#ifndef _STATE_ADV_H_
#define _STATE_ADV_H_

#include "Adafruit_SSD1306.h"
#include "state/init.h"
#include "state/menu.h"

extern Adafruit_SSD1306 display;

class Advanced : public State {
public:
	Advanced() {
		wantsBack = false;
	};
	~Advanced() {};

	virtual States process() {
		if (wantsBack)
			return MENU;

		display.clearDisplay();
		display.setCursor(0, 12);
		display.println("Advanced State");

		display.display();

		delay(30);

		return ADVAN;
	};

	virtual State* onEnter() {
		wantsBack = false;
		return this; 
	}
	virtual int8_t id() { return ADVAN; }
	virtual void backPressed() { wantsBack = true; }
	virtual void selectPressed() {}
	virtual void navTurned(int8_t delta) {}

private:
	bool wantsBack;
};

#endif // _STATE_ADV_H_