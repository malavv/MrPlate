/**
 * Arduino.h implements the built-in functionalities of the Arduino framework.
 *
 * Everything is kept inline and simple.
 */

#ifndef Arduino_h
#define Arduino_h

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <io.h>
#include <thread>
#include <iostream>

typedef bool boolean;

inline long random(long min, long max) {
	return rand() % (max - min) + min;
}
inline long random(long max) {
	return random(0, max);
}
inline void delay(unsigned long ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

struct ISerial {
	void begin(short baud) {}
	void println(const char* str) { std::cout << str << std::endl; }
	void println(char* str) { std::cout << str << std::endl; }
	void println(int16_t num) { std::cout << num << std::endl; }
};

extern ISerial Serial;

#ifndef NOMINMAX
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

#endif