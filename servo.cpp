#include <ESP32Servo.h>
#include "servo.h"

Servo servo;
const int servoPin = 32;
int servoPos = 0;

void servoSetup(){
  servo.attach(servoPin);
  servo.write(servoPos);
}


void moveServo(int pos){
  if(pos < 0){
    pos = 0;
  }else if(pos > 180){
    pos = 180;
  }
  servoPos = pos;
  servo.write(servoPos);
  Serial.println("Servo set to " + String(servoPos) + " degrees");
}
