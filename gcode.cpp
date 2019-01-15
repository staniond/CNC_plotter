#include <Arduino.h>
#include "gcode.h"
#include "servo.h"
#include "motors.h"
#include "CNC_plotter.h"


char buffer[bufferLen];
Field command[commandLen];

void processCommand(int commandLength) {
  for(int i = 0; i < commandLength; i++){
      Log(String(command[i].letter));
      Logln(command[i].num, 3);
      
      switch(command[i].letter) {
        case 'S':
          moveServo(command[i].num);
          break;
        case 'M':
          motorSetSpeed(command[i].num);
          break;
        case 'N':
          motorStep(command[i].num);
          break;
        case 'P':
          motorPower(command[i].num);
          break;
        default:
          Logln("Gcode command not recognized");
          break;
      }
  }
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
