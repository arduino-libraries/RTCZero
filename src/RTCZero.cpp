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

voidFuncPtr RTC_callBack = NULL;

void RTCZero::begin() 
{
  uint16_t tmp_reg = 0;
  
  PM->APBAMASK.reg |= PM_APBAMASK_RTC; // turn on digital interface clock
  config32kOSC();

  // Setup clock GCLK2 with OSC32K divided by 32
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2)|GCLK_GENDIV_DIV(4);
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
    ;
  GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_DIVSEL );
  while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
    ;
  GCLK->CLKCTRL.reg = (uint32_t)((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos)));
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  RTCdisable();

  RTCreset();

  tmp_reg |= RTC_MODE2_CTRL_MODE_CLOCK; // set clock operating mode
  tmp_reg |= RTC_MODE2_CTRL_PRESCALER_DIV1024; // set prescaler to 1024 for MODE2
  tmp_reg &= ~RTC_MODE2_CTRL_MATCHCLR; // disable clear on match
  
  //According to the datasheet RTC_MODE2_CTRL_CLKREP = 0 for 24h
  tmp_reg &= ~RTC_MODE2_CTRL_CLKREP; // 24h time representation

  RTC->MODE2.READREQ.reg &= ~RTC_READREQ_RCONT; // disable continuously mode

  RTC->MODE2.CTRL.reg = tmp_reg;
  while (RTCisSyncing())
    ;

  NVIC_EnableIRQ(RTC_IRQn); // enable RTC interrupt 
  NVIC_SetPriority(RTC_IRQn, 0x00);

  RTC->MODE2.INTENSET.reg |= RTC_MODE2_INTENSET_ALARM0; // enable alarm interrupt
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = MATCH_OFF; // default alarm match is off (disabled)
  
  while (RTCisSyncing())
    ;

  RTCenable();
  RTCresetRemove();
}

void RTC_Handler(void)
{
  if (RTC_callBack != NULL) {
    RTC_callBack();
  }

  RTC->MODE2.INTFLAG.reg = RTC_MODE2_INTFLAG_ALARM0; // must clear flag at end
}

void RTCZero::enableAlarm(Alarm_Match match)
{
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = match;
  while (RTCisSyncing())
    ;
}

void RTCZero::disableAlarm()
{
  RTC->MODE2.Mode2Alarm[0].MASK.bit.SEL = 0x00;
  while (RTCisSyncing())
    ;
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
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __WFI();
}

/*
 * Get Functions
 */

uint8_t RTCZero::getSeconds()
{
  return RTC->MODE2.CLOCK.bit.SECOND;
}

uint8_t RTCZero::getMinutes()
{
  return RTC->MODE2.CLOCK.bit.MINUTE;
}

uint8_t RTCZero::getHours()
{
  return RTC->MODE2.CLOCK.bit.HOUR;
}

uint8_t RTCZero::getDay()
{
  return RTC->MODE2.CLOCK.bit.DAY;
}

uint8_t RTCZero::getMonth()
{
  return RTC->MODE2.CLOCK.bit.MONTH;
}

uint8_t RTCZero::getYear()
{
  return RTC->MODE2.CLOCK.bit.YEAR;
}

uint8_t RTCZero::getAlarmSeconds()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND;
}

uint8_t RTCZero::getAlarmMinutes()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE;
}

uint8_t RTCZero::getAlarmHours()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR;
}

uint8_t RTCZero::getAlarmDay()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.DAY;
}

uint8_t RTCZero::getAlarmMonth()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.MONTH;
}

uint8_t RTCZero::getAlarmYear()
{
  return RTC->MODE2.Mode2Alarm[0].ALARM.bit.YEAR;
}

/*
 * Set Functions
 */

void RTCZero::setSeconds(uint8_t seconds)
{
  RTC->MODE2.CLOCK.bit.SECOND = seconds;
  while (RTCisSyncing())
    ;
}

void RTCZero::setMinutes(uint8_t minutes)
{
  RTC->MODE2.CLOCK.bit.MINUTE = minutes;
  while (RTCisSyncing())
    ;
}

void RTCZero::setHours(uint8_t hours)
{
  RTC->MODE2.CLOCK.bit.HOUR = hours;
  while (RTCisSyncing())
    ;
}

void RTCZero::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  setSeconds(seconds);
  setMinutes(minutes);
  setHours(hours);
}

