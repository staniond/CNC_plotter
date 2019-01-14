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
const int maxSteps = 265 * 16;  //check
const int maxSpeed = 1500 * 16; //RPM (0.5 seconds from start to max)

void motor_setup();
void motor_step(int);
void motorSetSpeed(int);
void motor_power(bool);
