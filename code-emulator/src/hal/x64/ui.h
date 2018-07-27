#ifndef _UI_H_
#define _UI_H_

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <mutex>
#include <atomic>
#include "LTexture.h"

class UI {
public:
	UI() {
		_ui = { 0, 0, 592, 248 };
		_lcd = { 10, 10, 512, 128 };
		_ssd1306 = { 0, 0, 128, 32 };
		_backBtn = { 587, 85, 40, 40 };
		_wheelBtn = { 584, 20, 45, 45 };

		nBytes = (_ssd1306.h * _ssd1306.w) / 8;
		_writeBuffer = new uint8_t[nBytes];
		_screenBuffer = new uint8_t[nBytes];
		_isInverted = false;
	}

	~UI() {
		delete[] _screenBuffer;
		delete[] _writeBuffer;

		// Destroy the various SDL Elements
		if (renderer)
			SDL_DestroyRenderer(renderer);
		if (window)
			SDL_DestroyWindow(window);
		if (gFont)
			TTF_CloseFont(gFont);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	UI(UI const&) = delete;             // Copy construct
	UI(UI&&) = delete;                  // Move construct
	UI& operator=(UI const&) = delete;  // Copy assign
	UI& operator=(UI &&) = delete;      // Move assign

	int setup();
	int evtloop();
	
	void setIsInverted(bool isInverted) { _isInverted = isInverted; }
	void writeToScreenBuffer(uint8_t* data, size_t length) {
		std::lock_guard<std::mutex> guard(_mutex);
		memcpy(_screenBuffer, data, length);
	}

private:
	void drawPolygon(SDL_Renderer * ren, int cx, int cy, int nSides, float radius);
	void drawControls(SDL_Renderer * ren, int left, int top, int right, int bottom, float rad);
	void logError(std::ostream &os, const std::string &msg);
	void redraw();
	void swapBufferToScreen();

	static const int width = 692;
	static const int height = 298;

	static UI* _instance;

	SDL_Renderer* renderer;
	SDL_Window* window;

	std::mutex _mutex;
	uint8_t* _screenBuffer;
	uint8_t* _writeBuffer;
	std::atomic<bool> _isInverted;

	float rad = 0;
	bool isWheelPressed = false;
	bool isBackPressed = false;

	size_t screenWidth;
	size_t screenHeight;
	size_t nBytes;

	SDL_Rect _ui, _lcd, _ssd1306, _pix;

	SDL_Rect _backBtn, _wheelBtn;

	TTF_Font* gFont;
  LTexture gTextTexture;
};

#endif // _UI_H_