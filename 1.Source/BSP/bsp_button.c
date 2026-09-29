#include "BSP_BUTTON.h"
#include "DATA_BUTTON.h"
#include <avr/io.h>
#include "common_macros.h"

uint8_t bsp_button_start_get_state(void){
	/* PC0 ?ang d�ng l�m input cho n�t nh?n */
	if (((PINC & (1 << PC0)) == 0) || ((PINC & (1 << PC1)) == 0))
	{
		/* ??c ???c m?c 0 ? n�t ?ang ???c nh?n */
		return BUTTON_PRESSED;
	}
	else
	{
		/* ??c ???c m?c 1 ? n�t ?ang th? */
		return BUTTON_RELEASED;
	}
}

uint8_t bsp_button_page_get_state(void)
{
	if ((PINB & (1 << PB1)) == 0) //THEM NUT CHUYEN PAGE
	{
		return BUTTON_PRESSED;
	}
	else
	{
		return BUTTON_RELEASED;
	}
}

void bsp_button_init(void)
{
	/* PC0, PC1 is input */
	CLEAR_BIT(DDRC, 0);
	CLEAR_BIT(DDRC, 1);
	
	CLEAR_BIT(DDRB, 1); //PB1 INPUT 

	/* Pull-up resistor enable */
	SET_BIT(PORTC, 0);
	SET_BIT(PORTC, 1);
	
	SET_BIT(PORTB, 1);
}