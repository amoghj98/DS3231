/*
   Example Arduino code to generate a square wave of period 1s using interrupts
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
  rtc.set_alarm1(every_second);     // set alarm 1 to trigger once every second
  rtc.arm_alarm1();                 // arm alarm 1
}

void loop() {
  digitalWrite(LED_BUILTIN,flag);  // LED changing state indicates triggering of the interrupt
  rtc.clear_alarm1();
}

void isr_a1()
{
  flag=!flag;
}
