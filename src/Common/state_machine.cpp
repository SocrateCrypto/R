#include "state_machine.hpp"

StateMachine::StateMachine() : currentState(State::Initial) {}

State StateMachine::getState() const { return currentState; }
void StateMachine::setState(State newState) { currentState = newState; }
bool StateMachine::is(State state) const { return currentState == state; }