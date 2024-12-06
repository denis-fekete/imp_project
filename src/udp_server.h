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

#define CMD_SET_ROW_UCMP 0x21 // ROW LEN [DATA] - non compressed version
#define CMD_SET_ROW_UCMP_NORM 0x22 // ROW LEN [DATA] - non compressed normalized
#define CMD_SET_ROW 0x23 // ROW LEN [DATA]
#define CMD_SET_PIX 0x25 // ROW COL Red Green Blue
#define CMD_SET_PIX_NORM 0x26 // ROW COL Red Green Blue

#define CMD_UNKNOWN 0xEE


typedef struct cmd_fill_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} CommandFill;

typedef struct cmd_set_pix_t {
    uint8_t row;
    uint8_t col;
    union
    {
        uint8_t red;
        uint8_t byte0;
    };
    union
    {
        uint8_t green;
        uint8_t byte1;
    };
    
    uint8_t blue;
} CommandSetPixel;


typedef struct cmd_set_row_t {
    uint8_t row;
} CommandSetRow;

int cmd_set_row(uint8_t command, uint8_t* data);
int cmd_fill(uint8_t* data);
int cmd_set_pix(uint8_t* data);
int cmd_set_pix_norm(uint8_t* data);

#endif /*UDP_SERVER*/