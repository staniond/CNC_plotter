//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_UTILS_H
#define CNC_PLOTTER_UTILS_H

long map(long x, long in_min, long in_max, long out_min, long out_max);
long constrain(long, long, long);
double constrain_double(double, double, double);

#endif //CNC_PLOTTER_UTILS_H
