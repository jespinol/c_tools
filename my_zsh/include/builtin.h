//
// Created by Jose Espinola-Lopez on 10/14/23.
//

#ifndef MY_ZSH_BUILTIN_H
#define MY_ZSH_BUILTIN_H

#include <limits.h>

#include "./my_zsh.h"

#define BUILTIN_NUM 9

struct BuiltinCommand {
  const char *name;
  int (*BuiltinFunction)(struct CommandData *command, char ***env);
};

typedef int (*BuiltinFunction)(struct CommandData *command, char ***env);

BuiltinFunction is_builtin(char *command);

int my_cd(struct CommandData *cmd, char ***env);

int my_echo(struct CommandData *cmd, char ***env);

int my_pwd(struct CommandData *cmd, char ***env);

int my_which(struct CommandData *cmd, char ***env);

char *get_path_from_PATH(char *command, char **env);

int my_exit(struct CommandData *cmd, char ***env);

int my_env(struct CommandData *cmd, char ***env);

char *my_getenv(const char *name, char **env);

int my_setenv(struct CommandData *cmd, char ***env);

void env_setter(const char *name, const char *value, char ***env);

int my_unsetenv(struct CommandData *cmd, char ***env);

#endif  // MY_ZSH_BUILTIN_H
