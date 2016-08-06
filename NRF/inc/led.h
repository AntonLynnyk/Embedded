#pragma once

#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13
#define LED_RCC_GPIO RCC_APB2Periph_GPIOC

#define LED_MAX_BRIGHTNESS 10

void InitLED();

void RefreshLEDState(bool ledIsOn, uint16_t ledLight);

void LedOn(volatile void *ptr);

void LedOff(volatile void *ptr);

void LedUp(volatile void *ptr);

void LedDown(volatile void *ptr);
