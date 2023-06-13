#ifndef __tscode_h
#define __tscode_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "tscode_cap_flags.h"
#include "tscode_command_types.h"
#include "tscode_units.h"

// Argument string buffers start at this size and realloc automatically in increments.
#ifndef TSCODE_ARG_STRING_LENGTH_INCREMENT
#define TSCODE_ARG_STRING_LENGTH_INCREMENT 40
#endif

// Input stream buffers for tscode_process_stream.
#ifndef TSCODE_ISTREAM_BUFFER_SIZE
#define TSCODE_ISTREAM_BUFFER_SIZE 120
#endif

// Output stream buffers for tscode_process_stream.
#ifndef TSCODE_OSTREAM_BUFFER_SIZE
#define TSCODE_OSTREAM_BUFFER_SIZE 120
#endif

// Ringbuffer size for any usage of it.
#ifndef TSCODE_RINGBUFFER_SIZE
#define TSCODE_RINGBUFFER_SIZE 120
#endif

/**
 * // Don't forget to keep this in sync with the strings decl below and in tscode.c!
 */
enum tscode_command_response {
    TSCODE_RESPONSE_OK,
    TSCODE_RESPONSE_HOLD,
    TSCODE_RESPONSE_OUT_OF_BOUNDS,
    TSCODE_RESPONSE_NO_CHANNEL,
    TSCODE_RESPONSE_NO_CAPABILITY,
    TSCODE_RESPONSE_FAULT,
};

typedef enum tscode_command_response tscode_command_response_t;

extern const char* tscode_command_response_str[6];

/**
 * Contains a full definition of a TS-code command, including all relevant arguments and command
 * type.
 */
struct tscode_command {
    tscode_command_type_t type;
    int channel;
    char* str;
    tscode_unit_t* speed;
    tscode_unit_t* feedrate;
    tscode_coordinate_t* end;
    tscode_coordinate_t* start;
    tscode_unit_t* radius;
};

typedef struct tscode_command tscode_command_t;

#define TSCODE_COMMAND_DEFAULT                                                                     \
    {                                                                                              \
        .type = TSCODE_COMMAND_INVALID, .channel = 0, .str = NULL, .speed = NULL,                  \
        .feedrate = NULL, .end = NULL, .start = NULL, .radius = NULL,                              \
    }

/**
 * A generic typed ringbuffer you can use for parsing various other data streams.
 */
struct tscode_ringbuffer {
    char buffer[TSCODE_RINGBUFFER_SIZE + 1];
    size_t head;
    size_t tail;
};

typedef struct tscode_ringbuffer tscode_ringbuffer_t;

/**
 * This is the callback you're going to want to write if you use the process_ functions.
 */
typedef tscode_command_response_t (*tscode_command_callback_t)(tscode_command_t*, char*, size_t);

/**
 * This frees up memory used for a command, as well as any structures pointed to by it.
 */
void tscode_dispose_command(tscode_command_t* cmd);

/**
 * Parses a string command type and returns the associated command type cast. After the command is
 * parsed, saveptr is updated to point to the next token in the string for continued parsing.
 *
 * @param cmd String in the format S000 or T000
 */
tscode_command_type_t tscode_parse_cmd_type(char* cmd, char** saveptr);

/**
 * Parses a string for any TS-code type argument. This is a letter followed by decimal numbers.
 * After each parse, `value` is updated to reflect the value parsed. This function returns a pointer
 * to the first character parsed in the current substring, or NULL if nothing is left to parse. This
 * will also return NULL if a T or S is encountered, indicating that we've processed all arguments
 * and made it to a new command.
 */
char* tscode_parse_argument(char* arg_string, char* argc, float* argv, char** args, char** saveptr);

/**
 * Parses a string command into a command structure. This functions kinda like strtok_r in that it
 * will maintain a saveptr which it uses to ensure an entire string gets parsed over multiple calls.
 * On each successful parse, *cmd is assigned data and a pointer to the string that was just parsed
 * is returned.
 *
 * Subsequent calls should pass NULL for the first argument, picking up parsing where saveptr left
 * off. Additionally, the command will be re-initialized on every entry.
 *
 * @example
 *  char *command = "S01V900\nT01X3 T02X1T09X3";
 *  char *ptr = NULL;
 *  char *saveptr = NULL;
 *
 *  tscode_command_t cmd;
 *  ptr = tscode_parse_command(command, &cmd, &saveptr);
 *
 *  while (ptr != NULL) {
 *      printf("Parsed: %s\n", ptr);
 *      // do_something_with(&cmd);
 *
 *      ptr = tscode_parse_command(NULL, &cmd, &saveptr);
 *  }
 */
char* tscode_parse_command(char* cmd_string, tscode_command_t* cmd, char** saveptr);

/**
 * This just prints the command and various debugging information. It is not implemented unless
 * DEBUG build.
 */
void _tscode_print_command(tscode_command_t* cmd);

/**
 * This will serialize a tscode command object.
 */
void tscode_serialize_command(char* buffer, tscode_command_t* cmd, size_t buflen);

/**
 * This will automatically process a buffer, scanning for commands and calling the callback for
 * each. You should pass a character buffer and length for response, so that your commands have
 * something to write to.
 */
size_t tscode_process_buffer(
    char* buffer, tscode_command_callback_t callback, char* response, size_t resp_len
);

/**
 * This will automatically process a ringbuffer. The pointers are updated.
 */
void tscode_process_ringbuffer(
    tscode_ringbuffer_t* buffer, tscode_command_callback_t callback, char* response, size_t resp_len
);

/**
 * This automatically processes streams, calling the callback every time a command is parsed in
 * full. It uses the single internal command buffer, so you should be weary of using it for multiple
 * streams. For multiple streams, use tscode_process_stream_r. If you only do multi-stream parsing,
 * define TSCODE_DISABLE_STREAM_BUFFERS.
 */
void tscode_process_stream(FILE* istream, FILE* ostream, tscode_command_callback_t callback);

/**
 * This automatically processes streams, calling the callback every time a command is parsed in
 * full. It requires external buffers, which makes it useful for multi-stream processing.
 */
void tscode_process_stream_r(
    FILE* istream,
    FILE* ostream,
    tscode_command_callback_t callback,
    char* istream_buf,
    size_t istream_buf_len,
    char* ostream_buf,
    size_t ostream_buf_len
);

#ifdef __cplusplus
}
#endif

#endif