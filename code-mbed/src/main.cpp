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
Adafruit_SSD1306 display(i2c, D20);
InterruptIn btn1(D14);
DigitalIn btn1_in(D14);
InterruptIn btn2(D15);
DigitalIn btn2_in(D15);

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

const char* menuModeTxt = "MenuMode";

class Mode {
  public:
    Mode(uint8_t type, const char* name) : name(name), type(type) { }
    virtual ~Mode() { }
    virtual void handleBtn1Pressed() { }
    virtual void handleBtn2Pressed() { }
    virtual void onEnter() { }
    virtual void onExit() { }

    const char* name;
    uint8_t type;
};

class MenuMode : public Mode {
  public:
    uint8_t mode;

    MenuMode(uint8_t type, const char* name) : Mode(type, name), mode(kPowerModulation) { }
    virtual ~MenuMode() { }
    virtual void handleBtn1Pressed();
    virtual void handleBtn2Pressed();
    virtual void onEnter();
};

class PowerModulationMode : public Mode {
  public:
    PowerModulationMode(uint8_t type, const char* name) : Mode(type, name) { }
    virtual ~PowerModulationMode() { }
    virtual void onEnter();
};

Mode* cmode = 0;
MenuMode menuMode(kMenuState, menuModeTxt);
PowerModulationMode powerMode(kPowerModulationState, "PowerModulationMode\0");

// Pre Declaration of Functions
void com_menu();
void com_logs();
void com_clear();

void print_welcome();
void print_menu();

void log(const char *format, ...);

void btn_pressed_1() {
  queue.call(&menuMode, &MenuMode::handleBtn1Pressed);
}
void btn_pressed_2() {
  queue.call(&menuMode, &MenuMode::handleBtn2Pressed);
}

void exited(Mode& mode);
void entering(Mode& mode);
void moveTo(uint8_t newState);

// Entry point
int main (void) {
  log("[INFO] MrPlate is initializing\n");
  i2c.frequency(400000 /* Hz */);
  i2c.start();
  display.init();
  btn1_in.mode(PullNone);
  btn1.fall(queue.event(btn_pressed_1));
  btn2_in.mode(PullNone);
  btn2.fall(queue.event(btn_pressed_2));
  // Start Threads
  com_thread.start(com_menu);
  eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

  // Initial Text
  log("[INFO] Printing Welcome Text\n");
  print_welcome();
  Thread::wait(2000);

  queue.call(moveTo, kMenuState);

  while (true) {
    Thread::wait(500);
  }
}

void MenuMode::handleBtn1Pressed() {
  mode = mode == kPowerModulation ? kAdvanced : kPowerModulation;

  MonoGfx selection(kWidth, kHeight);
  selection.fillRect(0, kHeight - 2, kWidth, 1, kBlack);
  selection.drawFastHLine(mode == kPowerModulation ? 0 : kWidth / 2, kHeight - 2, (kWidth / 2) - 10, kWhite);
  display.draw(0, 0, selection.image());
  display.draw(0, 0, selection.image());
  log("[INFO] Menu is changing selection\n");
}
void MenuMode::handleBtn2Pressed() {
  queue.call(moveTo, mode == kPowerModulation ? kPowerModulationState : kAdvancedModeState);
}
void MenuMode::onEnter() {
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
void PowerModulationMode::onEnter() {
  time_t now = time(0);   // get time now
  struct tm* t = localtime(&now);
  MonoGfx menu(kWidth, kHeight);
  menu.fillRect(0, 0, kWidth, kHeight, kBlack);

  menu.setTextSize(1);
  menu.setTextCursor(0, 0);
  menu.printf("%d:%01d:%01d", t->tm_hour, t->tm_min, t->tm_sec);

  // Power Sign
  menu.setTextSize(1);
  menu.setTextCursor(kWidth - 40, 0);
  menu.printf("Power");

  // Power Level
  menu.setTextSize(2);
  menu.setTextCursor(5, kHeight / 2);
  uint32_t displayp = ((float)5 / (float)100) * 10000;
  uint32_t top = displayp / 100;
  menu.printf("%3d.%02d%%\n", top, displayp - (top * 100));

  display.draw(0, 0, menu.image());
  display.draw(0, 0, menu.image());
}

void exited(Mode& mode) {
  if (state == 0)  return;
  log("[INFO] Exiting State : %s\n", mode.name);
  mode.onExit();
}
void entering(Mode& mode) {
  if (state == 0)  return;
  log("[INFO] Entering State : %s\n", mode.name);
  mode.onEnter();
}
void moveTo(uint8_t newState) {
  if (state == newState)
    return;
  exited(*cmode);
  state = newState;
  switch(state) {
    case kMenuState: cmode = &menuMode; break;
    case kPowerModulationState: cmode = &powerMode; break;
  }
  entering(*cmode);
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
