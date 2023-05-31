/*
 * sm_show_number.c
 *
 *  Created on: May 25, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "main.h"
#include "7seg.h"
#include "sm_show_number.h"


// The number to display
// Testing variable, should be read from elsewhere
const uint_fast16_t number = 42;
// The number that is currently being displayed
// Temporarily declared as const for debugging
const uint_fast16_t hundreds = number / 100;
const uint_fast16_t tens = (number - hundreds) / 10;
const uint_fast16_t ones = number - hundreds - tens;


enum {
	SM_SN_Init,
	SM_SN_Hundreds,
	SM_SN_Tens,
	SM_SN_Ones
} sm_sn_state;


void sm_sn_tick() {
	switch (sm_sn_state) {
		case SM_SN_Init:
			sm_sn_state = SM_SN_Hundreds;
			break;
		case SM_SN_Hundreds:
			sm_sn_state = SM_SN_Tens;
			break;
		case SM_SN_Tens:
			sm_sn_state = SM_SN_Ones;
			break;
		case SM_SN_Ones:
			sm_sn_state = SM_SN_Hundreds;
			break;
		default:
			sm_sn_state = SM_SN_Init;
			break;
	}

	switch (sm_sn_state) {
		case SM_SN_Hundreds:
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 1);
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 0);
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 0);
			writeDigit((number % 1000) / 100);
			break;
		case SM_SN_Tens:
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 0);
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 1);
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 0);
			writeDigit((number % 100) / 10);
			break;
		case SM_SN_Ones:
			HAL_GPIO_WritePin(digen2_GPIO_Port, digen2_Pin, 0);
			HAL_GPIO_WritePin(digen1_GPIO_Port, digen1_Pin, 0);
			HAL_GPIO_WritePin(digen0_GPIO_Port, digen0_Pin, 1);
			writeDigit((number % 10) / 1);
			break;
	}
}