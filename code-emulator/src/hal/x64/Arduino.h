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

enum PIN_MODE {
	INPUT = 0,
	OUTPUT,
	INPUT_PULLUP
};
enum PIN_LEVEL {
	LOW = 0,
	HIGH
};
enum INTERRUPT_MODE {
	LOW,
	CHANGE,
	RISING,
	FALLING
};

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
inline void digitalWrite(int8_t pin, int8_t val) {}
inline void pinMode(int8_t pin, int8_t val) {}
inline void attachInterrupt(int8_t pin, void (*callback)(void), int8_t val);


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