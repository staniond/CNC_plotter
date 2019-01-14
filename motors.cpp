#include <Arduino.h>
#include "motors.h"
#include "led.h"

Motor motor1;
Motor motor2;

int motorSpeed;
int motorDelay;

void motor_setup(){
  motor1.enabled = 33;
  motor1.step = 25;
  motor1.dir = 26;
  
  motor2.enabled = 27;
  motor2.step = 14;
  motor2.dir = 12;

  pinMode(motor1.step, OUTPUT);
  pinMode(motor1.enabled, OUTPUT);
  pinMode(motor1.dir, OUTPUT);
  
  pinMode(motor2.step, OUTPUT);
  pinMode(motor2.enabled, OUTPUT);
  pinMode(motor2.dir, OUTPUT);

  digitalWrite(motor1.enabled, HIGH); //turn off power to motors by default
  digitalWrite(motor2.enabled, HIGH);

  motorSetSpeed(35);
}

void motor_step(int steps){
  digitalWrite(motor1.dir, steps > 0 ? LOW:HIGH);
  digitalWrite(motor2.dir, steps > 0 ? LOW:HIGH);
  
  if(steps < 0){
    steps = -steps;
  }
  
  for(int i = 0; i < steps; i++) {
    digitalWrite(motor1.step,HIGH);
    digitalWrite(motor2.step,HIGH);
    delayMicroseconds(5);
    digitalWrite(motor1.step,LOW);
    digitalWrite(motor2.step,LOW);
    delayMicroseconds(motorDelay-5);
  }
}

void motor_power(bool on){
  led_light(on?YELLOW:GREEN);
  digitalWrite(motor1.enabled, !on);  //ENABLE pin needs to be low to turn off power
  digitalWrite(motor2.enabled, !on);
  delayMicroseconds(5);
}
  

void motorSetSpeed(int speed){
  motorSpeed = speed;
  motorDelay = (60*100000)/(speed * stepsPerRevolution * 16);
}
