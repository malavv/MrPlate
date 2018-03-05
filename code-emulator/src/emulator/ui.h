#ifndef _UI_H_
#define _UI_H_

#include <iostream>
#include <SDL.h>
#include <mutex>
#include <atomic>

class UI {
public:
	static UI& get() {
		// Since it's a static variable, if the class has already been created,
		// it won't be created again.
		// And it **is** thread-safe in C++11.
		static UI _instance;

		// Return a reference to our instance.
		return _instance;
	}

	// delete copy and move constructors and assign operators
	UI(UI const&) = delete;             // Copy construct
	UI(UI&&) = delete;                  // Move construct
	UI& operator=(UI const&) = delete;  // Copy assign
	UI& operator=(UI &&) = delete;      // Move assign

	int setup();
	int evtloop();
	int destroy();
	std::mutex& getMutex() { return _mutex; }
	
	void setIsInverted(bool isInverted) { _isInverted = isInverted; }
	void writeToScreenBuffer(uint8_t* data, size_t length) {
		std::lock_guard<std::mutex> guard(getMutex());
		memcpy(_screenBuffer, data, length);
	}

private:
	UI() {
		_ui = { 0, 0, 592, 148 };
		_lcd = { 10, 10, 512, 128 };
		_ssd1306 = { 0, 0, 128, 32 };
		_btn1 = { 537, 85, 40, 40 };
		_btn2 = { 534, 20, 45, 45 };

		nBytes = (_ssd1306.h * _ssd1306.w) / 8;
		_writeBuffer = new uint8_t[nBytes];
		_screenBuffer = new uint8_t[nBytes];
	}

	~UI() {
		delete[] _screenBuffer;
		delete[] _writeBuffer;
		delete renderer;
		delete window;
	}

	void drawPolygon(SDL_Renderer * ren, int cx, int cy, int nSides, float radius);
	void drawControls(SDL_Renderer * ren, int left, int top, int right, int bottom, float rad, bool isPressed);
	void logError(std::ostream &os, const std::string &msg);
	void redraw();

	static const int width = 592;
	static const int height = 148;

	static UI* _instance;

	SDL_Renderer* renderer;
	SDL_Window* window;

	std::mutex _mutex;
	uint8_t* _screenBuffer;
	uint8_t* _writeBuffer;
	std::atomic<bool> _isInverted;

	float rad = 0;
	bool isPressed = false;

	size_t screenWidth;
	size_t screenHeight;
	size_t nBytes;

	SDL_Rect _ui, _lcd, _ssd1306, _pix;

	SDL_Rect _btn1, _btn2;
};

#endif // _UI_H_