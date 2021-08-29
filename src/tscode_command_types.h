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

    TSCODE_HALT_IMMEDIATE = __S(0), // Unconditional Halt
    TSCODE_CONDITIONAL_STOP = __S(1), // Sleep or Conditional Stop
    TSCODE_END_OF_PROGRAM = __S(2), // End of Program
    TSCODE_VIBRATE_ON = __S(3), // Vibration On
    TSCODE_VIBRATE_PATTERN = __S(4), // Vibration Pattern
    TSCODE_VIBRATE_OFF = __S(5), // Vibration Off
    TSCODE_DEFAULT_CHANNEL_SET = __S(6), // Default Channel Select
    TSCODE_LUBE_MIST_ON = __S(7), // Lube Mist Pump ON
    TSCODE_LUBE_FLOOD_ON = __S(8), // Lube Flood Pump ON
    TSCODE_LUBE_OFF = __S(9), // Lube Pump OFF
    TSCODE_AIR_OUT_OPEN = __S(10), // Air Out Valve OPEN
    TSCODE_AIR_IN_OPEN = __S(11), // Air In Valve OPEN
    TSCODE_AIR_CLOSE = __S(12), // Air Valve CLOSE
    TSCODE_DISPLAY_MESSAGE = __S(70), // Display a Message
    TSCODE_PLAY_TONE = __S(72), // Play a Tone
    TSCODE_BUFFER_EMPTY = __S(96), // Empty Program Buffer
    TSCODE_BUFFER_START = __S(97), // Start Buffer Programming
    TSCODE_BUFFER_END = __S(98), // End Buffer Programming
    TSCODE_BUFFER_EXECUTE = __S(99), // Execute Program Buffer
    TSCODE_ENABLE_HEATER = __S(104), // Enable Heater
    TSCODE_GET_HEATER_TEMP = __S(105), // Get Heater Temperature
    TSCODE_FAN_ON = __S(106), // Fan On
    TSCODE_FAN_OFF = __S(107), // Fan Off
    TSCODE_CANCEL_HEATING = __S(108), // Cancel Heating
    TSCODE_ENABLE_HEATER_AND_WAIT = __S(109), // Set Heater and Wait
    TSCODE_PING = __S(113), // Ping / Pong
    TSCODE_GET_CURRENT_POSITIONS = __S(114), // Report Current Positions
    TSCODE_GET_CAPSTRING = __S(115), // Report Firmware and Capstring
    TSCODE_WAIT_MOVEMENT = __S(116), // Wait for Movement
    TSCODE_START_REALTIME_PROGRAMMING = __S(117), // Start Realtime Programming
    TSCODE_START_QUEUED_PROGRAMMING = __S(118), // Start Queued Programming
    TSCODE_SET_LED_COLOR = __S(150), // Set LED Color
    EXIT_TSCODE_MODE = __S(199), // Exit TS-code Programming Mode
};

typedef enum tscode_command_type tscode_command_type_t;

#ifdef __cplusplus
}
#endif

#endif