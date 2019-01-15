#include <Arduino.h>
#include "CNC_plotter.h"
#include "motors.h"
#include "led.h"

Motor motor1;
Motor motor2;

int motorSpeed;
int motorDelay;
bool powerEnabled = 0;

int xPos = 0;
int yPos = 0;

void motorSetup(){
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

  motorSetSpeed();
}

void plotLine(double xPosMM, double yPosMM) {
  motorPower(HIGH);
  
  xPosMM = constrain(xPosMM, 0, maxRangeMM);
  yPosMM = constrain(yPosMM, 0, maxRangeMM);
  
  int newX = (int)(stepsPerMM)*xPosMM;
  int newY = (int)(stepsPerMM)*yPosMM;

  if(xPos>newX){
    digitalWrite(motor1.dir, HIGH);
  }else{
    digitalWrite(motor1.dir, LOW);
  }
  if(yPos>newY){
    digitalWrite(motor2.dir, HIGH);
  }else{
    digitalWrite(motor2.dir, LOW);
  }
  
  if (abs(newY - yPos) < abs(newX - xPos)){
    if (xPos > newX){
        lineLow(newX, newY, xPos, yPos);
    }
    else{
        lineLow(xPos, yPos, newX, newY);
    }
  }
  else{
      if (yPos > newY){;
          lineHigh(newX, newY, xPos, yPos);
      }
      else{
          lineHigh(xPos, yPos, newX, newY);
      }
  }
  motorPower(LOW);
  xPos = newX;
  yPos = newY;
  Logln(String("New pos - ") + String(xPos) + ", " + String(yPos));
}

void lineHigh(int x0, int y0, int x1, int y1){
  int dx = x1 - x0;
  int dy = y1 - y0;
  int xi = 1;
  if (dx < 0){
    xi = -1;
    dx = -dx;
  }
  int d = 2 * dx - dy;
  int x = x0;

  for (int y=y0; y<y1; y++){
    //TODO
    digitalWrite(motor2.step, HIGH);
    if (d > 0){
      digitalWrite(motor1.step,HIGH);
      x = x + xi;
      d = d - 2 * dy;
    }
    delayMicroseconds(5);
    digitalWrite(motor1.step,LOW);
    digitalWrite(motor2.step,LOW);
    delayMicroseconds(motorDelay-5);
    d = d + 2 * dx;
  }
}

void lineLow(int x0, int y0, int x1, int y1){
  int dx = x1 - x0;
  int dy = y1 - y0;
  int yi = 1;
  if(dy < 0){
    yi = -1;
    dy = -dy;
  }
  int D = 2*dy - dx;
  int y = y0;

  for(int x = x0; x<x1; x++){
    //TODO
    digitalWrite(motor1.step, HIGH);
    if(D > 0){
       digitalWrite(motor2.step,HIGH);
       y = y + yi;
       D = D - 2*dx;
    }
    delayMicroseconds(5);
    digitalWrite(motor1.step,LOW);
    digitalWrite(motor2.step,LOW);
    delayMicroseconds(motorDelay-5);
    D = D + 2*dy;
  }
}

void motorStep(int steps){
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

void motorPower(bool on){
  led_light(on?YELLOW:GREEN);
  digitalWrite(motor1.enabled, !on);  //ENABLE pin needs to be low to turn off power
  digitalWrite(motor2.enabled, !on);
  delayMicroseconds(5);
  powerEnabled = on;
}

void motorPower(){
  motorPower(!powerEnabled);
}
  

void motorSetSpeed(int speed){
  motorSpeed = speed;
  motorDelay = (1000000)/(speed * 106);
}
