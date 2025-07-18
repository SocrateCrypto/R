#include <stdio.h>
#include "NRF24.h"
uint8_t dt_reg = 0;
uint8_t buf1[20] = {0};
char str1[64] = {0};

extern UART_HandleTypeDef huart1;
//------------------------------------------------

extern SPI_HandleTypeDef hspi2;

//------------------------------------------------

//------------------------------------------------

#define TX_ADR_WIDTH 3

#define TX_PLOAD_WIDTH 2

uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb3, 0xb4, 0x01};

uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};

//------------------------------------------------

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)

{

  micros *= (SystemCoreClock / 1000000) / 9;

  /* Wait till done */

  while (micros--)
    ;
}

//--------------------------------------------------
//--------------------------------------------------
//--------------------------------------------------

uint8_t NRF24_ReadReg(uint8_t addr)

{

  uint8_t dt = 0, cmd;

  CS_ON;

  HAL_SPI_TransmitReceive(&hspi2, &addr, &dt, 1, 1000);

  if (addr != STATUS) // если адрес равен адрес регистра статус то и возварщаем его состояние

  {

    cmd = 0xFF;

    HAL_SPI_TransmitReceive(&hspi2, &cmd, &dt, 1, 1000);
  }

  CS_OFF;

  return dt;
}

//------------------------------------------------

void NRF24_WriteReg(uint8_t addr, uint8_t dt)

{

  addr |= W_REGISTER; // включим бит записи в адрес

  CS_ON;

  HAL_SPI_Transmit(&hspi2, &addr, 1, 1000); // отправим адрес в шину

  HAL_SPI_Transmit(&hspi2, &dt, 1, 1000); // отправим данные в шину

  CS_OFF;
}

//------------------------------------------------
//------------------------------------------------

void NRF24_ToggleFeatures(void)

{

  uint8_t dt[1] = {ACTIVATE};

  CS_ON;

  HAL_SPI_Transmit(&hspi2, dt, 1, 1000);

  DelayMicro(1);

  dt[0] = 0x73;

  HAL_SPI_Transmit(&hspi2, dt, 1, 1000);

  CS_OFF;
}

//-----------------------------------------------
//-----------------------------------------------

//-----------------------------------------------

void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)

{

  CS_ON;

  HAL_SPI_Transmit(&hspi2,&addr,1,1000);//отправим адрес в шину

  HAL_SPI_Receive(&hspi2,pBuf,bytes,1000);//отправим данные в буфер

  CS_OFF;

}

void NRF24_DebugInfo(void)
{
  printf("Im NRF24_DebugInfo\r\n");
  dt_reg = NRF24_ReadReg(CONFIG);
  snprintf(str1, sizeof(str1), "CONFIG: 0x%02X\r\n", dt_reg);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  dt_reg = NRF24_ReadReg(EN_AA);
  snprintf(str1, sizeof(str1), "EN_AA: 0x%02X\r\n", dt_reg);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  dt_reg = NRF24_ReadReg(EN_RXADDR);
  snprintf(str1, sizeof(str1), "EN_RXADDR: 0x%02X\r\n", dt_reg);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  dt_reg = NRF24_ReadReg(STATUS);
  snprintf(str1, sizeof(str1), "STATUS: 0x%02X\r\n", dt_reg);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  dt_reg = NRF24_ReadReg(RF_SETUP);
  snprintf(str1, sizeof(str1), "RF_SETUP: 0x%02X\r\n", dt_reg);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  NRF24_Read_Buf(TX_ADDR, buf1, 3);
  snprintf(str1, sizeof(str1), "TX_ADDR: 0x%02X, 0x%02X, 0x%02X\r\n", buf1[0], buf1[1], buf1[2]);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);

  NRF24_Read_Buf(RX_ADDR_P1, buf1, 3);
  snprintf(str1, sizeof(str1), "RX_ADDR: 0x%02X, 0x%02X, 0x%02X\r\n", buf1[0], buf1[1], buf1[2]);
  HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);
}

//------------------------------------------------

void NRF24_Write_Buf(uint8_t addr, uint8_t *pBuf, uint8_t bytes)

{

  addr |= W_REGISTER; // включим бит записи в адрес

  CS_ON;

  HAL_SPI_Transmit(&hspi2, &addr, 1, 1000); // отправим адрес в шину

  DelayMicro(1);

  HAL_SPI_Transmit(&hspi2, pBuf, bytes, 1000); // отправим данные в буфер

  CS_OFF;
}

//------------------------------------------------
//------------------------------------------------

void NRF24_FlushRX(void)

{

  uint8_t dt[1] = {FLUSH_RX};

  CS_ON;

  HAL_SPI_Transmit(&hspi2, dt, 1, 1000);

  DelayMicro(1);

  CS_OFF;
}

//------------------------------------------------

void NRF24_FlushTX(void)

{

  uint8_t dt[1] = {FLUSH_TX};

  CS_ON;

  HAL_SPI_Transmit(&hspi2, dt, 1, 1000);

  DelayMicro(1);

  CS_OFF;
}

//------------------------------------------------
//------------------------------------------------

void NRF24L01_RX_Mode(void)

{

   uint8_t regval=0x00;

  regval = NRF24_ReadReg(CONFIG);

  //разбудим модуль и переведём его в режим приёмника, включив биты PWR_UP и PRIM_RX

  regval |= (1<<PWR_UP)|(1<<PRIM_RX);

  NRF24_WriteReg(CONFIG,regval);

  CE_SET;

  DelayMicro(150); //Задержка минимум 130 мкс

  // Flush buffers

  NRF24_FlushRX();

  NRF24_FlushTX();
}

//------------------------------------------------

void NRF24_ini(void)

{
  CE_RESET; // Сбросим модуль NRF24L01 chip enable pin =false

  DelayMicro(5000);
  NRF24_WriteReg(CONFIG, 0x0a);     // Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
  DelayMicro(5000);                 // Дадим модулю включиться, по даташиту около 1,5 мсек, а лучше 5
  NRF24_WriteReg(EN_AA, 0x02);      // Enable Pipe1
  NRF24_WriteReg(EN_RXADDR, 0x02);  // Enable Pipe1
  NRF24_WriteReg(SETUP_AW, 0x01);   // Setup address width=3 bytes
  NRF24_WriteReg(SETUP_RETR, 0x5F); // // 1500us, 15 retrans
  NRF24_ToggleFeatures();
  NRF24_WriteReg(FEATURE, 0);
  NRF24_WriteReg(DYNPD, 0);
  NRF24_WriteReg(STATUS, 0x70);   // Reset flags for IRQ
  NRF24_WriteReg(RF_CH, 76);      // частота 2476 MHz
  NRF24_WriteReg(RF_SETUP, 0x06); // TX_PWR:0dBm, Datarate:1Mbps
  NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
  NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH);
  NRF24_WriteReg(RX_PW_P1, TX_PLOAD_WIDTH); // Number of bytes in RX payload in data pipe 1
  // пока уходим в режим приёмника

  NRF24L01_RX_Mode();

  // LED_OFF;
}

//--------------------------------------------------