void RTCZero::setDay(uint8_t day)
{
  RTC->MODE2.CLOCK.bit.DAY = day;
  while (RTCisSyncing())
    ;
}

void RTCZero::setMonth(uint8_t month)
{
  RTC->MODE2.CLOCK.bit.MONTH = month;
  while (RTCisSyncing())
    ;
}

void RTCZero::setYear(uint8_t year)
{
  RTC->MODE2.CLOCK.bit.YEAR = year;
  while (RTCisSyncing())
    ;
}

void RTCZero::setDate(uint8_t day, uint8_t month, uint8_t year)
{
  setDay(day);
  setMonth(month);
  setYear(year);
}

void RTCZero::setAlarmSeconds(uint8_t seconds)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND = seconds;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmMinutes(uint8_t minutes)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE = minutes;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmHours(uint8_t hours)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR = hours;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
  setAlarmSeconds(seconds);
  setAlarmMinutes(minutes);
  setAlarmHours(hours);
}

void RTCZero::setAlarmDay(uint8_t day)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.DAY = day;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmMonth(uint8_t month)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.MONTH = month;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmYear(uint8_t year)
{
  RTC->MODE2.Mode2Alarm[0].ALARM.bit.YEAR = year;
  while (RTCisSyncing())
    ;
}

void RTCZero::setAlarmDate(uint8_t day, uint8_t month, uint8_t year)
{
  setAlarmDay(day);
  setAlarmMonth(month);
  setAlarmYear(year);
}

uint32_t RTCZero::getEpoch()
{
  struct tm tm;

  tm.tm_isdst = -1;
  tm.tm_yday = 0;
  tm.tm_wday = 0;
  tm.tm_year = getYear() + EPOCH_TIME_YEAR_OFF;
  tm.tm_mon = getMonth() - 1;
  tm.tm_mday = getDay();
  tm.tm_hour = getHours();
  tm.tm_min = getMinutes();
  tm.tm_sec = getSeconds();

  return mktime(&tm);
}

uint32_t RTCZero::getY2kEpoch()
{
  return (getEpoch() - EPOCH_TIME_OFF);
}

void RTCZero::setEpoch(uint32_t ts)
{
  if (ts < EPOCH_TIME_OFF) {
    ts = EPOCH_TIME_OFF;
  }

  time_t t = ts;
  struct tm* tmp = gmtime(&t);

  RTC->MODE2.CLOCK.bit.YEAR = tmp->tm_year - EPOCH_TIME_YEAR_OFF;
  RTC->MODE2.CLOCK.bit.MONTH = tmp->tm_mon + 1;
  RTC->MODE2.CLOCK.bit.DAY = tmp->tm_mday;
  RTC->MODE2.CLOCK.bit.HOUR = tmp->tm_hour;
  RTC->MODE2.CLOCK.bit.MINUTE = tmp->tm_min;
  RTC->MODE2.CLOCK.bit.SECOND = tmp->tm_sec;

  while (RTCisSyncing())
    ;
}

void RTCZero::setY2kEpoch(uint32_t ts)
{
  setEpoch(ts + EPOCH_TIME_OFF);
}

/*
 * Private Utility Functions
 */

/* Configure the 32768Hz Oscillator */
void RTCZero::config32kOSC() 
{
  SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ONDEMAND |
                         SYSCTRL_XOSC32K_RUNSTDBY |
                         SYSCTRL_XOSC32K_EN32K |
                         SYSCTRL_XOSC32K_XTALEN |
                         SYSCTRL_XOSC32K_STARTUP(6) |
                         SYSCTRL_XOSC32K_ENABLE;
}

/* Wait for sync in write operations */
bool RTCZero::RTCisSyncing()
{
  return (RTC->MODE2.STATUS.bit.SYNCBUSY);
}

void RTCZero::RTCdisable()
{
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_ENABLE; // disable RTC
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCenable()
{
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_ENABLE; // enable RTC
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCreset()
{
  RTC->MODE2.CTRL.reg |= RTC_MODE2_CTRL_SWRST; // software reset
  while (RTCisSyncing())
    ;
}

void RTCZero::RTCresetRemove()
{
  RTC->MODE2.CTRL.reg &= ~RTC_MODE2_CTRL_SWRST; // software reset remove
  while (RTCisSyncing())
    ;
}
