/*
 * scheduler.c
 *
 *  Created on: May 31, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "sm_show_number.h"
#include "scheduler.h"


const uint_fast16_t SCHEDULER_QUANTUM = 5;
const uint_fast8_t NUM_TASKS = 1;
static Task* tasks[] = {&task_show_number};


void scheduler_tick() {
	for (int i = 0; i < NUM_TASKS; ++i) {
		if (tasks[i]->t_waiting >= tasks[i]->period) {
			tasks[i]->tick_fn();
			tasks[i]->t_waiting = 0;
		}
		tasks[i]->t_waiting += SCHEDULER_QUANTUM;
	}
}
