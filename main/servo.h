//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_SERVO_H
#define CNC_PLOTTER_SERVO_H

#include <freertos/portmacro.h>
#include <esp_task.h>

#define SERVO_UP 100
#define SERVO_DOWN 0

extern int servo_paper_pos;
//TODO preference

void servo_setup();
void move_servo(int);
void servo_attach();
void servo_detach();

#endif //CNC_PLOTTER_SERVO_H
