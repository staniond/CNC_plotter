#include <ESP32Servo.h>
#include "servo.h"

Servo servo;

const int servoPwmMin = 1000;
const int servoPwmMax = 2000;

const int servoPin = 32;
const int servoDown = 140; //all the way down
const int servoUp = 40; //all the way up

int servoPos = 0; //in percent - 0 is down
int servoPaper = 0; //needs settings

void servoSetup(){
  servoAttach();
  moveServo(UP);
  servoDetach();
}

void servoAttach() {
  servo.attach(servoPin);
}

void servoDetach() {
  servo.detach();
}


void moveServo(int percent){
  percent = constrain(percent, 0, 100);
  servoPos = percent;
  
  int pos = map(percent, 0, 100, servoDown, servoUp);
  
  
  servo.write(pos);
//  delayMicroseconds(500000);
  vTaskDelay(servoDelay);
  
  Serial.println("Pen set to " + String(percent) + " percent height");
}
