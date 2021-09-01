#ifndef __tscode_capabilities_h
#define __tscode_capabilities_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#ifndef TSCODE_CAPS_VENDOR_STRINGS_LEN
#define TSCODE_CAPS_VENDOR_STRINGS_LEN 40
#endif

#ifndef TSCODE_CAPS_VERSION_STRINGS_LEN
#define TSCODE_CAPS_VERSION_STRINGS_LEN 20
#endif

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

#include "tscode_cap_flags.h"


/**
 * This holds information about a device, which is used either in host mode when querying the stream, or in client
 * mode when responding to a host query. This can be automatically handled if you use tscode_set_device_info and
 * process your stream using one of the process_ commands. Be sure to also add your caps.
 */
struct tscode_device_vendor_details {
    const char *vendor;
    const char *device;
    const char *version;
};

typedef struct tscode_device_vendor_details tscode_device_vendor_details_t;

#define TSCODE_DEVICE_VENDOR_DETAILS_DEFAULT { \
    .vendor = NULL, \
    .device = NULL, \
    .version = NULL, \
}

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
    .unit = TSCODE_UNIT_NONE, \
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
struct tscode_cap_channel {
    tscode_cap_flags_t capability;
    int channel;
    tscode_cap_range_t range;
    tscode_cap_axis_range_t *multi_axis;
};

typedef struct tscode_cap_channel tscode_cap_channel_t;

#define TSCODE_CAP_CHANNEL_DEFAULT { \
    .capability = 0x00, \
    .channel = 0, \
    .range = TSCODE_CAP_RANGE_DEFAULT, \
    .multi_axis = NULL, \
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
struct tscode_cap_details_node {
    tscode_cap_channel_t *cap;
    struct tscode_cap_details_node *next;
};

typedef struct tscode_cap_details_node tscode_cap_details_node_t;

/**
 * 
 */
struct tscode_cap_details_list {
    tscode_cap_details_node_t *first;
    tscode_cap_details_node_t *last;
};

typedef struct tscode_cap_details_list tscode_cap_details_list_t;

#define TSCODE_CAP_DETAILS_LIST_DEFAULT { \
    .first = NULL, \
    .last = NULL, \
}

/**
 * Add an item to a cap details list.
 */
void tscode_cap_details_list_append(tscode_cap_details_list_t *list, tscode_cap_channel_t *cap);

/**
 * This disposes *ONLY* the nodes in the list, it does not actually dispose the detail structs.
 */
void tscode_cap_details_list_dispose(tscode_cap_details_list_t *list);

/**
 * Register a channel with the internal TS-code engine. This is useful for client-side applications, and will
 * allow automatic validation of commands processed if registered.
 */
void tscode_register_channel(tscode_cap_channel_t *channel);

/**
 * This searches registered capabilities for a given channel for a given capability. If no such channel is found,
 * it returns NULL.
 */
tscode_cap_channel_t *tscode_find_channel(tscode_cap_flags_t capability, int channel_no);

/**
 * 
 */
void tscode_register_vendor_details(tscode_device_vendor_details_t *details);

/**
 *
 */
void tscode_generate_capstring(char *str, size_t str_len);

#ifdef __cplusplus
}
#endif

#endif
