/*
 * ssd1306.c
 *
 *  Created on: Jun 5, 2023
 *      Author: eshim009
 */
#include <stdint.h>
#include <stdbool.h>
#include "i2c.h"
#include "ssd1306.h"


static const uint16_t ADDRESS = 0x3C << 1;


void ssd1306_set_contrast(uint8_t contrast) {
	uint8_t data[] = {0x81, contrast};
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, data, 2, 1000);
}

void ssd1306_all_on(bool all_on) {
	uint8_t data = all_on ? 0xA5 : 0xA4;
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, &data, 1, 1000);
}

void ssd1306_set_inverted(bool inverted) {
	uint8_t data = inverted ? 0xA7 : 0xA6;
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, &data, 1, 1000);
}

void ssd1306_set_sleep_mode(bool sleep) {
	uint8_t data = sleep ? 0xAE : 0xAF;
	HAL_I2C_Master_Transmit(&hi2c1, ADDRESS, &data, 1, 1000);
}
