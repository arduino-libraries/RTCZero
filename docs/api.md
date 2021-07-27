# RTC Library

## Methods

### `begin()`

Initializes the internal RTC. begin() needs to be called before any other RTC library methods.

#### Syntax

``` Arduino
rtc.begin()
```


#### Parameters

none

### `setHours()`

Set the RTC hours value.

#### Syntax

``` Arduino
rtc.setHours(byte hours)
```

#### Parameters

- hours: the hours value to be set.

### `setMinutes()`

Set the RTC minutes value.

#### Syntax

``` Arduino
rtc.setMinutes(byte minutes)
```

#### Parameters

- minutes: the minutes value to be set.

### `setTime()`

Set the RTC hours, minutes and seconds.

#### Syntax

``` Arduino
rtc.setTime(byte hours, byte minutes, byte seconds)
```

#### Parameters

- hours: the hours value to be set.

- minutes: the minutes value to be set.

- seconds: the seconds value to be set.
### `setYear()`
Set the RTC year.

#### Syntax

``` Arduino
rtc.setYear(byte year)
```

#### Parameters

- year: the year value to be set.

### `setMonth()`

Set the RTC month.

#### Syntax

``` Arduino
rtc.setMonth(uint8_t month)
```

#### Parameters

- month: the month value to be set.

### `setDay()`

Set the RTC day.

#### Syntax

``` Arduino
rtc.setDay(byte day)
```

#### Parameters

- day: the day value to be set.

### `setDate()`

Set the RTC day, month and year.

#### Syntax

``` Arduino
rtc.setDate(byte day, byte month, byte year)
```

#### Parameters

- day: the day value to be set.

- month: the month value to be set.

- year: the year value to be set.

### `getHours()`

Get the RTC hours value.

#### Syntax

``` Arduino
rtc.getHours()
```

Return
the hours value.

### `getMinutes()`

Get the RTC minutes value.

#### Syntax

``` Arduino
rtc.getMinutes()
```

Return
the minutes value.

### `getSeconds()`

Get the RTC seconds value.

#### Syntax

``` Arduino
rtc.getSeconds()
```

Return
the seconds value.

### `getYear()`

Get the RTC Year value.

#### Syntax

``` Arduino
rtc.getYear()
```

Return
the year value.

### `getMonth()`

Get the RTC month value.

#### Syntax

``` Arduino
rtc.getMonth()
```

Return
the month value.
getDay()

Get the RTC day value.

#### Syntax

``` Arduino
rTC.getDay()
```

Return
the day value.

### `setAlarmHours()`

Set the RTC Alarm hours value.

#### Syntax

``` Arduino
rtc.setAlarmHours(uint8_t hours)
```

#### Parameters

- hours: the hours value to be set.

### `setAlarmMinutes()`

Set the RTC Alarm minutes value.

#### Syntax

``` Arduino
rtc.setAlarmMinutes(byte minutes)
```

#### Parameters

- minutes: the minutes value to be set.

### `setAlarmSeconds()`

Set the RTC Alarm seconds value.

#### Syntax

``` Arduino
rtc.setAlarmSeconds(byte seconds)
```

#### Parameters

- seconds: the seconds value to be set.

### `setAlarmTime()`

Set the RTC Alarm hours, minutes and seconds.

#### Syntax

``` Arduino
rtc.setAlarmTime(byte hours, byte minutes, byte seconds)
```

#### Parameters

- hours: the hours value to be set.

- minutes: the minutes value to be set.

- seconds: the seconds value to be set.

### `setAlarmYear()`

Set the RTC Alarm year.

#### Syntax

``` Arduino
rtc.setAlarmYear(byte year)
```

#### Parameters

- year: the year value to be set.

### `setAlarmMonth()`

Set the RTC Alarm month.

#### Syntax

``` Arduino
rtc.setAlarmMonth(byte month)
```

#### Parameters

- month: the month value to be set.

### `setAlarmDay()`

Set the RTC Alarm day.

#### Syntax

``` Arduino
rtc.setAlarmDay(uint8_t day)
```

#### Parameters

- day: the day value to be set.

### `setAlarmDate()`

Set the RTC Alarm day, month and year.

#### Syntax

``` Arduino
rtc.setAlarmDate(byte day, byte month, byte year)
```

#### Parameters

- day: the day value to be set.

- month: the month value to be set.

- year: the year value to be set.

### `enableAlarm()`

Enable the RTC alarm to be triggered on a chosen match type

#### Syntax

``` Arduino
rtc.enableAlarm(rtc.AlarmMatch)
```

#### Parameters

- AlarmMatch: the chosen match type that can be chosen between:

  - MATCH_SS: generate an alarm on seconds match;
  - MATCH_MMSS: generate an alarm on minutes and seconds match;
  - MATCH_HHMMSS: generate an alarm on hours, minutes and seconds match;
  - MATCH_DHHMMSS: generate an alarm on day, hours, minutes and seconds match;
  - MATCH_MMDDHHMMSS: generate an alarm on month, day, hours, minutes and seconds match;
  - MATCH_YYMMDDHHMMSS: generate an alarm on year, month, day, hours, minutes and seconds match;

### `disableAlarm()`

Disable the RTC alarm triggering.

#### Syntax

``` Arduino
rtc.disableAlarm()
```

#### Parameters

none

### `attachInterrupt()`

Attach an interrupt action on alarm match.

#### Syntax

``` Arduino
rtc.attachInterrupt(voidFuncPtr callback)
```

#### Parameters

- callback: The name of the function to be called on alarm match

### `detachInterrupt()`

Detach interrupt action on alarm match.

#### Syntax

``` Arduino
rtc.detachInterrupt()
```

#### Parameters

none

### `standbyMode()`

Send the board in standby mode.

#### Syntax

``` Arduino
rtc.standbyMode()
```


#### Parameters

None
