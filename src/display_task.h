/**
 * @file display_task.h
 * @author Denis Fekete (xfeket01@vutbr.cz)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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
 * @brief Sets RGB values to specific index in bitmap
 * 
 * @param index index of pixel to be changed
 * @param red new red value
 * @param green new green value
 * @param blue new blue value
 */
void bitmap_set_index(unsigned index, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Sets pixel values at given row
 * 
 * @param row row that will be changed
 * @param data data buffer containing compressed and normalized RGB565 values to be set 
 */
void bitmap_set_row(uint8_t row, uint8_t* data);

/**
 * @brief Sets pixel values at given row
 * 
 * @param row row that will be changed
 * @param data data buffer containing RGB values, 1 byte per color == 3 bytes per pixel
 */
void bitmap_set_row_uncompressed(uint8_t row, uint8_t* data);

/**
 * @brief Sets pixel values at given row
 * 
 * @param row row that will be changed
 * @param data data buffer containing RGB values, 1 byte per color == 3 bytes 
 * per pixel. Values must be normalized to RGB565 maximum values (31 for red 
 * and blue, 63 for green)
 */
void bitmap_set_row_uncompressed_normalized(uint8_t row, uint8_t* data);

/**
 * @brief Sets RGB values to specific pixel at given y(row) and x(col)
 * 
 * @param x column of pixel that will be changed
 * @param y row of pixel that will be changed
 * @param red new red value
 * @param green new green value
 * @param blue new blue value
 */
void bitmap_set_pix(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Sets RGB values to specific pixel at given y(row) and x(col), values must be normalized and in RGB565 format
 * 
 * @param row row of pixel that will be changed
 * @param col column of pixel that will be changed
 * @param byte0 byte containing 5 bits of Red, and more significant 3 bits from Green
 * @param byte1 byte containing 5 bits of Blue, and less significant 3 bits from Green
 */
void bitmap_set_pix_normalized(uint8_t row, uint8_t col, uint8_t byte0, uint8_t byte1);

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