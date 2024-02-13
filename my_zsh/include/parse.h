//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#ifndef MY_ZSH_PARSE_INPUT_H
#define MY_ZSH_PARSE_INPUT_H

#include "./my_zsh.h"

void parse_input(ssize_t read, char *line, struct CommandData *command);

void init_command_struct(struct CommandData *command);

void append_argument(struct CommandData *command, const char *arg);

void free_command(struct CommandData *command);

#endif  // MY_ZSH_PARSE_INPUT_H
