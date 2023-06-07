/*
 * sm_rhythm.c
 *
 *  Created on: Jun 6, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "main.h"
#include "sm_rhythm.h"


const int8_t RHYTHMS[3][16] = {
	// Straight Rhythm
	{QUARTER, QUARTER, 0},
	// Star Wars "Imperial March" Theme
	{QUARTER, QUARTER, QUARTER, EIGHTH,  EIGHTH, QUARTER, EIGHTH,  EIGHTH, QUARTER, 0},
	// 5/4 "Mission Impossible Clave"
	{DOTTED_QUARTER, DOTTED_QUARTER, QUARTER, QUARTER, 0}
};


int8_t const* current_rhythm = RHYTHMS[0];


enum sm_rh_state {
	SM_RH_Init,
	SM_RH_Straight,
	SM_RH_Imperial,
	SM_RH_Clave
};


int sm_rh_tick(int state);


Task task_rhythm = {
	.state = SM_RH_Init,
	.period = 100,
	.t_waiting = 0,
	.tick_fn = sm_rh_tick
};


int sm_rh_tick(int state) {
	switch (state) {
		case SM_RH_Init:
			state = SM_RH_Straight;
			break;
		case SM_RH_Straight:
			if (HAL_GPIO_ReadPin(btnrhy_GPIO_Port, btnrhy_Pin) == GPIO_PIN_RESET) {
				state = SM_RH_Imperial;
			} else {
				state = SM_RH_Straight;
			}
			break;
		case SM_RH_Imperial:
			if (HAL_GPIO_ReadPin(btnrhy_GPIO_Port, btnrhy_Pin) == GPIO_PIN_RESET) {
				state = SM_RH_Clave;
			} else {
				state = SM_RH_Imperial;
			}
			break;
		case SM_RH_Clave:
			if (HAL_GPIO_ReadPin(btnrhy_GPIO_Port, btnrhy_Pin) == GPIO_PIN_RESET) {
				state = SM_RH_Straight;
			} else {
				state = SM_RH_Clave;
			}
			break;
		default:
			state = SM_RH_Init;
			break;
	}

	switch (state) {
		case SM_RH_Straight:
			current_rhythm = RHYTHMS[0];
			break;
		case SM_RH_Imperial:
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_SET);
			current_rhythm = RHYTHMS[1];
			break;
		case SM_RH_Clave:
			HAL_GPIO_WritePin(ledboard_GPIO_Port, ledboard_Pin, GPIO_PIN_RESET);
			current_rhythm = RHYTHMS[2];
			break;
		default:
			break;
	}

	return state;
}
