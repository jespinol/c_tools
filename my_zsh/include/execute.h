//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#ifndef MY_ZSH_EXECUTE_H
#define MY_ZSH_EXECUTE_H

#include <wait.h> // comment this line for macOS

#include "./my_zsh.h"
#include "../include/builtin.h"

int execute(struct CommandData* command, char*** env);

void build_argv(struct CommandData* command, char** env);

void execute_binary(struct CommandData* command, char*** env);

#endif  // MY_ZSH_EXECUTE_H
