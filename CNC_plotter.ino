#include "CNC_plotter.h"
#include "led.h"
#include "motors.h"
#include "gcode.h"
#include "servo.h"

const char* ssid     = "UPC3618829";
const char* password = "chodnik23";
//const char* ssid     = "wghrab";
//const char* password = "Shakespeare19";
const int port = 8888;

WiFiServer server(port);
WiFiClient client;

void setup() {
  led_setup();
  led_light(RED);
  
  motorSetup();

  servoSetup();
  
  Serial.begin(115200);
  while(!Serial){
    ;
  }

  Serial.println("Booted");
  
  connectToWifi();
  server.begin();
  
  led_light(GREEN);
}

void loop() {
  client = server.available();
  
  if(client){
    Serial.println("New Client");
    client.println("Grbl 1.1 ['$' for help]");
    client.flush();
    while(client.connected()){
      if(client.available()){
        int length = client.readBytesUntil('\n', buffer, bufferLen-1);
        buffer[length] = '\0';
    
        int commandLength = parseBuffer(length);
        processCommand(commandLength);
        Serial.println();
      }
    }
    Serial.println("Client disconnected");
  }
}

void connectToWifi(){
//TODO check if still connected, reconnect if not    
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        led_light(YELLOW);
        delay(250);
        led_light(NONE);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.print(WiFi.localIP());
    Serial.println(":" + String(port));
}
