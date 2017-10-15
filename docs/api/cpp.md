# API - C++

Everything comes nice and neatly tucked away in the `Joytime` namespace,
so remember to prefix everything here with `Joytime::`. Or not, if you're
`using namespace Joytime`. But please don't. You might end up breaking something
with someone else's libraries.

By the way, all the `enum`s in the C++ interface are actually [`enum class`](http://en.cppreference.com/w/cpp/language/enum#Scoped_enumerations)es.

## `enum class ControllerType`

An enum class for the different types of controllers Joytime supports. Members:

  * `LeftJoycon` = 0 --- Left Joycon
  * `RightJoycon` = 1 --- Right JoyCon
  * `Pro` = 2 --- Pro Controller

## `enum class ControllerCommand`

An enum class for the controller command codes available. This should not need to
be used by any clients, and as of now, no method or property publicly uses it.
Members:

  * `RumbleAndSubcommand` = 0x01 (1) --- Command that sends a rumble and a subcommand
  * `SendRumble` = 0x10 (16) --- Command that sends only a rumble

## `enum class ControllerSubcommand`

An enum class for the controller subcommand codes available. This should not need
to be used by any clients, and as of now, no method or property publicly uses it.
Members:

  * `GetOnlyControllerState` = 0x00 (0) --- Subcommand that does nothing. Its only purpose is to return the same controller state data normally returned along with any other subcommand.
  * `GetDeviceInfo` = 0x02 (2) --- Subcommand that returns some device information (type, MAC address, etc.)
  * `SetInputReportMode` = 0x03 (3) -- Subcommand that set the input mode (see the ControllerInputReportMode enum)
  * `SetPowerState` = 0x06 (6) --- Subcommand that tells the JoyCon to switch to the specified power state (see the ControllerPowerState enum)
  * `ReadSPIFlash` = 0x10 (16) --- Subcommand that receives a desired address and size and returns data from the SPI flash
  * `SetPlayerLights` = 0x30 (48) --- Subcommand that receives a bitfield in little endian that specifies the states for the player number LEDs (most significant four bits specify whether to flash, least significant four bits specify whether to keep solid)
  * `SetSixAxisSensor` = 0x40 (64) --- Subcommand that receives a single byte (actually, a boolean) on whether to enable gyroscope and accelerometer
  * `SetVibration` = 0x48 (72) --- Subcommand that receives a single byte (actually, a boolean) on whether to enable vibration

## `enum class ControllerInputReportMode`

An enum class that contains the various input report modes available. Members:

  * `IRCamera` = 0x00 (0) --- TLDR: use NFCAndIR instead. If the controller is a Right JoyCon, packets will be identical to NFCAndIR mode, *except* IR data will only be sent on requests to the MCU.
  * `SpecialIR_` = 0x02 (2) --- Some sort of special IR mode? Don't use.
  * `MCUUpdateState_` = 0x20 (32) --- ? Don't use.
  * `StandardReport` = 0x30 (48) --- Standard input report. Contains the controller state (as all subcommands do), but if the gyroscope and accelerometer are enabled, also reports data from those too.
  * `NFCAndIR` = 0x31 (49) --- NFC and IR mode. Contains the same data as a StandardReport, but also contains a *bunch* of data from the NFC reader and IR camera.
  * `SimpleHID` = 0x3f (63) --- Standard OS report mode. This is what is reported to the OS when programs talk to the JoyCon as if it were a general controller. Reports only button data and stick data as hat (POV) data.

## `enum class ControllerLEDState`

An enum class for the available states for each LED. Members:

  * `Off` = 0 --- LED off
  * `On` = 1 --- LED on solid
  * `Flashing` = 2 --- LED on flashing

## `enum class ControllerPowerState`

An enum class for the power states the controllers can be set to. Members:

  * `Disconnect` = 0x00 (0) --- Disconnect from the host
  * `ColdReboot` = 0x01 (1) --- Reboot and reconnect to the host
  * `PairingReboot` = 0x02 (2) --- Reboot and start searching for a new host
  * `PowerOff`  = 0x04 (4) --- The name is misleading (was named when an older version of the reverse engineering documentation was available), as this does the same thing as ColdReboot

## `enum class ControllerReportCode`

An enum class for the report codes returned from a command reply. This should not need
to be used by any clients, and as of now, no method or property publicly uses it. Members:

  * `SubcommandReply` = 0x21 (33) --- Subcommand reply
  * `MCUFWUpdate` = 0x23 (35) --- An MCU firmware update reply
  * `Standard` = 0x30 (48) --- Standard input report
  * `NFCIR` = 0x31 (49) --- Standard input report + NFC + IR
  * `StandardOSController` = 0x3f (63) --- Simple HID/Standard OS controller report

## `enum class ControllerBatteryStatus`

An enum class for the possible battery statuses. Members

  * `Full` = 0x08 (8)
  * `Medium` = 0x06 (6)
  * `Low` = 0x04 (4)
  * `Critical` = 0x02 (2)
  * `Empty` = 0x00 (0)
  * `Charging` = 0x1 (1)

## `struct StickCalibrationData`

A POD structure for the stick calibration data. Members:

  * `uint16_t xCenter` --- The X value for the stick when it's centered
  * `uint16_t yCenter` --- The Y value for the stick when it's centered
  * `uint16_t xMax` --- The maximum allowed value for the X
  * `uint16_t yMax` --- The maximum allowed value for the Y
  * `uint16_t xMin` --- The minimum allowed value for the X
  * `uint16_t yMin` --- The minimum allowed value for the Y
  * `uint16_t deadZone` --- The stick's dead zone range. Same value in all directions
  * `uint16_t rangeRatio` --- ? Not actually used for anything

## `struct SixAxisCalibrationData`

A POD structure for the gyroscope and accelerometer calibration. Members:

  * `int16_t originX` --- Origin X
  * `int16_t originY` --- Origin Y
  * `int16_t originZ` --- Origin Z
  * `int16_t rawCoeffX` --- Raw "coefficient" X value, usually obtained from SPI flash
  * `int16_t rawCoeffY` --- Raw "coefficient" Y value, usually obtained from SPI flash
  * `int16_t rawCoeffZ` --- Raw "coefficient" Z value, usually obtained from SPI flash
  * `int16_t offsetX` --- Only valid for accelerometer. Used to obtain more accurate values for X
  * `int16_t offsetY` --- Only valid for accelerometer. Used to obtain more accurate values for Y
  * `int16_t offsetZ` --- Only valid for accelerometer. Used to obtain more accurate values for Z
  * `double coeffX` --- True X coefficient applied to values received on every update
  * `double coeffY` --- True Y coefficient applied to values received on every update
  * `double coeffZ` --- True Z coefficient applied to values received on every update

## `struct Buttons`

A POD structure for the buttons of the controller. Members:

  * `bool a` --- A
  * `bool b` --- B
  * `bool x` --- X
  * `bool y` --- Y
  * `bool up` --- Up
  * `bool down` --- Down
  * `bool left` --- Left
  * `bool right` --- Right
  * `bool l` --- Left Shoulder
  * `bool r` --- Right Shoulder
  * `bool zl` --- Left Trigger
  * `bool zr` --- Right Trigger
  * `bool sl` --- Inner Left Shoulder (only on JoyCons)
  * `bool sr` --- Inner Right Should (only on JoyCons)
  * `bool plus` --- Plus (+)
  * `bool minus` --- Minus (-)
  * `bool lStick` --- Left Stick Button
  * `bool rStick` --- Right Stick Button
  * `bool home` --- Home
  * `bool capture` --- Capture/Screenshot

## `struct Stick`

A POD structure for the stick data. Members:

  * `int16_t x` --- X value
  * `int16_t y` --- Y value

## `struct SixAxis`

A POD structure for the gyroscope and accelerometer data. Members:

  * `double x` --- X value
  * `double y` --- Y value
  * `double z` --- Z value

## `typedef TransmitBufferFunction`

```cpp
void TransmitBufferFunction(void* handle, std::vector<uint8_t> data);
```

A typedef for the function to send data passed in by C++ input libraries. Returns
nothing (`void`) and accepts a `void*` to the handle and a `std::vector` of `uint8_t`s
(`std::vector<uint8_t>`) with the data to send.

## `typedef ReceiveBufferFunction`

```cpp
std::vector<uint8_t> ReceiveBufferFunction(void* handle, int requestedBytes);
```

A typedef for the function to receive data passed in by C++ input libraries. Returns a
`std::vector` of `uint8_t`s (`std::vector<uint8_t>`) and accepts a `void*` to the
handle and the requested number of bytes (`int`).

## `typedef CTransmitBufferFunction`

```c
void CTransmitBufferFunction(void* handle, uint8_t* data, )
```

A typedef for the function to send data passed in by C input libraries. Returns
nothing (`void`) and accepts a `void*` to the handle, a `uint8_t` array of data
to send (`uint8_t*`), and the length of that array (`int`).

## `typedef CReceiveBufferFunction`

```c
uint8_t* CReceiveBufferFunction(void* handle, int requestedBytes, int* byteRead);
```

A typedef for the function to receive data passed in by C input libraries. Returns
a `uint8_t` array (`uint8_t*`) and accepts a `void*` to the handle, the requested
number of bytes to read (`int`), and a pointer to put the number of bytes read
into (`int*`).
