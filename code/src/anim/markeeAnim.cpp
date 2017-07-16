#include "markeeAnim.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 Display;

MarkeeAnimation::MarkeeAnimation(const char* msg, uint32_t duration) {
  this->duration = duration;
  this->msg = msg;
  cursor = 0;
}

void MarkeeAnimation::start() {
  startTime = millis();

  Display.setTextSize(2);
  Display.setTextWrap(false);
  Display.setTextColor(WHITE);
  Display.setCursor(0, 0);

  Display.clearDisplay();
  Display.println(msg);
}

void MarkeeAnimation::step() {
  cursor += deltaPx;
  Display.setCursor(-cursor * 4, 0);
  Display.clearDisplay();
  Display.println(msg);
  Display.display();
}

bool MarkeeAnimation::hasEnded() {
  return (millis() - startTime) > duration;
}
