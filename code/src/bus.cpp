#include "bus.h"


Event::Event(EventType type) : type(type) {}

Event::~Event() {}

EventBus::EventBus() {}
EventBus::~EventBus() {}

void EventBus::post(const Event* e) {
  queue.push_back(std::shared_ptr<const Event>(e));
}

std::shared_ptr<const Event> EventBus::next() {
  if (queue.empty())  return nil;
  std::shared_ptr<const Event> evt(queue.front());
  queue.pop_front();
  return evt;
}
