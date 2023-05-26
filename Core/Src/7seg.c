/*
 * 7seg.c
 *
 *  Created on: May 22, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "7seg.h"
#include "main.h"


void writeDigit(uint_fast8_t digit) {
	HAL_GPIO_WritePin(seg7b3_GPIO_Port, seg7b3_Pin, digit & 0x08);
	HAL_GPIO_WritePin(seg7b2_GPIO_Port, seg7b2_Pin, digit & 0x04);
	HAL_GPIO_WritePin(seg7b1_GPIO_Port, seg7b1_Pin, digit & 0x02);
	HAL_GPIO_WritePin(seg7b0_GPIO_Port, seg7b0_Pin, digit & 0x01);
}
