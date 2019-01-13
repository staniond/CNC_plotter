#include <Arduino.h>
#include "gcode.h"

char buffer[bufferLen];
Field command[commandLen];

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
      Serial.println("WTF?");
    }
  }
  return commandLength;
}
