#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "tscode_capabilities.h"
#include "tscode.h"

#define EXAMPLE_USE_CALLBACK_PROCESSING
#define EXAMPLE_USE_STREAM_PROCESSING

void process_buffer(char* buffer);
tscode_command_response_t cmd_callback(tscode_command_t* cmd, char* response, size_t resp_len);

/**
 * The example that follows is a verbose example of manually parsing a stream and handling the tokens as they're
 * found. You can, optionally, use `tscode_process_stream()` or `tscode_process_buffer()` to use callbacks, which
 * is also shown here. Check the defines above to switch methods.
 */

void app_main(void) {
#ifndef EXAMPLE_USE_STREAM_PROCESSING
    char buffer[120] = "";
    size_t idx = 0;
#endif

    // Register our test capabilities:
    tscode_cap_channel_t ch1 = TSCODE_CAP_CHANNEL_DEFAULT;
    ch1.capability = TSCODE_CAP_VIBRATING;
    ch1.range.unit = TSCODE_UNIT_BYTE;
    ch1.range.max = 255;

    tscode_register_channel(&ch1);

    // Recycle that object to register our message handler
    ch1.capability = TSCODE_CAP_MESSAGES;
    ch1.range.unit = TSCODE_UNIT_NONE;
    ch1.range.max = 40;

    tscode_register_channel(&ch1);

    tscode_device_vendor_details_t vendor = {
        .vendor = "Maus-Tec Electronics",
        .device = "TS-code Test Application",
        .version = "v0.1.9",
    };

    tscode_register_vendor_details(&vendor);

    // Loop and process messages.
    for (;;) {
#ifdef EXAMPLE_USE_STREAM_PROCESSING
        tscode_process_stream(stdin, stdout, cmd_callback);
#else // EXAMPLE_USE_CALLBACK_PROCESSING
        char c = getc(stdin);

        if (c != 0xFF) {
            if (c == '\n') {
                if (buffer[0] != '\0') {
#ifdef EXAMPLE_USE_CALLBACK_PROCESSING
                    char response[120] = "";
                    tscode_process_buffer(buffer, cmd_callback, response, 120);
                    puts(response);
#else // EXAMPLE_USE_CALLBACK_PROCESSING
                    process_buffer(buffer);
#endif // EXAMPLE_USE_CALLBACK_PROCESSING
                }

                buffer[0] = '\0';
                idx = 0;
            } else {
                buffer[idx++] = c;
                buffer[idx] = '\0';
            }
        }
#endif // EXAMPLE_USE_STREAM_PROCESSING

        vTaskDelay(1);
    }
}

void process_buffer(char* buffer) {
    tscode_command_t cmd;

    char* ptr = NULL;
    char* saveptr = NULL;

    printf("Parsing: <<%s>>\n", buffer);
    ptr = tscode_parse_command(buffer, &cmd, &saveptr);

    // Manually process each command. Note that in order to satisfy the requirements of the protocol, unless ACK 
    // has been disabled, you must manually send back TSCODE_ACK_STR, TSCODE_WAIT_STR, TSCODE_FAIL_STR etc.
    // When using one of the process_* functions, this is handled given the tscode_command_response_t returned from
    // the supplied callback function.
    while (ptr != NULL) {
        _tscode_print_command(&cmd);
        tscode_dispose_command(&cmd);
        printf("OK\n");

        ptr = tscode_parse_command(NULL, &cmd, &saveptr);
    }
}

tscode_command_response_t cmd_callback(tscode_command_t* cmd, char* response, size_t resp_len) {
    _tscode_print_command(cmd);
    return TSCODE_RESPONSE_OK;
}