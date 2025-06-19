/**
 * @brief Uart_driver.h Driver for working with UART in DMA mode.
 *  Uses semaphores when waiting for a message to be sent/received.
 *  Can operate in standby mode for a certain number of bytes or by
 *  IDLE interrupt.
 *
 *   Created on: Aug 7, 2023
 *   Author: Alexey
 */

#ifndef UARTDRIVER_H_
#define UARTDRIVER_H_


#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cmsis_os2.h"

class UartDriver
{
public:
	uint8_t isInit = 0x00;
	volatile uint8_t *buffer; ///< Buffer pointer
	volatile uint16_t receive_len; ///< Receive len
	volatile uint32_t baudrate;
	volatile uint32_t errorcnt = 0;
	volatile uint16_t buffer_size;
	volatile osSemaphoreId_t TX_Semaphore;
	volatile osSemaphoreId_t RX_Semaphore;
	UART_HandleTypeDef *select_huart; ///< Uart struct pointer
	uint8_t isError = 0;
	/**
	 * @brief Constructor of the uart. The name of the uart is transmitted,
	 * and two callback functions that will be called when receiving
	 * and transmitting bytes
	 *
	 * @param [in]baudrate - uart baudrate
	 * @param [in]buffer_size - size rx buffer
	 */
	UartDriver(UART_HandleTypeDef *select_huart, uint32_t baudrate = 115200,
			uint16_t buffer_size = 128);

	void init();

	/**
	 * @brief Transfer function. The input is the name of the port, a pointer
	 * to the buffer that is being transferred, and the number of bytes
	 *
	 * @param [in]msg - Pointer to transmit buffer
	 * @param [in]len - Length of transmit buffer
	 * @param [in]timeout - Transmitt timeout in mS
	 * @return 0x01 if transmit is oK
	 */
	uint8_t send(uint8_t *msg, uint16_t len, uint16_t timeout);
	/**
	 * @brief Starting reception by uart. The name of the uart and the
	 * number of bytes to be received are transmitted. If the
	 * required number of bytes is received, the callback that
	 * we specified during initialization is called
	 *
	 * @param [in]len - Length of transmit buffer.
	 * @param [in]iddle_mode - is 0x01, reception will be started in IDDLE mode.
	 *
	 */
	void receive(uint16_t len, uint8_t iddle_mode);

	/**
	 * @brief
	 * @param [out]receive_buffer - Buffer for data
	 * @param [in]len - Pointer to the receive size variable.
	 * If receiving in IDDLE mode, the number of bytes received
	 *  will be written to this variable
	 * @param [in]timeout - Receive timeout in mS
	 * @return 0x01 if receive is oK
	 */
	uint8_t wait_receive(uint8_t *receive_buffer, uint16_t *len,
			uint32_t timeout);
	/**
	 * @brief Restart uart

	 */
	void restart_uart();

	/**
	 * @brief Changing the operating speed of the uart port.
	 * The port name and operating speed are transmitted
	 *
	 * @param [in]uName - Port name
	 * @param [in]new_baudrate - Baudrate 4800...2000000
	 */
	void change_baudrate(uint32_t new_baudrate);

	/**
	 * @brief Transfer not in dma mode and with direct
	 * access to registers
	 *
	 * @param chr - byte transmitted
	 */
	void fast_send_one_byte(uint8_t chr);
	/**
	 * @brief Transfer not in dma mode and with direct
	 * access to registers
	 *
	 * @param msg - bytes transmitted
	 * @param len - bytes len
	 */
	void fast_send_bytes(uint8_t *msg, uint16_t len);

private:
	uint8_t check_baudrate(uint32_t baudrate); ///< Checks whether the required speed is within the limit

};

#endif /* UARTDRIVER_H_ */
