//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_MOTORS_H
#define CNC_PLOTTER_MOTORS_H

#include "stdbool.h"

#define HIGH 1
#define LOW 0

extern bool motors_enabled;

void motor_setup(void);

void motor_power(bool on);

void plot_line(double x_pos_mm, double y_pos_mm, uint32_t feed);

void line_high(int x0, int y0, int x1, int y1);

void line_low(int x0, int y0, int x1, int y1);

#endif //CNC_PLOTTER_MOTORS_H
