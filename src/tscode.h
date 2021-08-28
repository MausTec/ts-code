#ifndef __tscode_h
#define __tscode_h

#ifdef __cplusplus
extern "C" {
#endif

#include "tscode_command_types.h"

#ifndef TSCODE_ARG_STRING_LENGTH_INCREMENT
#define TSCODE_ARG_STRING_LENGTH_INCREMENT 40
#endif

/**
 * Various unit systems.
 */
enum tscode_unit_system {
    TSCODE_UNIT_PERCENTAGE,
    TSCODE_UNIT_METRIC,
    TSCODE_UNIT_INCH,
    TSCODE_UNIT_BYTE,
    TSCODE_UNIT_DEGREES,
};

typedef enum tscode_unit_system tscode_unit_system_t;

/**
 * Contains a unit and associated type as a command argument.
 */
struct tscode_unit {
    tscode_unit_system_t unit;
    float value;
};

typedef struct tscode_unit tscode_unit_t;

/**
 * Contains an x, y, z coordinate for a command argument.
 */
struct tscode_coordinate {
    tscode_unit_t x;
    tscode_unit_t y;
    tscode_unit_t z;
};

typedef struct tscode_coordinate tscode_coordinate_t;

/**
 * Contains a full definition of a TS-code command, including all relevant arguments and command type.
 */
struct tscode_command {
    tscode_command_type_t type;
    int channel;
    char *str;
    tscode_unit_t *speed;
    tscode_unit_t *feedrate;
    tscode_coordinate_t *end;
    tscode_coordinate_t *start;
    tscode_unit_t *radius;
};

typedef struct tscode_command tscode_command_t;

#define TSCODE_COMMAND_DEFAULT { \
    .type = TSCODE_COMMAND_INVALID, \
    .channel = 0, \
    .str = NULL, \
    .speed = NULL, \
    .feedrate = NULL, \
    .end = NULL, \
    .start = NULL, \
    .radius = NULL, \
}

/**
 * This frees up memory used for a command, as well as any structures pointed to by it.
 */
void tscode_dispose_command(tscode_command_t *cmd);

/**
 * Parses a string command type and returns the associated command type cast. After the command is parsed, saveptr is
 * updated to point to the next token in the string for continued parsing.
 * 
 * @param cmd String in the format S000 or T000
 */
tscode_command_type_t tscode_parse_cmd_type(char *cmd, char **saveptr);

/**
 * Parses a string for any TS-code type argument. This is a letter followed by decimal numbers. After each parse, 
 * `value` is updated to reflect the value parsed. This function returns a pointer to the first character parsed in
 * the current substring, or NULL if nothing is left to parse. This will also return NULL if a T or S is encountered,
 * indicating that we've processed all arguments and made it to a new command.
 */
char *tscode_parse_argument(char *arg_string, char *argc, float *argv, char **args, char **saveptr);

/**
 * Parses a string command into a command structure. This functions kinda like strtok_r in that it will maintain a 
 * saveptr which it uses to ensure an entire string gets parsed over multiple calls. On each successful parse, *cmd is
 * assigned data and a pointer to the string that was just parsed is returned.
 * 
 * Subsequent calls should pass NULL for the first argument, picking up parsing where saveptr left off. Additionally,
 * the command will be re-initialized on every entry.
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
char *tscode_parse_command(char *cmd_string, tscode_command_t *cmd, char **saveptr);

/**
 * This just prints the command.
 */
void _tscode_print_command(tscode_command_t *cmd);

#ifdef __cplusplus
}
#endif


#endif