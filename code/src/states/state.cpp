#include "state.h"

State::State(StateType type, EventBus& bus) : type(type), bus(bus) {}

State::~State() {}

StateChangeEvent::StateChangeEvent(StateType state)
    : Event(EventType::kStateChange), state(state) {
}
