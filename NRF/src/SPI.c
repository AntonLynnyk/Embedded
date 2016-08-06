#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stdbool.h>

#include "SPI.h"

void SPI_RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(SPI_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(SPI_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void SPI_GPIO_Configuration(uint16_t SPI_Mode)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SPI_PIN_SCK | SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }

    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MISO;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }

    GPIO_Init(SPI_GPIO, &GPIO_InitStructure);
}

void SPI_Initialize()
{
    SPI_RCC_Configuration();
    SPI_GPIO_Configuration(SPI_Mode_Master);

    SPI_InitTypeDef   SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI, &SPI_InitStructure);
    SPI_Cmd(SPI, ENABLE);

    SPI_NSSInternalSoftwareConfig(SPI, SPI_NSSInternalSoft_Set);
}

/**
 * @brief  Check SPI busy status
 */
#define SPI_IS_BUSY(SPIx)                   (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0)

/**
 * @brief  SPI wait till end
 */
#define SPI_WAIT(SPIx)                      while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY) == SET)
#define SPI_WAIT_TX(SPIx)                   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET)
#define SPI_WAIT_RX(SPIx)                   while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)

/**
 * @brief  Checks if SPI is enabled
 */
#define SPI_CHECK_ENABLED(SPIx)             if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return;}

/**
 * @brief  Checks if SPI is enabled and returns value from function if not
 */
#define SPI_CHECK_ENABLED_RESP(SPIx, val)   if (!((SPIx)->CR1 & SPI_CR1_SPE)) {return (val);}


uint8_t TM_SPI_Send(SPI_TypeDef* SPIx, uint8_t data)
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

void TM_SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint32_t count)
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

void TM_SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint32_t count)
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

void TM_SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint32_t count)
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


