//
// Created by standa on 2/8/19.
//

#ifndef CNC_PLOTTER_MAIN_H
#define CNC_PLOTTER_MAIN_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/queue.h>

#define QUEUE_SIZE 20

extern QueueHandle_t queue;

void app_main(void);
void restart(void);

#endif //CNC_PLOTTER_MAIN_H
