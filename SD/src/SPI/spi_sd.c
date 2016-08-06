#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stdbool.h>

#include "spi_sd.h"

static void SPI_RCC_Configuration(void)
{
    RCC_APB1PeriphClockCmd(SD_SPI_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(SD_SPI_GPIO_CLK, ENABLE);
}

static void SPI_GPIO_Configuration(uint16_t SPI_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_SCK | SD_SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }

    GPIO_Init(SD_SPI_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_CSN;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }

    GPIO_Init(SD_SPI_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SD_SPI_PIN_MISO;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }

    GPIO_Init(SD_SPI_GPIO, &GPIO_InitStructure);
}

void SD_SPI_Initialize(uint16_t baudRatePrescalerSPI)
{
    SPI_Cmd(SD_SPI, DISABLE);
    SPI_RCC_Configuration();
    SPI_GPIO_Configuration(SPI_Mode_Master);

    SPI_InitTypeDef   SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = baudRatePrescalerSPI;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SD_SPI, &SPI_InitStructure);
    SPI_Cmd(SD_SPI, ENABLE);

    SPI_NSSInternalSoftwareConfig(SD_SPI, SPI_NSSInternalSoft_Set);
}
