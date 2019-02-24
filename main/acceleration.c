//
// Created by standa on 2/23/19.
//

#include <stdio.h>
#include <stdint.h>
#include "acceleration.h"
#include "motors.h"
#include "utils.h"

uint32_t min_feed = MIN_FEED;
int acceleration = 5;

struct Profile {
    int steps;
    int step_num;
    int accel_steps;
    int accel_up;
    int accel_down;
    uint32_t feed;
    uint32_t max_feed;

} profile;

uint32_t next_feed(void) {
    if(profile.accel_steps == 0){
        return profile.max_feed;
    }

    uint32_t feed = profile.feed;
    profile.step_num++;

    if (profile.step_num <= profile.accel_up) {
        profile.feed += acceleration;
    }else if (profile.step_num > profile.accel_down) {
        profile.feed -= acceleration;
    }

    return feed;
}

void setup_acceleration(uint32_t steps, uint32_t max_feed) {
    profile.max_feed = CONSTRAIN(max_feed, min_feed, MAX_FEED);

    profile.steps = steps;
    profile.step_num = 0;
    if(acceleration > 0){
        profile.accel_steps = ((profile.max_feed - min_feed) / acceleration) + 1;
    }else{
        profile.accel_steps = 0;
    }
    profile.feed = min_feed;

    if (profile.steps >= 2 * profile.accel_steps) {
        profile.accel_up = profile.accel_steps;
        profile.accel_down = profile.steps - profile.accel_steps;
    } else {
        profile.accel_up = profile.steps / 2;
        profile.accel_down = profile.accel_up;
    }
}

void change_acceleration(int new_acceleration) {
    acceleration = CONSTRAIN(new_acceleration, MIN_ACCELERATION, MAX_ACCELERATION);
}

void change_min_feed(uint32_t new_min_feed) {
    min_feed = CONSTRAIN(new_min_feed, MIN_FEED, MAX_FEED);
}