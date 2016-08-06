#include <stdbool.h>
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "led.h"

void InitLED()
{
    RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void RefreshLEDState(bool ledIsOn, uint16_t ledLight)
{
    static uint16_t ledOn;

    if (ledIsOn)
    {
        // TODO: Change this peace of shit
        if (ledOn <= ledLight)
        {
            GPIO_ResetBits(LED_PORT, LED_PIN);

        }
        else
        {
            GPIO_SetBits(LED_PORT, LED_PIN);
        }
    }
    else
    {
        GPIO_SetBits(LED_PORT, LED_PIN);
    }

    ledOn++;
    ledOn %= LED_MAX_BRIGHTNESS;
}

void LedOn(volatile void *ptr)
{
    bool *ledIsOn = (bool *)ptr;
    *ledIsOn = true;
}

void LedOff(volatile void *ptr)
{
    bool *ledIsOn = (bool *)ptr;
    *ledIsOn = false;
}

void LedUp(volatile void *ptr)
{
    uint16_t *ledLight = (uint16_t *)ptr;

    if(*ledLight < LED_MAX_BRIGHTNESS)
    {
        (*ledLight)++;
    }
}

void LedDown(volatile void *ptr)
{
    uint16_t *ledLight = (uint16_t *)ptr;

    if(*ledLight > 0)
    {
        (*ledLight)--;
    }
}
