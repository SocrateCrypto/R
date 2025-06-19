#include "main.h"
#include "cmsis_os.h"
#include "hid_task.h"
#include "UartDriver.h"
#include <string.h>
#include  "state_machine_global.hpp"
extern UART_HandleTypeDef huart1;
// Создаём объект драйвера для UART1 (предполагается, что huart1 инициализирован)
UartDriver uart1(&huart1);

void HidTaskFnc(void *argument)
{
  uart1.init();
  for(;;)
  {
    stateMachine.setState(State::GiroScope); // Устанавливаем состояние State2
   const char* msg = "Hello from hid_task! 2\r\n";
    uart1.send((uint8_t*)msg, strlen(msg), 100); // <-- используем send
    osDelay(2000);
  }
}