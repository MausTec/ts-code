#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tscode.h"

void process_buffer(char *buffer);

void app_main(void) {
    char buffer[120] = "";
    size_t idx = 0;

    for (;;) {
        char c = getc(stdin);

        if (c != 0xFF) {
            if (c == '\n') {
                if (buffer[0] != '\0') {
                    process_buffer(buffer);
                }

                buffer[0] = '\0';
                idx = 0;
            } else {
                buffer[idx++] = c;
                buffer[idx] = '\0';
            }
        }
    
        vTaskDelay(1);
    }
}

void process_buffer(char *buffer) {
    tscode_command_t cmd;

    char* ptr = NULL;
    char* saveptr = NULL;

    printf("Parsing: <<%s>>\n", buffer);
    ptr = tscode_parse_command(buffer, &cmd, &saveptr);

    while (ptr != NULL) {
        _tscode_print_command(&cmd);
        tscode_dispose_command(&cmd);
        printf("\n");

        ptr = tscode_parse_command(NULL, &cmd, &saveptr);
    }
}