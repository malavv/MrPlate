#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include <stdio.h>
#include <ctime>

#define OLED_RESET 4
#define I2C_ADDR_1 0x3C
Adafruit_SSD1306 display(OLED_RESET);

enum AppState {
	Unknown,
	Loading,
	Menu,
	Power,
	Speed,
	About
};
AppState state = Loading;
AppState choice = Power;
std::time_t begin;

int16_t powLvl = 0;
int16_t speedLvl = 0;
int16_t speedRead = 0;

const char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const char* version = "v0.1 Amber Ale";
char buffer[50];

void onLeave(AppState state);
void onEnter(AppState state);
void setState(AppState newState);

void onReadSpeedInDeciRPM(int16_t deciRPM) {
	speedRead = deciRPM;
}

void onBackButtonReleased() {
	switch (state) {
		case Menu:
			setState(About);
			break;
		case About:
		case Power:
		case Speed:
			setState(Menu);
			break;
		default:
			break;
	}
}

void onWheelReleased() {
	if (state != Menu)
		return;

	setState(choice);
	return;
}

void onWheelScrolled(int8_t delta /* + or - */) {
	// Since there is only 2 options, not regarding the direction.
	if (state == Menu) {
		switch (choice) {
		case Power:
			choice = Speed;
			break;
		case Speed:
			choice = Power;
			break;
		}
	}

	if (state == Power) {
		powLvl += delta * delta * delta;
		powLvl = powLvl < 999 ? powLvl > 0 ? powLvl : 0 : 999;
	}
	if (state == Speed) {
		speedLvl += delta * delta * delta;
		speedLvl = speedLvl < 999 ? speedLvl > 0 ? speedLvl : 0 : 999;
	}
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDR_1);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  delay(100);
  setState(Menu);  
}

void printTime();

void ldisplay() {
	display.setCursor(0, 0);
	display.clearDisplay();

	switch (state) {
	case About:
		display.setCursor(3, 3);
		display.print(version);
		display.setCursor(100, 20);
		display.print("Back");
		break;
	case Menu:
		display.setCursor(3, 9);
		display.setTextSize(2);
		display.print("MrPlate");
		display.setTextSize(1);
		display.setCursor(100, 4);
		if (choice == Power) display.setTextColor(BLACK, WHITE);
		else display.setTextColor(WHITE);
		display.println(" pow");
		display.setCursor(100, 19);
		if (choice == Speed) display.setTextColor(BLACK, WHITE);
		else display.setTextColor(WHITE);
		display.println(" rpm");
		display.setTextColor(WHITE);
		break;
	case Speed:
		display.setTextSize(1);
		// Set Label
		display.setCursor(3, 4);
		display.println("set");
		// Read Label
		display.setCursor(3, 19);
		display.println("read");
		// Clock
		printTime();
		// RPM Label
		display.setCursor(95, 19);
		display.println("rpm");

		display.setTextSize(2);
		// Set Value
		display.setCursor(35, 0);
		sprintf(buffer, "%3u", speedLvl);
		display.println(&buffer[0]);
		// Read Value
		display.setCursor(35, 18);
		display.setTextSize(2);
		sprintf(buffer, "%3u", speedRead / 10);
		display.println(&buffer[0]);

		display.setCursor(67, 18);
		display.println(".");

		display.setCursor(77, 18);
		sprintf(buffer, "%u", speedRead % 10);
		display.println(&buffer[0]);
		break;
	case Power:
		printTime();
		// RPM Label
		display.setCursor(95, 13);
		display.println("o");
		display.setCursor(103, 19);
		display.println("oo");
		display.drawLine(98, 25, 103, 15, WHITE);

		display.setTextSize(3);
		// Set Value
		display.setCursor(15, 5);
		sprintf(buffer, "%4u", powLvl);
		display.println(&buffer[0]);
		break;
	default:
		display.print("state : ");
		display.write(digits[state]);
		break;
	}

	display.display();
}

void printTime() {
	uint16_t seconds = round(std::difftime(std::time(NULL), begin));

	display.setTextSize(1);
	// Clock
	display.setCursor(88, 4);
	sprintf(buffer, "%2d", seconds / 3600);
	display.println(&buffer[0]);

	display.setCursor(98, 4);
	display.println(":");
	display.setCursor(102, 4);
	sprintf(buffer, "%02d", (seconds % 3600) / 60);
	display.println(&buffer[0]);

	display.setCursor(112, 4);
	display.println(":");
	display.setCursor(116, 4);
	sprintf(buffer, "%02d", seconds % 60);
	display.println(&buffer[0]);
}

void loop() {
	while (true) {
		delay(100);
		ldisplay();
	}
}

void onLeave(AppState state) {
}

void onEnter(AppState state) {
	switch (state) {
	case Menu:
		choice = Power;
		break;
	case Power:
		powLvl = 0;
		std::time(&begin);
		break;
	case Speed:
		speedLvl = 0;
		std::time(&begin);
		break;
	case About:
		break;
	default:
		break;
	}
}

void setState(AppState newState) {
	onLeave(state);
	state = newState;
	onEnter(state);
}