# TS-code is like G-code but it goes up your butt.

```c
#include "tscode.h"

tscode_command_response_t cmd_callback(tscode_command_t* cmd, char* response, size_t resp_len) {
    switch (cmd->type) {
        case TSCODE_VIBRATE_ON:
            hardware_set_vibration_level(cmd->speed);
            break;
        default:
            return TSCODE_RESPONSE_FAULT;
    }

    return TSCODE_RESPONSE_OK;
}

void app_main(void) {
    for (;;) {
        tscode_process_stream(stdin, stdout, cmd_callback);
        yield();
    }
}
```

# About TS-code

There are T-codes and S-codes. T-codes control movement-based toys, and S-codes are System codes that either control 
metadata or basic system-level stuff. The majority of these have direct equivalents to G-code counterparts. When 
determining the control set to present for a TS-code enabled device, you should inspect its capabilities, which will 
return the capstring. The capstring can be decoded to figure out how to present a UI. Given this, in theory a generic 
control UI can be presented which operates any TS-code enabled device.

TS-code is extensible, where appropriate new commands can be added in to support future toys while maintaining backwards 
compatibility with devices that don't care about those features. Additionally, several commands accept a "C" parameter 
for Channel. The default channel is 0, but commands can target additional channels as needed to control multiple 
instances of similar things, such as devices with multiple vibrators or linear actuators or lube pumps.

## TS-code Format

```
S70 "Hang on, have fun!"    ' this will make the device show a message
S03 BV255                   ' this sets vibration to byte value 255 (100%, full ON)
T0F60                       ' start reciprocating at 60 RPM, note the leading 0 in command is optional
T0 C1 R100 I1000            ' reciprocate, channel 1, 100 RPM ramp to over 1000 ms
T22                         ' program coordinate in percent (for next example)
T1X1.0F0.5                  ' linear move X to 1.0 (percent, so 100%) at a rate of 0.5 (percent) / second
S07P300                     ' lube mist pump on for 300 ms
```

Spaces in the commands are optional for readability and discarded by the parser. Parameters start with a letter. To that
end, if the parser encounters a `S` or `T` token, the previous command's parsing is considered complete and pushed into
the operation buffer or holding buffer.

Each command is sent one line at a time, and consists of a series of Letter + Digit combinations to specify the command
and its parameters. The TS-code driver has two buffers: The operation buffer, and the holding buffer. In order to synch-
ronize movements with realtime data and avoid latency of transmitting and parsing those commands, the S-codes `S97-S99`
can be used to begin/end a buffer block, and then excute the buffer block. On devices which support multiple buffers,
`S97 - Start Buffer` can accept a `Lxxx` label, which can be used by `S99 - Execute Buffer`. The default buffer is
`S97 L00` which will be executed by `S99 L00` or `S97`/`S99` without arguments. The device capstring will indicate how
many program buffers it supports.

Conflicting commands for the same channel will overwrite the previous command in the buffer by default. It is therefore
advised to issue `S116 - Wait for Operation` commands if you intended to execute a series of commands which were
intended to run one after the other. If you do not desire realtime operation, you can issue `S118 - Start Queued 
Programming` to instruct the device to weit until an operation is complete before executing the next command. To return
to Realtime mode, use `S117 - Start Realtime Programming`.

# TS-code Commands

**T-codes**

