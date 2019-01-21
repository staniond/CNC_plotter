#pragma once
#include <WiFi.h>

const int queueSize = 20;

extern WiFiClient client;
extern QueueHandle_t queue;
