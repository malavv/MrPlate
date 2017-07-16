#include "textAnim.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 Display;

TextAnimation::TextAnimation(const char* msg, uint32_t duration) {
  this->duration = duration;
  this->msg = msg;
}

void TextAnimation::start() {
  startTime = millis();

  Display.setTextSize(2);
  Display.setTextWrap(false);
  Display.setTextColor(WHITE);
  Display.setCursor(0, 0);

  Display.clearDisplay();
  Display.println(msg);
  Display.display();
}

void TextAnimation::step() {
  return;
}

bool TextAnimation::hasEnded() {
  return (millis() - startTime) > duration;
}
