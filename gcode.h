typedef struct Field {
  char letter;
  double num;
} Field;

const int bufferLen = 128;
const int commandLen = 5;

extern char buffer[bufferLen];
extern Field command[commandLen];

int parseCommand(int);
