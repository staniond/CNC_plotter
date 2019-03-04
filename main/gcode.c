//
// Created by standa on 2/10/19.
//

#include <esp_log.h>
#include <string.h>
#include <ctype.h>

#include "gcode.h"
#include "math.h"
#include "servo.h"
#include "main.h"
#include "motors.h"
#include "fans.h"
#include "acceleration.h"
#include "utils.h"

#define COMMAND_LENGTH 20


static const char *TAG = "GCODE";

float next_line_cos_angle(int new_x, int new_y, Command command);


void process_commands(void *pvParameters) {
    for (;;) {
        Command command;
        xQueueReceive(queue, &command, portMAX_DELAY);
//        print_command(command);

        for (int i = 0; i < command.size; i++) {
            switch (command.fields[i].letter) {
                case 'G':
                    g_command(command, (int) command.fields[i].num);
                    break;
                case 'S':
                    s_command(command, (int) command.fields[i].num);
                    break;
                case 'E':
                    e_command(command, (int) command.fields[i].num);
                    break;
                case 'F':
                    f_command(command, (int) command.fields[i].num);
                    break;
                case 'M':
                    m_command(command, (int) command.fields[i].num);
                    break;
                case 'R':
                    restart();
                    break;
                default:
                    break;
            }
        };

        free(command.fields);
        ESP_LOGI(TAG, "Gcode succesfully executed");
        ESP_LOGI(TAG, "Free spaces in command queue: %d", (int) uxQueueSpacesAvailable(queue));
    }
}

void g_command(Command command, int num) {
    bool move = false;
    if (num != 0 && num != 1) {
        ESP_LOGI(TAG, "Gcode command not implemented");
        return;
    }

    int feed = NORMAL_FEED;
    if (num == 0) {
        feed = MAX_FEED;
    } else if (num == 1) {
        feed = NORMAL_FEED;
    }

    int newX = -1, newY = -1;
    for (int i = 0; i < command.size; i++) {
        if (command.fields[i].letter == 'X') {
            newX = (int) (command.fields[i].num * STEPS_PER_MM);
            move = true;
        } else if (command.fields[i].letter == 'Y') {
            newY = (int) (command.fields[i].num * STEPS_PER_MM);
            move = true;
        } else if (command.fields[i].letter == 'Z') {
            if (command.fields[i].num >= 0) {
                move_servo(SERVO_UP);
            } else {
                move_servo(servo_paper_pos);
            }
        } else if (command.fields[i].letter == 'F') {
            feed = (int) command.fields[i].num;
        }
    }
    if (move) {
        plot_line(newX, newY, feed);
    }
}

void s_command(Command command, int num) {
    move_servo(num);
}

void e_command(Command command, int num) {
    if (num) {
        motor_power(HIGH);
        servo_attach();
    } else {
        motor_power(LOW);
        servo_detach();
    }
}

void f_command(Command command, int num) {
    if (num == 1) {
        set_fans(ON);
    } else if (num == 0) {
        set_fans(OFF);
    }
}

void m_command(Command command, int num) {
    if (num != 1) {
        return;
    }
    for (int i = 0; i < command.size; ++i) {
        if (command.fields[i].letter == 'A') {
            change_acceleration((int) command.fields[i].num);
        }
    }
}

Command parse_buffer(const char *buffer, int buffer_length) {
    int size = 0;
    Command command;
    command.fields = (Field *) malloc(sizeof(Field) * COMMAND_LENGTH);

    for (int i = 0; i < buffer_length; i++) {
        if (size >= COMMAND_LENGTH) {
            break;
        }
        if (buffer[i] == ';') { //skip ; comment
            break;
        } else if (buffer[i] == '(') { //skip () comment
            while (++i < buffer_length) {
                if (buffer[i] == ')') {
                    i++;
                    if (i >= buffer_length)
                        command.size = size;
                    return command;
                }
            }
        } else if (buffer[i] == ' ') { //skip whitespaces
            continue;
        }
        if (isalpha((int) buffer[i])) {
            command.fields[size].letter = buffer[i];
            command.fields[size].num = atof(&buffer[++i]);
            while (i < buffer_length) {
                if (isdigit((int) buffer[i]) || buffer[i] == '.' || buffer[i] == ' ' || buffer[i] == '-' ||
                    buffer[i] == '+') {
                    i++;
                    continue;
                } else {
                    i--;
                    break;
                }
            }
            size++;
            if (size > COMMAND_LENGTH) {
                // TODO realloc more memory?
                ESP_LOGI(TAG, "Gcode with more fields than allocated buffer (%d) fields - restarting", COMMAND_LENGTH);
                esp_restart();
            }
        } else {
            ESP_LOGI(TAG, "WTF");
        }
    }
    command.size = size;
    command.fields = realloc(command.fields, sizeof(Field) * size);
    return command;
}

Command generate_R_command(void) {
    Command command = {
            .size = 1,
            .fields = malloc(sizeof(Field) * 1)
    };
    command.fields[0].letter = 'R';

    return command;
}

float next_line_cos_angle(int new_x, int new_y, Command command) {
    int next_x = 0, next_y = 0;
    bool move_command = 0;

    for (int i = 0; i < command.size; i++) {
        if (command.fields[i].letter == 'X') {
            next_x = (int) (command.fields[i].num * STEPS_PER_MM);
            move_command = true;
        } else if (command.fields[i].letter == 'Y') {
            next_y = (int) (command.fields[i].num * STEPS_PER_MM);
            move_command = true;
        }
    }
    if (!move_command) {
        return -1;
    }

    return cos_vector_angle(new_x - x_pos, new_y - y_pos, next_x - new_x, next_y - new_y);
}

void print_command(Command command) {
    // TODO only if log level info
    for (int i = 0; i < command.size; i++) {
        printf("%c%.4lf ", command.fields[i].letter, command.fields[i].num);
    }
    printf("\n");
}
