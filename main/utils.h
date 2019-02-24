//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_UTILS_H
#define CNC_PLOTTER_UTILS_H

#define CONSTRAIN(val, min, max) (val < min? min : val > max? max : val)
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

long map(long x, long in_min, long in_max, long out_min, long out_max);
double map_double(double x, double in_min, double in_max, double out_min, double out_max);

#endif //CNC_PLOTTER_UTILS_H
