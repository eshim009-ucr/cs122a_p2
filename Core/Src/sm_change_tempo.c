/*
 * sm_change_tempo.c
 *
 *  Created on: Jun 3, 2023
 *      Author: eshim009
 */
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "sm_tap_tempo.h"
#include "sm_change_tempo.h"


static const uint_fast8_t HOLD_THRESH = 3;


enum sm_ct_state {
	SM_CT_Init,
	SM_CT_Wait,
	SM_CT_PressInc,
	SM_CT_PressDec
};


int sm_ct_tick(int state);


static uint_fast8_t t_held;


Task task_change_tempo = {
	.state = SM_CT_Init,
	.period = 100,
	.t_waiting = 0,
	.tick_fn = sm_ct_tick
};


int sm_ct_tick(int state) {
	bool inc_pressed = (HAL_GPIO_ReadPin(btninc_GPIO_Port, btninc_Pin) == GPIO_PIN_RESET) ? true: false;
	bool dec_pressed = (HAL_GPIO_ReadPin(btndec_GPIO_Port, btndec_Pin) == GPIO_PIN_RESET) ? true: false;

	switch (state) {
		case SM_CT_Init:
			state = SM_CT_Wait;
			break;
		case SM_CT_Wait:
			//
			if (!inc_pressed && !dec_pressed) {
				state = SM_CT_Wait;
			} else if (inc_pressed && !dec_pressed) {
				// Transition
				state = SM_CT_PressInc;
				// Mealy actions
				t_held = 0;
			} else if (!inc_pressed && dec_pressed) {
				// Transition
				state = SM_CT_PressDec;
				// Mealy actions
				t_held= 0;
			} else {
				// TODO: Handle both buttons pressed
			}
			break;
		case SM_CT_PressInc:
			if (!inc_pressed) {
				// Transition
				state = SM_CT_Wait;
				// Mealy actions
				if (t_held < HOLD_THRESH) {
					tempo++;
				} else {
					tempo *= 2;
				}
			} else {
				state = SM_CT_PressInc;
			}
			break;
		case SM_CT_PressDec:
			if (!dec_pressed) {
				// Transition
				state = SM_CT_Wait;
				// Mealy actions
				if (t_held < HOLD_THRESH) {
					tempo--;
				} else {
					tempo /= 2;
				}
			} else {
				state = SM_CT_PressDec;
			}
			break;
		default:
			state = SM_CT_Init;
			break;
	}

	switch (state) {
		// Same action for both states
		case SM_CT_PressInc:
		case SM_CT_PressDec:
			t_held++;
		default:
			break;
	}

	return state;
}
