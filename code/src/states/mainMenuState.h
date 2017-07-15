#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "state.h"
#include "bus.h"
#include "../drivers/button.h"

class MainMenuState : public State {

public:
  MainMenuState(EventBus& bus);
  virtual ~MainMenuState();
  virtual void onEntering();
  virtual void loop(const std::shared_ptr<const Event>& evt);
  virtual void onLeaving();

private:
  enum Choice {
    Simple,
    Advanced
  };

  Choice choice;

  void printChoiceIndicator(Choice choice);
  void printMainMenu();
  void handleButtonEvt(const std::shared_ptr<const ButtonEvent>& evt);
};

#endif // MAIN_MENU_STATE_H
