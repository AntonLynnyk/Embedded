#include <stddef.h>
#include "delay.h"
#include "lcd.h"

static void ConfigurePins()
{
    RCC_APB2PeriphClockCmd(LCD_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LCD_CONFIG_PINS | LCD_DATA_PINS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT, &GPIO_InitStructure);
}

static void Strobe()
{
    GPIO_ResetBits(LCD_PORT, LCD_WR_PIN);
    GPIO_SetBits(LCD_PORT, LCD_WR_PIN);
}

static void SetLCDData(uint8_t data)
{
    LCD_PORT->ODR &= ~LCD_DATA_PINS;
    LCD_PORT->ODR |= (uint16_t)data;
}

static void SendCmd(uint8_t command)
{
    GPIO_ResetBits(LCD_PORT, LCD_RS_PIN);
    SetLCDData(command);
    Strobe();
}

static void SendData(uint8_t command)
{
    GPIO_SetBits(LCD_PORT, LCD_RS_PIN);
    SetLCDData(command);
    Strobe();
}

static void WriteRegister(uint8_t reg, uint8_t data[], uint8_t byteCount)
{
    GPIO_SetBits(LCD_PORT, LCD_RD_PIN);
    GPIO_ResetBits(LCD_PORT, LCD_CS_PIN);

    SendCmd(reg);

    int counter;
    for (counter = 0; counter < byteCount; counter++)
    {
        SendData(data[counter]);
    }

    GPIO_SetBits(LCD_PORT, LCD_CS_PIN);
}

static void LcdSetXY(uint8_t x, uint8_t y)
{
    uint8_t data[4];

    data[0] = 0x00;
    data[1] = x;
    data[2] = 0x00;
    data[3] = 0xEF;
    WriteRegister(LCD_COLADDRSET, data, sizeof(data)); // x-address

    data[0] = 0x00;
    data[1] = y;
    data[2] = 0x01;
    data[3] = 0x40;
    WriteRegister(LCD_PAGEADDRSET, data, sizeof(data)); // y-address
}

void LCDInitializeDataSend()
{
    LcdSetXY(0x00, 0x00);
    GPIO_SetBits(LCD_PORT, LCD_RD_PIN);
    GPIO_ResetBits(LCD_PORT, LCD_CS_PIN);
    SendCmd(LCD_MEMORYWRITE);
    GPIO_SetBits(LCD_PORT, LCD_RS_PIN);
}

void LCDFinishDataSend()
{
    GPIO_SetBits(LCD_PORT, LCD_CS_PIN);
}

void LCDFillRow(uint8_t dataRow[LCD_BYTES_PER_ROW])
{
    uint16_t i;
    uint16_t lcdPort = (LCD_PORT->ODR & ~LCD_DATA_PINS) & ~LCD_WR_PIN;

    for(i = 0; i < LCD_BYTES_PER_ROW; i += 3)
	{
        LCD_PORT->ODR = lcdPort | dataRow[i] ; // blue
        LCD_PORT->BSRR = LCD_WR_PIN;

        LCD_PORT->ODR = lcdPort | dataRow[i+1];  // green
        LCD_PORT->BSRR = LCD_WR_PIN;

        LCD_PORT->ODR = lcdPort | dataRow[i+2];  //red
        LCD_PORT->BSRR = LCD_WR_PIN;
	}
}

void LCD_Initialize()
{
    ConfigurePins();

    GPIO_ResetBits(LCD_PORT, LCD_RST_PIN);
    GPIO_SetBits(LCD_PORT, LCD_RST_PIN);
    Delay_ms(10);

    uint8_t data[] = { LCD_PIXEL_FORMAT_ARG };
    WriteRegister(LCD_SLEEPOUT, NULL, 0);
    WriteRegister(LCD_PIXELFORMAT, data, 1);
    WriteRegister(LCD_NORMALDISP, NULL, 0);

    data[0] = LCD_MADCTL_MX;
    WriteRegister(LCD_MEMCONTROL, data, 1);
    WriteRegister(LCD_DISPLAYON, NULL, 0);
    return;
}
