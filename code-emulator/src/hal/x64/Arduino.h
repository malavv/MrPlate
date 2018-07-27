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
#include <vector>
#include <algorithm>

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
	CHANGE,
	RISING,
	FALLING
};

typedef bool boolean;

static const char* digitalWriteName = "digitalWrite";
static const char* pinModeName = "pinMode";
static const char* analogWriteResolutionName = "analogWriteResolution";
static const char* analogWriteFrequencyName = "analogWriteFrequency";
static const char* analogWriteName = "analogWrite";

// Actual Implementations
inline int32_t random(int32_t min, int32_t max) { return rand() % (max - min) + min; }
inline int32_t random(int32_t max) { return random(0, max); }
inline void delay(uint32_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

// Define Implementations
#define constrain(v, l, h) ((v) < (l) ? (l) : (v) > (h) ? (h) : (v))
#ifndef NOMINMAX
#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

struct DebugMsg {
    int8_t pin;
    const char* msg;
    int32_t val;
    DebugMsg(int8_t pin, const char* msg, int32_t value) : pin(pin), msg(msg), val(value) {}
};

void debug(int8_t pin, const char* msg, int32_t value);

inline void digitalWrite(int8_t pin, int8_t val) { debug(pin, digitalWriteName, val); }
inline void pinMode(int8_t pin, int8_t val) { debug(pin, pinModeName, val); }
inline void attachInterrupt(int8_t pin, void (*callback)(), int8_t val) {}
inline void analogWriteResolution(int8_t resolution) { debug(-1, analogWriteResolutionName, resolution); }
inline void analogWriteFrequency(int8_t pin, float resolution) { debug(pin, analogWriteFrequencyName, (int)resolution); }
inline void analogWrite(int8_t pin, int32_t val) { debug(pin, analogWriteName, val); }

struct ISerial {
	void begin(short baud) {}
	void println(const char* str) { std::cout << str << std::endl; }
	void println(char* str) { std::cout << str << std::endl; }
	void println(int16_t num) { std::cout << num << std::endl; }
};

extern ISerial Serial;
extern std::vector<DebugMsg*> debugMsg;

#endif