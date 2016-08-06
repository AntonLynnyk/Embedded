#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include <stdbool.h>
#include <stdio.h>

#include "led.h"
#include "SPI.h"
#include "tm_stm32_nrf24l01.h"

//#define TRANSCEIVER
#define RECEIVER

#if defined(TRANSCEIVER) && defined(RECEIVER) || !defined(TRANSCEIVER) && !defined(RECEIVER)
    #error WHAT DO YOU WANT TO OBTAIN BY DOING THAT?
#endif

#ifdef TRANSCEIVER

uint8_t MyAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};

uint8_t TxAddress[] = {
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7E
};

#endif

#ifdef RECEIVER

// -------------------------
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

#endif

uint8_t dataOut[32], dataIn[32];

void Delay(unsigned long p)
{
	unsigned long i;
	for(i=0;i<p ;i++);
}

#define DEFAULT_CHANNEL 15
#define DEFAULT_PAYLOAD_SIZE 32

#ifdef TRANSCEIVER

int main(void)
{
    InitLED();
    RefreshLEDState(false, LED_MAX_BRIGHTNESS);

    // NRF24L01 goes to RX mode by default
    TM_NRF24L01_Init(DEFAULT_CHANNEL, DEFAULT_PAYLOAD_SIZE);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);
    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);

    while (true)
    {
        TM_NRF24L01_Transmit_Status_t transmissionStatus;

        Delay(200000);
        RefreshLEDState(false, LED_MAX_BRIGHTNESS);
        Delay(5000000);

        uint8_t messageString[32] = "Hello my friends. 01234567890!@";
        TM_NRF24L01_Transmit(messageString);

        do
        {
            RefreshLEDState(true, LED_MAX_BRIGHTNESS);
            transmissionStatus = TM_NRF24L01_GetTransmissionStatus();

        }
        while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
    }
}

#endif

#ifdef RECEIVER

int main(void)
{
    // NRF24L01 goes to RX mode by default
    InitLED();
    RefreshLEDState(true, LED_MAX_BRIGHTNESS);

    TM_NRF24L01_Init(DEFAULT_CHANNEL, DEFAULT_PAYLOAD_SIZE);
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);

    TM_NRF24L01_SetMyAddress(MyAddress);
    TM_NRF24L01_SetTxAddress(TxAddress);

    while (true)
    {
        RefreshLEDState(false, LED_MAX_BRIGHTNESS);

        if (TM_NRF24L01_DataReady())
        {
            RefreshLEDState(true, LED_MAX_BRIGHTNESS);
            TM_NRF24L01_GetData(dataIn);
        }
    }
}

#endif
