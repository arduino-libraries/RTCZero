/*
  Simple RTC Alarm for Arduino Zero

  Demonstrates the use of alarms using the RTC library for the Arduino Zero

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/SimpleRTCAlarm

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  25 Sept 2015
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const uint8_t seconds = 0;
const uint8_t minutes = 0;
const uint8_t hours = 17;

/* Change these values to set the current initial date */
const uint8_t day = 25;
const uint8_t month = 9;
const uint8_t year = 15;

void setup()
{
  Serial.begin(9600);

  rtc.begin(H24); // initialize RTC 24H format. The dual option is H12

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(16, 0, 10);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  
  rtc.attachInterrupt(alarmMatch);
}

void loop()
{

}

void alarmMatch()
{
  Serial.println("Alarm Match!");
}
