#include "GPIO.h"
#include "BSP_LED.h"
#include "stdint.h"

#define LED_PORT_ID  PORTB_ID
#define LED_PIN_ID   PIN0_ID // led PB0

void BSP_LED_Init(void)
{
	GPIO_setupPinDirection(LED_PORT_ID, LED_PIN_ID, PIN_OUTPUT);
	BSP_LED_Off();
}

void BSP_LED_On (void)
{
	GPIO_writePin(LED_PORT_ID, LED_PIN_ID, LOGIC_HIGH);
}

void BSP_LED_Off(void)
{
	GPIO_writePin(LED_PORT_ID, LED_PIN_ID, LOGIC_LOW);
}
