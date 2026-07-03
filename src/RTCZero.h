/*
  RTC library for Arduino Zero.
  Copyright (c) 2015 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef RTC_ZERO_H
#define RTC_ZERO_H

#include "Arduino.h"

typedef void(*voidFuncPtr)(void);

class RTCZero {
public:

  enum Alarm_Match: uint8_t // Should we have this enum or just use the identifiers from /component/rtc.h ?
  {
    MATCH_OFF          = RTC_MODE2_MASK_SEL_OFF_Val,          // Never
    MATCH_SS           = RTC_MODE2_MASK_SEL_SS_Val,           // Every Minute
    MATCH_MMSS         = RTC_MODE2_MASK_SEL_MMSS_Val,         // Every Hour
    MATCH_HHMMSS       = RTC_MODE2_MASK_SEL_HHMMSS_Val,       // Every Day
    MATCH_DHHMMSS      = RTC_MODE2_MASK_SEL_DDHHMMSS_Val,     // Every Month
    MATCH_MMDDHHMMSS   = RTC_MODE2_MASK_SEL_MMDDHHMMSS_Val,   // Every Year
    MATCH_YYMMDDHHMMSS = RTC_MODE2_MASK_SEL_YYMMDDHHMMSS_Val  // Once, on a specific date and a specific time
  };

  enum Prescaler: uint32_t // Same question as above applies
  {
    None = 0xffff,
    MODE0_DIV1 = RTC_MODE0_CTRL_PRESCALER_DIV1,
    MODE0_DIV2 = RTC_MODE0_CTRL_PRESCALER_DIV2,
    MODE0_DIV4 = RTC_MODE0_CTRL_PRESCALER_DIV4,
    MODE0_DIV8 = RTC_MODE0_CTRL_PRESCALER_DIV8,
    MODE0_DIV16 = RTC_MODE0_CTRL_PRESCALER_DIV16,
    MODE0_DIV32 = RTC_MODE0_CTRL_PRESCALER_DIV32,
    MODE0_DIV64 = RTC_MODE0_CTRL_PRESCALER_DIV64,
    MODE0_DIV128 = RTC_MODE0_CTRL_PRESCALER_DIV128,
    MODE0_DIV256 = RTC_MODE0_CTRL_PRESCALER_DIV256,
    MODE0_DIV512 = RTC_MODE0_CTRL_PRESCALER_DIV512,
    MODE0_DIV1024 = RTC_MODE0_CTRL_PRESCALER_DIV1024,

    MODE1_DIV1 = RTC_MODE1_CTRL_PRESCALER_DIV1,
    MODE1_DIV2 = RTC_MODE1_CTRL_PRESCALER_DIV2,
    MODE1_DIV4 = RTC_MODE1_CTRL_PRESCALER_DIV4,
    MODE1_DIV8 = RTC_MODE1_CTRL_PRESCALER_DIV8,
    MODE1_DIV16 = RTC_MODE1_CTRL_PRESCALER_DIV16,
    MODE1_DIV32 = RTC_MODE1_CTRL_PRESCALER_DIV32,
    MODE1_DIV64 = RTC_MODE1_CTRL_PRESCALER_DIV64,
    MODE1_DIV128 = RTC_MODE1_CTRL_PRESCALER_DIV128,
    MODE1_DIV256 = RTC_MODE1_CTRL_PRESCALER_DIV256,
    MODE1_DIV512 = RTC_MODE1_CTRL_PRESCALER_DIV512,
    MODE1_DIV1024 = RTC_MODE1_CTRL_PRESCALER_DIV1024,

    MODE2_DIV1 = RTC_MODE2_CTRL_PRESCALER_DIV1,
    MODE2_DIV2 = RTC_MODE2_CTRL_PRESCALER_DIV2,
    MODE2_DIV4 = RTC_MODE2_CTRL_PRESCALER_DIV4,
    MODE2_DIV8 = RTC_MODE2_CTRL_PRESCALER_DIV8,
    MODE2_DIV16 = RTC_MODE2_CTRL_PRESCALER_DIV16,
    MODE2_DIV32 = RTC_MODE2_CTRL_PRESCALER_DIV32,
    MODE2_DIV64 = RTC_MODE2_CTRL_PRESCALER_DIV64,
    MODE2_DIV128 = RTC_MODE2_CTRL_PRESCALER_DIV128,
    MODE2_DIV256 = RTC_MODE2_CTRL_PRESCALER_DIV256,
    MODE2_DIV512 = RTC_MODE2_CTRL_PRESCALER_DIV512,
    MODE2_DIV1024 = RTC_MODE2_CTRL_PRESCALER_DIV1024
  };

  enum Int_Source: uint8_t  // Helper to make interrupts simpler 
  {
    INT_COMP0 = 0,
    INT_COMP1 = 1,
    INT_ALARM0 = 2,
    INT_OVERFLOW = 3,
    INT_UNKNOWN = 4
  };

  RTCZero();
  void begin(bool resetTime = false, uint8_t rtc_mode = 2, bool clearOnMatch = false, Prescaler prescale = None);

  void enableAlarm(Alarm_Match match);
  void disableAlarm();

  void enableCounter(unsigned long comp0);
  void enableCounter(unsigned int comp0, unsigned int comp1);
  void disableCounter();

  void enableOverflow();
  void disableOverflow();

  void attachInterrupt(voidFuncPtr callback);
  void detachInterrupt();
  
  void standbyMode();
  
  /* Get Functions */

  uint8_t getIntSource();

  uint32_t getCount();

  uint32_t getCompare();
  uint16_t getCompare(uint8_t c);

  uint8_t getSeconds();
  uint8_t getMinutes();
  uint8_t getHours();
  
  uint8_t getDay();
  uint8_t getMonth();
  uint8_t getYear();
  
  uint8_t getAlarmSeconds();
  uint8_t getAlarmMinutes();
  uint8_t getAlarmHours();

  uint8_t getAlarmDay();
  uint8_t getAlarmMonth();
  uint8_t getAlarmYear();

  /* Set Functions */

  void setCount(uint32_t count);
  void setCount(uint16_t count);

  void setPeriod(uint16_t period);

  void setSeconds(uint8_t seconds);
  void setMinutes(uint8_t minutes);
  void setHours(uint8_t hours);
  void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

  void setDay(uint8_t day);
  void setMonth(uint8_t month);
  void setYear(uint8_t year);
  void setDate(uint8_t day, uint8_t month, uint8_t year);

  void setAlarmSeconds(uint8_t seconds);
  void setAlarmMinutes(uint8_t minutes);
  void setAlarmHours(uint8_t hours);
  void setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

  void setAlarmDay(uint8_t day);
  void setAlarmMonth(uint8_t month);
  void setAlarmYear(uint8_t year);
  void setAlarmDate(uint8_t day, uint8_t month, uint8_t year);

  /* Epoch Functions */

  time_t getEpoch();
  uint32_t getY2kEpoch();
  void setEpoch(uint32_t ts);
  void setY2kEpoch(uint32_t ts);
  void setAlarmEpoch(uint32_t ts);

  bool isConfigured() {
    return _configured;
  }

private:
  bool _configured;

  void config32kOSC(void);
  void configureClock(void);
  void RTCreadRequest();
  bool RTCisSyncing(void);
  void RTCdisable();
  void RTCenable();
  void RTCreset();
  void RTCresetRemove();
};

#endif // RTC_ZERO_H
