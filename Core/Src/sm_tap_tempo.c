/*
 * sm_tap_tempo.c
 *
 *  Created on: May 31, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "main.h"
#include "scheduler.h"
#include "sm_tap_tempo.h"


const uint_fast16_t TASK_PERIOD =  100;
const uint_fast8_t HOLD_THRESH = 3;
const uint_fast32_t TICKS_PER_MINUTE = (60 * 1000) / TASK_PERIOD;


enum sm_tt_state {
	SM_TT_Init,
	SM_TT_Wait,
	SM_TT_StartTap,
	SM_TT_Count,
	SM_TT_NextTap,
	SM_TT_LastTap
};

int sm_tt_tick(int state);


// Time since the tap began recording
uint_fast32_t t;
// Current recorded tempo
uint_fast8_t tempo = 0;
// Number of taps that have been recorded
uint_fast8_t num_taps;
// Number of ticks that the button has been held down for
uint_fast8_t t_held;


Task task_tap_tempo = {
	.state = SM_TT_Init,
	.period = TASK_PERIOD,
	.t_waiting = 0,
	.tick_fn = sm_tt_tick
};


int sm_tt_tick(int state) {
	switch (state) {
		case SM_TT_Init:
			state = SM_TT_Wait;
			break;
		case SM_TT_Wait:
			if (HAL_GPIO_ReadPin(btntap_GPIO_Port, btntap_Pin) == GPIO_PIN_SET) {
				state = SM_TT_Wait;
			} else {
				state = SM_TT_StartTap;
			}
			break;
		case SM_TT_StartTap:
			state = SM_TT_Count;
			break;
		case SM_TT_Count:
			state = SM_TT_NextTap;
			break;
		case SM_TT_NextTap:
			if (HAL_GPIO_ReadPin(btntap_GPIO_Port, btntap_Pin) == GPIO_PIN_SET) {
				state = SM_TT_Count;
				num_taps++;
			} else {
				if (t_held < HOLD_THRESH) {
					state = SM_TT_NextTap;
				} else {
					state = SM_TT_LastTap;
					num_taps++;
					// Don't consider time held as part of tempo count
					t -= t_held - 1;
				}
			}
			break;
		default:
			state = SM_TT_Init;
			break;
	}

	switch (state) {
		case SM_TT_StartTap:
			t = 0;
			num_taps = 0;
			break;
		case SM_TT_Count:
			t++;
			t_held = 0;
			break;
		case SM_TT_NextTap:
			t_held++;
			break;
		case SM_TT_LastTap:
			tempo = num_taps * TICKS_PER_MINUTE / t;
			break;
		default:
			break;
	}

	return state;
}
