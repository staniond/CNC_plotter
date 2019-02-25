//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_MOTORS_H
#define CNC_PLOTTER_MOTORS_H

#include "stdbool.h"

#define HIGH 1
#define LOW 0

#define MAX_STEPS 4240
#define MAX_RANGE_MM 40
#define STEPS_PER_MM ((float)MAX_STEPS/MAX_RANGE_MM)

extern bool motors_enabled;
extern int x_pos;
extern int y_pos;

void motor_setup(void);

void motor_power(bool on);

void plot_line(int new_x_pos, int new_y_pos, int feed);

#endif //CNC_PLOTTER_MOTORS_H
