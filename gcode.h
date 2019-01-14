#pragma once

typedef struct Field {
  char letter;
  double num;
} Field;

const int bufferLen = 128;
const int commandLen = 10;

extern char buffer[bufferLen];
extern Field command[commandLen];

int parseBuffer(int);
void processCommand(int);
