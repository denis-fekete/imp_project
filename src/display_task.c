#include "display_task.h"

spi_device_handle_t spi;


void setup(void) {
    // Setup the IO pins
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << TFT_DC) | (1ULL << TFT_RST) | (1ULL << TFT_CS),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_config);

    display_reset_hw();

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

void bitmap_fill(uint8_t red, uint8_t green, uint8_t blue) {
    for(unsigned i = 0; i  < BITMAP_SIZE; i+=2) {
        bitmap_set_index(i, red, green, blue);
    }
}

void bitmap_set_index(unsigned index, uint8_t red, uint8_t green, uint8_t blue) {
    // normalize colors
    red = min((red >> 3), 31);
    green = min((green >> 2), 63);
    blue = min((blue >> 3), 31);
    
    // store them into bitmap
    display.data8b[index] = (blue << 3) | (green >> 3);
    display.data8b[index + 1] = ((green & 0x07) << 5) | (red & 0x1F);
}

void bitmap_set_row_uncompressed(uint8_t row, uint8_t* data) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    const unsigned index = min(row, TFT_HEIGHT) * TFT_WIDTH;
    const unsigned len = TFT_WIDTH * 3;

    unsigned col = 0;

    for(unsigned i = 0; i < len; i+=3, col++) {
        // normalize colors
        red = min((data[i] >> 3), 31);
        green = min((data[i+1] >> 2), 63);
        blue = min((data[i+2] >> 2), 31);

        // store them into bitmap
        display.data8b[(index + col) * 2] = (blue << 3) | (green >> 3);
        display.data8b[(index + col) * 2 + 1] = ((green & 0x07) << 5) | (red & 0x1F);
    }
}

void bitmap_set_row_uncompressed_normalized(uint8_t row, uint8_t* data) {
    const unsigned index = min(row, TFT_HEIGHT) * TFT_WIDTH;
    const unsigned len = TFT_WIDTH * 3;
    unsigned col = 0;
    
    uint8_t byte0, byte1;

    for(unsigned i = 0; i < len; i+=3, col++) {
        byte0 = (data[i+2] << 3) | (data[i+1] >> 3);
        byte1 = (uint8_t)((data[i+1] & 0x07) << 5) | (data[i] & 0x1F);

        // store them into bitmap
        display.data8b[(index + col) * 2] = byte0;
        display.data8b[(index + col) * 2 + 1] = byte1;
    }
}

void bitmap_set_row(uint8_t row, uint8_t* data) {
    const unsigned index = min(row, TFT_HEIGHT) * TFT_WIDTH * 2;

    for (unsigned i = 0; i < TFT_WIDTH; i++) {
        display.data8b[index + (i * 2)] = (uint8_t)data[i * 2];
        display.data8b[index + (i * 2) + 1] = (uint8_t)data[i * 2 + 1];
    }
}

void bitmap_set_pix(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b) {
    // normalize colors
    uint8_t red = min((r >> 3), 31);
    uint8_t green = min((g >> 2), 63);
    uint8_t blue = min((b >> 3), 31);
    
    const unsigned index = (row * TFT_WIDTH + col) * 2;

    // store them into bitmap
    display.data8b[index] = (blue << 3) | (green >> 3);
    display.data8b[index + 1] = ((green & 0x07) << 5) | (red & 0x1F);
}

void bitmap_set_pix_normalized(uint8_t row, uint8_t col, uint8_t byte0, uint8_t byte1) {    
    const unsigned index = (row * TFT_WIDTH + col) * 2;
    
    // store them into bitmap
    display.data8b[index] = byte0;
    display.data8b[index + 1] = byte1;
}

RGB getRGB(unsigned x, unsigned y) {
    const unsigned index = (y * TFT_WIDTH + x) * 2;

    uint8_t red =  (display.data8b[index + 1] & 0x1F); ;
    uint8_t green = (display.data8b[index] << 5) | (display.data8b[index + 1] >> 5); 
    uint8_t blue = display.data8b[index] >> 3;

    const RGB tmp = {.red = red, .green = green, .blue = blue};
    return tmp;
}

void display_task(void) {
    setup();
    display_init();
    loop();
}

void loop(void) {
    esp_task_wdt_add(NULL);

    for(unsigned i = 0; 1; i++) {
        if(xSemaphoreTake(mutex, pdMS_TO_TICKS(500)) == pdTRUE ) {
            if(display.changed) {
                display_draw(display.data8b);
            }
            display.changed = false;
            xSemaphoreGive(mutex);
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));
            esp_task_wdt_reset(); // reset the watchdog
        }

        vTaskDelay(pdMS_TO_TICKS(10));
        esp_task_wdt_reset(); // reset the watchdog
    }
}