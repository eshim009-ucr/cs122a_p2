/*
 * 7seg.c
 *
 *  Created on: May 22, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include "main.h"


void writeDigit(uint_fast8_t digit) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, digit & 0x08);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, digit & 0x04);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, digit & 0x02);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, digit & 0x01);
}
