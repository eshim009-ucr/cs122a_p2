/*
 * sm_show_number.c
 *
 *  Created on: May 25, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "main.h"
#include "7seg.h"
#include "sm_tap_tempo.h"
#include "sm_show_number.h"


// Used to check if digits should be updated
static uint_fast16_t last_tempo = 0;
// Broken down into digits
static uint_fast8_t hundreds;
static uint_fast8_t tens;
static uint_fast8_t ones;


enum sm_sn_state {
	SM_SN_Init,
	SM_SN_Hundreds,
	SM_SN_Tens,
	SM_SN_Ones
};

int sm_sn_tick(int state);
void update_digits(void);


Task task_show_number = {
	.state = SM_SN_Init,
	.period = 5,
	.t_waiting = 0,
	.tick_fn = sm_sn_tick
};


int sm_sn_tick(int state) {
	switch (state) {
		case SM_SN_Init:
			state = SM_SN_Hundreds;
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 0);
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 0);
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 0);
			break;
		case SM_SN_Hundreds:
			state = SM_SN_Tens;
			break;
		case SM_SN_Tens:
			state = SM_SN_Ones;
			break;
		case SM_SN_Ones:
			state = SM_SN_Hundreds;
			break;
		default:
			state = SM_SN_Init;
			break;
	}

	switch (state) {
		case SM_SN_Hundreds:
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 0);
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 1);
			update_digits();
			writeDigit(hundreds);
			break;
		case SM_SN_Tens:
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 0);
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 1);
			update_digits();
			writeDigit(tens);
			break;
		case SM_SN_Ones:
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 0);
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 1);
			update_digits();
			writeDigit(ones);
			break;
		default:
			break;
	}

	return state;
}


void update_digits() {
	if (tempo != last_tempo) {
		hundreds = ((tempo % 1000) / 100);
		tens = ((tempo % 100) / 10);
		ones = ((tempo % 10) / 1);
		last_tempo = tempo;
	}
}
