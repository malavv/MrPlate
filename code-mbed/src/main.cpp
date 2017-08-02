/**
 * Stir Plate Controller.
 *
 * This does not need to be under mbed and rtos. Why is it so? Because it can.
 */
// Standard Libraries
#include <cstdio>

// Mbed Libraries
#include "mbed.h"
#include "mbed_events.h"
#include "rtos.h"
#include "USBSerial.h"

// Private Libraries
#include "ssd1306/ssd1306.h"
#include "imageMono/image.h"
#include "gfx/adafruit.h"

// Constants
const bool kIsDebug = true;
const uint8_t kWidth = 128;
const uint8_t kHeight = 32;
const uint8_t kComBufferLength = 80;
const char* kWelcomeMsg = "MrPlate welcomes you to a yeast extravaganza v1.0\n";

// Global Objects
DigitalOut debugLed(LED1);
USBSerial com;

// Components
I2C i2c(D18, D19);
Adafruit_SSD1306_ display(i2c, D20);
InterruptIn btn1(D14);
DigitalIn btn1_in(D14);

char logBuffer[1000];
uint16_t logCursor = 0;
Thread com_thread; // Serial Communication Thread
Thread eventThread;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
volatile bool isSimpleState = true;

const uint8_t kUnknownState = 0;
const uint8_t kMenuState = 1;
const uint8_t kPowerModulationState = 2;
const uint8_t kAdvancedModeState = 3;
const uint8_t kPowerModulation = 0;
const uint8_t kAdvanced = 1;
const uint8_t kChangeModeEvent = 0;
const uint8_t kEnterModeEvent = 1;
uint8_t state = kUnknownState;
uint8_t chosenMode = kPowerModulation;

struct MenuMode {
  uint8_t mode;
  void handleBtn1Pressed();
};

MenuMode menuMode = { .mode = kPowerModulation };

// Pre Declaration of Functions
void com_menu();
void com_logs();
void com_clear();

void print_welcome();
void print_menu();

void log(const char *format, ...);

void MenuMode::handleBtn1Pressed() {
  mode = mode == kPowerModulation ? kAdvanced : kPowerModulation;

  MonoGfx selection(kWidth, kHeight);
  selection.fillRect(0, kHeight - 2, kWidth, 1, kBlack);
  selection.drawFastHLine(mode == kPowerModulation ? 0 : kWidth / 2, kHeight - 2, (kWidth / 2) - 10, kWhite);
  display.draw(0, 0, selection.image());
  display.draw(0, 0, selection.image());
  log("[INFO] Menu is changing selection\n");
}

void btn_pressed_1() {
  isSimpleState != isSimpleState;
  queue.call(&menuMode, &MenuMode::handleBtn1Pressed);
}

void exited(uint8_t state);
void entering(uint8_t state);
void moveTo(uint8_t newState);

// Entry point
int main (void) {
  log("[INFO] MrPlate is initializing\n");
  i2c.frequency(400000 /* Hz */);
  i2c.start();
  display.init();
  btn1_in.mode(PullNone);
  btn1.fall(queue.event(btn_pressed_1));
  // Start Threads
  com_thread.start(com_menu);
  eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

  // Initial Text
  log("[INFO] Printing Welcome Text\n");
  print_welcome();
  Thread::wait(2000);

  queue.call(moveTo, kMenuState);

  while (true) {
    /*
    if (state == kMenuState) {
      if (event == kChangeModeEvent)
        chosenMode = kPowerModulation ? kAdvanced : kPowerModulation;
      else if (event == kEnterModeEvent)
        moveTo(chosenMode == kPowerModulation ? kPowerModulationState : kAdvancedModeState);
    }
    */

    Thread::wait(500);
  }
}

void exited(uint8_t state) {
  log("[INFO] Exiting State : %u\n", state);
}
void entering(uint8_t state) {
  log("[INFO] Entering State : %u\n", state);
  switch(state) {
    case kMenuState: print_menu(); break;
  }
}
void moveTo(uint8_t newState) {
  if (state == newState)
    return;
  exited(state);
  entering(newState);
  state = newState;
}

void log(const char *format, ...) {
  va_list arg;
  va_start (arg, format);
  logCursor += std::sprintf(&logBuffer[logCursor], format, arg);
  va_end (arg);
}

void print_welcome() {
  MonoGfx imggfx(kWidth, kHeight);
  imggfx.setTextWrap(true);
  imggfx.setTextColor(kWhite);
  imggfx.setTextCursor(0, 0);
  imggfx.printf(kWelcomeMsg);
  display.draw(0, 0, imggfx.image());
}
void print_menu() {
  MonoGfx menu(kWidth, kHeight);
  menu.fillRect(0, 0, kWidth, kHeight, kBlack);
  menu.setTextColor(kWhite);
  menu.setTextCursor(10, 0);
  menu.printf("-- Choose mode --\n");
  menu.setTextCursor(0, 32 / 2);
  menu.printf("Simple\n");
  menu.setTextCursor(kWidth / 2, kHeight / 2);
  menu.printf("Advanced\n");

  menu.drawFastHLine(0, kHeight - 2, (kWidth / 2) - 10, kWhite);
  display.draw(0, 0, menu.image());
  display.draw(0, 0, menu.image());
}

// Prints the accumulated logs
void com_logs() {
  com.printf(logBuffer);
}
// Clears the Screen
void com_clear() {
  logCursor += std::sprintf(&logBuffer[logCursor], "[INFO] Clearing Screen\n");
  com.printf("[INFO] Clearing Screen\n");
  display.clear();
}
/** Serial Communication Menu */
void com_menu() {
  char buffer[kComBufferLength];
  while(com.scanf("%79s", buffer) == 1) {
    if (strcmp(buffer, "logs") == 0) com_logs();
    else if (strcmp(buffer, "clear") == 0) com_clear();
    else com.printf("Unrecognized command : %s\n", buffer);
  }
}
