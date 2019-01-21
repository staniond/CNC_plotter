#pragma once
#include "CNC_plotter.h"

#define UP 100
#define DOWN 0

extern int servoPaper;
const TickType_t servoDelay = 500 / portTICK_PERIOD_MS; // 500ms delay

void servoSetup();
void moveServo(int);
void servoAttach();
void servoDetach();
