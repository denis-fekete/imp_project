#ifndef DISPLAY_TASK
#define DISPLAY_TASK

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include "stdio.h"

#include "project_setup.h"
#include "driver.h"

/**
 * @brief Main function of display task, calls setup and enters into loop
 */
void display_task(void);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
void setRGB(unsigned index, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Returns RGB values of given pixel
 * 
 * @param x x position of pixel
 * @param y y position of pixel
 * @return RGB struct with Red,Green and Blue values 
 */
RGB getRGB(unsigned x, unsigned y);

/**
 * @brief Fills whole bitmap with given color
 * 
 * @param red
 * @param green 
 * @param blue 
 */
void fillBitmap(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Setups display settings and SPI connection
 * 
 */
void setup(void);

/**
 * @brief Main loop of display task
 */
void loop(void);

#endif /*DISPLAY_TASK*/