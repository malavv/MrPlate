#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>

#define nil 0
#define kIsDebug false

/** Free Wheeling Delay for the main loop */
const unsigned long loopDelayMs = 30;

enum Pin : uint8_t {
  kPwm = 10,
  kDebugLed = 13,
  kBtn1 = 14,
  kBtn2 = 15,
  kDisplayRst = 20,
};

enum StateType : uint8_t {
    PreWelcome,
    Welcome,
    MainMenu,
    Simple,
    Advanced,
    NUMBER_OF_STATE_TYPES
};

const char * const StateTypeNames[] = {
  "PreWelcome",
  "Welcome",
  "MainMenu",
  "Simple",
  "Advanced"
};

enum EventType : uint8_t {
    kBtn,
    kStateChange
};

#endif // BOARD_H_
