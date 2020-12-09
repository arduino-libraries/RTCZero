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

#include <time.h>

#include "RTCZero.h"

#define EPOCH_TIME_OFF      946684800  // This is 1st January 2000, 00:00:00 in epoch time
#define EPOCH_TIME_YEAR_OFF 100        // years since 1900

// Default date & time after reset
#define DEFAULT_YEAR    2000    // 2000..2063
#define DEFAULT_MONTH   1       // 1..12
#define DEFAULT_DAY     1       // 1..31
#define DEFAULT_HOUR    0       // 1..23
#define DEFAULT_MINUTE  0       // 0..59
#define DEFAULT_SECOND  0       // 0..59

voidFuncPtr RTC_callBack = NULL;

RTCZero::RTCZero()
{
  _configured = false;
}

void RTCZero::begin(bool resetTime)
{
#ifdef __SAMD51__
  MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC;
#else // SAMD21
  PM->APBAMASK.reg |= PM_APBAMASK_RTC; // turn on digital interface clock
#endif

  config32kOSC();

  // If the RTC is in clock mode and the reset was
  // not due to POR or BOD, preserve the clock time
  // POR causes a reset anyway, BOD behaviour is?
  bool validTime = false;
  RTC_MODE2_CLOCK_Type oldTime; // same for SAMD21 and SAMD51

  if ((!resetTime) && isHotReset() && RTCisClock()) {
    validTime = true;
    oldTime.reg = RTC->MODE2.CLOCK.reg;
  }

  configureClock();

  RTCdisable();

  RTCreset();

  RTCconfigureRegs();

  NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt 
  NVIC_SetPriority(RTC_IRQn, 0x00);

  // same for SAMD21 and SAMD51
  RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0; // enable alarm interrupt
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = MATCH_OFF; // default alarm match is off (disabled)
  
  while (RTCisSyncing())
    ;

  RTCenable();
  RTCresetRemove();

  // If desired and valid, restore the time value, else use first valid time value
  if ((!resetTime) && (validTime) && (oldTime.reg != 0L)) {
    RTC->MODE2.CLOCK.reg = oldTime.reg; // same for SAMD21 and SAMD51
  }
  else {
    // same for SAMD21 and SAMD51
    RTC->MODE2.CLOCK.reg = RTC_MODE2_CLOCK_YEAR(DEFAULT_YEAR - 2000) | RTC_MODE2_CLOCK_MONTH(DEFAULT_MONTH) 
        | RTC_MODE2_CLOCK_DAY(DEFAULT_DAY) | RTC_MODE2_CLOCK_HOUR(DEFAULT_HOUR) 
        | RTC_MODE2_CLOCK_MINUTE(DEFAULT_MINUTE) | RTC_MODE2_CLOCK_SECOND(DEFAULT_SECOND);
  }
  while (RTCisSyncing())
    ;

  _configured = true;
}

void RTC_Handler(void)
{
  if (RTC_callBack != NULL) {
    RTC_callBack();
  }

  // same for SAMD21 and SAMD51
  RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0; // must clear flag at end
}

void RTCZero::enableAlarm(Alarm_Match match)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = match; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::disableAlarm()
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = 0x00; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::attachInterrupt(voidFuncPtr callback)
{
  RTC_callBack = callback;
}

void RTCZero::detachInterrupt()
{
  RTC_callBack = NULL;
}

void RTCZero::standbyMode()
{
  // Entering standby mode when connected
  // via the native USB port causes issues.
  // same for SAMD21 and SAMD51
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB();
  __WFI();
}

/*
 * Get Functions
 */

uint8_t RTCZero::getSeconds()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.SECOND; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getMinutes()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.MINUTE; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getHours()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.HOUR; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getDay()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.DAY; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getMonth()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.MONTH; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getYear()
{
  RTCreadRequest();
  return RTC->MODE2.CLOCK.bit.YEAR; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmSeconds()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmMinutes()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmHours()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmDay()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.DAY; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmMonth()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MONTH; // same for SAMD21 and SAMD51
}

uint8_t RTCZero::getAlarmYear()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.YEAR; // same for SAMD21 and SAMD51
}

/*
 * Set Functions
 */

void RTCZero::setSeconds(uint8_t seconds)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.SECOND = seconds; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setMinutes(uint8_t minutes)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.MINUTE = minutes; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setHours(uint8_t hours)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.HOUR = hours; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  if (_configured) {
    setSeconds(seconds);
    setMinutes(minutes);
    setHours(hours);
  }
}

