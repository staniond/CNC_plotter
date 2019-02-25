#include <string.h>
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/queue.h>
#include "freertos/event_groups.h"
#include <freertos/queue.h>
#include "nvs_flash.h"

#include "main.h"
#include "wifi.h"
#include "leds.h"
#include "servo.h"
#include "gcode.h"
#include "motors.h"
#include "fans.h"
#include "acceleration.h"

static const char *TAG = "MAIN";

#define TCP_SERVER_TASK_PRIORITY 1 // TODO what priority?
#define PROCESS_COMMANDS_TASK_PRIORITY 1

#define TCP_SERVER_TASK_CORE 0
#define PROCESS_COMMANDS_TASK_CORE 1

QueueHandle_t queue;

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    esp_log_level_set("*", ESP_LOG_NONE);   // TODO command to turn on and off debug messages

    led_setup();
    led_light(YELLOW);

    queue = xQueueCreate(QUEUE_SIZE, sizeof(Command));

    motor_setup();

    servo_setup();

    fans_setup();

    initialise_wifi();
    wait_for_ip();

    // TODO check free stack space and optimize
    xTaskCreatePinnedToCore(tcp_server_task, "tcp_server_task", 4096, NULL, TCP_SERVER_TASK_PRIORITY, NULL,
                            TCP_SERVER_TASK_CORE);
    xTaskCreatePinnedToCore(process_commands, "process_commands_task", 4096, NULL, PROCESS_COMMANDS_TASK_PRIORITY, NULL,
                            PROCESS_COMMANDS_TASK_CORE);
}

void restart(void) {
    move_servo(SERVO_UP);
    plot_line(0, 0, MAX_FEED);
    motor_power(LOW);
    servo_detach();
    set_fans(OFF);

    ESP_LOGI(TAG, "Reset complete");
}
