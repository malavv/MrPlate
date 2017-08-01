/**
 * Stir Plate Controller.
 *
 * This does not need to be under mbed and rtos. Why is it so? Because it can.
 */
// Standard Libraries
#include <stdio.h>
#include <string.h>
#include <string>
// Mbed Libraries
#include "mbed.h"
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
Queue<uint32_t, 5> queue;
DigitalOut debugLed(LED1);
USBSerial com;

// Components
I2C i2c(D18, D19);
Adafruit_SSD1306_ display(i2c, D20);

std::string logs; // Accumulated Logs buffer
Thread com_thread; // Serial Communication Thread

// Pre Declaration of Functions
void com_menu();
void com_logs();
void com_clear();

// Entry point
int main (void) {
  logs.append("[INFO] MrPlate is initializing\n");
  com_thread.start(com_menu);
  debugLed = kIsDebug;

  i2c.frequency(400000 /* Hz */);
  i2c.start();
  display.init();

  // Initial Text
  logs.append("[INFO] Printing Welcome Text\n");

  MonoGfx imggfx(kWidth, kHeight);
  imggfx.setTextWrap(true);
  imggfx.setTextColor(kWhite);
  imggfx.setTextCursor(0, 0);
  imggfx.printf(kWelcomeMsg);
  display.draw(0, 0, imggfx.image());

  Thread::wait(2000);
  display.clear();

  // Print Menu
  logs.append("[INFO] Printing Menu\n");
  MonoGfx menu(kWidth, kHeight);
  menu.setTextColor(kWhite);
  menu.setTextCursor(10, 0);
  menu.printf("-- Choose mode --\n");
  menu.setTextCursor(0, 32 / 2);
  menu.printf("Simple\n");
  menu.setTextCursor(kWidth / 2, kHeight / 2);
  menu.printf("Advanced\n");
  display.draw(0, 0, menu.image());

  // Print Choice
  logs.append("[INFO] Printing Default Simple Choice\n");
  MonoGfx whiteLine(kWidth, kHeight);
  whiteLine.drawFastHLine(0, kHeight - 2, (kWidth / 2) - 10, kWhite);
  display.draw(0, kHeight - 2, whiteLine.image());
  display.draw(0, kHeight - 2, whiteLine.image()); // Find why I need two draw. (same as with default library)

  while (true) { /* nop */ }
}

// Prints the accumulated logs
void com_logs() {
  com.printf(logs.c_str());
}
// Clears the Screen
void com_clear() {
  logs.append("[INFO] Clearing Screen\n");
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
