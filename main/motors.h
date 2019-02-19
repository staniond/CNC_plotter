//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_MOTORS_H
#define CNC_PLOTTER_MOTORS_H

#define HIGH 1
#define LOW 0

typedef struct Motor {
    int step;
    int dir;
    int enabled;
} Motor;

extern Motor motor_x;
extern Motor motor_y;

extern bool motors_enabled;

#define STEPS_PER_REVOLUTION 20
#define STEP_RESOLUTION 16

#define MAX_STEPS 4240
#define MAX_RANGE_MM 40
#define STEPS_PER_MM ((double)MAX_STEPS/MAX_RANGE_MM)

#define MAX_SPEED 1000 // mm/min
#define NORMAL_SPEED 750

void motor_setup(void);

uint32_t feed_to_delay(int feed);  // mm/min

void motor_power(bool on);

void plot_line(double x_pos_mm, double y_pos_mm, int feed);

void line_high(int x0, int y0, int x1, int y1, uint32_t motorDelay);

void line_low(int x0, int y0, int x1, int y1, uint32_t motorDelay);

#endif //CNC_PLOTTER_MOTORS_H
