#ifndef SPI_SD_H
#define SPI_SD_H

#include <stm32f10x.h>

#define SD_SPI           SPI2
#define SD_SPI_CLK       RCC_APB1Periph_SPI2
#define SD_SPI_GPIO      GPIOB
#define SD_SPI_GPIO_CLK  RCC_APB2Periph_GPIOB

#define SD_SPI_PIN_CSN   GPIO_Pin_12
#define SD_SPI_PIN_SCK   GPIO_Pin_13
#define SD_SPI_PIN_MISO  GPIO_Pin_14
#define SD_SPI_PIN_MOSI  GPIO_Pin_15

#define SD_SPI_CSN_LOW  GPIO_ResetBits(SD_SPI_GPIO, SD_SPI_PIN_CSN)
#define SD_SPI_CSN_HIGH GPIO_SetBits(SD_SPI_GPIO, SD_SPI_PIN_CSN)

void SD_SPI_Initialize(uint16_t baudRatePrescalerSPI);

#endif
