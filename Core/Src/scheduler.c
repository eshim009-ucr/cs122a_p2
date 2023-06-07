/*
 * scheduler.c
 *
 *  Created on: May 31, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "sm_show_number.h"
#include "sm_tap_tempo.h"
#include "sm_change_tempo.h"
#include "sm_playback.h"
#include "sm_rhythm.h"
#include "scheduler.h"


const uint_fast16_t SCHEDULER_QUANTUM = 5;
static Task* tasks[] = {
	&task_show_number,
	&task_tap_tempo,
	&task_change_tempo,
	&task_playback,
	&task_rhythm,
	0
};


void scheduler_tick() {
	for (Task** task_ptr = tasks; *task_ptr != 0; task_ptr++) {
		Task* task = *task_ptr;

		if (task->t_waiting >= task->period) {
			task->state = task->tick_fn(task->state);
			task->t_waiting = 0;
		}
		task->t_waiting += SCHEDULER_QUANTUM;
	}
}