|CMD|Name|Symbol|Arguments|
|---|---|---|---|
|[T00](#tscode-command-T00)|Reciprocating Move|`RECIPROCATING_MOVE`|[`C`?](#tscode-argument-c), [`V`](#tscode-argument-v)|
|[T01](#tscode-command-T01)|Linear Move|`LINEAR_MOVE`|[`C`?](#tscode-argument-c), [`X`](#tscode-argument-x), [`Y`](#tscode-argument-y), [`Z`](#tscode-argument-z), [`F`?](#tscode-argument-f)|
|[T02](#tscode-command-T02)|Clockwise Circular Move|`CW_CIRCULAR_MOVE`|[`C`?](#tscode-argument-c), [`I`](#tscode-argument-i), [`J`](#tscode-argument-j), [`K`](#tscode-argument-k), [`R`](#tscode-argument-r), [`X`](#tscode-argument-x), [`Y`](#tscode-argument-y), [`Z`](#tscode-argument-z), [`F`?](#tscode-argument-f)|
|[T03](#tscode-command-T03)|Counter-clockwise Circular Move|`CCW_CIRCULAR_MOVE`|[`C`?](#tscode-argument-c), [`I`](#tscode-argument-i), [`J`](#tscode-argument-j), [`K`](#tscode-argument-k), [`R`](#tscode-argument-r), [`X`](#tscode-argument-x), [`Y`](#tscode-argument-y), [`Z`](#tscode-argument-z), [`F`?](#tscode-argument-f)|
|[T04](#tscode-command-T04)|Dwell|`DWELL`|[`C`?](#tscode-argument-c), [`V`](#tscode-argument-v)|
|[T05](#tscode-command-T05)|Rotational Move|`ROTATIONAL_MOVE`|[`C`?](#tscode-argument-c), [`X`](#tscode-argument-x), [`Y`](#tscode-argument-y), [`Z`](#tscode-argument-z), [`F`?](#tscode-argument-f)|
|...||||
|[T17](#tscode-command-T17)|Select X-Y Plane|`SELECT_XY_PLANE`|[`C`?](#tscode-argument-c)|
|[T18](#tscode-command-T18)|Select X-Z Plane|`SELECT_XZ_PLANE`|[`C`?](#tscode-argument-c)|
|[T19](#tscode-command-T19)|Select Y-Z Plane|`SELECT_YZ_PLANE`|[`C`?](#tscode-argument-c)|
|[T20](#tscode-command-T20)|Program Unit: Inches|`PROGRAM_UNIT_INCH`||
|[T21](#tscode-command-T21)|Program Unit: Metric (Millimeter)|`PROGRAM_UNIT_METRIC`||
|[T22](#tscode-command-T22)|Program Unit: Percentage|`PROGRAM_UNIT_PERCENTAGE`||
|[T23](#tscode-command-T23)|Program Unit: Byte|`PROGRAM_UNIT_BYTE`||
|...||||
|[T28](#tscode-command-T28)|Return to Home Position|`RETURN_HOME`||
|...||||
|[T90](#tscode-command-T90)|Absolute Programming|`PROGRAM_POSITION_ABSOLUTE`||
|[T91](#tscode-command-T91)|Relative (Incremental) Programming|`PROGRAM_POSITION_RELARIVE`||

**S-codes**

|CMD|Name|Symbol|Arguments|
|---|---|---|---|
|[S00](#tscode-command-S00)|Program Stop (Immediate)|`HALT_IMMEDIATE`||
|[S01](#tscode-command-S01)|Sleep or Conditional Stop|`CONDITIONAL_STOP`||
|[S02](#tscode-command-S02)|End of Program|`END_OF_PROGRAM`||
|[S03](#tscode-command-S03)|Vibration On|`VIBRATE_ON`||
|[S04](#tscode-command-S04)|Vibration Pattern|`VIBRATE_PATTERN`||
|[S05](#tscode-command-S05)|Vibration Off|`VIBRATE_OFF`||
|[S06](#tscode-command-S06)|Default Channel Select|`DEFAULT_CHANNEL_SET`|[`V`](#tscode-argument-v)|
|[S07](#tscode-command-S07)|Lube Mist Pump ON|`LUBE_MIST_ON`||
|[S08](#tscode-command-S08)|Lube Flood Pump ON|`LUBE_FLOOD_ON`||
|[S09](#tscode-command-S09)|Lube Pump OFF|`LUBE_OFF`||
|[S10](#tscode-command-S10)|Air Out valve Open|`AIR_OUT_OPEN`||
|[S11](#tscode-command-S11)|Air In Valve Open|`AIR_IN_OPEN`||
|[S12](#tscode-command-S12)|Air Valve Close|`AIR_CLOSE`||
|...||||
|[S70](#tscode-command-S70)|Display a Message|`DISPLAY_MESSAGE`||
|...||||
|[S72](#tscode-command-S72)|Play a Tone|`PLAY_TONE`||
|...||||
|[S96](#tscode-command-S96)|Empty Program Buffer|`BUFFER_EMPTY`||
|[S97](#tscode-command-S97)|Start Buffer Programming|`BUFFER_START`||
|[S98](#tscode-command-S98)|End Buffer Programming|`BUFFER_END`||
|[S99](#tscode-command-S99)|Execute Program Buffer|`BUFFER_EXECUTE`||
|...||||
|[S104](#tscode-command-S104)|Enable Heater|`ENABLE_HEATER`||
|[S105](#tscode-command-S105)|Get Heater Temperature|`GET_HEATER_TEMP`||
|[S106](#tscode-command-S106)|Fan On|`FAN_ON`||
|[S107](#tscode-command-S107)|Fan Off|`FAN_OFF`||
|[S108](#tscode-command-S108)|Cancel Heating|`CANCEL_HEATING`||
|[S109](#tscode-command-S109)|Set Heater and Wait|`ENABLE_HEATER_AND_WAIT`||
|...||||
|[S113](#tscode-command-S113)|Ping / Pong|`PING`||
|[S114](#tscode-command-S114)|Report Current Positions|`GET_CURRENT_POSITIONS`||
|[S115](#tscode-command-S115)|Report Firmware and Capstring|`GET_CAPSTRINGS`||
|[S116](#tscode-command-S116)|Wait for Movement|`WAIT_MOVEMENT`||
|[S117](#tscode-command-S117)|Start Realtime Programming|`START_REALTIME_PROGRAMMING`||
|[S118](#tscode-command-S118)|Start Queued Programming|`START_QUEUED_PROGRAMMING`||
|...||||
|[S150](#tscode-command-S150)|Set LED Color|`SET_LED_COLOR`||
|...||||
|[S199](#tscode-command-S199)|Exit TS-code Mode|`EXIT_TSCODE_MODE`||

? denotes an optional argument.

## Capstrings

Using `S115` will report back a capabilities string and firmware version, or capstring. This capstring will contain
a number of tokens indicating which features and channels the device supports, as well as its ranges and limits.

