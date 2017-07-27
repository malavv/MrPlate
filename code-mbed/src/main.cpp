#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "Adafruit_SSD1306.h"

#include <string.h>
#include <string>

// Global Objects
Queue<uint32_t, 5> queue;
DigitalOut debugLed(LED1);
USBSerial com;
const bool kIsDebug = true;
const char* kWelcomeMsg = "MrPlate welcomes you to a yeast extravaganza v1.0\n";
std::string logs;
struct I2c : I2C { I2c(PinName sda, PinName scl) : I2C(sda, scl)  { frequency(400000); start(); }; } i2c(D18, D19);
Adafruit_SSD1306_I2c display(i2c, D20);
Thread thread_cmd;

// Pre Declaration of Functions
void com_menu();

// Entry point
int main (void) {
  logs.append("[INFO] MrPlate is initializing\n");
  thread_cmd.start(com_menu);
  debugLed = kIsDebug;

  // Initial Text
  logs.append("[INFO] Printing Welcome Text\n");
  display.setTextWrap(true);
  display.setTextColor(WHITE);
  display.setTextCursor(0, 0);
  display.clearDisplay();
  display.printf(kWelcomeMsg);
  display.display();
  display.display();

  Thread::wait(2000);

  // Print Menu
  logs.append("[INFO] Printing Menu\n");
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextCursor(10, 0);
  display.printf("-- Choose mode --\n");
  display.setTextCursor(0, 32 / 2);
  display.printf("Simple\n");
  display.setTextCursor(128 / 2, 32 / 2);
  display.printf("Advanced\n");
  display.display();
  display.display();

  // Print Choice
  logs.append("[INFO] Printing Default Simple Choice\n");
  const char choice = 'L';
  display.drawFastHLine(0, 32 - 2, (128 / 2) - 10,
                        choice == 'L'? WHITE : BLACK);
  display.drawFastHLine(128 / 2, 32 - 2, (128 / 2) - 10,
                        choice == 'R' ? WHITE : BLACK);
  display.display();
  display.display();

  while (true) { /* nop */ }
}

void com_menu() {
  char str[80];
  while(true) {
    com.scanf("%79s", str);
    if (strcmp(str, "logs") == 0)
      com.printf(logs.c_str());
    else
      com.printf("Unrecognized command %s\n", str);
  }
}
