#pragma once

enum Led { RED = 23,
           YELLOW = 22,
           GREEN = 21,
           NONE = 0
         };
         
const int rLedPin = 23;
const int yLedPin = 22;
const int gLedPin = 21;

void led_setup();
void led_light(enum Led);
