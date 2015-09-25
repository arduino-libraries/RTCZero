/*
  Simple RTC for Arduino Zero

 Demonstrates the use of the RTC library for the Arduino Zero

 This example code is in the public domain

 http://arduino.cc/en/Tutorial/SimpleRTC

  created by Arturo Guadalupi <a.guadalupi@arduino.cc>
  15 Jun 2015
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

/* Change these values to set the current initial time */
const uint8_t seconds = 0;
const uint8_t minutes = 0;
const uint8_t hours = 16;

/* Change these values to set the current initial date */
const uint8_t day = 15;
const uint8_t month = 6;
const uint8_t year = 15;

void setup()
{
  Serial.begin(9600);
  
  rtc.begin(H24); // initialize RTC 24H format. The dual option is H12
  
  // Set the time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);
  
  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);
  
  // you can use also
  //rtc.setTime(hours, minutes, seconds);
  //rtc.setDate(day, month, year);
}

void loop()
{
  // Print date...
  Serial.print(rtc.getDay());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getYear());
  Serial.print("\t");
  
  // ...and time
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
 
  Serial.println();
  
  delay(1000);
}
