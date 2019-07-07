/*
   Example Arduino code to trigger an alarm at 1600hrs, i.e., at 4PM everyday.
 */
#include "ds3231.h"
#include <Wire.h>

#define interruptpin 2

class ds3231 rtc;
volatile byte flag=false;

void setup() {
  Wire.begin();
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,false);
  pinMode(interruptpin,INPUT_PULLUP);     // Setup interrupt pin
  digitalWrite(interruptpin,true);
  attachInterrupt(digitalPinToInterrupt(interruptpin),isr_a1,FALLING);  // setup interrupt
  rtc.init();
  rtc.set_alarm1(hr_min_sec_match,0x00,0x00,0x16);   // Setup alarm 1 to trigger once everyday at the given time. (0x16 due to the use of 24 hr format)
  rtc.arm_alarm1();                                  // arm alarm 1
}

void loop() {
  digitalWrite(LED_BUILTIN,flag);  // LED changing state indicates triggering of the alarm
  rtc.clear_alarm1();
}

void isr_a1()
{
  flag=!flag;
}
