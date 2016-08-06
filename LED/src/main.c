#include "button.h"
#include "led.h"
#include "main.h"

#include <stddef.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"

/* Private variables ---------------------------------------------------------*/
__IO uint8_t PrevXferComplete = 1;

void ConfigureButtons(struct Button *leftButton, struct Button *rightButton, struct Button *upButton, struct Button *downButton);
void InitializeTimers();

int main(void)
{
    ConfigureButtons(&leftButton, &rightButton, &upButton, &downButton);

    InitLED();
    InitializeButtons();
    InitializeTimers();

    Set_System();
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();

	while (true)
    {
        RefreshButtonState(&leftButton, &ledIsOn);
        RefreshButtonState(&rightButton, &ledIsOn);
        RefreshButtonState(&upButton, &ledLight);
        RefreshButtonState(&downButton, &ledLight);

        if (bDeviceState == CONFIGURED)
        {
           if (PrevXferComplete)
           {
              RHIDCheckState();
           }
        }

        RefreshLEDState(ledIsOn, ledLight);
    }

	return 0;
}

void ConfigureButtons(struct Button *leftButton, struct Button *rightButton, struct Button *upButton, struct Button *downButton)
{
    InitializeButton(leftButton);
    leftButton->GPIOButtonPin = BUTTON_LEFT;
    leftButton->ButtonPressedAction = LedOn;

    InitializeButton(rightButton);
    rightButton->GPIOButtonPin = BUTTON_RIGHT;
    rightButton->ButtonPressedAction = LedOff;

    InitializeButton(upButton);
    upButton->GPIOButtonPin = BUTTON_UP;
    upButton->ButtonPressedAction = LedUp;

    InitializeButton(downButton);
    downButton->GPIOButtonPin = BUTTON_DOWN;
    downButton->ButtonPressedAction = LedDown;
}

#define TIMER_PRESCALER		720

uint16_t previousState;
GPIO_InitTypeDef port;
TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
uint16_t buttonPreviousState;

void InitializeTimer()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_AF_PP;
    port.GPIO_Pin = GPIO_Pin_6;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &port);

    TIM_TimeBaseStructInit(&timer);
    timer.TIM_Prescaler = TIMER_PRESCALER;
    timer.TIM_Period = 10;
    TIM_TimeBaseInit(TIM4, &timer);

    TIM_OCStructInit(&timerPWM);
    timerPWM.TIM_Pulse = 50;
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM4, &timerPWM);
}

void InitializeTimers()
{
    __enable_irq();
    InitializeTimer();

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
    NVIC_EnableIRQ(TIM4_IRQn);
}

void TIM4_IRQHandler()
{
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    if (!ledIsOn)
    {
        if (ledLight != 0)
        {
            ledLight = 0;
        }
    }

    if (TIM4->CCR1 != ledLight)
    {
        TIM4->CCR1 = ledLight;
    }
}

