#include <stm32f10x.h>

#define LCD_PORT GPIOA
#define LCD_D0_PIN GPIO_Pin_0
#define LCD_D1_PIN GPIO_Pin_1
#define LCD_D2_PIN GPIO_Pin_2
#define LCD_D3_PIN GPIO_Pin_3
#define LCD_D4_PIN GPIO_Pin_4
#define LCD_D5_PIN GPIO_Pin_5
#define LCD_D6_PIN GPIO_Pin_6
#define LCD_D7_PIN GPIO_Pin_7
#define LCD_RST_PIN GPIO_Pin_8
#define LCD_CS_PIN GPIO_Pin_9
#define LCD_RS_PIN GPIO_Pin_10
#define LCD_WR_PIN GPIO_Pin_11
#define LCD_RD_PIN GPIO_Pin_12
#define LCD_RCC_GPIO RCC_APB2Periph_GPIOA

#define LCD_CONFIG_PINS \
        (LCD_RST_PIN |\
        LCD_CS_PIN  |\
        LCD_RS_PIN  |\
        LCD_WR_PIN  |\
        LCD_RD_PIN)

#define LCD_DATA_PINS \
        (LCD_D0_PIN |\
        LCD_D1_PIN |\
        LCD_D2_PIN |\
        LCD_D3_PIN |\
        LCD_D4_PIN |\
        LCD_D5_PIN |\
        LCD_D6_PIN |\
        LCD_D7_PIN)

#define LCD_SOFTRESET     0x01
#define LCD_SLEEPIN       0x10
#define LCD_SLEEPOUT      0x11
#define LCD_NORMALDISP    0x13
#define LCD_INVERTOFF     0x20
#define LCD_INVERTON      0x21
#define LCD_GAMMASET      0x26
#define LCD_DISPLAYOFF    0x28
#define LCD_DISPLAYON     0x29
#define LCD_COLADDRSET    0x2A
#define LCD_PAGEADDRSET   0x2B
#define LCD_MEMORYWRITE   0x2C
#define LCD_PIXELFORMAT   0x3A
#define LCD_FRAMECONTROL  0xB1
#define LCD_DISPLAYFUNC   0xB6
#define LCD_ENTRYMODE     0xB7
#define LCD_POWERCONTROL1 0xC0
#define LCD_POWERCONTROL2 0xC1
#define LCD_VCOMCONTROL1  0xC5
#define LCD_VCOMCONTROL2  0xC7
#define LCD_MEMCONTROL    0x36
#define LCD_MADCTL        0x36
#define LCD_MADCTL_MY     0x80
#define LCD_MADCTL_MX     0x40
#define LCD_MADCTL_MV     0x20
#define LCD_MADCTL_ML     0x10
#define LCD_MADCTL_RGB    0x00
#define LCD_MADCTL_BGR    0x08
#define LCD_MADCTL_MH     0x04

#define LCD_WIDTH_PIXELS 240
#define LCD_HEIGHT_PIXELS 320
#define LCD_PIXELS_COUNT LCD_WIDTH_PIXELS * LCD_HEIGHT_PIXELS

#define LCD_BYTES_PER_PIXEL 3
#define LCD_BYTES_PER_ROW LCD_WIDTH_PIXELS * LCD_BYTES_PER_PIXEL

#if (LCD_BYTES_PER_PIXEL  == 3)
    #define LCD_PIXEL_FORMAT_ARG 0x06
#elif (LCD_BYTES_PER_PIXEL  == 2)
    #define LCD_PIXEL_FORMAT_ARG 0x05
#else
    #error Please choose a correct pixel format
#endif

void LCD_Initialize();
void LCDFinishDataSend();
void LCDInitializeDataSend();
void LCDFillRow(uint8_t dataRow[LCD_WIDTH_PIXELS]);
