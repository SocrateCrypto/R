#include "main.h"
#include "cmsis_os.h"
#include "default_task.h"
#include <cstdio>
#include  "state_machine_global.hpp"

void StartDefaultTask(void *argument)
{
  for(;;)
  {
    stateMachine.setState(State::Scan); // Устанавливаем состояние State1
    printf("Im default_task! 1\r\n");
    osDelay(2700); // задержка 3 секунды (3000 мс)
  }
}