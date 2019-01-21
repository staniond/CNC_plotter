#pragma once
#include <WiFi.h>

const int queueSize = 20;
const int taskPriority = 1;

extern WiFiClient client;
extern QueueHandle_t queue;

void restart();
