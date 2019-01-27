#include <ESP32Servo.h>
#include "servo.h"

Servo servo;

const int servoPwmMin = 1000;
const int servoPwmMax = 2000;

const int servoPin = 32;
const int servoDown = 140; //all the way down
const int servoUp = 40; //all the way up

int servoPos = DOWN; //in percent - 0 is down
int servoPaper = DOWN; //needs settings

const char* servoPreference = "servoPaper";

void servoSetup(){
  preferences.begin("CNC_plotter", false);
  servoPaper = preferences.getUInt(servoPreference, DOWN);
  preferences.end();
  
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
  if(servoPos == percent){
    return;
  }
  servoPos = percent;
  
  int pos = map(percent, 0, 100, servoDown, servoUp);
  
  
  servo.write(pos);
  vTaskDelay(servoDelay);
  
  Serial.println("Pen set to " + String(percent) + " percent height");
}
