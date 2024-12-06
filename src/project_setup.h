#ifndef PROJECT_SETUP
#define PROJECT_SETUP

#define TFT_MOSI  23 // master out, data for spi
#define TFT_SCLK  18 // spi clock

#define TFT_CS    5  // Chip select control pin D8
#define TFT_DC    27  // Data Command control pin
#define TFT_RST   17  // Reset pin (could connect to NodeMCU RST, see next line)


/**
 * @brief Display dimensions 
 * There seems to be more pixels than 128x128, use slightly different values 
 * based on experimenting that do not leave undefined rows
 */
#define TFT_HEIGHT 129  
#define TFT_WIDTH  131
// #define TFT_HEIGHT 128
// #define TFT_WIDTH  128

/**
 * @brief Size of bitmap that will represent display, 2 Bytes will be used 
 * per pixel using 16bit RGB565 configuration
 */
#define BITMAP_SIZE TFT_WIDTH * TFT_HEIGHT * 2

typedef struct display_bitmap {
    bool changed;
    uint8_t data8b[BITMAP_SIZE];
} DisplayBitmap;

typedef struct rgb_t {
    uint8_t red;
    uint8_t blue;
    uint8_t green;
} RGB;;

#endif /*PROJECT_SETUP*/