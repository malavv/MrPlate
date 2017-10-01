#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Time.h>

#include <Wire.h>

#include "board.h"
#include "drivers/button.h"
#include "drivers/pwm.h"
#include "drivers/digitalOut.h"

#include "states/mainMenuState.h"
#include "states/simpleState.h"
#include "states/welcomeState.h"
#include "states/advancedState.h"

// Display Driver
Adafruit_SSD1306 Display(Pin::kDisplayRst);
// Control Button
ButtonDriver btn1(Pin::kBtn1);
ButtonDriver btn2(Pin::kBtn2);

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
  btn1.registerInterrupt([]{ btn1.onChange(bus); }, CHANGE);
  btn2.registerInterrupt([]{ btn2.onChange(bus); }, CHANGE);

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

  std::shared_ptr<const Event> evt = bus.next();

  if (evt != nil) {
    // Chance to Process Event of Global Importance Event
    if (evt->type == EventType::kStateChange)
      changeStateByID(std::static_pointer_cast<const StateChangeEvent>(evt)->state); 
  }

  // Then Loop the current state with any given event.
  currentState->loop(evt);
}
