#include "udp_server.h"
int cmd_fill(uint8_t* data) {
    CommandFill* cmd = (void*)(data);

    if(xSemaphoreTake(mutex, portMAX_DELAY)) {
        display.changed = true;
        bitmap_fill(cmd->red, cmd->green, cmd->blue);
        xSemaphoreGive(mutex);
    }

    return 0;
}

int cmd_set_row(uint8_t command, uint8_t* data) {
    uint8_t res = 0;
    CommandSetRow* cmd = (void*)data;

    if(xSemaphoreTake(mutex, portMAX_DELAY)) {
        display.changed = true;
        switch(command) {
            case CMD_SET_ROW:;
                ESP_LOGI("UDP", "compressed Row: %i\n", cmd->row);
                res = bitmap_set_row(cmd->row, (data + sizeof(CommandSetRow)));
                break;
            case CMD_SET_ROW_UCMP:;
                ESP_LOGI("UDP", "uncompressed Row: %i\n", cmd->row);
                res = bitmap_set_row_uncompressed(cmd->row, (data + sizeof(CommandSetRow)));
                break;
            case CMD_SET_ROW_UCMP_NORM:;
                ESP_LOGI("UDP", "normalized uncompressed Row: %i\n", cmd->row);
                res = bitmap_set_row_uncompressed_normalized(cmd->row, (data + sizeof(CommandSetRow)));
                break;
        }
        xSemaphoreGive(mutex);
    }

    return res;
}

int cmd_set_pix_norm(uint8_t* data) {
    uint8_t res = 0;
    CommandSetPixel* cmd = (void*)data;

    if(xSemaphoreTake(mutex, portMAX_DELAY)) {
        display.changed = true;
        res = bitmap_set_pix_normalized(cmd->row, cmd->col, cmd->byte0, cmd->byte1);
        xSemaphoreGive(mutex);
    }

    return res;
}

int cmd_set_pix(uint8_t* data) {
    uint8_t res = 0;
    CommandSetPixel* cmd = (void*)data;

    if(xSemaphoreTake(mutex, portMAX_DELAY)) {
        display.changed = true;
        res = bitmap_set_pix(cmd->row, cmd->col, cmd->red, cmd->green, cmd->blue);
        xSemaphoreGive(mutex);
    }

    return res;
}

uint8_t server_fsm(uint8_t* data) {
    uint8_t res = 0;
    switch (data[0])
    {
    case CMD_CLEAR:
        ESP_LOGI("UDP", "Received clear command\n");
        display_reset_sw();
        return data[0];
        break;
        
    case CMD_FILL:;
        ESP_LOGI("UDP", "Received fill command\n");
        cmd_fill((data + 1));
        return data[0];
        break;

    case CMD_FILL_RECT:;
        ESP_LOGI("UDP", "Received fill rect command\n");
        return data[0];
        break;

    case CMD_SET_PIX:;
        cmd_set_pix((data + 1));
        ESP_LOGI("UDP", "Received set pix command\n");
        return data[0];
        break;

    case CMD_SET_PIX_NORM:;
        cmd_set_pix_norm((data + 1));
        ESP_LOGI("UDP", "Received set pix command\n");
        return data[0];
        break;


    case CMD_SET_ROW_UCMP:;
    case CMD_SET_ROW_UCMP_NORM:;
    case CMD_SET_ROW:;
        ESP_LOGI("UDP", "Received set row command\n");
        res = cmd_set_row(data[0], (data + 1));

        if(res == 0) {
            return data[0];
        }
        break;
    }

    ESP_LOGE("UDP", "Received unknown command\n");
    return CMD_UNKNOWN;
}

void udp_server(void) {
    esp_task_wdt_add(NULL);
    
    esp_err_t status = esp_task_wdt_status(NULL); // Check for the current task
    if (status == ESP_ERR_NOT_FOUND) {
        ESP_LOGE("TASK_WDT", "Task not registered with the watchdog!\n");
    } else {
        ESP_LOGI("TASK_WDT", "Task was correctly registered with the watchdog!\n");
    }

    ESP_LOGI("UDP Server", "Server is staring\n");
    int sock;
    struct sockaddr_in server_addr, client_addr;
    uint8_t buffer[TFT_WIDTH * 3 + 10];
    socklen_t addr_len = sizeof(client_addr);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1234);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // set timeout for watchdog resseting
    struct timeval timeout;
    timeout.tv_sec = 2; // 5 seconds
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    uint8_t cmd = CMD_UNKNOWN;
    const uint8_t CMD_CONF = CMD_CONFIRM; 

    ESP_LOGI("UDP Server", "Server is waiting for packets\n");
    while (1) {
        int len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (len > 0) {
            cmd = server_fsm(buffer);

            if(cmd == CMD_UNKNOWN) {
                esp_task_wdt_reset(); // reset the watchdog
                continue;
            }

            ESP_LOGI("UDP Server", "Sending confirm to client: %s\n", inet_ntoa(client_addr.sin_addr));
            sendto(sock, &CMD_CONF, sizeof(CMD_CONF), 0, (struct sockaddr *)&client_addr, addr_len);

        }
        esp_task_wdt_reset(); // reset the watchdog
    }
}

