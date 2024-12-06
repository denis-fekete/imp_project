#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#include "display_task.h"
#include "wifi_task.h"


SemaphoreHandle_t mutex;

// Display Bitmap declared as global and static to be accessible through whole 
// program and keep it in memory until end of program
DisplayBitmap display;

static const char *TAG = "App main";

void display_core(void *arg) {
    display_task();
}

void wifi_core(void *arg) {
    wifi_task();

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main() {
    memset(display.data8b, 0, BITMAP_SIZE);
    display.changed = true; 

    mutex = xSemaphoreCreateMutex();

    if(mutex == NULL) {
        ESP_LOGI(TAG, "Failed to initialize mutex\n");
    }
    
    // Display task on Core 1
    xTaskCreatePinnedToCore(display_core, "Display Task", 4096, NULL, 5, NULL, 1);

    // Wi-Fi task on Core 0
    xTaskCreatePinnedToCore(wifi_core, "WiFi Task", 4096, NULL, 5, NULL, 0);
}
