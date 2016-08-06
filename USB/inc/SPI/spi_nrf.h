#ifndef SPI_NRF_H
#define SPI_NRF_H

#include <stm32f10x.h>

#define NRF_SPI           SPI1
#define NRF_SPI_CLK       RCC_APB2Periph_SPI1
#define NRF_SPI_GPIO      GPIOA
#define NRF_SPI_GPIO_CLK  RCC_APB2Periph_GPIOA
#define NRF_SPI_PIN_CSN   GPIO_Pin_4
#define NRF_SPI_PIN_SCK   GPIO_Pin_5
#define NRF_SPI_PIN_MISO  GPIO_Pin_6
#define NRF_SPI_PIN_MOSI  GPIO_Pin_7

void SPI_Initialize(void);
#endif
