#pragma once
#include <WiFi.h>
#include <Preferences.h>

const int queueSize = 20;
const int taskPriority = 1; //main sketch is 1
const int taskCore = 1; //main sketch is 1

extern WiFiClient client;
extern QueueHandle_t queue;
extern Preferences preferences;

void restart();
