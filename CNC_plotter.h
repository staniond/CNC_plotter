#pragma once
#include <WiFi.h>

#define WIFI
#define DEBUG

#ifdef DEBUG
  #ifdef WIFI
    #define Logln(...) Serial.println(__VA_ARGS__)
    #define Log(...) Serial.print(__VA_ARGS__)
  #else
    #define Logln(...) Serial.println(__VA_ARGS__)
    #define Log(...) Serial.print(__VA_ARGS__)
  #endif
#else
  #ifdef WIFI
    #define Logln(...)
    #define Log(...)
  #else
    #define Logln(...)
    #define Log(...)
  #endif
#endif

#ifdef WIFI
  #define Println(...) client.println(__VA_ARGS__)
  #define Print(...) client.print(__VA_ARGS__)
#else
  #define Println(...) Serial.println(__VA_ARGS__)
  #define Print(...) Serial.print(__VA_ARGS__)
#endif

extern WiFiClient client;
