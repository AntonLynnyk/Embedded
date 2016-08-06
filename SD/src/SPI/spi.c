#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stdbool.h>

#include "spi.h"

#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0)
#define SPI_WAIT(SPIx)    {while (SPI_I2S_GetFlagStatus((SPIx), SPI_I2S_FLAG_BSY) == SET); }
#define SPI_WAIT_TX(SPIx) {while (SPI_I2S_GetFlagStatus((SPIx), SPI_I2S_FLAG_TXE) == RESET) ; }
#define SPI_WAIT_RX(SPIx) {while (SPI_I2S_GetFlagStatus((SPIx), SPI_I2S_FLAG_RXNE) == RESET) ; }

#define SPI_CHECK_ENABLED(SPIx)           if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return;}
#define SPI_CHECK_ENABLED_RESP(SPIx, val) if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}

uint8_t SPI_Send(SPI_TypeDef* SPIx, uint8_t data)
{
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED_RESP(SPIx, 0);

	/* Wait for previous transmissions to complete if DMA TX enabled for SPI */
	SPI_WAIT_TX(SPIx);

	/* Fill output buffer with data */
	SPIx->DR = data;

    /* Wait finish sending */
    SPI_WAIT(SPIx);

	/* Wait for transmission to complete */
	SPI_WAIT_RX(SPIx);

	/* Return data from buffer */
	return SPIx->DR;
}

void SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count)
{
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);

	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);

		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = dummy;

        /* Wait finish sending */
        SPI_WAIT(SPIx);

		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);

		/* Save data to buffer */
		*dataIn++ = *(__IO uint8_t *)&SPIx->DR;
	}
}

void SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count)
{
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);

	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);

		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = *dataOut++;

		/* Wait finish sending */
        SPI_WAIT(SPIx);

		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);

		/* Read data register */
		(void)*(__IO uint16_t *)&SPIx->DR;
	}
}

void SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count)
{
	/* Check if SPI is enabled */
	SPI_CHECK_ENABLED(SPIx);

	while (count--) {
		/* Wait busy */
		SPI_WAIT_TX(SPIx);

		/* Fill output buffer with data */
		*(__IO uint8_t *)&SPIx->DR = *dataOut++;

        /* Wait finish sending */
        SPI_WAIT(SPIx);

		/* Wait for SPI to end everything */
		SPI_WAIT_RX(SPIx);

		/* Read data register */
		*dataIn++ = *(__IO uint8_t *)&SPIx->DR;
	}
}


