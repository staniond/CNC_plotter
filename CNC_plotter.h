#pragma once
#include <WiFi.h>

#define WIFI

#ifdef WIFI
  #define Logln(...) client.println(__VA_ARGS__)
  #define Log(...) client.print(__VA_ARGS__)
#else
  #define Logln(...) Serial.println(__VA_ARGS__)
  #define Log(...) Serial.print(__VA_ARGS__)
#endif

extern WiFiClient client;
