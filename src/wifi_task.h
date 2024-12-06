#ifndef WIFI_TASK
#define WIFI_TASK

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "project_setup.h"
#include "udp_server.h"


void wifi_task(void);
#endif /*WIFI_TASK*/