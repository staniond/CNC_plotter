//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_MOTORS_H
#define CNC_PLOTTER_MOTORS_H

#define HIGH 1
#define LOW 0

#define MAX_SPEED 1000 // mm/min
#define NORMAL_SPEED 750

extern bool motors_enabled;

void motor_setup(void);

uint32_t feed_to_delay(int feed);  // mm/min

void motor_power(bool on);

void plot_line(double x_pos_mm, double y_pos_mm, int feed);

void line_high(int x0, int y0, int x1, int y1, uint32_t motorDelay);

void line_low(int x0, int y0, int x1, int y1, uint32_t motorDelay);

#endif //CNC_PLOTTER_MOTORS_H
