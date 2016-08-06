/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    09-September-2013
  * @brief   RHID demo main file
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "nrf24l01.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
__IO uint8_t PrevXferComplete = 1;
uint8_t dataIn[32];
extern uint8_t blink;

/* Private functions ---------------------------------------------------------*/

void Delay(volatile uint32_t nCount) {
	for (; nCount != 0; nCount--);
}

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

void InitializeNRF24L01(void);

#define LED_PORT GPIOC
#define LED_PIN GPIO_Pin_13
#define LED_RCC_GPIO RCC_APB2Periph_GPIOC

void InitLED()
{
    RCC_APB2PeriphClockCmd(LED_RCC_GPIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : ************************************************************/
int main(void)
{
  InitLED();

  InitializeNRF24L01();
  Set_System();

  USB_Interrupts_Config();

  Set_USBClock();

  USB_Init();

  GPIO_SetBits(LED_PORT, LED_PIN);

  while(1)
  {
      if (blink)
      {
          GPIO_ResetBits(LED_PORT, LED_PIN);
          Delay(20000000);
          GPIO_SetBits(LED_PORT, LED_PIN);
          Delay(20000000);

          blink = 0;
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

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif
