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

enum sm_ct_state {
	SM_PB_Init,
	SM_PB_Wait,
	SM_PB_SpeakerOn,
	SM_PB_SpeakerOff
};


int sm_pb_tick(int state);


Task task_playback = {
	.state = SM_PB_Init,
	.period = 500,
	.t_waiting = 0,
	.tick_fn = sm_pb_tick
};


int sm_pb_tick(int state) {
	switch (state) {
		case SM_PB_Init:
			state = SM_PB_Wait;
			break;
		case SM_PB_Wait:
//			if (tempo != 0) {
				state = SM_PB_SpeakerOn;
//			} else {
//				state = SM_PB_Wait;
//			}
			break;
		case SM_PB_SpeakerOn:
			state = SM_PB_SpeakerOff;
			break;
		case SM_PB_SpeakerOff:
			state = SM_PB_SpeakerOn;
			break;
		default:
			state = SM_PB_Init;
			break;
	}

	switch (state) {
		case SM_PB_SpeakerOn:
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_SET);
			HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
			break;
		case SM_PB_SpeakerOff:
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_RESET);
			HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
			break;
		default:
			break;
	}

	return state;
}



