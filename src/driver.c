#include "driver.h"

void sendCommand(uint8_t command) {
    gpio_set_level(TFT_DC, 0); // Command mode
    spi_transaction_t t = {
        .length = 8,  // Command is 1 byte
        .tx_buffer = &command,
    };
    spi_device_transmit(spi, &t);
}

void sendData(uint8_t data) {
    gpio_set_level(TFT_DC, 1); // Data mode
    spi_transaction_t t = {
        .length = 8,  // Data is 1 byte
        .tx_buffer = &data,
    };
    spi_device_transmit(spi, &t);
}

void sendDataBuffer(const uint8_t *data, size_t size) {
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


void initDisplay() {
    sendCommand(CMD_SWRESET); // Software reset
    vTaskDelay(pdMS_TO_TICKS(150));
    sendCommand(CMD_SLPOUT);  // Exit sleep mode
    vTaskDelay(pdMS_TO_TICKS(150));

    // 14.2.33 0101 16 bit/pixel
    sendCommand(CMD_COLMOD);  // Set color mode
    sendData(0x05);           // 16-bit color (RGB565)

    sendCommand(CMD_MADCTL);  // Memory Access Control
    sendData(0x00);           // Default orientation

    sendCommand(CMD_DISPON);  // Turn on display
    vTaskDelay(pdMS_TO_TICKS(100));
}

void resetDisplay() {
    gpio_set_level(TFT_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(TFT_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));
}

void drawBitmap(const uint8_t *bitmap) {
    // Page 117 describes flow of these actions

    sendCommand(CMD_CASET); // setting columns to which will be written
    sendData(0x00); // High byte starting column 
    sendData(0x00); // Low byte starting column
    sendData(0x00); // Hight byte end column
    sendData(TFT_WIDTH - 1); // Low byte end column

    sendCommand(CMD_RASET); // setting rows to which will be written
    sendData(0x00); // High byte staring row
    sendData(0x00); // Low byte staring row
    sendData(0x00); // High byte ending row
    sendData(TFT_HEIGHT - 1); // Low byte ending row

    sendCommand(CMD_RAMWR); // write data
    sendDataBuffer(bitmap, TFT_WIDTH * TFT_HEIGHT * 2); // 2 bytes per pixel
}
