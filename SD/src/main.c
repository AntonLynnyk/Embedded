/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "stm32f10x_conf.h"
#include "delay.h"
#include "SPI.h"
#include "led.h"
#include "sd.h"
#include "pff.h"
#include "bmp.h"
#include "lcd.h"
#include "nrf24l01.h"
#include "pff.h"
#include "button.h"
#include "string.h"

uint8_t startCommand[] = "I start to send a new image now!";
struct Button leftButton, rightButton, upButton, downButton;
void ConfigureButtons(struct Button *leftButton, struct Button *rightButton, struct Button *upButton, struct Button *downButton);

void InitializeFileSystem();
void HollyFuckHappened();
void InitializeNRF24L01();
void StartReceiveImages();
uint8_t dataBuffer[LCD_BYTES_PER_ROW];
uint8_t dataIn[32] = "";
uint8_t _receivingImage;

BMPFileHeader fileHeader;
BMPImageHeader imageHeader;
FATFS fs;
UINT br;

FILINFO fno;
DIR dir;
char previousFileName[30];
uint8_t previousFileIsValid;

uint8_t TxAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};

uint8_t MyAddress[] = {
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7E
};

//#define MY_DEBUG

void OpenFile(char *fileName)
{
  uint8_t buffer[SD_BLOCK_SIZE];

  if (pf_open(fileName) == FR_OK)
  {
      pf_read(buffer, SD_BLOCK_SIZE, &br);
      BMPReadInfoHeaders(&fileHeader, &imageHeader, buffer);

      if (BMPValidateImageParameters(&fileHeader, &imageHeader) != RES_OK)
      {
          HollyFuckHappened();
      }
  }
}

static void MemCopy(uint8_t *source, uint8_t *target, uint16_t offset, uint16_t targetOffset, uint16_t count)
{
    while(count != 0)
    {
        target[targetOffset] = source[offset];
        targetOffset++;
        offset++;
        count--;
    }
}

int main(void)
{
  InitLED();
  RefreshLEDState(true, LED_MAX_BRIGHTNESS);
  LCD_Initialize();
  InitializeNRF24L01();

  ConfigureButtons(&leftButton, &rightButton, &upButton, &downButton);
  InitializeButtons();

  InitializeFileSystem();
  StartReceiveImages();
  return 0;
}

uint8_t CheckBMPImage(char *fileName)
{
    uint8_t buffer[SD_BLOCK_SIZE];

    if (pf_open(fileName) == FR_OK)
    {
      pf_read(buffer, SD_BLOCK_SIZE, &br);
      if (br != SD_BLOCK_SIZE)
      {
          return 1;
      }

      BMPReadInfoHeaders(&fileHeader, &imageHeader, buffer);
      if (BMPValidateImageParameters(&fileHeader, &imageHeader) != RES_OK)
      {
          return 1;
      }
    }
    else
    {
        return 1;
    }

    return 0;
}

void PrintImage(char *fileName)
{
    OpenFile(fileName);

    uint16_t i;
    uint8_t dataRow[LCD_BYTES_PER_ROW];

    pf_lseek(fileHeader.bfOffBits);
    LCDInitializeDataSend();

    for(i = 0; i < LCD_HEIGHT_PIXELS; i++)
    {
        pf_read(dataRow, LCD_BYTES_PER_ROW, &br);

        if (br != LCD_BYTES_PER_ROW)
        {
            HollyFuckHappened();
        }

        LCDFillRow(dataRow);
    }

    LCDFinishDataSend();
}

void PrintNextImage(volatile void *uselessParameter)
{
    _receivingImage = 0;

    if (previousFileIsValid)
    {
        uint16_t stringLength = strlen(fno.fname) + 1;
        MemCopy(fno.fname, previousFileName, 0, 0, stringLength);
    }

    FRESULT res;
    res = pf_readdir(&dir, &fno);

    if (res == FR_NO_FILE)
    {
        char *rootDir = " ";
        pf_opendir(&dir, rootDir);

        res = pf_readdir(&dir, &fno);
        if (res == FR_NO_FILE)
        {
            return;
        }
    }

    if (!CheckBMPImage(fno.fname))
    {
        previousFileIsValid = true;
        PrintImage(fno.fname);
    }
}

void PrintPreviousImage(volatile void *uselessParameter)
{
    _receivingImage = false;

    if (previousFileIsValid)
    {
        if (!CheckBMPImage(previousFileName))
        {
            PrintImage(previousFileName);
        }
    }
}

void InitializeFileSystem()
{
    pf_mount(&fs);
    char *rootDir = " ";
    pf_opendir(&dir, rootDir);
}

uint8_t rowData[736];
void StartReceiveImages()
{
    uint16_t rowReceived = 0;
    uint16_t bytesInRowReceived = 0;
    uint16_t const bytesInARow = 720;

    while (true)
        {
            RefreshButtonState(&leftButton, NULL);
            RefreshButtonState(&rightButton, NULL);

            RefreshLEDState(false, LED_MAX_BRIGHTNESS);

            if (NRF24L01_DataReady())
            {
                NRF24L01_GetData(dataIn);

                if (!mem_cmp(startCommand, dataIn, 32))
                {
                    RefreshLEDState(true, LED_MAX_BRIGHTNESS);
                    Delay_ms(50);

                    _receivingImage = 1;
                    rowReceived = 0;
                    bytesInRowReceived = 0;

                    LCDInitializeDataSend();
                    continue;
                }

                if (_receivingImage)
                {
                    MemCopy(dataIn, rowData, 0, bytesInRowReceived, 32);
                    bytesInRowReceived += 32;

                    if (bytesInRowReceived >= bytesInARow)
                    {
                        LCDFillRow(rowData);
                        bytesInRowReceived = bytesInRowReceived - bytesInARow;
                        MemCopy(dataIn, rowData, bytesInRowReceived, 0, bytesInRowReceived);

                        rowReceived++;

                        if (rowReceived == 320)
                        {
                            RefreshLEDState(false, LED_MAX_BRIGHTNESS);

                            _receivingImage = 0;
                            rowReceived = 0;
                            bytesInRowReceived = 0;

                            LCDFinishDataSend();
                        }
                    }
                }
            }
        }
}

void InitializeNRF24L01()
{
    // NRF24L01 goes to RX mode by default;
    NRF24L01_Init(DEFAULT_CHANNEL, DEFAULT_PAYLOAD_SIZE);
    NRF24L01_SetRF(NRF24L01_DataRate_2M, NRF24L01_OutputPower_M18dBm);

    NRF24L01_SetMyAddress(MyAddress);
    NRF24L01_SetTxAddress(TxAddress);
}

void HollyFuckHappened()
{
    while(true)
    {
        RefreshLEDState(true, LED_MAX_BRIGHTNESS);
        Delay_mks(2000000);

        RefreshLEDState(false, LED_MAX_BRIGHTNESS);
        Delay_mks(2000000);
    }
}

void ConfigureButtons(struct Button *leftButton, struct Button *rightButton, struct Button *upButton, struct Button *downButton)
{
    InitializeButton(leftButton);
    leftButton->GPIOButtonPin = BUTTON_LEFT;
    leftButton->ButtonPressedAction = PrintPreviousImage;

    InitializeButton(rightButton);
    rightButton->GPIOButtonPin = BUTTON_RIGHT;
    rightButton->ButtonPressedAction = PrintNextImage;
}
