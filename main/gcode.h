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

Command* parse_buffer(const char*, int);
void process_commands();
void g_command(Command*, int);
void s_command(Command*, int);
void e_command(Command*, int);
void f_command(Command*, int);
void print_command(Command *);
Command* generate_R_command();

#endif //CNC_PLOTTER_GCODE_H
