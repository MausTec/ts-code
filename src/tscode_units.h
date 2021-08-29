#ifndef __tscode_units_h
#define __tscode_units_h

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Various unit systems.
 */
enum tscode_unit_system {
    TSCODE_UNIT_NONE,
    TSCODE_UNIT_PERCENTAGE,
    TSCODE_UNIT_METRIC,
    TSCODE_UNIT_INCH,
    TSCODE_UNIT_BYTE,
    TSCODE_UNIT_DEGREES,
};

typedef enum tscode_unit_system tscode_unit_system_t;

extern char tscode_unit_str[6];

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

#ifdef __cplusplus
}
#endif

#endif