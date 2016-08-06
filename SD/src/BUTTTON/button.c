#include <stddef.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "button.h"

void InitializeButtons()
{
    RCC_APB2PeriphClockCmd(BUTTONS_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = BUTTON_LEFT | BUTTON_RIGHT | BUTTON_UP | BUTTON_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUTTONS_PORT, &GPIO_InitStructure);
}

void InitializeButton(struct Button *button)
{
    button->privateButtonPressedCounter = 0;
    button->privateCounter = 0;
    button->privatePreviousState = false;
    button->privateStateChanged = false;

    button->ButtonsPort = BUTTONS_PORT;
    button->GPIOButtonPin = 0;
    button->GPIOButtonPressed = BUTTONS_PRESSED_GPIO;
    button->ButtonPressedAction = NULL;
    button->ButtonUnpressedAction = NULL;
}

void RefreshButtonState(struct Button *button, volatile void * ptr)
{
    uint16_t mediumbuttonPressed = (((uint16_t)(button->ButtonsPort->IDR)) & button->GPIOButtonPin);
    bool buttonPressed = (mediumbuttonPressed == button->GPIOButtonPressed);

    if (!button->privateStateChanged) // State has been changed
    {
        if (buttonPressed != button->privatePreviousState)
        {
            button->privateStateChanged = true;
            button->privateCounter = 0;
            button->privateButtonPressedCounter = 0;
        }
    }

    if (button->privateStateChanged)
    {
        if (buttonPressed)
        {
            button->privateButtonPressedCounter++;
        }
        else
        {
            button->privateButtonPressedCounter--;
        }

        button->privateCounter++;
        if(button->privateCounter == 1000)
        {
            if (button->privateButtonPressedCounter > 0)
            {
                if (button->ButtonPressedAction != NULL)
                {
                    button->ButtonPressedAction(ptr);
                }

                button->privatePreviousState = true;
            }
            else
            {
                if (button->ButtonUnpressedAction != NULL)
                {
                    button->ButtonUnpressedAction(ptr);
                }

                button->privatePreviousState = false;
            }

            button->privateStateChanged = false;
        }
    }
}
