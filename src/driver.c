#include "driver.h"

void send_cmd(uint8_t command) {
    gpio_set_level(TFT_DC, 0); // Command mode
    spi_transaction_t t = {
        .length = 8,  // Command is 1 byte
        .tx_buffer = &command,
    };
    spi_device_transmit(spi, &t);
}

void send_data(uint8_t data) {
    gpio_set_level(TFT_DC, 1); // Data mode
    spi_transaction_t t = {
        .length = 8,  // Data is 1 byte
        .tx_buffer = &data,
    };
    spi_device_transmit(spi, &t);
}

void send_data_buffer(const uint8_t *data, size_t size) {
    gpio_set_level(TFT_DC, 1); // Data mode
    spi_transaction_t t = {
        .length = size * 8,  // `size` bytes to transmit (multiply by 8 for bits)
        .tx_buffer = data,
    };
    esp_err_t ret = spi_device_transmit(spi, &t); // Transmit the data
    if (ret != ESP_OK) {
        printf("SPI transmission failed: %s\n", esp_err_to_name(ret));
    }
}

void display_draw(const uint8_t *bitmap) {
    // Page 117 describes flow of these actions

    send_cmd(CMD_CASET); // setting columns to which will be written
    send_data(0x00); // High byte starting column 
    send_data(0x00); // Low byte starting column
    send_data(0x00); // Hight byte end column
    send_data(TFT_WIDTH - 1); // Low byte end column

    send_cmd(CMD_RASET); // setting rows to which will be written
    send_data(0x00); // High byte staring row
    send_data(0x00); // Low byte staring row
    send_data(0x00); // High byte ending row
    send_data(TFT_HEIGHT - 1); // Low byte ending row

    send_cmd(CMD_RAMWR); // write data
    send_data_buffer(bitmap, BITMAP_SIZE); // *2 because conversion from 16bit to 8bit array is done 
}


void display_init() {
    send_cmd(CMD_SWRESET); // Software reset
    vTaskDelay(pdMS_TO_TICKS(150));
    send_cmd(CMD_SLPOUT);  // Exit sleep mode
    vTaskDelay(pdMS_TO_TICKS(150));

    // setting default color mode of display (14.2.33) 0101  == 16 bit/pixel
    send_cmd(CMD_COLMOD);  // Set color mode
    send_data(0x05);           // 16-bit color (RGB565)

    // setting display orientation
    send_cmd(CMD_MADCTL);  // Memory Access Control
    send_data(0x00);           // Default orientation

    send_cmd(CMD_DISPON);  // Turn on display
    vTaskDelay(pdMS_TO_TICKS(100));
}

void display_reset_sw() {
    send_cmd(CMD_SWRESET); // Software reset
    vTaskDelay(pdMS_TO_TICKS(150));
}

void display_reset_hw() {
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(TFT_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));
}