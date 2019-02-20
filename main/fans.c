//
// Created by standa on 2/11/19.
//

#include <driver/gpio.h>
#include <esp_log.h>
#include "fans.h"

#define FANS_PIN 0
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<FANS_PIN)

static const char *TAG = "FANS";


void fans_setup(void) {
    gpio_config_t io_conf;

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.intr_type = 0;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);
}

void set_fans(enum FanState state) {
    gpio_set_level(FANS_PIN, state);
    ESP_LOGI(TAG, "Fans turned %s", state == ON? "ON":"OFF");
}
