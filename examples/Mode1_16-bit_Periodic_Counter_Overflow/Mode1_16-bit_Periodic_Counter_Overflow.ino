/*
  RTC counter for SAMD21 controllers (Arduino Zero, MKR1000, Adafruit Feather M0)

  Demonstrates the use of the RTC library Mode 1 (16-bit periodic counter) for SAMD21 controllers
  This example starts a counter that interrupts at 10 and 30 seconds* (for the compare values) 
  and at 60 (since the overflow interrupt is also enabled).  The count is then reset after 60
  since that is the set counter period. 

  (*seconds, based on GCLK_RTC = 1.024khz and default 1/1024 prescaler)

  This example code is in the public domain

  created by A. McMahon
  20 Feb 2020
*/

#include "RTCZero.h"

/* Create an rtc object */
RTCZero rtc;

void setup()
{
  Serial.begin(9600);

  rtc.begin(true, 1);                           // initialize RTC: reset starting value, Mode 1 (16-bit counter)
  rtc.enableCounter(10, 30);                    // set counter compare values (interrupt at 10 and 30)
  rtc.setPeriod(60);                            // set counter period
  rtc.enableOverflow();                         // enable interrupt on overflow
  rtc.attachInterrupt(countInt);                // attach interrupt
}

void loop()
{
  Serial.println(rtc.getCount());               // print the current count
  delay(1000);
}

void countInt()                                 // interrupt when compare value is reached
{
  uint8_t source;
  source = rtc.getIntSource();                  // check what caused the interrupt

  if (source == rtc.INT_COMP0) Serial.println("Count = Compare 0!");
  if (source == rtc.INT_COMP1) Serial.println("Count = Compare 1!");
  if (source == rtc.INT_OVERFLOW) Serial.println("Overflow!");
} 
