#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <string.h>
#include "stdio.h"

#include "project_setup.h"
#include "driver.h"

// Display Bitmap declared as global and static to be accessible through whole 
// program and keep it in memory until end of program
static DisplayBitmap display;

spi_device_handle_t spi;

void setup();
void setRGB(unsigned index, uint8_t red, uint8_t green, uint8_t blue);
RGB getRGB(unsigned index);
void setBitmap(uint8_t red, uint8_t green, uint8_t blue);

void app_main() {
    printf("\n\nESP32 started\n\n");

    setup();
    initDisplay();
   
    printf("\n\nInitial setup completed\n\n");

    memset(display.data8b, 0x00, sizeof(display.data8b));
    drawBitmap(display.data8b);

}

void setup() {
    // Setup the IO pins
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << TFT_DC) | (1ULL << TFT_RST) | (1ULL << TFT_CS),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_config);

    resetDisplay();

    // Configure the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = TFT_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = TFT_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = TFT_WIDTH * TFT_HEIGHT * 2 + 8,
    };
    spi_bus_initialize(HSPI_HOST, &buscfg, SPI_DMA_CH_AUTO);

    // Attach the TFT device to the SPI bus
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10 * 1000 * 1000, // 10 MHz
        .mode = 0,                          // SPI mode 0
        .spics_io_num = TFT_CS,
        .queue_size = 1,
    };
    spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
}

void setBitmap(uint8_t red, uint8_t green, uint8_t blue) {
    for(unsigned i = 0; i  < BITMAP_SIZE; i+=2) {
        setRGB(i, red, green, blue);
    }
}

/**
 * @brief 
 * 
 * @param index 
 * @param red 
 * @param green 
 * @param blue 
 * 
 * Source: how to send correct bits page 41/200, order is low
 */
void setRGB(unsigned index, uint8_t red, uint8_t green, uint8_t blue) {
    display.data8b[index] = (blue << 3) | (green >> 3);
    display.data8b[index + 1] = ((green & 0x07) << 5) | (red & 0x1F);
}


RGB getRGB(unsigned index) {
    uint8_t red =  (display.data8b[index + 1] & 0x1F); ;
    uint8_t green = (display.data8b[index] << 5) | (display.data8b[index + 1] >> 5); 
    uint8_t blue = display.data8b[index] >> 3;

    RGB tmp = {.red = red, .green = green, .blue = blue};
    return tmp;
}
