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

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#endif