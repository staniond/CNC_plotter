//
// Created by standa on 2/10/19.
//

#ifndef CNC_PLOTTER_GCODE_H
#define CNC_PLOTTER_GCODE_H

typedef struct Field {
    char letter;
    double num;
} Field;

typedef struct Command {
    int size;
    Field *fields;
} Command;

Command parse_buffer(const char *buffer, int buffer_length);

void process_commands(void *pvParameters);

void g_command(Command command, int num);

void s_command(Command command, int num);

void e_command(Command command, int num);

void f_command(Command command, int num);

void print_command(Command command);

Command generate_R_command(void);

#endif //CNC_PLOTTER_GCODE_H
