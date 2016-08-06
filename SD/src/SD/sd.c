#include <stdbool.h>
#include "stm32f10x_conf.h"
#include "spi_sd.h"
#include "spi.h"
#include "sd.h"

static void Delay(unsigned long);
static void Initialize(void);
static void EnableSPIMode(void);
static uint8_t GetSDVersion(void);

static void Delay(unsigned long p)
{
	unsigned long i;
	for(i=0;i<p ;i++);
}

static void Initialize()
{
    // Initialize delay
    Delay(500000);

    SD_SPI_CSN_HIGH;

    int i;
    for (i = 0; i < 10; i++)
    {
        SPI_Send(SD_SPI, 0xFF);
    }

    SD_SPI_CSN_LOW;

    SPI_Send(SD_SPI, 0x40);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x95);
    while(SPI_Send(SD_SPI, 0xFF)!= 0x01);

    SD_SPI_CSN_HIGH;
}

static uint8_t GetSDVersion()
{
    uint8_t answer;

    SD_SPI_CSN_LOW;

    SPI_Send(SD_SPI, 0x48);

    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x01);
    SPI_Send(SD_SPI, 0xAA);
    SPI_Send(SD_SPI, 0x87);
    while((answer = SPI_Send(SD_SPI, 0xFF)) == 0xFF);
    SPI_Send(SD_SPI, 0xFF);
    SPI_Send(SD_SPI, 0xFF);
    SPI_Send(SD_SPI, 0xFF);
    SPI_Send(SD_SPI, 0xFF);

    SD_SPI_CSN_HIGH;

    return answer;
}

static void EnableSPIMode()
{
    uint8_t status_ = 0xFF;
    SD_SPI_CSN_LOW;

    do
    {
        //CMD55
        SPI_Send(SD_SPI, 0X77);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);

        while (SPI_Send(SD_SPI, 0xFF) != 0x01) ;

        //CMD41
        SPI_Send(SD_SPI, 0X69);
        SPI_Send(SD_SPI, 0x40);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0x00);
        SPI_Send(SD_SPI, 0XFF);

        SPI_Send(SD_SPI, 0xFF);
        status_ = SPI_Send(SD_SPI, 0xFF) ;
    }
    while (status_!= 0X00);

    SD_SPI_CSN_HIGH;
}

void SD_ReadCIDRegister(uint8_t block16[16])
{
    uint8_t i;

    SD_SPI_CSN_LOW;

    SPI_Send(SD_SPI, 0x4A);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    while(SPI_Send(SD_SPI, 0xFF) != 0x00) ;
    while(SPI_Send(SD_SPI, 0xFF) != 0xFE) ;

    for (i = 0; i < 16; i++)
    {
        block16[i] = SPI_Send(SD_SPI, 0xFF);
    }

    SPI_Send(SD_SPI, 0xFF);
    SPI_Send(SD_SPI, 0xFF);

    SD_SPI_CSN_HIGH;
}

void SD_WriteDataBlock(const uint8_t writeBlock[], uint32_t sector, uint32_t writeBytesCount)
{
    uint16_t i;
    SD_SPI_CSN_LOW;

    SPI_Send(SD_SPI, 0x58);
    SPI_Send(SD_SPI, (uint8_t)(sector >> 24));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 16));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 8));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 0));
    SPI_Send(SD_SPI, 0x00);
    while(SPI_Send(SD_SPI, 0xFF) != 0x00);
    SPI_Send(SD_SPI, 0xFF);

    SPI_Send(SD_SPI, 0xFE);
    for(i = 0; i < SD_BLOCK_SIZE; i++)
    {
        if (i < writeBytesCount)
        {
            SPI_Send(SD_SPI, writeBlock[i]);
        }
        else
        {
            SPI_Send(SD_SPI, 0);
        }

    }

    SPI_Send(SD_SPI, 0x00);
    SPI_Send(SD_SPI, 0x00);
    while ((SPI_Send(SD_SPI, 0xFF) & 0b00011111) != 0x05);
    while (SPI_Send(SD_SPI, 0xFF) == 0x00);
    SD_SPI_CSN_HIGH;
}

int SD_ReadDataBlock(uint8_t readBlock[], uint32_t sector, uint16_t offset, uint16_t readBytesCount)
{
    uint16_t i;

    SD_SPI_CSN_LOW;

    SPI_Send(SD_SPI, 0x51);
    SPI_Send(SD_SPI, (uint8_t)(sector >> 24));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 16));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 8));
    SPI_Send(SD_SPI, (uint8_t)(sector >> 0));
    SPI_Send(SD_SPI, 0x00);
    while(SPI_Send(SD_SPI, 0xFF) != 0x00);

    while(SPI_Send(SD_SPI, 0xFF) != 0xFE);

    uint8_t data;
    for(i = 0; i < SD_BLOCK_SIZE; i++)
    {
        data = SPI_Send(SD_SPI, 0xFF);
        if (i >= offset)
        {
            if ((i - offset) < readBytesCount)
            {
                readBlock[i - offset] = data;
            }
        }
    }

    SPI_Send(SD_SPI, 0xFF);
    SPI_Send(SD_SPI, 0xFF);

    SD_SPI_CSN_HIGH;

    return 0;
}

void SD_Initialize()
{
  SD_SPI_Initialize(SPI_BaudRatePrescaler_128);

  Initialize();
  GetSDVersion();
  EnableSPIMode();

  SD_SPI_Initialize(SPI_BaudRatePrescaler_2);
}
