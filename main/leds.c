//
// Created by standa on 2/8/19.
//

#include <driver/gpio.h>
#include "leds.h"

// TODO led task to be able to blink easily

void led_setup(void) {
    gpio_config_t io_conf;

    uint64_t led_pins = (1ULL << RED_LED_PIN) | (1ULL << YELLOW_LED_PIN) | (1ULL << GREEN_LED_PIN);

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = led_pins;
    io_conf.intr_type = 0;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);
}

void led_light(enum Led led) {
    gpio_set_level(RED_LED_PIN, 0);
    gpio_set_level(YELLOW_LED_PIN, 0);
    gpio_set_level(GREEN_LED_PIN, 0);

    if (led == NONE) {
        return;
    }

    gpio_set_level((gpio_num_t) led, 1);
}
