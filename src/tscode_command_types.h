/**
 * THIS IS AN AUTOMATICALLY GENERATED FILE
 * DO NOT EDIT THIS BY HAND
 * 
 * This is created by bin/autogen.rb
 */
#ifndef __tscode_command_types_h
#define __tscode_command_types_h

#ifdef __cplusplus
extern "C" {
#endif

#define __T(x) (0000 + x)
#define __S(x) (1000 + x)

/**
 * For this enumeration, T-codes occupy 0-999, and S-codes occupy 1000-1999
 */

enum tscode_command_type {
    TSCODE_COMMAND_INVALID = -1,
    TSCODE_RECIPROCATING_MOVE = __T(0),
    TSCODE_LINEAR_MOVE = __T(1),
    TSCODE_CW_CIRCULAR_MOVE = __T(2),
    TSCODE_CCW_CIRCULAR_MOVE = __T(3),
    TSCODE_DWELL = __T(4),
    TSCODE_ROTATIONAL_MOVE = __T(5),
    TSCODE_SELECT_XY_PLANE = __T(17),
    TSCODE_SELECT_XZ_PLANE = __T(18),
    TSCODE_SELECT_YZ_PLANE = __T(19),
    TSCODE_PROGRAM_UNIT_INCH = __T(20),
    TSCODE_PROGRAM_UNIT_METRIC = __T(21),
    TSCODE_PROGRAM_UNIT_PERCENTAGE = __T(22),
    TSCODE_PROGRAM_UNIT_BYTE = __T(23),
    TSCODE_RETURN_HOME = __T(28),
    TSCODE_PROGRAM_POSITION_ABSOLUTE = __T(90),
    TSCODE_PROGRAM_POSITION_RELARIVE = __T(91),
    TSCODE_HALT_IMMEDIATE = __S(0),
    TSCODE_CONDITIONAL_STOP = __S(1),
    TSCODE_END_OF_PROGRAM = __S(2),
    TSCODE_VIBRATE_ON = __S(3),
    TSCODE_VIBRATE_PATTERN = __S(4),
    TSCODE_VIBRATE_OFF = __S(5),
    TSCODE_DEFAULT_CHANNEL_SET = __S(6),
    TSCODE_LUBE_MIST_ON = __S(7),
    TSCODE_LUBE_FLOOD_ON = __S(8),
    TSCODE_LUBE_OFF = __S(9),
    TSCODE_AIR_OUT_OPEN = __S(10),
    TSCODE_AIR_IN_OPEN = __S(11),
    TSCODE_AIR_CLOSE = __S(12),
    TSCODE_DISPLAY_MESSAGE = __S(70),
    TSCODE_PLAY_TONE = __S(72),
    TSCODE_BUFFER_EMPTY = __S(96),
    TSCODE_BUFFER_START = __S(97),
    TSCODE_BUFFER_END = __S(98),
    TSCODE_BUFFER_EXECUTE = __S(99),
    TSCODE_ENABLE_HEATER = __S(104),
    TSCODE_GET_HEATER_TEMP = __S(105),
    TSCODE_FAN_ON = __S(106),
    TSCODE_FAN_OFF = __S(107),
    TSCODE_CANCEL_HEATING = __S(108),
    TSCODE_ENABLE_HEATER_AND_WAIT = __S(109),
    TSCODE_PING = __S(113),
    TSCODE_GET_CURRENT_POSITIONS = __S(114),
    TSCODE_GET_CAPSTRINGS = __S(115),
    TSCODE_WAIT_MOVEMENT = __S(116),
    TSCODE_START_REALTIME_PROGRAMMING = __S(117),
    TSCODE_START_QUEUED_PROGRAMMING = __S(118),
    TSCODE_SET_LED_COLOR = __S(150),
    TSCODE_EXIT_TSCODE_MODE = __S(199),
};

typedef enum tscode_command_type tscode_command_type_t;

#ifdef __cplusplus
}
#endif

#endif
