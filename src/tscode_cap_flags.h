#ifndef __tscode_cap_flags_h
#define __tscode_cap_flags_h

#ifdef __cplusplus
extern "C" {
#endif

#include "tscode_units.h"

#define CAP(x) (1UL << (x - 'A'))

enum tscode_cap_flags {
    TSCODE_CAP_AIR_VALVES = CAP('A'), // A
    TSCODE_CAP_MULTIBUFFERS = CAP('B'), // B
    TSCODE_CAP_CONDITIONAL_STOP = CAP('C'), // C
    TSCODE_CAP_LEDS = CAP('D'), // D
    TSCODE_CAP_FANS = CAP('F'), // F
    TSCODE_CAP_HEATERS = CAP('H'), // H
    TSCODE_CAP_LINEAR = CAP('L'), // L
    TSCODE_CAP_MESSAGES = CAP('M'), // M
    TSCODE_CAP_LUBE_PUMPS = CAP('P'), // P
    TSCODE_CAP_RECIPROCATING = CAP('R'), // R
    TSCODE_CAP_SPEAKERS = CAP('S'), // S
    TSCODE_CAP_ROTATING = CAP('T'), // T
    TSCODE_CAP_VIBRATING = CAP('V'), // V
};

typedef enum tscode_cap_flags tscode_cap_flags_t;


/**
 * Maus-Tec Electronics; Mercury 1000; v0.1.2; ABMRC
 * AIR_VALVES= channels:1
 * MULTIBUFFERS= limit:5
 * MESSAGES= limit:20
 * RECIPROCATING= c:1; u:P; range:0.0,1.0
 * CONDITIONAL_STOP
 */

// For something that has an X axis like a thruster:
// actually that'd be a reciprocating, but pretend it supports atomic linear control
// The reciprocating cap doesn't have args, so it assumes 1 channel, percent control, 0-100%

/**
 * Himsmath XLR Tech; Fucko'Matic 6900; v1.0.4; RL
 * RECIPROCATING
 * LINEAR= range-x[0]:-100,100M
 */

// Formatting the string that way lets us specify range-axis[channel]:min,max
// specifying limit: will add a range starting at 0 for that channel. Omitting channel applies to all channels.
// if a given channel uses different units, you can add that to the end of the range end measurement

/**
 * 
 */
struct tscode_cap_range {
    tscode_unit_system_t unit;
    float min;
    float max;
};

typedef struct tscode_cap_range tscode_cap_range_t;

#define TSCODE_CAP_RANGE_DEFAULT { \
    .unit = TSCODE_UNIT_PERCENTAGE, \
    .min = 0.0, \
    .max = 0.0, \
}

/**
 * 
 */
struct tscode_cap_axis_range {
    int num_axis;
    tscode_cap_range_t x;
    tscode_cap_range_t y;
    tscode_cap_range_t z;
};

typedef struct tscode_cap_axis_range tscode_cap_axis_range_t;

#define TSCODE_CAP_AXIS_RANGE_DEFAULT { \
    .num_axis = 0, \
    .x = TSCODE_CAP_RANGE_DEFAULT, \
    .y = TSCODE_CAP_RANGE_DEFAULT, \
    .z = TSCODE_CAP_RANGE_DEFAULT, \
}

/**
 * 
 */
struct tscode_cap_details {
    tscode_cap_flags_t capability;
    int num_channels;
    tscode_cap_axis_range_t *channels;
};

typedef struct tscode_cap_details tscode_cap_details_t;

#define TSCODE_CAP_DETAILS_DEFAULT { \
    .capability = 0x00, \
    .num_channels = 0, \
    .channels = NULL, \
}

/**
 * 
 */
void tscode_str2capflags(const char *str, tscode_cap_flags_t *flags);

/**
 * 
 */
void tscode_capflags2str(char *str, size_t len, tscode_cap_flags_t flags);

#ifdef __cplusplus
}
#endif

#endif