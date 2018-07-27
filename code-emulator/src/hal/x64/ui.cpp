#include "ui.h"

#include <iostream>
#include <sstream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <atomic>
#include <cmath>
#include "LTexture.h"
#include "Arduino.h"

void onBackButtonReleased();
void onWheelReleased();
void onWheelScrolled(int8_t/* +1 or -1 */);
size_t fakeRPM = 0;
void onReadSpeedInDeciRPM(int16_t /* in deciRPM */);

int UI::setup() {
	// Start up SDL and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logError(std::cout, "SDL_Init");
		return 1;
	}

	// Setup our window and renderer, this time let's put our window in the center
	// of the screen
	window = SDL_CreateWindow("Mr. Emulator", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logError(std::cout, "CreateRenderer");
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}

	if (TTF_Init() == -1)
		logError(std::cout, "LoadFontSystem");

	gFont = TTF_OpenFont("hack.ttf", 28);
	if (gFont == nullptr) {
		logError(std::cout, "LoadFont");
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
    return 1;
	}

	return 0;
}

void UI::redraw() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &_lcd);
	drawControls(renderer, 532, 10, 10, 160, rad);
	swapBufferToScreen();

	// Draw Buttom
	SDL_Color textColor = { 255, 255, 255 };
	TTF_Font* font12 = TTF_OpenFont("hack.ttf", 12);

	std::ostringstream oss;
	const int dx = 240;
	const int dy = 15;
	for (auto &msg : debugMsg) {
		const int x = msg->pin > 16 ? 2 : msg->pin > 8 ? 1 : 0;
		const int y = msg->pin % 8;
		oss << "<" << (int)msg->pin << "> " << msg->msg << " : " << msg->val;
		gTextTexture.loadFromRenderedText(oss.str(), textColor, font12, renderer);
		gTextTexture.render(10 + x * dx, 155 + y * dy, nullptr, 0.0, nullptr, SDL_FLIP_NONE, renderer);
		oss.str("");
	}

	SDL_RenderPresent(renderer);
}

void UI::swapBufferToScreen() {
	{
		std::lock_guard<std::mutex> guard(_mutex);
		memcpy(_writeBuffer, _screenBuffer, nBytes);
	}

	size_t mult = _lcd.w / _ssd1306.w;
	_pix.h = mult;
	_pix.w = mult;
	for (int x = 0; x < _ssd1306.w; x++) {
		for (int y = 0; y < _ssd1306.h; y++) {
			bool isLit = _writeBuffer[x + (y / 8) * _ssd1306.w] & (1 << (y & 7));
			isLit = _isInverted ? !isLit : isLit;
			if (isLit) {
				_pix.x = 10 + x * mult;
				_pix.y = 10 + y * mult;
				SDL_RenderFillRect(renderer, &_pix);
			}
		}
	}
}

int UI::evtloop() {
	// Our event structure
	SDL_Event e;
	// For tracking if we want to quit
	bool quit = false;
	while (!quit) {
		//Read any events that occured, for now we'll just quit if any event occurs
		while (SDL_PollEvent(&e)) {
			//If user closes the window
			if (e.type == SDL_QUIT) quit = true;
			// if (e.type == SDL_KEYDOWN) { quit = true; }

			if (e.type == SDL_MOUSEWHEEL) {
				if (e.wheel.y == 1 /* scroll up */) {
					// Pull up code here!
					rad = rad - ((float)M_PI / 10);
				}
				else if (e.wheel.y == -1 /* scroll down */) {
					// Pull down code here!
					rad = rad + ((float)M_PI / 10);
				}
				redraw();
				onWheelScrolled(e.wheel.y);
			}
			// If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN) {

				SDL_Point mouse;
				SDL_GetMouseState(&mouse.x, &mouse.y);

				if (SDL_PointInRect(&mouse, &_backBtn)) {
					isBackPressed = true;
					redraw();
				}
				if (SDL_PointInRect(&mouse, &_wheelBtn)) {
					isWheelPressed = true;
					redraw();
				}
			}

			if (e.type == SDL_MOUSEBUTTONUP) {
				if (isWheelPressed) {
					isWheelPressed = false;
					onWheelReleased();
				}
				if (isBackPressed) {
					isBackPressed = false;
					onBackButtonReleased();
				}
				redraw();
			}
		}
		redraw();
		if (fakeRPM < 1000)
			onReadSpeedInDeciRPM(fakeRPM++);
		else
			onReadSpeedInDeciRPM(fakeRPM + rand() % 100);
	}
	return 0;
}

void UI::logError(std::ostream &os, const std::string &msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

void UI::drawPolygon(SDL_Renderer *ren, int cx, int cy, int nSides, float radius) {
	double px = 1;
	double py = 0;
	double nx = 0;
	double ny = 0;

	SDL_RenderDrawPoint(ren, cx, cy);

	for (double theta = 0; theta <= 2 * M_PI; theta += ((2 * M_PI) / nSides)) {
		nx = cos(theta);
		ny = sin(theta);

		SDL_RenderDrawLine(ren,
			(int)round(px * radius + cx), (int)round(py * radius + cy),
			(int)round(nx * radius + cx), (int)round(ny * radius + cy));

		px = nx;
		py = ny;
	}
}

void UI::drawControls(SDL_Renderer *ren, int left, int top, int right, int bottom, float rad) {
	// Wheel body
	int w = width - left - right;
	int h = height - top - bottom;
	int cx = (int)round(left + w / 2.0);
	int cy = (int)round(top + h / 4.0);
	float radius = 25.0;
	drawPolygon(ren, cx, cy, 18, radius);

	// Wheel indicator
	int dx = (int)round(cos(rad) * (radius + 2));
	int dy = (int)round(sin(rad) * (radius + 2));
	SDL_RenderDrawLine(ren, cx, cy, cx + dx, cy + dy);

	// Button Outline
	int btnWidth = 40;
	SDL_Rect rct = { cx - btnWidth / 2, (int)round(top + 3 * (h / 4.0)) - btnWidth / 2, btnWidth, btnWidth };

	// Button Infill
	if (isBackPressed)
		SDL_RenderFillRect(ren, &rct);
	else
		SDL_RenderDrawRect(ren, &rct);

	// RPM Text

}
