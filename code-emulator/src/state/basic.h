#ifndef _STATE_BASIC_H_
#define _STATE_BASIC_H_

#include "Adafruit_SSD1306.h"
#include "state/init.h"
#include "state/menu.h"
#include "pins.h"

extern Adafruit_SSD1306 display;

class Basic : public State {
public:
	Basic() {
		wantsBack = false;
		power = 0;
	};
	~Basic() {};

	virtual States process() {
		if (wantsBack)
		  return MENU;

		display.clearDisplay();
		display.setCursor(0, 12);
		display.println("Basic State");

		display.setCursor(85, 12);
		display.println(power);
		analogWrite(PIN_PWM_FTM0, power);
		display.display();

		delay(30);

		return BASIC;
	};

	virtual State* onEnter() {
		wantsBack = false;
		power = 0;
		return this; 
	}
	virtual int8_t id() { return BASIC; }
	virtual void backPressed() { wantsBack = true; }
	virtual void selectPressed() {
		Serial.println("Select Pressed in basic");
	}
	virtual void navTurned(int8_t delta) {
		power -= delta;
		if (power > 4096) power = 4096;
		if (power < 0) power = 0;
	}

private:
	bool wantsBack;
	int16_t power;
};

#endif // _STATE_BASIC_H_