#ifndef BTN_DRIVER_H
#define BTN_DRIVER_H

#include <stdint.h>
#include <functional>
#include <vector>
#include <map>

#include "../bus.h"

enum ButtonAction : uint8_t { PRESSED = 0, RELEASED = 1 };

struct ButtonEvent : public Event {
  public:
  ButtonEvent();
  ButtonEvent(uint8_t pin, ButtonAction action);
  const uint8_t pin;
  const ButtonAction action;
};

struct ButtonData {
  volatile bool isPressed;
  volatile bool isDirty;
  uint8_t pin;
  ButtonData(uint8_t pin): isPressed(false), isDirty(false), pin(pin) {}
};

class Button {
  public:
  Button(const uint8_t pin);
  ~Button() {};
  bool isPressed() const { return data_->isPressed; }

  private:
  ButtonData* data_;
};

// Design From https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/1008289#1008289
class ButtonDriver {
  public:
    // Need to update the button driver
    void update();
    // Get the instance of the driver
    static ButtonDriver& getInstance() {
      static ButtonDriver instance;
      return instance;
    }

    ButtonDriver(ButtonDriver const&) = delete;
    void operator=(ButtonDriver const&) = delete;

  private:
    friend class Button;
    ButtonDriver() {}

    ButtonData* getBtnById(uint8_t pin) const;
    ButtonData* bind(const uint8_t pin);
    void onAnyBtnTrigger();
    bool isDirty() const;
    ButtonData* addBtnData(const uint8_t pin);

    void broadcast(bool isPressed, uint8_t pin);

    std::vector<ButtonData*> buttonsData_;
};

#endif // BTN_DRIVER_H
