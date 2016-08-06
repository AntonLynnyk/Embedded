#pragma once

#include <stdbool.h>
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#define BUTTONS_PORT GPIOB
#define BUTTON_DOWN  GPIO_Pin_0
#define BUTTON_UP GPIO_Pin_1
#define BUTTON_LEFT    GPIO_Pin_10
#define BUTTON_RIGHT  GPIO_Pin_11
#define BUTTONS_RCC_GPIO RCC_APB2Periph_GPIOB
#define BUTTONS_PRESSED_GPIO 0x0000

struct Button
{
    int16_t privateButtonPressedCounter;
    uint16_t privateCounter;
    bool privatePreviousState;
    bool privateStateChanged;

    GPIO_TypeDef *ButtonsPort;
    uint16_t GPIOButtonPin;
    uint16_t GPIOButtonPressed;
    void (*ButtonPressedAction)(volatile void *ptr);
    void (*ButtonUnpressedAction)(volatile void *ptr);
};

void InitializeButtons();

void InitializeButton(struct Button *);

void RefreshButtonState(struct Button *, volatile void *);
