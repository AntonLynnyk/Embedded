#ifndef SPI_H
#define SPI_H

#include <stm32f10x.h>

uint8_t SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
void SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count);
void SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count);
void SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count);

#endif
