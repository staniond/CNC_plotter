//
// Created by standa on 2/10/19.
//

#include "utils.h"

// TODO c overloading using _Generic

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double map_double(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long constrain(long val, long min, long max) {
    if(val > max){
        return max;
    }
    if(val < min) {
        return min;
    }
    return val;
}

double constrain_double(double val, double min, double max) {
    if(val > max){
        return max;
    }
    if(val < min) {
        return min;
    }
    return val;
}
