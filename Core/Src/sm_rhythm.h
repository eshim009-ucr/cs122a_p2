/*
 * sm_rhythm.h
 *
 *  Created on: Jun 6, 2023
 *      Author: eshim009
 */

#ifndef SRC_SM_RHYTHM_H_
#define SRC_SM_RHYTHM_H_
#include <stdint.h>
#include "scheduler.h"


#define WHOLE 1
#define HALF 2
#define DOTTED_QUARTER 3
#define QUARTER 4
#define DOTTED_EIGHTH 6
#define EIGHTH 8


extern Task task_rhythm;
extern int8_t const* current_rhythm;


#endif /* SRC_SM_RHYTHM_H_ */
