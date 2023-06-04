/*
 * scheduler.h
 *
 *  Created on: May 31, 2023
 *      Author: eshim009
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdbool.h>


/**
 * @property state The current state of the task's state machine.
 * @property period The period in milliseconds of the task.
 * @property t_waiting The amount of time the task has waited since its last
 * execution.
 * @property tick_fn The function to execute on each tick of the task state
 * machine.
 */
typedef struct {
	int state;
	uint_fast16_t period;
	uint_fast16_t t_waiting;
	void (*tick_fn)(void);
} Task;

extern const uint_fast16_t SCHEDULER_QUANTUM;

/**
 * Execute one iteration of the scheduler. This function should be run once
 * every scheduler quantum.
 */
void scheduler_tick();
/**
 * @param task The task to add to the scheduler.
 * @retval true if the task was successfully added, false if there is not enough
 * room in the task list.
 */
//bool add_task(Task* task);


#endif /* SRC_SCHEDULER_H_ */
