#include "display_task.h"
#include "wifi_task.h"
#include "esp_http_server.h"

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
    // Display task on Core 1
    xTaskCreatePinnedToCore(display_core, "Display Task", 4096, NULL, 5, NULL, 1);

    // Wi-Fi task on Core 0
    xTaskCreatePinnedToCore(wifi_core, "WiFi Task", 4096, NULL, 5, NULL, 0);
}