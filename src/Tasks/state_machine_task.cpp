#include "main.h"
#include "cmsis_os.h"
#include "state_machine_task.h"
#include <stdio.h>
#include <state_machine.hpp>

StateMachine stateMachine; // Экземпляр должен быть extern, если он создаётся в main

const char *stateToStr(State state)
{
  switch (state)
  {
  case State::Initial:
    return "Initial";
  case State::Manual:
    return "Manual";
  case State::GiroScope:
    return "GiroScope";
  case State::Scan:
    return "Scan";
  default:
    return "Unknown";
  }
}

void StateMachineTask(void *argument)
{
  for (;;)
  {
    State current = stateMachine.getState();
    printf("State machine task: current state = %s\r\n", stateToStr(current));
    osDelay(3000);
  }
}