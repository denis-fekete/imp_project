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

void app_main() {
    printf("\n\nESP32 started\n\n");

    setup();
    initDisplay();
   
    printf("\n\nInitial setup completed\n\n");

    memset(display.data, 0x00, sizeof(display.data));

    for(unsigned i = 0 ; i < 10; i++) {

        unsigned index = 0;
        for(unsigned y = 0; y < TFT_HEIGHT; y++) {
            for(unsigned x = 0; x < TFT_HEIGHT; x++) {
                index = (y * TFT_WIDTH + x) * 2;

                display.data[index] += RGB(25, 25, 25);
                display.data[index+1] += RGB(25, 25, 25);
            }
        }

        drawBitmap(display.data);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
    resetDisplay();
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
