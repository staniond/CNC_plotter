//
// Created by standa on 2/8/19.
//

#ifndef CNC_PLOTTER_LED_H
#define CNC_PLOTTER_LED_H

#define RED_LED_PIN 26
#define YELLOW_LED_PIN 25
#define GREEN_LED_PIN 33

enum Led { RED = RED_LED_PIN,
    YELLOW = YELLOW_LED_PIN,
    GREEN = GREEN_LED_PIN,
    NONE = 0
};

void led_setup(void);
void led_light(enum Led led);

#endif //CNC_PLOTTER_LED_H
