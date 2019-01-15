#pragma once

typedef struct Motor {
  int step;
  int dir;
  int enabled;
} Motor;

extern Motor motor1;
extern Motor motor2;

const int stepsPerRevolution = 20;
const int resolution = 16;
const int maxSteps = 4240;  //check
const int maxSpeed = 1500 * 16; //RPM (0.5 seconds from start to max)

void motorSetup();
void motorStep(int);
void motorSetSpeed(int);
void motorPower(bool);
