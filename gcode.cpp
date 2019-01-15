#include <Arduino.h>
#include "gcode.h"
#include "servo.h"
#include "motors.h"
#include "CNC_plotter.h"


char buffer[bufferLen];
Field command[commandLen];

void processCommand(int commandLength) {
  printCommand(commandLength);
  for(int i = 0; i < commandLength; i++){
      switch(command[i].letter) {
        case 'G':
          gCommand(command[i].num, commandLength);
          break;
        case 'S':
          sCommand(command[i].num, commandLength);
          break;
        case 'M':
          motorSetSpeed(command[i].num);
          break;
        case 'N':
          motorStep(command[i].num);
          break;
        case 'E':
          motorPower(command[i].num);
          break;
      }
  }
  Println("ok");
  client.flush();
}

void gCommand(double num, int commandLength) {
  boolean move = false;
  if(num != 0 && num != 1){
    Logln("gcommand not implemented");
    return;
  }
  
  double feedRate = normalSpeed;
  if(num == 0){
      feedRate = maxSpeed;
  }else if(num == 1){
      feedRate = normalSpeed;
  }
  
  Logln("Line motion");
  
  double newX = 0, newY = 0;
  for(int i = 0; i < commandLength; i++){
    if(command[i].letter == 'X'){
        newX = command[i].num;
        move = true;
    }else if(command[i].letter == 'Y'){
        newY = command[i].num;
        move = true;
    }else if(command[i].letter == 'Z'){
        if(command[i].num >= 0){
          moveServo(UP);
        }else{
          moveServo(DOWN);
        }
     }else if(command[i].letter == 'F'){
       feedRate = command[i].num;
     }
  }
  motorSetSpeed(feedRate);
  if(move){
      plotLine(newX, newY); 
  }
}

void sCommand(double num, int commandLength){
  moveServo(num);
}

int parseBuffer(int bufferLength){
  int commandLength = 0;
  for(int i = 0; i < bufferLength; i++){
    if(commandLength >= commandLen){
      break;
    }
    if(buffer[i] == ';'){ //skip ; comment
      break;
    }else if(buffer[i] == '('){ //skip () comment
      while(++i<bufferLength){
        if(buffer[i] == ')'){
          i++;
          if(i >= bufferLength)
            return commandLength;
          break;
        }
      }
    }else if(buffer[i] == ' '){ //skip whitespaces
      continue;
    }
    if(isAlpha(buffer[i])){
      command[commandLength].letter = buffer[i];
      command[commandLength].num = atof(&buffer[++i]);
      while(i<bufferLength){
        if(isDigit(buffer[i]) || buffer[i] == '.' || buffer[i] == ' ' || buffer[i] == '-' || buffer[i] == '+'){
          i++;
          continue;
        }
        else{
          i--;
          break; 
        }
      }
      commandLength++;
    }else{
      Logln("WTF?");
    }
  }
  return commandLength;
}

void printCommand(int commandLength) {
  for(int i = 0; i < commandLength; i++){
    Log(String(command[i].letter) + String(command[i].num, 4));
  }
  Logln();
}
