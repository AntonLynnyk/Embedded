#ifndef SPI_H
#define SPI_H

#include <stm32f10x.h>

#define SPI           SPI1
#define SPI_CLK       RCC_APB2Periph_SPI1
#define SPI_GPIO      GPIOB
#define SPI_GPIO_CLK  RCC_APB2Periph_GPIOB
#define SPI_PIN_CSN   GPIO_Pin_6
#define SPI_PIN_SCK   GPIO_Pin_3
#define SPI_PIN_MISO  GPIO_Pin_4
#define SPI_PIN_MOSI  GPIO_Pin_5

void SPI_Initialize(void);
void SPI_RCC_Configuration(void);
void SPI_GPIO_Configuration(uint16_t SPI_Mode);
uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
void TM_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count);
void TM_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count);
void TM_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count);
#endif
