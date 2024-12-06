#ifndef UDP_SERVER
#define UDP_SERVER

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "esp_task_wdt.h"

#include "project_setup.h"
#include "display_task.h"

extern SemaphoreHandle_t mutex;
extern DisplayBitmap display;

void udp_server(void);

#define CMD_CONFIRM 0x00

#define CMD_CLEAR 0x10
#define CMD_FILL 0x11
#define CMD_FILL_RECT 0x12

#define CMD_SET_PIX 0x20
#define CMD_SET_ROW_UCMP 0x21 // ROW LEN [DATA] - non compressed version
#define CMD_SET_ROW_UCMP_NORM 0x22 // ROW LEN [DATA] - non compressed normalized
#define CMD_SET_ROW 0x23 // ROW LEN [DATA]

#define CMD_UNKNOWN 0xEE


typedef struct cmd_fill_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} CommandFill;

typedef struct cmd_set_row_t {
    uint8_t row;
} CommandSetRow;

#endif /*UDP_SERVER*/