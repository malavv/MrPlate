#ifndef STATE_H
#define STATE_H

#include "board.h"
#include "bus.h"

class StateChangeEvent : public Event {
public:
  StateChangeEvent(StateType state);
  const StateType state;
};

class State {
public:
  State(StateType type, EventBus& bus);
  virtual ~State();
  virtual void onEntering() = 0;
  virtual void loop(const std::shared_ptr<const Event>& evt) = 0;
  virtual void onLeaving() = 0;

  const StateType type;

protected:
  EventBus& bus;
};

#endif // STATE_H
