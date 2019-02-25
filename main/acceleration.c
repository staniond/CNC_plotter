//
// Created by standa on 2/23/19.
//

#include <stdio.h>
#include <stdint.h>
#include "acceleration.h"
#include "utils.h"

float acceleration = 0;

struct Profile {
    int steps;
    int step_num;
    int accel_steps;
    int accel_up;
    int accel_down;
    float start_feed;
    uint32_t end_feed;

} profile;

uint32_t next_feed(void) {
    if(profile.accel_steps == 0){
        return profile.end_feed;
    }

    uint32_t feed = (uint32_t) profile.start_feed;
    profile.step_num++;

    if (profile.step_num <= profile.accel_up) {
        profile.start_feed += acceleration;
    }else if (profile.step_num > profile.accel_down) {
        profile.start_feed -= acceleration;
    }

    return feed;
}

void setup_acceleration(uint32_t steps, uint32_t start_feed, uint32_t end_feed) {
    profile.steps = steps;
    profile.step_num = 0;
    profile.end_feed = end_feed;
    if(acceleration > 0){
        profile.accel_steps = (int) (((profile.end_feed - start_feed) / acceleration) + 1);
    }else{
        profile.accel_steps = 0;
    }
    profile.start_feed = start_feed;

    if (profile.steps >= 2 * profile.accel_steps) {
        profile.accel_up = profile.accel_steps;
        profile.accel_down = profile.steps - profile.accel_steps;
    } else {
        profile.accel_up = profile.steps / 2;
        profile.accel_down = profile.accel_up;
    }
}

void change_acceleration(float new_acceleration) {
    acceleration = CONSTRAIN(new_acceleration, MIN_ACCELERATION, MAX_ACCELERATION);
}