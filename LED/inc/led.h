#pragma once

#define LED_PORT GPIOA
#define LED_PIN GPIO_Pin_1
#define LED_RCC_GPIO RCC_APB2Periph_GPIOA

void InitLED();

void RefreshLEDState(bool ledIsOn, uint16_t ledLight);

void LedOn(volatile void *ptr);

void LedOff(volatile void *ptr);

void LedUp(volatile void *ptr);

void LedDown(volatile void *ptr);
