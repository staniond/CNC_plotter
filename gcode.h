#pragma once

typedef struct Field {
  char letter;
  double num;
} Field;

typedef struct Command {
  int size;
  Field *fields;
} Command;

const int bufferLen = 128;
const int commandLen = 20;

extern char buffer[bufferLen];

Command* parseBuffer(int);
void processCommands();
void gCommand(Command*, int);
void sCommand(Command*, int);
void eCommand(Command*, int);
void printCommand(Command*);
