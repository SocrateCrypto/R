/*
 * Uart_driver.c
 *
 *  Created on: Aug 7, 2023
 *      Author: Alexey
 */

#include "UartDriver.h"

#include "Util.h"
#define UART_QUANTITY	3
static volatile UartDriver *uart_driver_p[UART_QUANTITY] = { NULL, NULL, NULL };

UartDriver::UartDriver(UART_HandleTypeDef *select_huart, uint32_t baudrate,
		uint16_t buffer_size)
{
	this->select_huart = select_huart;
	this->baudrate = baudrate;
	this->buffer_size = buffer_size;
}

uint8_t UartDriver::check_baudrate(uint32_t baudrate)
{
	if (baudrate < 9600 || baudrate > 2000000)
	{
		return 0x00;
	}

	return 0x01;
}

void UartDriver::init()
{
	errorcnt = 0;
	if (isInit)
	{
		return;
	}

	if (buffer == NULL)
		buffer = (uint8_t*) malloc(buffer_size);
	TX_Semaphore = osSemaphoreNew(1, 0, NULL);
	RX_Semaphore = osSemaphoreNew(1, 0, NULL);

	osSemaphoreAcquire(TX_Semaphore, 0);
	osSemaphoreAcquire(RX_Semaphore, 0);

	for (uint8_t i = 0; i < UART_QUANTITY; i++)
	{
		if (uart_driver_p[i] == NULL)
		{
			uart_driver_p[i] = this;
			isInit = 0x01;
			change_baudrate(baudrate);
			HAL_UART_DMAStop(select_huart);
			HAL_UART_DeInit(select_huart);
			if (HAL_UART_Init(select_huart) != HAL_OK)
			{
				Error_Handler();
			}
			return;
		}
	}
	isInit = 0x00;
}

/*-----------------------------------------------------------------------------------------*/
uint8_t UartDriver::send(uint8_t *msg, uint16_t len, uint16_t timeout)
{
	if (!isInit)
	{
		return 0x00;
	}

	osSemaphoreAcquire(TX_Semaphore, 0); ///> Reset semaphore
	HAL_UART_Transmit_DMA(select_huart, msg, len);
	if (osSemaphoreAcquire(TX_Semaphore, timeout) == osOK)
	{
		if (isError == 0)
		{
			return 0x01;
		} else
		{
			restart_uart();
			errorcnt++;
			isError = 0;
			return 0x00;
		}
	}

	return 0x00;

}
/*-----------------------------------------------------------------------------------------*/
void UartDriver::fast_send_one_byte(uint8_t chr)
{
	while (!(select_huart->Instance->SR & USART_SR_TC))
		;
	select_huart->Instance->DR = chr;
}
/*-----------------------------------------------------------------------------------------*/
void UartDriver::fast_send_bytes(uint8_t *msg, uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		fast_send_one_byte(msg[i]);
		__wait(40);
	}
}

/*-----------------------------------------------------------------------------------------*/
void UartDriver::receive(uint16_t len, uint8_t iddle_mode)
{
	if (!isInit)
	{
		return;
	}
	osSemaphoreAcquire(RX_Semaphore, 0); ///> Reset semaphore
	HAL_UART_AbortReceive_IT(select_huart);
	if (!iddle_mode)
	{
		HAL_UART_Receive_DMA(select_huart, (uint8_t*) buffer, len);
	} else
	{
		HAL_UARTEx_ReceiveToIdle_DMA(select_huart, (uint8_t*) buffer, len);
	}
	select_huart->hdmarx->Instance->CCR &= ~DMA_IT_HT; // disable half receive interrupt!
}
/*-----------------------------------------------------------------------------------------*/
uint8_t UartDriver::wait_receive(uint8_t *receive_buffer, uint16_t *len,
		uint32_t timeout)
{
	if (!isInit)
	{
		return 0x00;
	}

	if (osSemaphoreAcquire(RX_Semaphore, timeout) == osOK)
	{

		memcpy(receive_buffer, (uint8_t*) buffer, receive_len);
		if (len != NULL)
		{
			*len = receive_len;
		}

		if (isError == 0)
		{
			return 0x01;
		} else
		{
			restart_uart();
			errorcnt++;
			isError = 0;
			return 0x00;
		}
	}

	return 0x00;
}
/*-----------------------------------------------------------------------------------------*/
void UartDriver::restart_uart()
{
	if (!isInit)
	{
		return;
	}
	if (select_huart->hdmarx->Lock == HAL_UNLOCKED)
	{
		HAL_UART_DMAStop(select_huart);
		osDelay(1);
		HAL_UART_DeInit(select_huart);
		osDelay(1);
		HAL_UART_Init(select_huart);
		osDelay(1);
	}
}
/*-----------------------------------------------------------------------------------------*/
void UartDriver::change_baudrate(uint32_t new_baudrate)
{
	if (!isInit)
	{
		return;
	}
	if (!check_baudrate(new_baudrate))
	{
		return;
	}

	select_huart->Init.BaudRate = new_baudrate;
	baudrate = new_baudrate;

}
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------STATIC FUNCTIONS(CALLBACK)----------------------------*/
/*-----------------------------------------------------------------------------------------*/
static uint8_t UartDriverPtr(UART_HandleTypeDef *huart)
{
	for (uint8_t i = 0; i < UART_QUANTITY; i++)
	{
		if (uart_driver_p[i]->select_huart == huart)
		{
			return i;
		}
	}
	return 0xff;
}
/*-----------------------------------------------------------------------------------------*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	uint8_t index = UartDriverPtr(huart);
	uart_driver_p[index]->receive_len = Size;
	osSemaphoreRelease(uart_driver_p[index]->RX_Semaphore);
}
/*-----------------------------------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t index = UartDriverPtr(huart);
	uart_driver_p[index]->receive_len = huart->RxXferSize;
	osSemaphoreRelease(uart_driver_p[index]->RX_Semaphore);
}
/*-----------------------------------------------------------------------------------------*/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t index = UartDriverPtr(huart);
	osSemaphoreRelease(uart_driver_p[index]->TX_Semaphore);
}
/*-----------------------------------------------------------------------------------------*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	uint8_t index = UartDriverPtr(huart);
	uart_driver_p[index]->isError = 1;
	osSemaphoreRelease(uart_driver_p[index]->RX_Semaphore);
	osSemaphoreRelease(uart_driver_p[index]->TX_Semaphore);

}
/*-----------------------------------STATIC FUNCTIONS(CALLBACK)----------------------------*/
/*-----------------------------------------------------------------------------------------*/
