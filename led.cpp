#include <Arduino.h>
#include "led.h"

void led_setup(){
  pinMode(rLedPin, OUTPUT);
  pinMode(yLedPin, OUTPUT);
  pinMode(gLedPin, OUTPUT);
}

void led_light(enum Led led){
  digitalWrite(rLedPin, LOW);
  digitalWrite(yLedPin, LOW);
  digitalWrite(gLedPin, LOW);
  
  if(led == 0){
    return;
  }

  digitalWrite(led, HIGH);
}
