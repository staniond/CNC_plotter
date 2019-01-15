#include <ESP32Servo.h>
#include "servo.h"

Servo servo;
const int servoPin = 32;
const int servoDown = 110; //all the way down
const int servoUp = 20; //all the way up

int servoPaper = servoUp; //needs settings

int servoPos = 0; //in percent - 0 is down

void servoSetup(){
  servo.attach(servoPin);
  moveServo(UP);
}


void moveServo(int percent){
  percent = constrain(percent, 0, 100);
  servoPos = percent;
  
  int pos = map(percent, 0, 100, servoDown, servoUp);
  
  
  servo.write(pos);
  delayMicroseconds(500);
  
  Logln("Pen set to " + String(percent) + " percent height");
}
