/**
 * @file project_setup.h
 * @author Denis Fekete (xfeket01@vutbr.cz)
 * @brief 
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PROJECT_SETUP
#define PROJECT_SETUP

// SPI Master Out, slave in - connected to GPIO23
#define TFT_MOSI  23
// SPI clock - connected to GPIO18
#define TFT_SCLK  18
// SPI Chip select - for selecting correct SPI device - connected to GPIO 5
#define TFT_CS    5
// Data Command - for selecting between data or commands - connected to GPIO27
#define TFT_DC    27
// Hardware reset for display - connected to GPIO07
#define TFT_RST   17


/**
 * @brief Display dimensions 
 * There seems to be more pixels than 128x128, use slightly different values 
 * based on experimenting that do not leave undefined rows
 */
#define TFT_HEIGHT 129  
/**
 * @brief Display dimensions 
 * There seems to be more pixels than 128x128, use slightly different values 
 * based on experimenting that do not leave undefined rows
 */
#define TFT_WIDTH  131
// #define TFT_HEIGHT 128
// #define TFT_WIDTH  128

/**
 * @brief Size of bitmap that will represent display, 2 Bytes will be used 
 * per pixel using 16bit RGB565 configuration
 */
#define BITMAP_SIZE TFT_WIDTH * TFT_HEIGHT * 2

/**
 * @brief Display bitmap structure with data and changed which will trigger redrawing of display
 * 
 */
typedef struct display_bitmap {
    bool changed;
    uint8_t data8b[BITMAP_SIZE];
} DisplayBitmap;

/**
 * @brief Structure for easier returning RGB values from function
 */
typedef struct rgb_t {
    uint8_t red;
    uint8_t blue;
    uint8_t green;
} RGB;;

#endif /*PROJECT_SETUP*/