#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include "tscode.h"

void _tscode_fmt_unit(char* buffer, size_t n, tscode_unit_t* unit) {
    if (unit->unit == TSCODE_UNIT_BYTE) {
        snprintf(buffer, n, "0x%02x", (uint8_t) unit->value);
    } else {
        snprintf(buffer, n, "%0.2f", unit->value * (unit->unit == TSCODE_UNIT_PERCENTAGE ? 100 : 1));

        switch (unit->unit) {
        case TSCODE_UNIT_DEGREES:
            strncat(buffer, "deg", n);
            break;
        case TSCODE_UNIT_INCH:
            strncat(buffer, "in", n);
            break;
        case TSCODE_UNIT_METRIC:
            strncat(buffer, "mm", n);
            break;
        case TSCODE_UNIT_PERCENTAGE:
            strncat(buffer, "%", n);
            break;

        default:
            break;
        }
    }
}

void _tscode_fmt_coordinate(char* buffer, size_t n, tscode_coordinate_t* coordinate) {
    char lbuf[41] = "";
    buffer[0] = '\0';

    strncat(buffer, "x: ", n);
    _tscode_fmt_unit(lbuf, 20, &coordinate->x);
    strncat(buffer, lbuf, n);

    strncat(buffer, ", y: ", n);
    _tscode_fmt_unit(lbuf, 20, &coordinate->y);
    strncat(buffer, lbuf, n);

    strncat(buffer, ", z: ", n);
    _tscode_fmt_unit(lbuf, 20, &coordinate->z);
    strncat(buffer, lbuf, n);
}

void _tscode_print_command(tscode_command_t* cmd) {
    printf("--- %c%02d ---\n", cmd->type > 1000 ? 'S' : 'T', cmd->type % 1000);
    printf("Channel: %d\n", cmd->channel);
    char buffer[41] = "";

    if (cmd->str != NULL) {
        printf("String: %s\n", cmd->str);
    }

    if (cmd->speed != NULL) {
        _tscode_fmt_unit(buffer, 40, cmd->speed);
        printf("Speed: %s\n", buffer);
    }

    if (cmd->feedrate != NULL) {
        _tscode_fmt_unit(buffer, 40, cmd->feedrate);
        printf("Feedrate: %s / second\n", buffer);
    }

    if (cmd->radius != NULL) {
        _tscode_fmt_unit(buffer, 40, cmd->radius);
        printf("Radius: %s\n", buffer);
    }

    if (cmd->start != NULL) {
        _tscode_fmt_coordinate(buffer, 40, cmd->start);
        printf("Start: %s\n", buffer);
    }

    if (cmd->end != NULL) {
        _tscode_fmt_coordinate(buffer, 40, cmd->end);
        printf("End: %s\n", buffer);
    }
}

char* tscode_parse_command(char* cmd_string, tscode_command_t* cmd, char** saveptr) {
    tscode_command_t out = TSCODE_COMMAND_DEFAULT;
    char* start_ptr = NULL;

    if (cmd_string != NULL) {
        start_ptr = cmd_string;
    } else {
        start_ptr = *saveptr;
    }

    out.type = tscode_parse_cmd_type(start_ptr, saveptr);

    char argc = '\0';
    float argv = 0.0;
    char* args = NULL;
    char* rollbackptr = *saveptr;
    char* ptr = tscode_parse_argument(NULL, &argc, &argv, &args, saveptr);
    tscode_unit_system_t unit_override = TSCODE_UNIT_PERCENTAGE;

    while (ptr != NULL) {
        if (argc == 'T' || argc == 'S') {
            // we went too far, so i'm gonna back it up
            // also this isn't in the switch because i need break and don't wanna use a goto
            // out of visceral fear that someone on the internet will see this and throw boiling
            // hot pitch on me for it prior to immediately releasing raptors
            // xkcd promised raptors if i use goto
            // i'd rather not risk it
            *saveptr = rollbackptr;
            break;
        }

        switch (argc) {
        case '"': {
            if (out.str == NULL) {
                out.str = args;
            } else {
                // any further strings we don't care about tbh
                // i'm not going to realloc that to concat them
                free(args);
            }
            args = NULL;
            break;
        }

        case 'C': {
            out.channel = argv;
            break;
        }

        case 'X': {
            if (out.end == NULL) out.end = malloc(sizeof(tscode_coordinate_t));
            out.end->x.unit = unit_override;
            out.end->x.value = argv;
            break;
        }

        case 'Y': {
            if (out.end == NULL) out.end = malloc(sizeof(tscode_coordinate_t));
            out.end->y.unit = unit_override;
            out.end->y.value = argv;
            break;
        }

        case 'Z': {
            if (out.end == NULL) out.end = malloc(sizeof(tscode_coordinate_t));
            out.end->z.unit = unit_override;
            out.end->z.value = argv;
            break;
        }

        case 'I': {
            if (out.start == NULL) out.start = malloc(sizeof(tscode_coordinate_t));
            out.start->x.unit = unit_override;
            out.start->x.value = argv;
            break;
        }

        case 'J': {
            if (out.start == NULL) out.start = malloc(sizeof(tscode_coordinate_t));
            out.start->y.unit = unit_override;
            out.start->y.value = argv;
            break;
        }

        case 'K': {
            if (out.start == NULL) out.start = malloc(sizeof(tscode_coordinate_t));
            out.start->z.unit = unit_override;
            out.start->z.value = argv;
            break;
        }

        case 'R': {
            if (out.radius == NULL) out.radius = malloc(sizeof(tscode_unit_t));
            out.radius->unit = unit_override;
            out.radius->value = argv;
            break;
        }

        case 'F': {
            if (out.feedrate == NULL) out.feedrate = malloc(sizeof(tscode_unit_t));
            out.feedrate->unit = unit_override;
            out.feedrate->value = argv;
            break;
        }

        case 'V': {
            if (out.speed == NULL) out.speed = malloc(sizeof(tscode_unit_t));
            out.speed->unit = unit_override;
            out.speed->value = argv;
            break;
        }

        case 'P': {
            unit_override = TSCODE_UNIT_PERCENTAGE;
            break;
        }

        case 'B': {
            unit_override = TSCODE_UNIT_BYTE;
            break;
        }

        case 'N': {
            unit_override = TSCODE_UNIT_INCH;
            break;
        }

        case 'M': {
            unit_override = TSCODE_UNIT_METRIC;
            break;
        }

        case 'D': {
            unit_override = TSCODE_UNIT_DEGREES;
            break;
        }
        }

        rollbackptr = *saveptr;
        ptr = tscode_parse_argument(NULL, &argc, &argv, &args, saveptr);
    }

    if (out.type != TSCODE_COMMAND_INVALID) {
        *cmd = out;
        return start_ptr;
    } else {
        return NULL;
    }
}

