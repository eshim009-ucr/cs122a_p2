/*
 * ssd1306.h
 *
 *  Created on: Jun 5, 2023
 *      Author: eshim009
 */

#ifndef SRC_SSD1306_H_
#define SRC_SSD1306_H_
#include <stdint.h>
#include <stdbool.h>


// Datasheet URL: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

/**
 * From Datasheet:
 * Contrast increases as the value increases
 * (RESET = 7Fh)
 */
void ssd1306_set_contrast(uint8_t contrast);
/**
 * From Datasheet:
 * Entire display ON
 * Output ignores RAM content
 */
void ssd1306_all_on(bool all_on);
/**
 * From Datasheet:
 * Inverse display
 * 0 in RAM: ON in display panel
 * 1 in RAM: OFF in display pane
 */
void ssd1306_set_inverted(bool inverted);
/**
 * From Datasheet:
 * Display OFF (sleep mode)
 * (RESET)
 */
void ssd1306_set_sleep_mode(bool sleep);


#endif /* SRC_SSD1306_H_ */
