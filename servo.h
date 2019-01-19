#pragma once
#include "CNC_plotter.h"

#define UP 100
#define DOWN 0

extern int servoPaper;

void servoSetup();
void moveServo(int);
void servoAttach();
void servoDetach();