void RTCZero::setDay(uint8_t day)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.DAY = day; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setMonth(uint8_t month)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.MONTH = month; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setYear(uint8_t year)
{
  if (_configured) {
    RTC->MODE2.CLOCK.bit.YEAR = year; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setDate(uint8_t day, uint8_t month, uint8_t year)
{
  if (_configured) {
    setDay(day);
    setMonth(month);
    setYear(year);
  }
}

void RTCZero::setAlarmSeconds(uint8_t seconds)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND = seconds; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmMinutes(uint8_t minutes)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE = minutes; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmHours(uint8_t hours)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR = hours; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  if (_configured) {
    setAlarmSeconds(seconds);
    setAlarmMinutes(minutes);
    setAlarmHours(hours);
  }
}

void RTCZero::setAlarmDay(uint8_t day)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.DAY = day; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmMonth(uint8_t month)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.MONTH = month; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmYear(uint8_t year)
{
  if (_configured) {
    RTC->MODE2.Mode2Alarm[0].ALARM.bit.YEAR = year; // same for SAMD21 and SAMD51
    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setAlarmDate(uint8_t day, uint8_t month, uint8_t year)
{
  if (_configured) {
    setAlarmDay(day);
    setAlarmMonth(month);
    setAlarmYear(year);
  }
}

time_t RTCZero::getEpoch()
{
  RTCreadRequest();
  RTC_MODE2_CLOCK_Type clockTime;
  clockTime.reg = RTC->MODE2.CLOCK.reg; // same for SAMD21 and SAMD51

  struct tm tm;

  tm.tm_isdst = -1;
  tm.tm_yday = 0;
  tm.tm_wday = 0;
  tm.tm_year = clockTime.bit.YEAR + EPOCH_TIME_YEAR_OFF;
  tm.tm_mon = clockTime.bit.MONTH - 1;
  tm.tm_mday = clockTime.bit.DAY;
  tm.tm_hour = clockTime.bit.HOUR;
  tm.tm_min = clockTime.bit.MINUTE;
  tm.tm_sec = clockTime.bit.SECOND;

  return mktime(&tm);
}

uint32_t RTCZero::getY2kEpoch()
{
  return (getEpoch() - EPOCH_TIME_OFF);
}

void RTCZero::setAlarmEpoch(uint32_t ts)
{
  if (_configured) {
    if (ts < EPOCH_TIME_OFF) {
      ts = EPOCH_TIME_OFF;
    }

    time_t t = ts;
    struct tm tmp;

    gmtime_r(&t, &tmp);
    setAlarmDate(tmp.tm_mday, tmp.tm_mon + 1, tmp.tm_year - EPOCH_TIME_YEAR_OFF);
    setAlarmTime(tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
  }
}

void RTCZero::setEpoch(uint32_t ts)
{
  if (_configured) {
    if (ts < EPOCH_TIME_OFF) {
      ts = EPOCH_TIME_OFF;
    }

    time_t t = ts;
    struct tm tmp;

    gmtime_r(&t, &tmp);

    RTC_MODE2_CLOCK_Type clockTime; // same for SAMD21 and SAMD51

    clockTime.bit.YEAR = tmp.tm_year - EPOCH_TIME_YEAR_OFF;
    clockTime.bit.MONTH = tmp.tm_mon + 1;
    clockTime.bit.DAY = tmp.tm_mday;
    clockTime.bit.HOUR = tmp.tm_hour;
    clockTime.bit.MINUTE = tmp.tm_min;
    clockTime.bit.SECOND = tmp.tm_sec;

    RTC->MODE2.CLOCK.reg = clockTime.reg; // same for SAMD21 and SAMD51

    while (RTCisSyncing())
      ;
  }
}

void RTCZero::setY2kEpoch(uint32_t ts)
{
  if (_configured) {
    setEpoch(ts + EPOCH_TIME_OFF);
  }
}

/* Attach peripheral clock to 32k oscillator */
void RTCZero::configureClock() {
#ifdef __SAMD51__

#ifdef CRYSTALLESS
  OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_ULP1K;
#else
  OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K;
#endif

#else // SAMD21

  // Setup clock GCLK2 with OSC32K divided by 32
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2)|GCLK_GENDIV_DIV(4);
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
    ;
#ifdef CRYSTALLESS
  GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_DIVSEL );
#else
  GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_DIVSEL );
#endif
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
    ;
  GCLK->CLKCTRL.reg = (uint32_t)((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos)));
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

#endif
}

/*
 * Private Utility Functions
 */

/* whether cpu rebooted from a reasonably good running state */
inline bool RTCZero::isHotReset(void) {
#ifdef __SAMD51__
  return (RSTC->RCAUSE.reg & (RSTC_RCAUSE_SYST | RSTC_RCAUSE_WDT | RSTC_RCAUSE_EXT));
#else // SAMD21
  return (PM->RCAUSE.reg & (PM_RCAUSE_SYST | PM_RCAUSE_WDT | PM_RCAUSE_EXT));
#endif
}

