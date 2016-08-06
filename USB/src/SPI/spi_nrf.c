#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stdbool.h>

#include "spi_nrf.h"

static void SPI_RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(NRF_SPI_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(NRF_SPI_GPIO_CLK, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

static void SPI_GPIO_Configuration(uint16_t SPI_Mode)
{
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = NRF_SPI_PIN_SCK | NRF_SPI_PIN_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }

    GPIO_Init(NRF_SPI_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = NRF_SPI_PIN_MISO;

    if(SPI_Mode == SPI_Mode_Master)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    }
    else
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    }

    GPIO_Init(NRF_SPI_GPIO, &GPIO_InitStructure);
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
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(NRF_SPI, &SPI_InitStructure);
    SPI_Cmd(NRF_SPI, ENABLE);

    SPI_NSSInternalSoftwareConfig(NRF_SPI, SPI_NSSInternalSoft_Set);
}

