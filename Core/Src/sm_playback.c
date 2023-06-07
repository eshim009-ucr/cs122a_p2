/*
 * sm_playback.c
 *
 *  Created on: Jun 6, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "scheduler.h"
#include "sm_tap_tempo.h"
#include "sm_rhythm.h"
#include "sm_playback.h"
#include "tim.h"


static const uint_fast8_t TASK_PERIOD = 50;

// Used to check if interval should be updated
static uint_fast16_t last_tempo = 0;
static int8_t const* last_rhythm = 0;
// The time between metronome beeps
// Equivalent to a quarter note
static uint_fast16_t t_off = 0;
// Used to determine practical note delays
static uint_fast16_t note_len_lut[9];
// Track where we are in the rhyhtm
static uint_fast8_t i_rhythm = 0;


enum sm_ct_state {
	SM_PB_Init,
	SM_PB_Wait,
	SM_PB_SpeakerOn,
	SM_PB_SpeakerOff
};


int sm_pb_tick(int state);
static inline void update_interval(void);
static inline void update_rhyhtm(void);


Task task_playback = {
	.state = SM_PB_Init,
	.period = TASK_PERIOD,
	.t_waiting = 0,
	.tick_fn = sm_pb_tick
};


int sm_pb_tick(int state) {
	static int i_note;

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
			if (i_note >= note_len_lut[current_rhythm[i_rhythm]]) {
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
			update_interval();
			i_note = 0;
			i_rhythm++;
			// Loop back around if necessary
			if (current_rhythm[i_rhythm] == 0) {
				i_rhythm = 0;
			}
			break;
		case SM_PB_SpeakerOff:
			HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
			i_note += TASK_PERIOD;
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
		update_rhyhtm();
	}

	if (current_rhythm != last_rhythm) {
		update_rhyhtm();
	}
}


static inline void update_rhyhtm() {
	note_len_lut[WHOLE] = t_off * 4;
	note_len_lut[HALF] = t_off * 2;
	note_len_lut[DOTTED_QUARTER] = t_off * 3 / 2;
	note_len_lut[QUARTER] = t_off;
	note_len_lut[DOTTED_EIGHTH] = t_off * 3 / 4;
	note_len_lut[EIGHTH] = t_off / 2;
	i_rhythm = 0;
	last_rhythm = current_rhythm;
}
