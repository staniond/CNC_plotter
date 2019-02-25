//
// Created by standa on 2/23/19.
//

#ifndef CNC_PLOTTER_ACCELERATION_H
#define CNC_PLOTTER_ACCELERATION_H

#include <stdint.h>

#define MAX_FEED 1500
#define NORMAL_FEED 1000
#define MIN_FEED 250
#define MIN_ACCELERATION 0
#define MAX_ACCELERATION 1000

void setup_acceleration(uint32_t steps, uint32_t start_feed, uint32_t end_feed);

uint32_t next_feed(void);

void change_acceleration(float new_acceleration);

void change_min_feed(uint32_t new_min_feed);

#endif //CNC_PLOTTER_ACCELERATION_H
