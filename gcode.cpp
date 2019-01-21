#include <Arduino.h>
#include "gcode.h"
#include "servo.h"
#include "motors.h"
#include "CNC_plotter.h"


char buffer[bufferLen];

void processCommand(Command *command) {
  printCommand(command);
  for (int i = 0; i < command->size; i++) {
    switch (command->fields[i].letter) {
      case 'G':
        gCommand(command, command->fields[i].num);
        break;
      case 'S':
        sCommand(command, command->fields[i].num);
        break;
      case 'E':
        eCommand(command, command->fields[i].num);
        break;
      case 'R':
        ESP.restart();
        break;
    }
  };

  free(command->fields);
  free(command);

  client.println("ok");
  client.flush();
}

void gCommand(Command *command, int num) {
  boolean move = false;
  if (num != 0 && num != 1) {
    Serial.println("gcommand not implemented");
    return;
  }

  double feedRate = normalSpeed;
  if (num == 0) {
    feedRate = maxSpeed;
  } else if (num == 1) {
    feedRate = normalSpeed;
  }

  Serial.println("Line motion");

  double newX = 0, newY = 0;
  for (int i = 0; i < command->size; i++) {
    if (command->fields[i].letter == 'X') {
      newX = command->fields[i].num;
      move = true;
    } else if (command->fields[i].letter == 'Y') {
      newY = command->fields[i].num;
      move = true;
    } else if (command->fields[i].letter == 'Z') {
      if (command->fields[i].num >= 0) {
        moveServo(UP);
      } else {
        moveServo(servoPaper);
      }
    } else if (command->fields[i].letter == 'F') {
      feedRate = command->fields[i].num;
    }
  }
  motorSetSpeed(feedRate);
  if (move) {
    plotLine(newX, newY);
  }
}

void sCommand(Command *command, int num) {
  moveServo(num);
}

void eCommand(Command *command, int num) {
  if (num) {
    motorPower(HIGH);
    servoAttach();
  } else {
    motorPower(LOW);
    servoDetach();
  }
}

Command* parseBuffer(int bufferLength) {
  int size = 0;
  Command *command = (Command*) malloc(sizeof(Command));
  command->fields = (Field*) malloc(sizeof(Field) * commandLen);
  
  for (int i = 0; i < bufferLength; i++) {
    if (size >= commandLen) {
      break;
    }
    if (buffer[i] == ';') { //skip ; comment
      break;
    } else if (buffer[i] == '(') { //skip () comment
      while (++i < bufferLength) {
        if (buffer[i] == ')') {
          i++;
          if (i >= bufferLength)
            command->size = size;
            return command;
          break;
        }
      }
    } else if (buffer[i] == ' ') { //skip whitespaces
      continue;
    }
    if (isAlpha(buffer[i])) {
      command->fields[size].letter = buffer[i];
      command->fields[size].num = atof(&buffer[++i]);
      while (i < bufferLength) {
        if (isDigit(buffer[i]) || buffer[i] == '.' || buffer[i] == ' ' || buffer[i] == '-' || buffer[i] == '+') {
          i++;
          continue;
        }
        else {
          i--;
          break;
        }
      }
      size++;
    } else {
      Serial.println("WTF?");
    }
  }
  command->size = size;
  return command;;
}

void printCommand(Command *command) {
  for (int i = 0; i < command->size; i++) {
    Serial.print(String(command->fields[i].letter) + " " + String(command->fields[i].num, 4));
  }
  Serial.println();
}
