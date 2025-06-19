#include "main.h"
#include "cmsis_os.h"
#include "nrf_task.h"
#include  "state_machine_global.hpp"
#include <cstdio>
void NrfTaskFnc(void *argument)
{
  for(;;)
  {
    stateMachine.setState(State::Manual); // Устанавливаем состояние State3
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
   // osDelay(2000);
     printf("Im NrfTask! 3\r\n");
    osDelay(2500); // задержка 1 секунда (1000 мс)
  }
}