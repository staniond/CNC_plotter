//
// Created by standa on 2/10/19.
//

#include <math.h>
#include "utils.h"

// TODO c overloading using _Generic

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float cos_vector_angle(int x0, int y0, float x1, float y1) {
    if((x0 == 0 && y0 == 0) || (x1 == 0 && y1 == 0)) {
        return -1;
    }
    return (x0*x1 + y0*y1)/(sqrtf(x0*x0+y0*y0)*sqrtf(x1*x1 + y1*y1));
}
