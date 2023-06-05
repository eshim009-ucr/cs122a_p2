/*
 * sm_display.c
 *
 *  Created on: Jun 5, 2023
 *      Author: eshim009
 */
#include <stdbool.h>
#include "ssd1306.h"
#include "sm_display.h"


enum sm_dp_state {
	SM_DP_Start,
	SM_DP_Init,
	SM_DP_On,
	SM_DP_Off
};


int sm_dp_tick(int state);


Task task_display = {
	.state = SM_DP_Start,
	.period = 500,
	.t_waiting = 0,
	.tick_fn = sm_dp_tick
};


int sm_dp_tick(int state) {
	switch (state) {
		case SM_DP_Start:
			state = SM_DP_Init;
			break;
		case SM_DP_Init:
			state = SM_DP_On;
			break;
		case SM_DP_On:
			state = SM_DP_Off;
			break;
		case SM_DP_Off:
			state = SM_DP_On;
			break;
		default:
			state = SM_DP_Start;
			break;
	}

	switch (state) {
		case SM_DP_Init:
			ssd1306_set_sleep_mode(false);
			break;
		case SM_DP_On:
			ssd1306_all_on(true);
			break;
		case SM_DP_Off:
			ssd1306_all_on(false);
			break;
		default:
			break;
	}

	return state;
}
