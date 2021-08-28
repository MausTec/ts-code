#ifndef __tscode_command_types_h
#define __tscode_command_types_h

#define T(x) (0000 + x)
#define S(x) (1000 + x)

/**
 * For this enumeration, T-codes occupy 0-999, and S-codes occupy 1000-1999
 */

enum tscode_command_type {
    TSCODE_COMMAND_INVALID = -1,

    TSCODE_HALT_IMMEDIATE = S(1), // Unconditional Halt
    TSCODE_CONDITIONAL_STOP = S(1), // Sleep or Conditional Stop
    TSCODE_END_OF_PROGRAM = S(2), // End of Program
    TSCODE_VIBRATE_ON = S(3), // Vibration On
    TSCODE_VIBRATE_PATTERN = S(4), // Vibration Pattern
    TSCODE_VIBRATE_OFF = S(5), // Vibration Off
    TSCODE_DEFAULT_CHANNEL_SET = S(6), // Default Channel Select
    TSCODE_LUBE_MIST_ON = S(7), // Lube Mist Pump ON
    TSCODE_LUBE_FLOOD_ON = S(8), // Lube Flood Pump ON
    TSCODE_LUBE_OFF = S(9), // Lube Pump OFF
    TSCODE_AIR_OUT_OPEN = S(10), // Air Out Valve OPEN
    TSCODE_AIR_IN_OPEN = S(11), // Air In Valve OPEN
    TSCODE_AIR_CLOSE = S(12), // Air Valve CLOSE
    TSCODE_DISPLAY_MESSAGE = S(70), // Display a Message
    TSCODE_PLAY_TONE = S(72), // Play a Tone
    TSCODE_BUFFER_EMPTY = S(96), // Empty Program Buffer
    TSCODE_BUFFER_START = S(97), // Start Buffer Programming
    TSCODE_BUFFER_END = S(98), // End Buffer Programming
    TSCODE_BUFFER_EXECUTE = S(99), // Execute Program Buffer
    TSCODE_ENABLE_HEATER = S(104), // Enable Heater
    TSCODE_GET_HEATER_TEMP = S(105), // Get Heater Temperature
    TSCODE_FAN_ON = S(106), // Fan On
    TSCODE_FAN_OFF = S(107), // Fan Off
    TSCODE_CANCEL_HEATING = S(108), // Cancel Heating
    TSCODE_ENABLE_HEATER_AND_WAIT = S(109), // Set Heater and Wait
    TSCODE_PING = S(113), // Ping / Pong
    TSCODE_GET_CURRENT_POSITIONS = S(114), // Report Current Positions
    TSCODE_GET_CAPSTRING = S(115), // Report Firmware and Capstring
    TSCODE_WAIT_MOVEMENT = S(116), // Wait for Movement
    TSCODE_START_REALTIME_PROGRAMMING = S(117), // Start Realtime Programming
    TSCODE_START_QUEUED_PROGRAMMING = S(118), // Start Queued Programming
    TSCODE_SET_LED_COLOR = S(150), // Set LED Color
    EXIT_TSCODE_MODE = S(199), // Exit TS-code Programming Mode
};

typedef enum tscode_command_type tscode_command_type_t;

#endif