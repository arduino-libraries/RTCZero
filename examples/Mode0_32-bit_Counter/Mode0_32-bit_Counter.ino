/*
  RTC counter for SAMD21 controllers (Arduino Zero, MKR1000, Adafruit Feather M0)

  Demonstrates the use of the RTC library Mode 0 (32-bit counter) for SAMD21 controllers
  This example starts a counter that interrupts and resets after 10 seconds* since
  "reset on match" is set to true.
  
  (*seconds, based on GCLK_RTC = 1.024khz and default 1/1024 prescaler)

  This example code is in the public domain

  created by A. McMahon
  20 Feb 2020
*/

#include <RTCZero.h>

/* Create an rtc object */
RTCZero rtc;

void setup()
{
  Serial.begin(9600);

  rtc.begin(true, 0, true);                     // initialize RTC: reset starting value, Mode 0 (32-bit counter), reset on match
  rtc.enableCounter(10);                        // set counter compare value
  rtc.attachInterrupt(countDone);               // attach interrupt
}

void loop()
{
  Serial.println(rtc.getCount());               // print the current count
  delay(1000);
}

void countDone()                                // interrupt when compare value is reached
{                              
  Serial.println("Reset!");
}
