/*
 Arduino code to put the Arduino to sleep and wake it up at 1AM everyday.
 The builtin LED on pin 13 blinks twice with a 500ms period while going to sleep and with a 2s period after waking up.
 ***NOTE : This code works only on AVR based boards such as the Arduino Uno, Mega, etc.***
 */

#include <avr/sleep.h>
#include <Wire.h>
#include "ds3231.h"

#define interruptpin 2

class ds3231 rtc;

volatile byte flag=false;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  pinMode(LED_BUILTIN,OUTPUT);                                // initialise digital pin 13
  digitalWrite(LED_BUILTIN,false);
  pinMode(interruptpin,INPUT_PULLUP);                         // initialise interrupt pin
  digitalWrite(interruptpin,true);
  attachInterrupt(digitalPinToInterrupt(2),wakeup,FALLING);   // initialise interrupt
  rtc.init();
  rtc.set_alarm2(hr_min_match,0x00,0x01);                     // set alarm 2 to trigger at 1AM everyday
  rtc.arm_alarm2();                                           // arm alarm 2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);                        // set sleep mode to SLEEP_MODE_PWR_DOWN
  rtc.clear_alarm2();                                         // clear spurious alarm generated due to attachInterrupt function calling the isr(wakeup() in this case)
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag)
  {
    rtc.clear_alarm2();
    for(uint8_t i=0;i<2;i++)
    {
      digitalWrite(LED_BUILTIN,true);
      delay(1000);
      digitalWrite(LED_BUILTIN,false);
      delay(1000);
    }
    flag=false;
  }
  else
  {
    for(uint8_t i=0;i<2;i++)
    {
      digitalWrite(LED_BUILTIN,true);
      delay(250);
      digitalWrite(LED_BUILTIN,false);
      delay(250);
    }
    sleep_mode();                                             // put the Arduino to sleep
  }
}

void wakeup()
{
  flag=true;
}
