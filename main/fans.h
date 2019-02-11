//
// Created by standa on 2/11/19.
//

#ifndef CNC_PLOTTER_FANS_H
#define CNC_PLOTTER_FANS_H

enum FanState {
    ON = 1,
    OFF = 0
};

void fans_setup(void);
void set_fans(enum FanState);

#endif //CNC_PLOTTER_FANS_H
