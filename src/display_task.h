#ifndef DISPLAY_TASK
#define DISPLAY_TASK

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

#include "project_setup.h"
#include "driver.h"

extern SemaphoreHandle_t mutex;
extern DisplayBitmap display;

#define min(a, b) ((a < b) ? a : b)

/**
 * @brief Main function of display task, calls setup and enters into loop
 */
void display_task(void);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
void bitmap_set(uint8_t x, uint8_t y, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
void bitmap_set_index(unsigned index, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
uint8_t bitmap_set_row(uint8_t row, uint8_t* data);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
uint8_t bitmap_set_row_uncompressed(uint8_t row, uint8_t* data);

/**
 * Source: how to send correct bits page 41/200, order is low
 */
uint8_t bitmap_set_row_uncompressed_normalized(uint8_t row, uint8_t* data);

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
void bitmap_fill(uint8_t red, uint8_t green, uint8_t blue);

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