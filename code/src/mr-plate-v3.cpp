#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Time.h>

#include <Wire.h>

#include "board.h"
#include "button/driver.h"
#include "drivers/pwm.h"
#include "drivers/digitalOut.h"

#include "states/mainMenuState.h"
#include "states/simpleState.h"
#include "states/welcomeState.h"
#include "states/advancedState.h"

// Display Driver
Adafruit_SSD1306 Display(Pin::kDisplayRst);
// Control Button
Button btn1(Pin::kBtn1);
Button btn2(Pin::kBtn2);
ButtonDriver buttons(2 /** Number of Buttons */);

// Lights
DigitalOut debugLed(Pin::kDebugLed);

// PWM Management
PwmDriver pwm(Pin::kPwm);
// App. main Event Bus
EventBus bus;
// Current State
State* currentState;
// Existing State
State* states[StateType::NUMBER_OF_STATE_TYPES] = {
  nil /* EventBus is never called pre-welcome. */,
  new WelcomeState(bus),
  new MainMenuState(bus),
  new SimpleState(bus),
  new AdvancedState(bus)
};

void initSerialCommunication(uint8_t timeToWaitIn100Ms) {
  Serial.begin(9600 /* Serial Speed in Baud */);
  /* Wait for serial port to connect. Needed for native USB */
  for (uint8_t i = 0; !Serial && i < timeToWaitIn100Ms; i++) delay(100/* ms */);
}

void changeStateByID(const StateType state) {
  if (currentState != nil)  
    currentState->onLeaving();
  currentState = states[state];
  if (currentState != nil)  
    currentState->onEntering();
}

void setup() {
  debugLed.setup();

  // Sets uC Pins
  pinMode(Pin::kPwm, OUTPUT);

  // Running Light is First Step to Help Debugging
  debugLed = kIsDebug;

  // Register Buttons on the Event Bus.
  btn1 = buttons.bind(Pin::kBtn1);
  btn2 = buttons.bind(Pin::kBtn2);

  btn1.onPressed([&]() { bus.post(new ButtonEvent(Pin::kBtn1, PRESSED)); });
  btn1.onReleased([&]() { bus.post(new ButtonEvent(Pin::kBtn1, RELEASED)); });

  // Initialize Display
  Display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, true /* reset */);
  Display.clearDisplay();
  Display.display();

  initSerialCommunication(30 /* timeToWaitIn100Ms */);

  Serial.println("[INFO] Moving to Welcome State");
  changeStateByID(StateType::Welcome);
}

void loop() {
  delay(loopDelayMs);
  buttons.update();

  std::shared_ptr<const Event> evt = bus.next();

  if (evt != nil && evt->type == EventType::kStateChange)
    changeStateByID(std::static_pointer_cast<const StateChangeEvent>(evt)->state); 

  currentState->loop(evt);
}