tscode_command_type_t tscode_parse_cmd_type(char* cmd, char** saveptr) {
    char arg_char = '\0';
    float value = 0.0;
    char* args = NULL;

    char* ptr = tscode_parse_argument(cmd, &arg_char, &value, &args, saveptr);

    while (ptr != NULL) {
        if (arg_char == 'T') return (tscode_command_type_t) T((int) value);
        if (arg_char == 'S') return (tscode_command_type_t) S((int) value);

        ptr = tscode_parse_argument(NULL, &arg_char, &value, &args, saveptr);
    }

    return TSCODE_COMMAND_INVALID;
}

char* tscode_parse_argument(char* arg_string, char* arg_char, float* value, char** args, char** saveptr) {
    char tok = '\0';

    *arg_char = '\0';
    *value = 0.0;
    int decimal = 0;
    size_t args_len = 0;
    size_t buffer_len = 0;

    if (arg_string != NULL) {
        *saveptr = arg_string;
    }

    char* startptr = *saveptr;

    enum {
        PARSER_NORMAL = 0x0,
        PARSER_IN_COMMENT = 0x1,
        PARSER_IN_STRING = 0x2,
        PARSER_ESCAPED = 0x4,
    } parser_state = PARSER_NORMAL;

    while ((tok = toupper(**saveptr)) != '\0') {
        // oh it's the end of the line, eh? well, this is my stop.
        if (tok == '\n') {
            (*saveptr)++;
            break;
        }

        // we're in a comment, yeet to the end
        else if (parser_state & PARSER_IN_COMMENT) {
            (*saveptr)++;
            continue;
        }

        // string mode activated!
        else if (parser_state & PARSER_IN_STRING) {
            // escape character, enter escape mode
            if (tok == '\\' && !(parser_state & PARSER_ESCAPED)) {
                parser_state |= PARSER_ESCAPED;
            }

            // unescaped quotes, end string
            else if (tok == '"' && !(parser_state & PARSER_ESCAPED)) {
                parser_state &= ~(PARSER_IN_STRING);
            }

            // valid string character, not escaped
            else {
                // initialize a new string buffer
                if (*args == NULL) {
                    buffer_len = TSCODE_ARG_STRING_LENGTH_INCREMENT;
                    *args = malloc(TSCODE_ARG_STRING_LENGTH_INCREMENT + 1);
                }

                // reallocate a larger string buffer
                else if (args_len + 1 > buffer_len) {
                    buffer_len += TSCODE_ARG_STRING_LENGTH_INCREMENT;
                    *args = realloc(*args, buffer_len + 1);
                }

                (*args)[args_len++] = **saveptr;
                (*args)[args_len] = '\0';

                parser_state &= ~(PARSER_ESCAPED);
            }
        }

        // this is the start of an argument. if we hit it again, we went too far.
        else if (isalpha(tok) || tok == '"') {
            if (*arg_char == '\0') {
                *arg_char = tok;
            }

            else {
                return startptr;
            }

            if (tok == '"') {
                parser_state = PARSER_IN_STRING;
            }
        }

        // enter decimal parsing mode
        else if (tok == '.') {
            decimal = 1;
        }

        // parse digits
        else if (isdigit(tok)) {
            float digit = (float) (tok - '0');

            // we haven't hit a decimal point yet
            if (decimal == 0) {
                *value *= 10.0;
            }

            // at this point we're just tacking on precision
            else {
                digit /= powf(10.0, decimal++);
            }

            *value += digit;
        }

        // set the comment flag and ignore the rest of the line
        else if (tok == ';') {
            parser_state = PARSER_IN_COMMENT;
        }

        // next character
        (*saveptr)++;
    }

    // We got to the end of the string. If there was an argument here, return our startptr, otherwise NULL.
    return *arg_char == '\0' ? NULL : startptr;
}

void tscode_dispose_command(tscode_command_t* cmd) {
    if (cmd->str != NULL) free(cmd->str);
    if (cmd->end != NULL) free(cmd->end);
    if (cmd->start != NULL) free(cmd->start);
    if (cmd->speed != NULL) free(cmd->speed);
    if (cmd->radius != NULL) free(cmd->radius);
}
