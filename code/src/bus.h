#ifndef SRC_EVENTBUS_H
#define SRC_EVENTBUS_H

#include <Arduino.h>
#include <list>
#include <memory>

#include "board.h"

class Event {
public:
  Event(EventType type);
  ~Event();

  const EventType type;
};

class EventBus {
public:
  EventBus();
  ~EventBus();
  void post(const Event* evt);

  std::shared_ptr<const Event> next();

private:
  std::list<std::shared_ptr<const Event>> queue;
};


#endif //SRC_EVENTBUS_H
