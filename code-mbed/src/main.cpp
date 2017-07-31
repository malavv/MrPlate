#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "ssd1306/ssd1306.h"
#include "imageMono/image.h"
#include "gfx/adafruit.h"

#include <stdio.h>
#include <string.h>
#include <string>

#define WIDTH 128
#define HEIGHT 32

const bool kIsDebug = true;
const char* kWelcomeMsg = "MrPlate welcomes you to a yeast extravaganza v1.0\n";

// Global Objects
Queue<uint32_t, 5> queue;
DigitalOut debugLed(LED1);
USBSerial com;

std::string logs;
I2C i2c(D18, D19);
Adafruit_SSD1306_ display(i2c, D20);
Thread thread_cmd;
ImageMonoImpl img(WIDTH, HEIGHT);
MonoGfx imggfx(WIDTH, HEIGHT);

// Pre Declaration of Functions
void com_menu();

// Entry point
int main (void) {
  logs.append("[INFO] MrPlate is initializing\n");
  thread_cmd.start(com_menu);
  debugLed = kIsDebug;

  i2c.frequency(400000 /* Hz */);
  i2c.start();
  display.init();

  //imggfx.drawFastHLine(0, 0, 127, kWhite);
  //imggfx.drawFastHLine(0, 31, 127, kWhite);
  //imggfx.drawFastVLine(0, 0, 31, kWhite);
  //imggfx.drawFastVLine(127, 0, 31, kWhite);

  //display.draw(0, 0, imggfx.image());

  // Initial Text
  logs.append("[INFO] Printing Welcome Text\n");

  imggfx.setTextWrap(true);
  imggfx.setTextColor(kWhite);
  imggfx.setTextCursor(0, 0);
  //imggfx.fillRect(0, 0, 128, 32, kBlack);
  imggfx.printf(kWelcomeMsg);

  display.draw(0, 0, imggfx.image());


  Thread::wait(2000);

  // Print Menu
  logs.append("[INFO] Printing Menu\n");
  /*
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
*/
  // Print Choice
  logs.append("[INFO] Printing Default Simple Choice\n");
  //const char choice = 'L';
  /*
  display.drawFastHLine(0, 32 - 2, (128 / 2) - 10,
                        choice == 'L'? WHITE : BLACK);
  display.drawFastHLine(128 / 2, 32 - 2, (128 / 2) - 10,
                        choice == 'R' ? WHITE : BLACK);
  display.display();
  display.display();
*/
  while (true) { /* nop */ }
}

void com_menu() {
  char str[80];


  while(true) {
    com.scanf("%79s", str);
    if (strcmp(str, "logs") == 0)
      com.printf(logs.c_str());
    else if (strcmp(str, "clear") == 0) {
      logs.append("[INFO] Clearing Screen\n");
      com.printf("[INFO] Clearing Screen\n");
      display.clear();
    } else
      com.printf("Unrecognized command %s\n", str);
  }
}
