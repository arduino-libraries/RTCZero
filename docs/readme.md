# RTC Library

This library allows an enables an Arduino based on SAMD architectures (es. [Zero](<https://www.arduino.cc/en/Main/ArduinoBoardZero>), [MKRZero](<https://www.arduino.cc/en/Main/ArduinoBoardMKRZero>) or [MKR1000](<https://www.arduino.cc/en/Main/ArduinoMKR1000>) Board) to control and use the internal RTC (Real Time Clock). A real-time clock is a clock that keeps track of the current time and that can be used in order to program actions at a certain time. Most RTCs use a crystal oscillator (like in the Arduino Zero) whose frequency is 32.768 kHz (same frequency used in quartz clocks and watches). Namely this the frequency equal to 2^15 cycles per second and so is a convenient rate to use with simple binary counter circuits. Furthermore the RTC can continue to operate in any sleep mode, so it can be used to wake up the device from sleep modes in a programmed way. Every time the board is powered, the RTC is reset and starts from a standard date. To keep the time and the RTC running it is necessary to keep the board powered. A button sized lithium battery or any battery in the 3V range, connected through a diode to the 3.3V pin, is enough to keep RTC alive if the CPU is put in sleep mode before the standard USB or VIN power is disconnected.

To use this library

``` arduino
#include <RTCZero.h>
```

## Examples

- [Simple RTC](<https://www.arduino.cc/en/Tutorial/SimpleRTC>)
- [Simple RTC Alarm](<https://www.arduino.cc/en/Tutorial/SimpleRTCAlarm>)
- [Sleep RTC Alarm](<https://www.arduino.cc/en/Tutorial/SleepRTCAlarm>)
