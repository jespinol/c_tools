//
// Created by Jose Espinola-Lopez on 8/22/23.
//

#ifndef MY_ZSH_MY_ZSH_H
#define MY_ZSH_MY_ZSH_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/helpers.h"

#define PATH "PATH"
#define HOME "HOME"
#define PWD "PWD"
#define OLDPWD "OLDPWD"

struct CommandData {
  char *input_cmd;
  char **input_args;
  int num_args;
  char **argv;
};

void my_zsh(char **envp);

#endif  // MY_ZSH_MY_ZSH_H
