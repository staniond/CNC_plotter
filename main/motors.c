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


Motor motor1;
Motor motor2;

static int xPos = 0;
static int yPos = 0;

bool motors_enabled;

static const char *TAG = "MOTORS";

void motor_setup(void){
    motor1.enabled = 27;
    motor1.step = 14;
    motor1.dir = 12;

    motor2.enabled = 33;
    motor2.step = 25;
    motor2.dir = 26;

    gpio_config_t io_conf;

    uint64_t motor1_pins = ((1ULL<<motor1.enabled) | (1ULL<<motor1.step) | (1ULL<<motor1.dir));
    uint64_t motor2_pins = ((1ULL<<motor2.enabled) | (1ULL<<motor2.step) | (1ULL<<motor2.dir));

    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (motor1_pins | motor2_pins);
    io_conf.intr_type = 0;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);

    gpio_set_level(motor1.enabled, HIGH);   //turn off power to motors by default
    gpio_set_level(motor2.enabled, HIGH);

    motors_enabled = 0;
}

void plot_line(double x_pos_mm, double y_pos_mm, int feed) {
    vTaskSuspendAll();  // TODO try suspending interrupts also?

    x_pos_mm = constrain_double(x_pos_mm, 0, MAX_RANGE_MM);
    y_pos_mm = constrain_double(y_pos_mm, 0, MAX_RANGE_MM);

    int newX = (int) (STEPS_PER_MM * x_pos_mm);
    int newY = (int) (STEPS_PER_MM * y_pos_mm);
    uint32_t motorDelay = feed_to_delay(feed);

    if(xPos>newX){
        gpio_set_level(motor1.dir, HIGH);
    }else{
        gpio_set_level(motor1.dir, LOW);
    }
    if(yPos>newY){
        gpio_set_level(motor2.dir, HIGH);
    }else{
        gpio_set_level(motor2.dir, LOW);
    }

    if (abs(newY - yPos) < abs(newX - xPos)){
        if (xPos > newX){
            line_low(newX, newY, xPos, yPos, motorDelay);
        }
        else{
            line_low(xPos, yPos, newX, newY, motorDelay);
        }
    }
    else{
        if (yPos > newY){;
            line_high(newX, newY, xPos, yPos, motorDelay);
        }
        else{
            line_high(xPos, yPos, newX, newY, motorDelay);
        }
    }
    xTaskResumeAll();

    xPos = newX;
    yPos = newY;
    ESP_LOGI(TAG, "New pos - %d, %d", xPos, yPos);
}

void line_high(int x0, int y0, int x1, int y1, uint32_t motorDelay){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0){
        xi = -1;
        dx = -dx;
    }
    int d = 2 * dx - dy;
    int x = x0;

    for (int y=y0; y<y1; y++){
        gpio_set_level(motor2.step, HIGH);
        if (d > 0){
            gpio_set_level(motor1.step, HIGH);
            x = x + xi;
            d = d - 2 * dy;
        }
        ets_delay_us(5);
        gpio_set_level(motor1.step, LOW);
        gpio_set_level(motor2.step, LOW);
        ets_delay_us(motorDelay - 5);
        d = d + 2 * dx;
    }
}

void line_low(int x0, int y0, int x1, int y1, uint32_t motorDelay){
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int D = 2*dy - dx;
    int y = y0;

    for(int x = x0; x<x1; x++){
        gpio_set_level(motor1.step, HIGH);
        if(D > 0){
            gpio_set_level(motor2.step, HIGH);
            y = y + yi;
            D = D - 2*dx;
        }
        ets_delay_us(5);
        gpio_set_level(motor1.step, LOW);
        gpio_set_level(motor2.step, LOW);
        ets_delay_us(motorDelay - 5);
        D = D + 2*dy;
    }
}


void motor_power(bool on){
    led_light(on?YELLOW:GREEN);
    gpio_set_level(motor1.enabled, (uint32_t) !on); //ENABLE pin needs to be low to turn off power
    gpio_set_level(motor2.enabled, (uint32_t) !on);
    motors_enabled = on;
    ets_delay_us(5);
    ESP_LOGI(TAG, "Motor power switched %s", on?"ON":"OFF");
}


uint32_t feed_to_delay(int feed){
    uint32_t motorDelay;
    feed = constrain(feed, feed, MAX_SPEED);
    if(feed <= 0) {
        feed = NORMAL_SPEED;
    }
    motorDelay = (uint32_t) ((60 * 1000000) / (feed * 106));
    return motorDelay;
}