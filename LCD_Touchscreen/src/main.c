#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stddef.h>
#include <stdbool.h>
#include "lcd.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    InitLED();
    LCD_Initialize();

    RefreshLEDState(true, LED_MAX_BRIGHTNESS);
    Delay_ms(1000);
    RefreshLEDState(false, LED_MAX_BRIGHTNESS);

    while(true)
    {
        //LED_PORT->BSRR = LED_PIN;
        LCDFillScreen(0xFF, 0x00, 0x00);
        Delay_ms(1000);

        //LED_PORT->BRR = LED_PIN;
        LCDFillScreen(0x00, 0xFF, 0x00);
        Delay_ms(1000);

        //LED_PORT->BSRR = LED_PIN;
        LCDFillScreen(0x00, 0x00, 0xFF);
        Delay_ms(1000);

        //LED_PORT->BRR = LED_PIN;
        LCDFillScreen(0xFF, 0xFF, 0xFF);
        Delay_ms(1000);
    }
}
