/*
 * sm_playback.c
 *
 *  Created on: Jun 6, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "scheduler.h"
#include "sm_tap_tempo.h"
#include "sm_playback.h"
#include "tim.h"


static const uint_fast8_t TASK_PERIOD = 50;

// Used to check if interval should be updated
static uint_fast16_t last_tempo = 0;
// The time between metronome beeps
// Equivalent to a quarter note
static uint_fast16_t t_off = 0;


enum sm_ct_state {
	SM_PB_Init,
	SM_PB_Wait,
	SM_PB_SpeakerOn,
	SM_PB_SpeakerOff
};


int sm_pb_tick(int state);
static inline void update_interval(void);


Task task_playback = {
	.state = SM_PB_Init,
	.period = TASK_PERIOD,
	.t_waiting = 0,
	.tick_fn = sm_pb_tick
};


int sm_pb_tick(int state) {
	static int i;

	switch (state) {
		case SM_PB_Init:
			state = SM_PB_Wait;
			break;
		case SM_PB_Wait:
			if (tempo != 0) {
				state = SM_PB_SpeakerOn;
			} else {
				state = SM_PB_Wait;
			}
			break;
		case SM_PB_SpeakerOn:
			state = SM_PB_SpeakerOff;
			break;
		case SM_PB_SpeakerOff:
			if (i >= t_off) {
				state = SM_PB_SpeakerOn;
			} else {
				state = SM_PB_SpeakerOff;
			}
			break;
		default:
			state = SM_PB_Init;
			break;
	}

	switch (state) {
		case SM_PB_SpeakerOn:
			HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_SET);
			update_interval();
			i = 0;
			break;
		case SM_PB_SpeakerOff:
			HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_RESET);
			i += TASK_PERIOD;
			break;
		default:
			break;
	}

	return state;
}


static inline void update_interval() {
	if (tempo != last_tempo) {
		t_off = 60000L / tempo / 2;
		last_tempo = tempo;
	}
}
