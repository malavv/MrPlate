#include "Arduino.h"

ISerial Serial;
std::vector<DebugMsg*> debugMsg;

void debug(int8_t pin, const char* msg, int32_t value) {
  DebugMsg *found = nullptr;

  for (auto &it : debugMsg) {
    if (it->pin == pin)
      found = it;
  }

  if (found) {
    found->msg = msg;
    found->val = value;
  } else
    debugMsg.push_back(new DebugMsg(pin, msg, value));
}