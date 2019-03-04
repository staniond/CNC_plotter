//
// Created by standa on 2/10/19.
//

#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include "motors.h"
#include "utils.h"
#include "leds.h"
#include "acceleration.h"

#define STEPS_PER_REVOLUTION 20
#define STEP_RESOLUTION 16

#define uS_IN_MINUTE 60000000

typedef struct Motor {
    int step;
    int dir;
    int enabled;
} Motor;

uint32_t feed_to_delay(int feed);

int leading_axis_steps(int new_x_pos, int new_y_pos);

void line_high(int x0, int y0, int x1, int y1, uint32_t motor_delay);

void line_low(int x0, int y0, int x1, int y1, uint32_t motor_delay);

Motor motor_x = {
        .enabled = 27,
        .step = 14,
        .dir = 12,
};
Motor motor_y = {
        .enabled = 33,
        .step = 25,
        .dir = 26,
};

int x_pos = 0;
int y_pos = 0;

bool motors_enabled;

static const char *TAG = "MOTORS";

void motor_setup(void) {
    gpio_config_t io_conf;

    uint64_t motor_x_pins = ((1ULL << motor_x.enabled) | (1ULL << motor_x.step) | (1ULL << motor_x.dir));
    uint64_t motor_y_pins = ((1ULL << motor_y.enabled) | (1ULL << motor_y.step) | (1ULL << motor_y.dir));

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (motor_x_pins | motor_y_pins);
    io_conf.intr_type = 0;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);

    gpio_set_level(motor_x.enabled, HIGH);   //turn off power to motors by default
    gpio_set_level(motor_y.enabled, HIGH);

    motors_enabled = 0;
}

void plot_line(int new_x_pos, int new_y_pos, int feed) {
    vTaskSuspendAll();  // TODO try suspending interrupts also?
    
    if(new_x_pos == -1) {
        new_x_pos = x_pos;
    }
    if(new_y_pos == -1) {
        new_y_pos = y_pos;
    }

    new_x_pos = CONSTRAIN(new_x_pos, 0, MAX_STEPS);
    new_y_pos = CONSTRAIN(new_y_pos, 0, MAX_STEPS);

    uint32_t motor_delay = feed_to_delay(feed);

    //set motor direction
    if (x_pos > new_x_pos) {
        gpio_set_level(motor_x.dir, HIGH);
    } else {
        gpio_set_level(motor_x.dir, LOW);
    }
    if (y_pos > new_y_pos) {
        gpio_set_level(motor_y.dir, HIGH);
    } else {
        gpio_set_level(motor_y.dir, LOW);
    }

    if (abs(new_y_pos - y_pos) < abs(new_x_pos - x_pos)) {
        if (x_pos > new_x_pos) {
            line_low(new_x_pos, new_y_pos, x_pos, y_pos, motor_delay);
        } else {
            line_low(x_pos, y_pos, new_x_pos, new_y_pos, motor_delay);
        }
    } else {
        if (y_pos > new_y_pos) { ;
            line_high(new_x_pos, new_y_pos, x_pos, y_pos, motor_delay);
        } else {
            line_high(x_pos, y_pos, new_x_pos, new_y_pos, motor_delay);
        }
    }

    xTaskResumeAll();

    x_pos = new_x_pos;
    y_pos = new_y_pos;

    ESP_LOGI(TAG, "New pos - %d, %d", x_pos, y_pos);
}

void line_high(int x0, int y0, int x1, int y1, uint32_t motor_delay) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int d = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y < y1; y++) {
        gpio_set_level(motor_y.step, HIGH);
        if (d > 0) {
            gpio_set_level(motor_x.step, HIGH);
            x += xi;
            d -= 2 * dy;
        }
        ets_delay_us(5);
        gpio_set_level(motor_x.step, LOW);
        gpio_set_level(motor_y.step, LOW);
        ets_delay_us(motor_delay - 5);
        d += 2 * dx;
    }
}

void line_low(int x0, int y0, int x1, int y1, uint32_t motor_delay) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x < x1; x++) {
        gpio_set_level(motor_x.step, HIGH);
        if (D > 0) {
            gpio_set_level(motor_y.step, HIGH);
            y += yi;
            D -= 2 * dx;
        }
        ets_delay_us(5);
        gpio_set_level(motor_x.step, LOW);
        gpio_set_level(motor_y.step, LOW);
        ets_delay_us(motor_delay - 5);
        D += 2 * dy;
    }
}


void motor_power(bool on) {
    led_light(on ? YELLOW : GREEN);
    gpio_set_level(motor_x.enabled, (uint32_t) !on); //ENABLE pin needs to be low to turn off power
    gpio_set_level(motor_y.enabled, (uint32_t) !on);
    motors_enabled = on;
    ets_delay_us(5);
    ESP_LOGI(TAG, "Motor power switched %s", on ? "ON" : "OFF");
}


uint32_t feed_to_delay(int feed) {
    uint32_t motorDelay;
    feed = CONSTRAIN(feed, feed, MAX_FEED);
    if (feed <= 0) {
        feed = NORMAL_FEED;
    }
    motorDelay = (uint32_t) (uS_IN_MINUTE / (feed * STEPS_PER_MM));
    return motorDelay;
}

int leading_axis_steps(int new_x_pos, int new_y_pos) {
    int delta_x = abs(new_x_pos - x_pos);
    int delta_y = abs(new_y_pos - y_pos);
    return MAX(delta_x, delta_y);
}
