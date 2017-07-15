#include "mainMenuState.h"

#include <Adafruit_SSD1306.h>


extern Adafruit_SSD1306 Display;

MainMenuState::MainMenuState(EventBus& bus)
    : State(StateType::MainMenu, bus), choice(Choice::Simple) {}
MainMenuState::~MainMenuState() {}

void MainMenuState::onEntering() {
  choice = Choice::Simple;
  printMainMenu();
}

void MainMenuState::loop(const std::shared_ptr<const Event>& evt) {
  if (evt != nil && evt->type == EventType::kBtn)
    handleButtonEvt(std::static_pointer_cast<const ButtonEvent>(evt));

  printChoiceIndicator(choice);
}

void MainMenuState::onLeaving() {
  Display.clearDisplay();
}

void MainMenuState::printChoiceIndicator(Choice choice) {
  // Assuming Simple on the left
  Display.drawFastHLine(0, SSD1306_LCDHEIGHT - 2, (SSD1306_LCDWIDTH / 2) - 10,
                        choice == Choice::Simple ? WHITE : BLACK);
  Display.drawFastHLine(SSD1306_LCDWIDTH / 2, SSD1306_LCDHEIGHT - 2, (SSD1306_LCDWIDTH / 2) - 10,
                        choice == Choice::Advanced ? WHITE : BLACK);
  Display.display();
}

void MainMenuState::handleButtonEvt(const std::shared_ptr<const ButtonEvent>& evt) {
  if (evt->action == ButtonAction::RELEASED)
    return;

  switch (evt->pin) {
    case Pin::kBtn1:
      choice = (choice == Choice::Simple) ? Choice::Advanced : Choice::Simple;
      break;
    case Pin::kBtn2:
      bus.post(new StateChangeEvent(choice == Choice::Simple ? StateType::Simple : StateType::Advanced));
      break;
  }
}

void MainMenuState::printMainMenu() {
  Display.clearDisplay();
  Display.setTextColor(WHITE);
  Display.setTextSize(1);
  Display.setCursor(10, 0);
  Display.println("-- Choose mode --");

  Display.setTextSize(1);
  Display.setCursor(0, SSD1306_LCDHEIGHT / 2);
  Display.println("Simple");
  Display.setCursor(SSD1306_LCDWIDTH / 2, SSD1306_LCDHEIGHT / 2);
  Display.println("Advanced");

  Display.display();
}