/* Configure the 32768Hz Oscillator */
void RTCZero::config32kOSC() 
{
#ifdef __SAMD51__

#ifdef CRYSTALLESS
  // enable 1k output
  OSC32KCTRL->OSCULP32K.reg |= OSC32KCTRL_OSCULP32K_EN1K;
#else
  // XOSC32K activated in startup.c but activating again in case USER-code stopped it
  // enable 1k output
  OSC32KCTRL->XOSC32K.reg = OSC32KCTRL_XOSC32K_ENABLE |
    OSC32KCTRL_XOSC32K_EN32K |
    OSC32KCTRL_XOSC32K_EN1K |
    OSC32KCTRL_XOSC32K_CGM_XT |
    OSC32KCTRL_XOSC32K_RUNSTDBY |
    OSC32KCTRL_XOSC32K_ONDEMAND |
    OSC32KCTRL_XOSC32K_XTALEN;
  while( (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY) == 0 )
    ;
#endif

#else // SAMD21

#ifndef CRYSTALLESS
  SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ONDEMAND |
                         SYSCTRL_XOSC32K_RUNSTDBY |
                         SYSCTRL_XOSC32K_EN32K |
                         SYSCTRL_XOSC32K_XTALEN |
                         SYSCTRL_XOSC32K_STARTUP(6) |
                         SYSCTRL_XOSC32K_ENABLE;
#endif

#endif
}

/* whether RTC is configured as a clock (MODE2) */
inline bool RTCZero::RTCisClock(void) {
#ifdef __SAMD51__
  return (RTC->MODE2.CTRLA.reg & RTC_MODE2_CTRLA_MODE_CLOCK);
#else // SAMD21
  return (RTC->MODE2.CTRL.reg & RTC_MODE2_CTRL_MODE_CLOCK);
#endif
}

/* configure the device registers */
inline void RTCZero::RTCconfigureRegs(void) {
#ifdef __SAMD51__
  uint16_t tmp_reg = 0;
  tmp_reg |= RTC_MODE2_CTRLA_MODE_CLOCK; // set clock operating mode
  tmp_reg |= RTC_MODE2_CTRLA_PRESCALER_DIV1024; // set prescaler to 1024 for MODE2
  tmp_reg &= ~RTC_MODE2_CTRLA_MATCHCLR; // disable clear on match
  //According to the datasheet RTC_MODE2_CTRLA_CLKREP = 0 for 24h
  tmp_reg &= ~RTC_MODE2_CTRLA_CLKREP; // 24h time representation
  tmp_reg |= RTC_MODE2_CTRLA_CLOCKSYNC; // synchronize reads

  RTC->MODE2.CTRLA.reg = tmp_reg;
#else // SAMD21
  uint16_t tmp_reg = 0;
  tmp_reg |= RTC_MODE2_CTRL_MODE_CLOCK; // set clock operating mode
  tmp_reg |= RTC_MODE2_CTRL_PRESCALER_DIV1024; // set prescaler to 1024 for MODE2
  tmp_reg &= ~RTC_MODE2_CTRL_MATCHCLR; // disable clear on match
  //According to the datasheet RTC_MODE2_CTRL_CLKREP = 0 for 24h
  tmp_reg &= ~RTC_MODE2_CTRL_CLKREP; // 24h time representation

  RTC->MODE2.READREQ.reg &= ~RTC_READREQ_RCONT; // disable continuously mode

  RTC->MODE2.CTRL.reg = tmp_reg;
#endif
  while (RTCisSyncing())
    ;
}

/* Synchronise the CLOCK register for reading*/
inline void RTCZero::RTCreadRequest() {
  if (_configured) {
#ifdef __SAMD51__
    // CTRLA.CLOCKSYNC set so no action needed here
#else // SAMD21
    RTC->MODE2.READREQ.reg = RTC_READREQ_RREQ;
#endif
    while (RTCisSyncing())
      ;
  }
}

/* Wait for sync in write operations */
inline bool RTCZero::RTCisSyncing()
{
#ifdef __SAMD51__
  return (RTC->MODE2.SYNCBUSY.reg);
#else // SAMD21
  return (RTC->MODE2.STATUS.bit.SYNCBUSY);
#endif
}

void RTCZero::RTCdisable()
{
#ifdef __SAMD51__
  RTC->MODE2.CTRLA.reg &= ~RTC_MODE2_CTRLA_ENABLE; // disable RTC
#else // SAMD21
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE; // disable RTC
#endif
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCenable()
{
#ifdef __SAMD51__
  RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_ENABLE; // enable RTC
#else // SAMD21
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE; // enable RTC
#endif
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCreset()
{
#ifdef __SAMD51__
  RTC->MODE2.CTRLA.reg |= RTC_MODE2_CTRLA_SWRST; // software reset
#else // SAMD21
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_SWRST; // software reset
#endif
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCresetRemove()
{
#ifdef __SAMD51__
  RTC->MODE2.CTRLA.reg &= ~RTC_MODE2_CTRLA_SWRST; // software reset remove
#else // SAMD21
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_SWRST; // software reset remove
#endif
  while (RTCisSyncing())
    ;
}
