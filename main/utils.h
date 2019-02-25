//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_UTILS_H
#define CNC_PLOTTER_UTILS_H

#define CONSTRAIN(val, min, max) (val < min? min : val > max? max : val)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

long map(long x, long in_min, long in_max, long out_min, long out_max);

float mapf(float x, float in_min, float in_max, float out_min, float out_max);

float cos_vector_angle(int x0, int y0, float x1, float y1);

#endif //CNC_PLOTTER_UTILS_H
