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
QueueHandle_t queue;

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

  queue = xQueueCreate(queueSize, sizeof(Command*));
  if(queue == NULL){
    Serial.println("Error creating the queue");
    ESP.restart();
  }

//  xTaskCreatePinnedToCore(      
//                    processCommandsLoop,   /* Function to implement the task */
//                    "processCommandsTask", /* Name of the task */
//                    1500,      /* Stack size in bytes */
//                    NULL,       /* Task input parameter */
//                    taskPriority, /* Priority of the task */
//                    NULL,  /* Task handle */
//                    taskCore);  /* Core where the task should run */


  xTaskCreate(
                  processCommandsLoop,   /* Function to implement the task */
                  "processCommandsTask", /* Name of the task */
                  1500,      /* Stack size in bytes */
                  NULL,       /* Task input parameter */
                  taskPriority, /* Priority of the task */
                  NULL);  /* Task handle */
  
  led_light(GREEN);
}

void loop() {
  processCommands();
}

void processCommandsLoop(void *parameters) {
  for(;;) {
    client = server.available();
    
    if(client){
      Serial.println("New Client");
      client.println("Grbl 1.1 ['$' for help]");
      client.flush();
      while(client.connected()){
        if(client.available()){
          int length = client.readBytesUntil('\n', buffer, bufferLen-1);
          buffer[length] = '\0';
      
          Command *command = parseBuffer(length);
          xQueueSend(queue, &command, portMAX_DELAY);
          client.println("ok");
          client.flush();
          vTaskDelay(1/portTICK_PERIOD_MS);
        }
      }
      Serial.println("Client disconnected");
    }
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
}

void connectToWifi(){
//TODO check if still connected, reconnect if not    
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.mode (WIFI_STA);
    WiFi.setSleep(false);
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

void restart() {
  moveServo(UP);
  motorSetSpeed(maxSpeed);
  plotLine(0, 0);
  motorPower(LOW);
  servoDetach();

  Serial.println("Reset complete");
}
