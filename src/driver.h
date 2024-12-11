/**
 * @file driver.h
 * @author Denis Fekete (xfeket01@vutbr.cz)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef DISPLAY_DRIVER
#define DISPLAY_DRIVER

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "project_setup.h"

#define CMD_SWRESET 0x01 /*Software reset 14.2.1*/
#define CMD_SLPOUT  0x11 /*Sleep out 14.2.12*/
#define CMD_DISPOFF  0x28 /*Display off 14.2.18*/
#define CMD_DISPON  0x29 /*Display onn 14.2.19*/
#define CMD_CASET   0x2A /*Column address set 14.2.20*/
#define CMD_RASET   0x2B /*Page Address Set 14.2.21*/
#define CMD_RAMWR   0x2C /*Memory Write 14.2.22*/
#define CMD_MADCTL  0x36 /*Memory access control 14.2.29*/
#define CMD_COLMOD  0x3A /*Interface Pixel Format 14.2.33*/

/**
 * @brief Sends command code to display
 * 
 * Sets DC pin to command writing mode then sends command over SPI
 * 
 * @param command command to be sent
 */
void send_cmd(uint8_t command);

/**
 * @brief Sends data to display
 * 
 * Sets DC pin to data writing mode then sends data over SPI
 * 
 * @param data data to be sent
 */
void send_data(uint8_t data);

/**
 * @brief Sends buffer of data to display
 * 
 * Sets DC pin to data writing mode then sends data over SPI
 * 
 * @param data data to be sent
 * @param size size of data buffer, in bytes
 */
void send_data_buffer(const uint8_t *data, size_t size);

/**
 * @brief Initializes display, sets correct color mode, memory access more, etc.
 */
void display_init();

/**
 * @brief Draws whole bitmap buffer into the display
 * @param bitmap buffer to containing values that will be written to display, 
 * array of compressed rgb values in form of RGB565 in order
 */
void display_draw(const uint8_t *bitmap);

/**
 * @brief Software resets display
 */
void display_reset_sw();

/**
 * @brief Hardware resets display
 * 
 */
void display_reset_hw();

extern spi_device_handle_t spi;

#endif /*DISPLAY_DRIVER*/