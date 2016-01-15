/*
  Multiple RTC Alarm for Arduino Zero

  Demonstrates multiple use of alarms using the RTC library for the Arduino Zero

  This example code is in the public domain

  http://arduino.cc/en/Tutorial/MultipleRTCAlarm

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  15 Jan 2016
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 16;

/* Change these values to set the current initial date */
const byte day = 25;
const byte month = 9;
const byte year = 15;

void setup()
{
  Serial.begin(9600);

  rtc.begin(); // initialize RTC 24H format

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.addAlarm(16, 0, 10, 0, 0, 0, rtc.MATCH_HHMMSS, alarmMatch1);
  rtc.addAlarm(16, 0, 20, 0, 0, 0, rtc.MATCH_HHMMSS, alarmMatch2);
  rtc.addAlarm(16, 0, 30, 0, 0, 0, rtc.MATCH_HHMMSS, alarmMatch3);

  rtc.updateAlarms();

  delay(3000);
  Serial.println("First alarm is:");
  Serial.print(RTC->MODE2.Mode2Alarm[0].ALARM.bit.HOUR);
  Serial.print(RTC->MODE2.Mode2Alarm[0].ALARM.bit.MINUTE);
  Serial.println(RTC->MODE2.Mode2Alarm[0].ALARM.bit.SECOND);
}

void loop()
{

}

void alarmMatch1()
{
  Serial.println("Alarm Match1!");
  rtc.updateAlarms();
  Serial.println("Next alarm is");
  Serial.print(rtc.getAlarmHours());
  Serial.print(":");
  Serial.print(rtc.getAlarmMinutes());
  Serial.print(":");
  Serial.println(rtc.getAlarmSeconds());
}

void alarmMatch2()
{
  Serial.println("Alarm Match2!");
  rtc.updateAlarms();
  Serial.println("Next alarm is");
  Serial.print(rtc.getAlarmHours());
  Serial.print(":");
  Serial.print(rtc.getAlarmMinutes());
  Serial.print(":");
  Serial.println(rtc.getAlarmSeconds());
}

void alarmMatch3()
{
  Serial.println("Alarm Match3!");
  rtc.updateAlarms();
  Serial.println("Next alarm is");
  Serial.println("Next alarm is");
  Serial.print(rtc.getAlarmHours());
  Serial.print(":");
  Serial.print(rtc.getAlarmMinutes());
  Serial.print(":");
  Serial.println(rtc.getAlarmSeconds());
}
