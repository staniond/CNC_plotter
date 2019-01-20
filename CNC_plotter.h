#pragma once
#include <WiFi.h>
//  TODO debug through serial, gcode through wifi
#define WIFI
#define DEBUG

#ifdef DEBUG
    #define Logln(...) Serial.println(__VA_ARGS__)
    #define Log(...) Serial.print(__VA_ARGS__)
#else
    #define Logln(...)
    #define Log(...)
#endif

#ifdef WIFI
  #define Println(...) client.println(__VA_ARGS__)
  #define Print(...) client.print(__VA_ARGS__)
#else
  #define Println(...) Serial.println(__VA_ARGS__)
  #define Print(...) Serial.print(__VA_ARGS__)
#endif

extern WiFiClient client;
