#include "DATA_BUTTON.h"
#include "BSP_BUTTON.h"

static uint8_t button_state = BUTTON_RELEASED;

void data_button_set_state(uint8_t state)
{
	button_state = state;
}

uint8_t data_button_get_state(void)
{
	return button_state;
}