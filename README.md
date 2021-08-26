# TS-code is like G-code but it goes up your butt.

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
S03 B255                    ' this sets vibration to byte value 255 (100%, full ON)
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

## TS-code Listing

**T-codes**

|CMD|Name|Symbol|Arguments|Description|
|---|----|------|---------|-----------|
|T00|Reciprocating Move|RECIPROCATING_MOVE|||
|T01|Linear Move|LINEAR_MOVE|||
|T02|CW Circular Move|CW_CIRCULAR_MOVE|||
|T03|CCW Circular Move|CCW_CIRCULAR_MOVE|||
|T04|Dwell|DWELL|||
|T05|Rotational Move|ROTATIONAL_MOVE|||
|...|||||
|T17|Select X-Y Plane|SELECT_XY_PLANE|||
|T18|Select X-Z Plane|SELECT_XZ_PLANE|||
|T19|Select Y-Z Plane|SELECT_YZ_PLANE|||
|T20|Program Unit: Inches|PROGRAM_UNIT_INCH|||
|T21|Program Unit: Millimeter|PROGRAM_UNIT_METRIC|||
|T22|Program Unit: Percentage|PROGRAM_UNIT_PERCENT|||
|T23|Program Unit: Byte|PROGRAM_UNIT_BYTE|||
|...|||||
|T28|Return to Home Position|RETURN_HOME|||
|...|||||
|T90|Absolute Programming|PROGRAM_ABSOLUTE|||
|T91|Relative Programming|PROGRAM_RELATIVE|||

**S-codes**

|CMD|Name|Symbol|Arguments|Description|
|---|----|------|---------|-----------|
|...|||||

## Capstrings

Using `S115` will report back a capabilities string and firmware version, or capstring. This capstring will contain
a number of tokens indicating which features and channels the device supports, as well as its ranges and limits.

