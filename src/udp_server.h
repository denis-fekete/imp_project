/**
 * @file udp_server.h
 * @author Denis Fekete (xfeket01@vutbr.cz)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef UDP_SERVER
#define UDP_SERVER

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "esp_task_wdt.h"
#include "project_setup.h"
#include "display_task.h"


#define CMD_CONFIRM 0x00 // confirmation of received message

#define CMD_CLEAR 0x10
#define CMD_FILL 0x11 // RED GREEN BLUE

// sets row with given values, [] is a array
#define CMD_SET_ROW_UCMP 0x20 // ROW [RED, GREEN, BLUE] - non compressed version
#define CMD_SET_ROW_UCMP_NORM 0x21 // ROW [RED, GREEN, BLUE] - non compressed normalized
#define CMD_SET_ROW 0x22 // ROW [COLOR_BYTE_0, COLOR_BYTE_1] - compressed and normalized

// sets a pixel and row and col
#define CMD_SET_PIX 0x30 // ROW COL RED GREEN BLUE - sets color to pixel
#define CMD_SET_PIX_NORM 0x31 // ROW COL BYTE0 BYTE1 - sets color to pixel, values must be normalized to RGB565 values

#define CMD_SET_PIX_VECTOR 0x40 // NUMBER_OF_PIXEL [ROW COL BYTE0 BYTE1]
#define CMD_UNKNOWN 0xEE


typedef struct cmd_fill_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} CommandFill;

typedef struct cmd_set_pix_t {
    uint8_t row;
    uint8_t col;
    union
    {
        uint8_t red;
        uint8_t byte0;
    };
    union
    {
        uint8_t green;
        uint8_t byte1;
    };
    
    uint8_t blue;
} CommandSetPixel;


typedef struct cmd_set_row_t {
    uint8_t row;
} CommandSetRow;

typedef struct cmd_pix_vector {
    uint8_t no_high;
    uint8_t no_low;
} CommandPixelVector;


extern SemaphoreHandle_t mutex;
extern DisplayBitmap display;

/**
 * @brief Executes set row command (variations based on command), takes mutex and
 * notifies display that changes were made
 * 
 * @param command command that will be executed
 * @param data sends data
 */
void cmd_set_row(uint8_t command, uint8_t* data);

/**
 * @brief Executes fill command takes mutex and
 * notifies display that changes were made
 * 
 * @param data data containing RGB values, 3 bytes total
 */
void cmd_fill(uint8_t* data);

/**
 * @brief Executes fill command takes mutex and
 * notifies display that changes were made
 * 
 * @param data data containing RGB values, 3 bytes total
 */
void cmd_set_pix(uint8_t* data);

/**
 * @brief Executes fill command takes mutex and
 * notifies display that changes were made
 * 
 * @param data data containing RGB565 values that are normalized
 */
void cmd_set_pix_norm(uint8_t* data);

/**
 * @brief Changes color of given pixels from data, values must be normalized and 
 * in RGB565 format
 * 
 * @param data data buffer
 * @param len length of received packet for pointer control
 */
void cmd_set_pix_vector(uint8_t* data, size_t len);

void udp_server(void);
#endif /*UDP_SERVER*/