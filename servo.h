#pragma once
#include <ESP32Servo.h>

extern int servoPos;

void servoSetup();
void moveServo(int);
